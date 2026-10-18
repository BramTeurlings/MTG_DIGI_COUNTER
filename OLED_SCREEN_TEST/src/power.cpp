#include "power.h"
#include <USB.h>
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
  sleep_run_from_xosc();
  sleep_goto_dormant_until_any_level_high((1u << UP_BUTTON_PIN) | (1u << DOWN_BUTTON_PIN) | (1u << MENU_BUTTON_PIN));

  // Execution stops here until a button is pressed

  sleep_power_up();
  set_sys_clock_khz(SYS_CLOCK_KHZ, true);
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
