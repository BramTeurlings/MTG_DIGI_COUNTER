#include "power.h"
#include <USB.h>
#include <hardware/adc.h>
#include <hardware/sync.h>
#include "config.h"
#include "pico_rtc/pico_sleep.h"

void Power::powerOff() {
  Serial.println("Powering off...");
  renderer.clear();
  renderer.show();
  renderer.displayOff();

  sleepUntilButtonPress();
}

void Power::sleepUntilButtonPress() {
  // Todo: Remove the debug logic to toggle the LED
  digitalWrite(DEBUG_LED_PIN, LOW);

  // Wake-up is level triggered, so wait until the button that sent us to sleep is released
  input.waitUntilReleased();

  Serial.flush();
  USB.disconnect(); // Detach cleanly, USB is unclocked while dormant

  // No interrupts while the clocks are switched, e.g. the USB task would touch unclocked hardware
  const uint32_t interrupts = save_and_disable_interrupts();

  // Turn off the ADC (used for the battery reading) while dormant to save power.
  // The first analogRead() turns the ADC on and nothing turns it off again. Dormant mode stops the ADC's clock
  // (clk_adc), so it can't take readings, but its analog circuitry stays powered and keeps drawing current.
  // Clearing the enable bit powers it down. Only that bit changes, so the selected input pin is kept.
  hw_clear_bits(&adc_hw->cs, ADC_CS_EN_BITS);

  sleep_run_from_xosc();
  sleep_goto_dormant_until_any_level_high((1u << UP_BUTTON_PIN) | (1u << DOWN_BUTTON_PIN) | (1u << MENU_BUTTON_PIN));

  // Execution stops here until a button is pressed

  sleep_power_up();
  set_sys_clock_khz(SYS_CLOCK_KHZ, true);

  // Turn the ADC back on. analogRead() only does that the first time it's called, so without this the next battery
  // reading would wait forever for an ADC that's off.
  // This has to come after sleep_power_up(): the ADC runs on its own clock (clk_adc), which dormant mode stopped and
  // sleep_power_up() starts again. The ADC only reports ready once it has that clock, so before that the loop below
  // would never end.
  hw_set_bits(&adc_hw->cs, ADC_CS_EN_BITS);
  while (!(adc_hw->cs & ADC_CS_READY_BITS)) {
    tight_loop_contents();
  }

  restore_interrupts(interrupts);

  USB.connect();

  // Todo: Remove the debug logic to toggle the LED
  digitalWrite(DEBUG_LED_PIN, HIGH);

  // Does nothing if the display was still on
  renderer.displayOn();

  // Swallow the wake-up press, so it doesn't also change anything
  input.waitUntilReleased();
  input.reset();
  Serial.println("Woke up.");
}
