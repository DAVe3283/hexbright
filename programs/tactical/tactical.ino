/*
Copyright (c) 2013, "David Hilton" <dhiltonp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include <print_power.h>

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

hexbright hb;

#define HOLD_TIME 250 // milliseconds before going to strobe
#define OFF_TIME 2000 // milliseconds before going off on the next normal button press

int           brightness[]      = { OFF_LEVEL,  1, 260, 500, 750, 1000 };
unsigned char gled_brightness[] = {         0, 32, 255, 255, 255,  255 };
long          transition_time[] = {       200,  0, 100, 100, 100,  100 };
#define BRIGHTNESS_OFF 0  // the index of OFF_LEVEL (be sure to update!)

int current_brightness = BRIGHTNESS_OFF; // start with the LED off
#define BRIGHTNESS_COUNT (sizeof(brightness) / sizeof(*brightness))

unsigned long short_press_time = 0;

void setup() {
  hb.init_hardware(); 
}

void loop() {
  hb.update();
  if(hb.button_just_released()) {
    
    if(hb.button_pressed_time()<HOLD_TIME) {
      // we just had a normal duration press
      if(short_press_time+OFF_TIME<millis() && brightness[current_brightness]!=OFF_LEVEL) {
        // it's been a while since our last button press, turn off
        current_brightness = BRIGHTNESS_OFF;
      } else {
        short_press_time = millis();
        current_brightness = (current_brightness+1)%BRIGHTNESS_COUNT;
      }
    } else {
      // we have been doing strobe, set light at the previous light level (do nothing to current_brightness)
    }
    // actually change the brightness
    hb.set_light(CURRENT_LEVEL, brightness[current_brightness], transition_time[current_brightness]);
    // Set rear green LED on with a timeout to indicate when a press will increase brightness
    if ((current_brightness == BRIGHTNESS_OFF) || (current_brightness == BRIGHTNESS_COUNT - 1)) {
      // We are off, or about to turn off
      hb.set_led(
        GLED, // Green LED
        0, // Remain on for 0 ms (turn off)
        100, // Wait time, using default from header
        gled_brightness[current_brightness]); // Use appropriate brightness
    } else {
      // The next press increases brightness
      hb.set_led(GLED, OFF_TIME, 100, gled_brightness[current_brightness]);
    }
  } else if (hb.button_pressed() && hb.button_pressed_time()>HOLD_TIME) {
    // held for over HOLD_TIME ms, go to strobe
    static unsigned long flash_time = millis();
    if(flash_time+70<millis()) { // flash every 70 milliseconds
      flash_time = millis(); // reset flash_time
      hb.set_light(MAX_LEVEL, 0, 20); // and pulse (going from max to min over 20 milliseconds)
      // actually, because of the refresh rate, it's more like 'go from max brightness on high
      //  to max brightness on low to off.
    }
  } 
  print_power();
}

