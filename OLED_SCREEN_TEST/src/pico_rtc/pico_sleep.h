/*
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#if defined(ARDUINO_ARCH_RP2040)

#ifndef _PICO_SLEEP_H_
#define _PICO_SLEEP_H_

#include "pico.h"
#include "hardware/rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \file sleep.h
 *  \defgroup hardware_sleep hardware_sleep
 *
 * Lower Power Sleep API
 *
 * The difference between sleep and dormant is that ALL clocks are stopped in dormant mode,
 * until the source (either xosc or rosc) is started again by an external event.
 * In sleep mode some clocks can be left running controlled by the SLEEP_EN registers in the clocks
 * block. For example you could keep clk_rtc running. Some destinations (proc0 and proc1 wakeup logic)
 * can't be stopped in sleep mode otherwise there wouldn't be enough logic to wake up again.
 *
 * \subsection sleep_example Example
 * \addtogroup hardware_sleep
 * \include hello_sleep.c

 */
typedef enum {
    DORMANT_SOURCE_NONE,
    DORMANT_SOURCE_XOSC,
    DORMANT_SOURCE_ROSC
} dormant_source_t;

/*! \brief Set all clock sources to the the dormant clock source to prepare for sleep.
 *  \ingroup hardware_sleep
 *
 * \param dormant_source The dormant clock source to use
 */
void sleep_run_from_dormant_source(dormant_source_t dormant_source);

/*! \brief Set the dormant clock source to be the crystal oscillator
 *  \ingroup hardware_sleep
 */
static inline void sleep_run_from_xosc(void) {
    sleep_run_from_dormant_source(DORMANT_SOURCE_XOSC);
}

/*! \brief Set the dormant clock source to be the ring oscillator
 *  \ingroup hardware_sleep
 */
static inline void sleep_run_from_rosc(void) {
    sleep_run_from_dormant_source(DORMANT_SOURCE_ROSC);
}

/*! \brief Send system to sleep until the specified time
 *  \ingroup hardware_sleep
 *
 * One of the sleep_run_* functions must be called prior to this call
 *
 * \param t The time to wake up
 * \param callback Function to call on wakeup.
 */
void sleep_goto_sleep_until(datetime_t *t, rtc_callback_t callback);

/*! \brief Send system to sleep until the specified GPIO changes
 *  \ingroup hardware_sleep
 *
 * One of the sleep_run_* functions must be called prior to this call
 *
 * \param gpio_pin The pin to provide the wake up
 * \param edge true for leading edge, false for trailing edge
 * \param high true for active high, false for active low
 */
void sleep_goto_dormant_until_pin(uint gpio_pin, bool edge, bool high);

/*! \brief Send system to sleep until a leading high edge is detected on GPIO
 *  \ingroup hardware_sleep
 *
 * One of the sleep_run_* functions must be called prior to this call
 *
 * \param gpio_pin The pin to provide the wake up
 */
static inline void sleep_goto_dormant_until_edge_high(uint gpio_pin) {
    sleep_goto_dormant_until_pin(gpio_pin, true, true);
}

/*! \brief Send system to sleep until a high level is detected on GPIO
 *  \ingroup hardware_sleep
 *
 * One of the sleep_run_* functions must be called prior to this call
 *
 * \param gpio_pin The pin to provide the wake up
 */
static inline void sleep_goto_dormant_until_level_high(uint gpio_pin) {
    sleep_goto_dormant_until_pin(gpio_pin, false, true);
}

/*! \brief Send system to sleep until any of the given GPIOs is high
 *  \ingroup hardware_sleep
 *
 * One of the sleep_run_* functions must be called prior to this call.
 * Level triggered, so a pin that is already high wakes the system straight away
 * instead of the wake-up being missed.
 *
 * \param gpio_mask Bit mask of the pins that can wake the system (bit n = GPIO n)
 */
void sleep_goto_dormant_until_any_level_high(uint32_t gpio_mask);

/*! \brief Restore the clocks after waking up from sleep or dormant mode
 *  \ingroup hardware_sleep
 *
 * Restarts the oscillator that sleep_run_from_dormant_source() switched off and brings the
 * PLLs and all clocks (sys, usb, adc, peri, rtc) back to the SDK defaults. Without this the
 * ADC and USB stay unclocked, so e.g. analogRead() hangs forever.
 * Call set_sys_clock_khz() afterwards if you ran at a non-default system clock.
 */
void sleep_power_up(void);

#ifdef __cplusplus
}
#endif

#endif
#endif /* ARDUINO_ARCH_RP2040 */
