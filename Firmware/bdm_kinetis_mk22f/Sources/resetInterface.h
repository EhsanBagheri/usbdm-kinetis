/*
 * ResetInterface.h
 *
 *  Created on: 22Dec.,2016
 *      Author: podonoghue
 */

#ifndef SOURCES_RESETINTERFACE_H_
#define SOURCES_RESETINTERFACE_H_

#include "hardware.h"

/**
 * Reset signal
 * Bidirection control
 * Reset event detection
 */
class ResetInterface {

private:

   // Controls direction of transceiver
   using Direction = USBDM::Reset_Dir; //USBDM::GpioC<0>;

   // Transceiver data (in/out)
   using Pin      = USBDM::Reset_IO; // USBDM::GpioC<1>;

   static bool  fResetActivity;

   /**
    * Callback used to monitor reset events
    *
    * @param status
    */
   static void callback(uint32_t status) {

      // Check if RESET pin event and pin is low
      if ((Pin::BITMASK & status) && isLow()) {
         fResetActivity = true;
      }
   }

public:
   /**
    * Initialise Transceiver
    *
    * Initial state:
    *    Reset signal Input/HighZ
    *    Reset monitoring enabled
    */
   static void initialise() {
      using namespace USBDM;

      static constexpr PcrInit pinInit(PinDriveStrength_High, PinDriveMode_PushPull, PinSlewRate_Slow);

      // Enable pins
      Pin::setInput(pinInit);
      Direction::setOutput();

      // Initially target reset is not driven
      highZ();

      // IRQ on falling edge - reset detection
      Pin::setPcrOption(USBDM::PinAction_IrqFalling);
      Pin::setPinCallback(callback);
      Pin::enableNvicPinInterrupts(USBDM::NvicPriority_Normal);

      fResetActivity = false;
   }

   /**
    * Drive signal low
    */
   static void low() {
      Pin::low();
      Pin::setOut();
      Direction::high();
   }
   /**
    * Drive signal high
    */
   static void high() {
      Pin::high();
      Pin::setOut();
      Direction::high();
   }
   /**
    * Disable Transceiver (high-impedance)\n
    * Actually sets to input
    */
   static void highZ() {
      // Pulse RST=H (speed-up)
      Pin::high();
      Pin::setOut();
      Direction::high();
      // 3-state
      Direction::low();
      Pin::setIn();
   }

   /**
    * Read value from receiver
    *
    * @return value on pin or driven value if output
    *
    * @note Assumes already set as input or returns driven value
    */
   static bool read() {
      return Pin::read();
   }

   /**
    * Check if receiver input is low
    *
    * @return true if input is low
    *
    * @note Assumes already set as input or returns driven value
    */
   static bool isLow() {
      return !Pin::read();
   }

   /**
    * Check if receiver input is high
    *
    * @return true if input is high
    *
    * @note Assumes already set as input or returns driven value
    */
   static bool isHigh() {
      return Pin::read();
   }

   /**
    * Check and clear reset activity flag
    *
    * @return True  Reset has been active since last polled
    * @return False Reset has not been active since last polled
    */
   static bool resetActivity() {
      bool temp = fResetActivity;
      fResetActivity = false;
      return temp;
   }
};

#endif /* SOURCES_RESETINTERFACE_H_ */
