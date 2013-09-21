#ifndef UTILITIES_H
#define UTILITIES_H

#include "hexbright.h"

// returns the opposite color from the one passed in
// Takes up 12 bytes.
extern unsigned char flip_color(unsigned char color);

// A convenience function that will print the charge state over the led specified
//  CHARGING = 350 ms on, 350 ms off.
//  CHARGED = solid on
//  BATTERY = nothing.
// If you are using print_number, call it before this function if possible.
// I recommend the following (if using print_number()):
//  ...code that may call print number...
//  if(!printing_number())
//    print_charge(GLED);
//  ...end of loop...
// See also: print_power
extern void print_charge(unsigned char led);

// prints charge state (using print_charge).
//  if in a low battery state, flashes red for 50 ms, followed by a 1 second delay
// If you are using print_number, call it before this function if possible.
// I recommend the following (if using print_number()):
//  ...code that may call print number...
//  if(!printing_number())
//    print_charge(GLED);
//  ...end of loop...
// see also print_charge for usage
extern void print_power();


#endif // UTILITIES_H
