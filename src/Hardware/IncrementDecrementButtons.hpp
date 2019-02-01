#pragma once

#include "Button.hpp"

class IncrementDecrementButtons {
  public:
    IncrementDecrementButtons(const Button &incrementButton,
                              const Button &decrementButton)
        : incrementButton(incrementButton), decrementButton(decrementButton) {}

    void begin() {
        incrementButton.begin();
        decrementButton.begin();
    }

    // TODO: add states for initial press
    enum State { Nothing = 0, Increment, Decrement, Reset };

    State getState() {
        Button::State incrState = incrementButton.getState();
        Button::State decrState = decrementButton.getState();

        if (decrState == Button::Released && incrState == Button::Released) {
            // Both released
        } else if ((decrState == Button::Rising &&
                    incrState == Button::Released) ||
                   (incrState == Button::Rising &&
                    decrState == Button::Released)) {
            // One released, the other rising → nothing
            // now both released, so go to initial state
            longPressState = Initial;
        } else if (incrState == Button::Falling &&
                   decrState == Button::Falling) {
            // Both falling → reset
            // (rather unlikely, but just in case)
            longPressState = AfterReset;
            return Reset;
        } else if (incrState == Button::Falling) {
            if (decrState == Button::Pressed) {
                // One pressed, the other falling → reset
                longPressState = AfterReset;
                return Reset;
            } else {
                // Increment falling, the other released → increment
                return Increment;
            }
        } else if (decrState == Button::Falling) {
            if (incrState == Button::Pressed) {
                // One pressed, the other falling → reset
                longPressState = AfterReset;
                return Reset;
            } else {
                // Decrement falling, the other released → decrement
                return Decrement;
            }
        } else if (incrState == Button::Pressed &&
                   decrState == Button::Pressed) {
            // Both pressed → nothing
        } else if (longPressState != AfterReset &&
                   incrState == Button::Pressed) {
            // Not reset and increment pressed → long press?
            if (longPressState == LongPress) {
                if (millis() - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                    longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                    return Increment;
                }
            } else if (incrementButton.stableTime() >= LONG_PRESS_DELAY) {
                longPressState = LongPress;
                longPressRepeat = millis();
                return Increment;
            }
        } else if (longPressState != AfterReset &&
                   decrState == Button::Pressed) {
            // Not reset and decrement pressed → long press?
            if (longPressState == LongPress) {
                if (millis() - longPressRepeat >= LONG_PRESS_REPEAT_DELAY) {
                    longPressRepeat += LONG_PRESS_REPEAT_DELAY;
                    return Decrement;
                }
            } else if (decrementButton.stableTime() >= LONG_PRESS_DELAY) {
                longPressState = LongPress;
                longPressRepeat = millis();
                return Decrement;
            }
        }
        return Nothing;
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() {
        incrementButton.invert();
        decrementButton.invert();
    }
#endif

  private:
    Button incrementButton;
    Button decrementButton;

    enum {
        Initial,
        LongPress,
        AfterReset,
    } longPressState = Initial;
    unsigned long longPressRepeat;
};