/**
 * @file      mcg.h (180.ARM_Peripherals/Project_Headers/mgc.h)
 *
 * @brief    Abstraction layer for MCG interface
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */

#ifndef INCLUDE_USBDM_MCG_H_ 
#define INCLUDE_USBDM_MCG_H_
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "derivative.h"
#include "system.h"
#include "pin_mapping.h"

namespace USBDM {

/**
 * @addtogroup MCG_Group MCG, Multipurpose Clock Generator
 * @brief Abstraction for Multipurpose Clock Generator
 * @{
 */

/**
 * Clock configurations
 */
enum ClockConfig : uint8_t {
   ClockConfig_PEE_IRC48MHz,

   ClockConfig_default = 0,
};

   /// Structure for clock configurations
   struct ClockInfo {

      /// SIM CLKDIV1 - System Clock Divider Register 1
      const uint32_t clkdiv1;

      /// SIM SOPT2 - Clock selectors for various peripherals
      const uint32_t sopt2;

      /// Clock Mode
      const McgClockMode clockMode;

      /// Run Mode
      const SmcRunMode runMode; 

      /// Control Register 1 - FRDIV, IRCLKEN, IREFSTEN, (-CLKS, -IREFS)
      const uint8_t c1; 
      /// Control Register 2 - LOCRE0, RANGE0, HGO0, EREFS0, IRCS, (-LP, -FCTRIM)
      const uint8_t c2;
      /// Control Register 4 - DMX32, DRST_DRS, (-FCTRIM, -SCFTRIM)
      const uint8_t c4;
      /// Control Register 5 - PLLCLKEN0, PLLSTEN0, PRDIV0
      const uint8_t c5;
      /// Control Register 6 - LOLIE0, CME0, VDIV0, (-PLLS)
      const uint8_t c6;
      /// Status and Control Register - FCRDIV
      const uint8_t sc;
      /// Control Register 7 - OSCSEL
      const uint8_t c7;
      /// Control Register 8 - LOCRE1, CME1, LOLRE
      const uint8_t c8;
   };



class ClockChangeCallback {

friend class Mcg;

private:
      // Pointer to next in chain
      ClockChangeCallback *next = nullptr;

public:
      virtual ~ClockChangeCallback() = default;

      /**
       * This method is overridden to obtain notification before clock change
       */
      virtual void beforeClockChange(){}

      /**
       * This method is overridden to obtain notification after clock change
       */
      virtual void afterClockChange(){};
};

/**
 * @brief Class representing the MCG
 *
 * <b>Example</b>
 * @code
 *    Mcg::initialise();
 * @endcode
 */
class Mcg : public McgInfo {

private:
#if false
   static ClockChangeCallback *clockChangeCallbackQueue;

   static void notifyBeforeClockChange() {
      ClockChangeCallback *p = clockChangeCallbackQueue;
      while (p != nullptr) {
         p->beforeClockChange();
         p = p->next;
      }
   }
   static void notifyAfterClockChange() {
      ClockChangeCallback *p = clockChangeCallbackQueue;
      while (p != nullptr) {
         p->afterClockChange();
         p = p->next;
      }
   }

public:
   /**
    * Add callback for clock configuration changes
    *
    * @param callback Call-back class to notify on clock configuration changes
    */
   static void addClockChangeCallback(ClockChangeCallback &callback) {
      callback.next = clockChangeCallbackQueue;
      clockChangeCallbackQueue = &callback;
   }
#endif

private:

   /** Hardware instance */
   static constexpr HardwarePtr<MCG_Type> mcg = McgInfo::baseAddress;

// No private methods found

public:

// No public methods found

   /**
    * Table of clock settings
    */
   static const ClockInfo clockInfo[];

   /** Current clock mode (FEI out of reset) */
   static McgClockMode currentClockMode;

   /**
    * Update SystemCoreClock variable
    *
    * Updates the SystemCoreClock variable with current core Clock retrieved from CPU registers.
    */
   static void SystemCoreClockUpdate(void);

#if true // /MCG/enablePeripheralSupport

   /**
    * Write main MCG registers from clockInfo
    * - Clock monitors are masked out
    * - PLL is not selected (C6.PLLS=0)
    * - Not low power (C2.LP = 0 since clockInfo.C2 does not include LP)
    * - TRIM bits are preserved (C2.FCFTRIM, C4.FCTRIM, C4.SCFTRIM)
    * - Bugfix version: Errata e7993
    *
    * @param clockInfo  Clock settings information
    * @param bugFix     Mask to flip MCG.C4 value
    */
   static void writeMainRegs(const ClockInfo &clockInfo, uint8_t bugFix);

   /**
    * Transition from current clock mode to mode given
    *
    * @param[in]  clockInfo Clock mode to transition to
    *
    * @return E_NO_ERROR          on success
    * @return E_CLOCK_INIT_FAILED on failure
    */
   static ErrorCode clockTransition(const ClockInfo &clockInfo);

   /**
    * Get current clock mode
    *
    * @return
    */
   static McgClockMode getClockMode() {
      return currentClockMode;
   }

   /**
    * Get name for clock mode
    *
    * @return Pointer to static string
    */
   static const char *getClockModeName(McgClockMode);

   /**
    * Get name for current clock mode
    *
    * @return Pointer to static string
    */
   static const char *getClockModeName() {
      return getClockModeName(getClockMode());
   }

   /**
    *  Configure the MCG for given mode
    *
    *  @param[in]  settingNumber Clock setting number
    */
   static void configure(ClockConfig settingNumber=ClockConfig_default) {
      clockTransition(clockInfo[settingNumber]);
   }

   /**
    *   Finalise the MCG
    */
   static void finalise() {
      clockTransition(clockInfo[ClockConfig_default]);
   }

#endif

   /**
    * Initialise MCG as part of startup sequence
    */
   static void startupConfigure();

// No /MCG/InitMethod methods found
};

// /MCG/No declarations methods found

/**
 * @}
 */

} // End namespace USBDM

#endif /* INCLUDE_USBDM_MCG_H_ */
