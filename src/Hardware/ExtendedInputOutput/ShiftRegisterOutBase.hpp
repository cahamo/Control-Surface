#pragma once

#include "StaticSizeExtendedIOElement.hpp"
#include <Helpers/BitArray.hpp>

/**
 * @brief   A class for serial-in/parallel-out shift registers, 
 *          like the 74HC595.
 * 
 * @tparam  N
 *          The number of bits in total. Usually, shift registers (e.g. the
 *          74HC595) have eight bits per chip, so `length = 8 * k` where `k`
 *          is the number of cascaded chips.
 * 
 * @ingroup ExtIO
 */
template <uint8_t N>
class ShiftRegisterOutBase : public StaticSizeExtendedIOElement<N> {
  protected:
    /**
     * @brief   Create a new ShiftRegisterOutBase object with a given bit order,
     *          and a given number of outputs.
     * 
     * @param   latchPin
     *          The digital output pin connected to the latch pin (ST_CP or 
     *          RCLK) of the shift register.
     * @param   bitOrder
     *          Either `MSBFIRST` (most significant bit first) or `LSBFIRST`
     *          (least significant bit first).
     */
    ShiftRegisterOutBase(pin_t latchPin, uint8_t bitOrder);

  public:
    /**
     * @brief   The pinMode function is not implemented because the mode is
     *          `OUTPUT` by definition.
     */
    void pinMode(UNUSED_PARAM pin_t pin, UNUSED_PARAM uint8_t mode) override
        __attribute__((deprecated)) {}

    /**
     * @brief   Set the state of a given output pin.
     * 
     * @param   pin
     *          The shift register pin to set.
     * @param   val
     *          The value to set the pin to.
     *          (Either `HIGH` (1) or `LOW` (0))
     */
    void digitalWrite(pin_t pin, uint8_t val) override;

    /**
     * @brief   Get the current state of a given output pin.
     * 
     * @param   pin
     *          The shift register pin to read from.
     * @return  0
     *          The state of the pin is `LOW`.
     * @return  1
     *          The state of the pin is `HIGH`.
     */
    int digitalRead(pin_t pin) override;

    /**
     * @brief   The analogRead function is deprecated because a shift
     *          is always digital.
     * @param   pin
     *          The shift register pin to read from.
     * @return  0
     *          The state of the pin is `LOW`.
     * @return  1023
     *          The state of the pin is `HIGH`.
     */
    analog_t analogRead(pin_t pin) override __attribute__((deprecated)) {
        return 1023 * digitalRead(pin);
    }

    /**
     * @brief   The analogWrite function is not deprecated because a shift
     *          is always digital.
     * @param   pin
     *          The shift register pin to set.
     * @param   val
     *          The value to set the pin to. A value greater or equal to 0x80
     *          will set the pin to a `HIGH` state, a value less than 0x80 will
     *          set the pin to a `LOW` state.
     */
    void analogWrite(pin_t pin, analog_t val) override
        __attribute__((deprecated)) {
        digitalWrite(pin, val >= 0x80);
    }

    /**
     * @brief   Get the red output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t red(pin_t id);
    /**
     * @brief   Get the green output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t green(pin_t id);
    /**
     * @brief   Get the blue output pin of the given LED.
     * 
     * @param   id
     *          The zero-based LED number.
     */
    pin_t blue(pin_t id);

  protected:
    const pin_t latchPin;
    const uint8_t bitOrder;

    BitArray<N> buffer;
    bool dirty = true;
};

#include "ShiftRegisterOutBase.ipp"