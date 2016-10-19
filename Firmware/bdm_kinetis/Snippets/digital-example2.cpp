/**
 * @file digital-example2.cpp
 */
#include <stdio.h>
#include "system.h"
#include "derivative.h"
#include "hardware.h"

using namespace USBDM;

/*
 * Simple Digital I/O example
 *
 * Echoes an external switch to an external LED
 * Uses arduino aliases if available
 *
 *  Switch + LED
 *  1 x Digital input
 *  1 x Digital output
 *
 */

// Connection mapping - change as required
using Switch =   USBDM::GpioB<0>;
using Led    =   USBDM::GpioB<1>;

int main(void) {
   Led::setOutput();
   Switch::setInput();

   for(;;) {
      Led::write(!Switch::read());
   }
}
