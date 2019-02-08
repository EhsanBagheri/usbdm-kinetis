/*
 * @file mcg.cpp (180.ARM_Peripherals/Startup_Code/mcg.cpp)
 *
 * Generic MCG
 *    1 FLL (FRDIV=/1-/128, /32-/1024, /1280, /1536)
 *    1 PLL (VCO PRDIV=/1-/24, VDIV=pll_vdiv_min+x) (pll_vdiv_min=x16,x24)
 *
 *  Created on: 04/03/2012
 *      Author: podonoghue
 */
#include "string.h"
#include "derivative.h" /* include peripheral declarations */
#include "system.h"
#include "utilities.h"
#include "stdbool.h"
#include "hardware.h"
#ifdef USBDM_RTC_IS_DEFINED
#include "rtc.h"
#endif
#include "mcg.h"
#include "osc.h"
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
extern "C" uint32_t SystemCoreClock;
extern "C" uint32_t SystemBusClock;

namespace USBDM {

#ifndef SIM_CLKDIV1_OUTDIV2
#define SIM_CLKDIV1_OUTDIV2(x) (0)
#endif

#ifndef SIM_CLKDIV1_OUTDIV3
#define SIM_CLKDIV1_OUTDIV3(x) (0)
#endif

   /**
    * Table of clock settings
    */
   const McgInfo::ClockInfo Mcg::clockInfo[] = {
      {  // ClockConfig_PEE_60MHz (ClockMode_PEE)
         
         //! SIM CLKDIV1 System Clock Divider Register 1
         SIM_CLKDIV1_OUTDIV4(5)|  // Flash clock
         SIM_CLKDIV1_OUTDIV3(3)|  // FlexBus clock
         SIM_CLKDIV1_OUTDIV2(1)|  // Bus clock
         SIM_CLKDIV1_OUTDIV1(1),  // Core/system clock

   #ifdef SIM_CLKDIV3_PLLFLLDIV
         //! SIM CLKDIV3 System Clock Divider Register 3
         0,
   #endif

         //! SIM SOPT2 System Options Register 2 (PLLFLLSEL only)
         SIM_SOPT2_PLLFLLSEL(3),      // PLL/FLL clock select

         //! Clock Mode
         McgInfo::ClockMode_PEE,

         //! Control Register 1 - Excluding CLKS, IREFS
         MCG_C1_FRDIV(6)   | // FRDIV    FLL External Reference Divider
         MCG_C1_IRCLKEN(1) | // IRCLEN   Internal Reference Clock Enable
         MCG_C1_IREFSTEN(0), // IREFSTEN Internal Reference Stop Enable

         //! Control Register 2 - Excluding LP
         MCG_C2_LOCRE0(0) | // LOCRE0  Loss of Clock Reset Enable
         MCG_C2_RANGE0(1) | // RANGE   Frequency Range Select
         MCG_C2_HGO0(0)   | // HGO     High Gain Oscillator Select
         MCG_C2_EREFS0(0) | // EREFS   External Reference Select
         MCG_C2_IRCS(1),    // IRCS    Internal Reference Clock Select

         //! Control Register 4
         MCG_C4_DMX32(0)     | // DMX32    DCO lock range
         MCG_C4_DRST_DRS(0),   // DRST_DRS DCO Range Select

         //! Control Register 5
         MCG_C5_PLLCLKEN0(0)  | // PLLCLKEN0 PLL Clock Enable
         MCG_C5_PLLSTEN0(0)   | // PLLSTEN0  PLL Stop Enable
         MCG_C5_PRDIV0(11),     // PRDIV0    PLL External Reference Divider

         //! Control Register 6 - Excluding PLLS
         MCG_C6_LOLIE0(0) | // LOLIE0 Loss of Lock interrupt Enable
         MCG_C6_CME0(0)   | // CME0   Clock Monitor Enable
         MCG_C6_VDIV0(6),   // VDIV0  PLL VCO Divider

         //! Status and Control Register
         MCG_SC_FCRDIV(0), // FCRDIV Internal Clock Reference Divider

         //! Control Register 7
         MCG_C7_OSCSEL(2), // OSCSEL MCG OSC Clock Select

         //! Control Register 8
         MCG_C8_LOCRE1(0) | // LOCRE1 RTC Loss of Clock Reset Enable
         MCG_C8_LOLRE(0)  | // LOLRE  PLL Loss of Lock Reset Enable
         MCG_C8_CME1(0),    // CME1   Clock Monitor Enable 1
      },
      {  // ClockConfig_BLPE_4MHz (ClockMode_BLPE)
         
         //! SIM CLKDIV1 System Clock Divider Register 1
         SIM_CLKDIV1_OUTDIV4(11)|  // Flash clock
         SIM_CLKDIV1_OUTDIV3(11)|  // FlexBus clock
         SIM_CLKDIV1_OUTDIV2(11)|  // Bus clock
         SIM_CLKDIV1_OUTDIV1(5),  // Core/system clock

   #ifdef SIM_CLKDIV3_PLLFLLDIV
         //! SIM CLKDIV3 System Clock Divider Register 3
         0,
   #endif

         //! SIM SOPT2 System Options Register 2 (PLLFLLSEL only)
         SIM_SOPT2_PLLFLLSEL(3),      // PLL/FLL clock select

         //! Clock Mode
         McgInfo::ClockMode_BLPE,

         //! Control Register 1 - Excluding CLKS, IREFS
         MCG_C1_FRDIV(6)   | // FRDIV    FLL External Reference Divider
         MCG_C1_IRCLKEN(1) | // IRCLEN   Internal Reference Clock Enable
         MCG_C1_IREFSTEN(0), // IREFSTEN Internal Reference Stop Enable

         //! Control Register 2 - Excluding LP
         MCG_C2_LOCRE0(0) | // LOCRE0  Loss of Clock Reset Enable
         MCG_C2_RANGE0(1) | // RANGE   Frequency Range Select
         MCG_C2_HGO0(0)   | // HGO     High Gain Oscillator Select
         MCG_C2_EREFS0(0) | // EREFS   External Reference Select
         MCG_C2_IRCS(1),    // IRCS    Internal Reference Clock Select

         //! Control Register 4
         MCG_C4_DMX32(0)     | // DMX32    DCO lock range
         MCG_C4_DRST_DRS(0),   // DRST_DRS DCO Range Select

         //! Control Register 5
         MCG_C5_PLLCLKEN0(0)  | // PLLCLKEN0 PLL Clock Enable
         MCG_C5_PLLSTEN0(0)   | // PLLSTEN0  PLL Stop Enable
         MCG_C5_PRDIV0(0),     // PRDIV0    PLL External Reference Divider

         //! Control Register 6 - Excluding PLLS
         MCG_C6_LOLIE0(0) | // LOLIE0 Loss of Lock interrupt Enable
         MCG_C6_CME0(0)   | // CME0   Clock Monitor Enable
         MCG_C6_VDIV0(0),   // VDIV0  PLL VCO Divider

         //! Status and Control Register
         MCG_SC_FCRDIV(0), // FCRDIV Internal Clock Reference Divider

         //! Control Register 7
         MCG_C7_OSCSEL(2), // OSCSEL MCG OSC Clock Select

         //! Control Register 8
         MCG_C8_LOCRE1(0) | // LOCRE1 RTC Loss of Clock Reset Enable
         MCG_C8_LOLRE(0)  | // LOLRE  PLL Loss of Lock Reset Enable
         MCG_C8_CME1(0),    // CME1   Clock Monitor Enable 1
      },
      {  // ClockConfig_PEE_120MHz (ClockMode_PEE)
         
         //! SIM CLKDIV1 System Clock Divider Register 1
         SIM_CLKDIV1_OUTDIV4(4)|  // Flash clock
         SIM_CLKDIV1_OUTDIV3(4)|  // FlexBus clock
         SIM_CLKDIV1_OUTDIV2(3)|  // Bus clock
         SIM_CLKDIV1_OUTDIV1(0),  // Core/system clock

   #ifdef SIM_CLKDIV3_PLLFLLDIV
         //! SIM CLKDIV3 System Clock Divider Register 3
         0,
   #endif

         //! SIM SOPT2 System Options Register 2 (PLLFLLSEL only)
         SIM_SOPT2_PLLFLLSEL(3),      // PLL/FLL clock select

         //! Clock Mode
         McgInfo::ClockMode_PEE,

         //! Control Register 1 - Excluding CLKS, IREFS
         MCG_C1_FRDIV(6)   | // FRDIV    FLL External Reference Divider
         MCG_C1_IRCLKEN(1) | // IRCLEN   Internal Reference Clock Enable
         MCG_C1_IREFSTEN(0), // IREFSTEN Internal Reference Stop Enable

         //! Control Register 2 - Excluding LP
         MCG_C2_LOCRE0(0) | // LOCRE0  Loss of Clock Reset Enable
         MCG_C2_RANGE0(1) | // RANGE   Frequency Range Select
         MCG_C2_HGO0(0)   | // HGO     High Gain Oscillator Select
         MCG_C2_EREFS0(0) | // EREFS   External Reference Select
         MCG_C2_IRCS(1),    // IRCS    Internal Reference Clock Select

         //! Control Register 4
         MCG_C4_DMX32(0)     | // DMX32    DCO lock range
         MCG_C4_DRST_DRS(0),   // DRST_DRS DCO Range Select

         //! Control Register 5
         MCG_C5_PLLCLKEN0(0)  | // PLLCLKEN0 PLL Clock Enable
         MCG_C5_PLLSTEN0(0)   | // PLLSTEN0  PLL Stop Enable
         MCG_C5_PRDIV0(11),     // PRDIV0    PLL External Reference Divider

         //! Control Register 6 - Excluding PLLS
         MCG_C6_LOLIE0(0) | // LOLIE0 Loss of Lock interrupt Enable
         MCG_C6_CME0(0)   | // CME0   Clock Monitor Enable
         MCG_C6_VDIV0(6),   // VDIV0  PLL VCO Divider

         //! Status and Control Register
         MCG_SC_FCRDIV(0), // FCRDIV Internal Clock Reference Divider

         //! Control Register 7
         MCG_C7_OSCSEL(2), // OSCSEL MCG OSC Clock Select

         //! Control Register 8
         MCG_C8_LOCRE1(0) | // LOCRE1 RTC Loss of Clock Reset Enable
         MCG_C8_LOLRE(0)  | // LOLRE  PLL Loss of Lock Reset Enable
         MCG_C8_CME1(0),    // CME1   Clock Monitor Enable 1
      },

   };

/** MCGFFCLK - Fixed frequency clock (input to FLL) */
volatile uint32_t SystemMcgffClock;

/** MCGOUTCLK - Primary output from MCG, various sources */
volatile uint32_t SystemMcgOutClock;

/** MCGFLLCLK - Output of FLL */
volatile uint32_t SystemMcgFllClock;

/** MCGPLLCLK - Output of PLL */
volatile uint32_t SystemMcgPllClock;

/** Core/System clock (from MCGOUTCLK/CLKDIV) */
//volatile uint32_t SystemCoreClock;

/** Bus clock (from MCGOUTCLK/CLKDIV) */
//volatile uint32_t SystemBusClock;

/** LPO - Low power oscillator 1kHz clock available in LP modes */
volatile uint32_t SystemLpoClock;

typedef void (*set_sys_dividers_asm_t)(uint32_t simClkDiv1);

/**
 *  Change SIM->CLKDIV1 value
 *
 *  @param simClkDiv1 - new SIM->CLKDIV1 value
 *
 *  @note This routine must be copied to RAM. It is a workaround for errata e2448.
 *  Flash pre-fetch must be disabled when the flash clock divider is changed.
 *  This cannot be performed while executing out of flash.
 *  There must be a short delay after the clock dividers are changed before prefetch
 *  can be re-enabled.
 *
 *  @note This routine must be placed in ROM immediately before setSysDividersStub()
 */
static void setSysDividers_asm(uint32_t simClkDiv1) {
   (void) simClkDiv1;
   __asm__ volatile (
       "    .equ   FMC_PFAPR_VALUE,0xffFF0000   \n"
       "    .equ   FMC_PFAPR_ADDR,0x4001F000    \n"
       "    .equ   SIM_CLKDIV1_ADDR,0x40048044  \n"
       "     movw  r1,#FMC_PFAPR_ADDR&0xFFFF    \n" // Point R1 @FMC_PFAPR
       "     movt  r1,#FMC_PFAPR_ADDR/65536     \n"
       "     ldr   r2,[r1,#0]                   \n" // R2 = original FMC_PFAPR
       "     movw  r3,#FMC_PFAPR_VALUE&0xFFFF   \n" // R3 = mask for new FMC_PFAPR
       "     movt  r3,#FMC_PFAPR_VALUE>>16      \n"
       "     orr   r3,r3,r2                     \n" // Merge with existing (Disable Flash pre-fetch)
       "     str   r3,[r1,#0]                   \n" // Write back to FMC_PFAPR

       "     movw  r0,#SIM_CLKDIV1_ADDR&0xFFFF  \n" // Point R0 @SIM->CLKDIV1
       "     movt  r0,#SIM_CLKDIV1_ADDR/65536   \n"
       "     str   %[value],[r0,#0]             \n" // SIM->CLKDIV1 = simClkDiv
       "     movw  r3,#100                      \n" // Wait a while
       "loop:                                   \n"
       "     subs  r3,r3,#1                     \n"
       "     bhi   loop                         \n"
       "                                        \n"
       "     str   r2,[r1,#0]                   \n" // Restore original FMC_PFAPR
         :: [value] "r" (simClkDiv1) : "r0", "r1", "r2", "r3"
      );
}

/**
 *  Change SIM->CLKDIV1 value
 *
 * @param simClkDiv1 - Value to write to SIM->CLKDIV1 register
 *
 * @note This routine must be placed in ROM immediately following setSysDividers_asm()
 */
void setSysDividersStub(uint32_t simClkDiv1) {
   uint8_t space[100]; // Space for RAM copy of setSysDividers_asm()
   set_sys_dividers_asm_t fp = (set_sys_dividers_asm_t)((uint32_t)space|1);

   // Copy routine to RAM (stack)
   memcpy(space, (void*)((uint32_t)setSysDividers_asm&~1), ((uint32_t)setSysDividersStub)-((uint32_t)setSysDividers_asm));

   // Call the function on the stack
   (*fp)(simClkDiv1);
}

/** Callback for programmatically set handler */
MCGCallbackFunction Mcg::callback = {0};

/** Current clock mode (FEI out of reset) */
McgInfo::ClockMode Mcg::currentClockMode = McgInfo::ClockMode::ClockMode_FEI;

constexpr uint8_t clockTransitionTable[8][8] = {
         /*  from                 to =>   ClockMode_FEI,           ClockMode_FEE,           ClockMode_FBI,           ClockMode_BLPI,          ClockMode_FBE,           ClockMode_BLPE,          ClockMode_PBE,           ClockMode_PEE */
         /* ClockMode_FEI,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_FEE,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_FBI,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FEI,  McgInfo::ClockMode_BLPI, McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE, },
         /* ClockMode_BLPI, */ { McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI, },
         /* ClockMode_FBE,  */ { McgInfo::ClockMode_FEI,  McgInfo::ClockMode_FEE,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FBI,  McgInfo::ClockMode_FEI,  McgInfo::ClockMode_BLPE, McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE, },
         /* ClockMode_BLPE, */ { McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE, },
         /* ClockMode_PBE,  */ { McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_FBE,  McgInfo::ClockMode_BLPE, McgInfo::ClockMode_FBE,  McgInfo::ClockMode_PEE, },
         /* ClockMode_PEE,  */ { McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE,  McgInfo::ClockMode_PBE, },
   };

/**
 * Get name for clock mode
 *
 * @return Pointer to static string
 */
const char *Mcg::getClockModeName(McgInfo::ClockMode clockMode) {
   static const char *modeNames[] {
         "FEI",
         "FEE",
         "FBI",
         "BLPI",
         "FBE",
         "BLPE",
         "PBE",
         "PEE",
   };

   if (clockMode<0) {
      return "Not set";
   }
   if ((unsigned)clockMode>=(sizeof(modeNames)/sizeof(modeNames[0]))) {
      return "Illegal";
   }
   return modeNames[clockMode];
}

/**
 * Transition from current clock mode to mode given
 *
 * @param to Clock mode to transition to
 *
 * @return E_NO_ERROR on success
 */
ErrorCode Mcg::clockTransition(const McgInfo::ClockInfo &clockInfo) {
   McgInfo::ClockMode to = clockInfo.clockMode;

   //TODO move!
#ifdef USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK
   if (clockInfo.c7&&MCG_C7_OSCSEL_MASK) {
      // Note IRC48M Internal Oscillator automatically enable if MCG_C7_OSCSEL = 2
      SIM->SCGC4 |= SIM_SCGC4_USBOTG_MASK;
      USB0->CLK_RECOVER_IRC_EN = USB_CLK_RECOVER_IRC_EN_IRC_EN_MASK|USB_CLK_RECOVER_IRC_EN_REG_EN_MASK;
   }
#endif

   // Set conservative clock dividers
   setSysDividers(SIM_CLKDIV1_OUTDIV4(5)|SIM_CLKDIV1_OUTDIV3(5)|SIM_CLKDIV1_OUTDIV2(5)|SIM_CLKDIV1_OUTDIV1(5));

#ifdef MCG_C7_OSCSEL
   // Select OSCCLK Source
   mcg().C7 = clockInfo.c7; // OSCSEL = 0,1,2 -> XTAL/XTAL32/IRC48M
#endif

   // Set Fast Internal Clock divider
   mcg().SC = clockInfo.sc;

   // Disable clock monitors while clocks change
   mcg().C8 = 0;

   McgInfo::ClockMode next;
   if (currentClockMode == McgInfo::ClockMode_None) {
      next = McgInfo::ClockMode_FEI;
   }
   else {
      // Initial destination chosen to ensure that set-up occurs
      next = (McgInfo::ClockMode)clockTransitionTable[currentClockMode][currentClockMode];
   }
   if (to != McgInfo::ClockMode_None) {
      int transitionCount = 0;
      do {
         // Used to indicate that clock stabilisation wait is needed
         bool externalClockInUse = false;

         switch (next) {

         case McgInfo::ClockMode_None:
         case McgInfo::ClockMode_FEI: // From FEE, FBI, FBE or reset(FEI)

            // Select OSCCLK Source (for exit to FEE,FBE)
            mcg().C7 = clockInfo.c7; // OSCSEL = 0,1,2 -> XTAL/XTAL32/IRC48M

            // EREFS0 (for exit to FEE,FBE)
            mcg().C2 = clockInfo.c2|MCG_C2_LP(0);

            mcg().C1 =
                  MCG_C1_CLKS(0)   | // CLKS     = 0     -> MCGOUTCLK = Output of FLL/PLL (depends on mcg_c6.PLLS)
                  MCG_C1_IREFS(1)  | // IREFS    = 1     -> FLL source = Slow IRC
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            // Set FLL Parameters
            mcg().C4 = (mcg().C4&(MCG_C4_FCTRIM_MASK|MCG_C4_SCFTRIM_MASK))|clockInfo.c4;

            // Wait for S_IREFST to indicate FLL Reference has switched to IRC
            // Wait for S_CLKST to indicating that OUTCLK has switched to FLL
            do {
               __asm__("nop");
            } while ((mcg().S & (MCG_S_CLKST_MASK|MCG_S_IREFST_MASK)) != (MCG_S_CLKST(0)|MCG_S_IREFST(1)));
            break;

         case McgInfo::ClockMode_FEE: // from FEI, FBI or FBE
            mcg().C1 =
                  MCG_C1_CLKS(0)   | // CLKS     = 0     -> MCGOUTCLK = Output of FLL/PLL (depends on mcg_c6.PLLS)
                  MCG_C1_IREFS(0)  | // IREFS    = 0     -> FLL source = External reference clock
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            // Wait for S_CLKST to indicating that MCGOUTCLK has switched to FLL
            // Wait for S_IREFST to indicate FLL Reference has switched to ERC
            do {
               __asm__("nop");
            } while ((mcg().S & (MCG_S_CLKST_MASK|MCG_S_IREFST_MASK)) != (MCG_S_CLKST(0)|MCG_S_IREFST(0)));

            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_FBI: // from BLPI, FEI, FEE, FBE

            // Select OSCCLK Source (for exit to FEE,FBE)
            mcg().C7 = clockInfo.c7; // OSCSEL = 0,1,2 -> XTAL/XTAL32/IRC48M

            // Clear LP
            mcg().C2 = clockInfo.c2|MCG_C2_LP(0);

            mcg().C1 =
                  MCG_C1_CLKS(1)   | // CLKS     = 1     -> MCGOUTCLK = Internal reference clock is selected
                  MCG_C1_IREFS(1)  | // IREFS    = 1     -> FLL source = Slow IRC
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            // Set FLL Parameters
            mcg().C4 = (mcg().C4&(MCG_C4_FCTRIM_MASK|MCG_C4_SCFTRIM_MASK))|clockInfo.c4;

            // Wait for S_CLKST to indicating that MCGOUTCLK has switched to IRC
            // Wait for S_IREFST to indicate FLL Reference has switched to IRC
            do {
               __asm__("nop");
            } while ((mcg().S & (MCG_S_CLKST_MASK|MCG_S_IREFST_MASK)) != (MCG_S_CLKST(1)|MCG_S_IREFST(1)));
            break;

         case McgInfo::ClockMode_FBE: // from FEI, FEE, FBI, PBE, BLPE
            // Clear LP
            mcg().C2 = clockInfo.c2|MCG_C2_LP(0);

            mcg().C1 =
                  MCG_C1_CLKS(2)   | // CLKS     = 2     -> MCGOUTCLK = External reference clock
                  MCG_C1_IREFS(0)  | // IREFS    = 0     -> FLL source = External reference clock
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            // Select FLL as MCG clock source
            mcg().C6  = clockInfo.c6;

            // Wait for S_CLKST to indicating that MCGOUTCLK has switched to ERC
            // Wait for S_IREFST to indicate FLL Reference has switched to ERC
            do {
               __asm__("nop");
            } while ((mcg().S & (MCG_S_CLKST_MASK|MCG_S_IREFST_MASK)) != (MCG_S_CLKST(2)|MCG_S_IREFST(0)));

            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_PBE: // from FBE, BLPE, PEE
            // Clear LP
            mcg().C2 = clockInfo.c2|MCG_C2_LP(0);
            mcg().C5 = clockInfo.c5;
            // Select PLL as MCG clock source and set VDIV0
            mcg().C6 = clockInfo.c6|MCG_C6_PLLS_MASK;

            mcg().C1 =
                  MCG_C1_CLKS(2)   | // CLKS     = 2     -> MCGOUTCLK = External reference clock
                  MCG_C1_IREFS(0)  | // IREFS    = 1     -> FLL source = External reference clock
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            externalClockInUse = true;
            while ((mcg().S&MCG_S_PLLST_MASK) == 0) {
               __asm__("nop");
            }
            break;

         case McgInfo::ClockMode_PEE: // from PBE
            mcg().C1 =
                  MCG_C1_CLKS(0)   | // CLKS     = 0     -> MCGOUTCLK = Output of FLL/PLL (depends on mcg_c6.PLLS)
                  MCG_C1_IREFS(0)  | // IREFS    = 0     -> FLL source = External reference clock
                  clockInfo.c1;      // FRDIV, IRCLKEN, IREFSTEN

            externalClockInUse = true;
            break;

         case McgInfo::ClockMode_BLPE: // from FBE, PBE (registers differ depending on transition)
            externalClockInUse = true;
            // Fall through - no break

         case McgInfo::ClockMode_BLPI: // from FBI
            // Set LP
            mcg().C2 = clockInfo.c2|MCG_C2_LP(1);
            break;
         }
         // Wait for oscillator stable (if used)
         if (externalClockInUse && (clockInfo.c2&MCG_C2_EREFS0_MASK)) {
            do {
               __asm__("nop");
            } while ((mcg().S & MCG_S_OSCINIT0_MASK) == 0);
         }
         currentClockMode = next;
         next = (McgInfo::ClockMode)clockTransitionTable[currentClockMode][to];
         if (transitionCount++>5) {
            return setErrorCode(E_CLOCK_INIT_FAILED);
         }
      } while (currentClockMode != to);
   }

   // Main clock dividers
   setSysDividers(clockInfo.clkdiv1);

#ifdef SIM_CLKDIV3_PLLFLLDIV
   // Peripheral clock divider
   SimInfo::setPeripheralClockDivider((SimPeripheralClockDivider)clockInfo.clkdiv3);
#endif

   // Clock sources
   SIM->SOPT2 = (SIM->SOPT2&~SIM_SOPT2_PLLFLLSEL_MASK)|clockInfo.sopt2;

   SystemCoreClockUpdate();

   mcg().C8 = clockInfo.c8; // Enable clock monitors

   return E_NO_ERROR;
}

/**
 * Update SystemCoreClock variable
 *
 * Updates the SystemCoreClock variable with current core Clock retrieved from CPU registers.
 */
void Mcg::SystemCoreClockUpdate(void) {

   uint32_t mcg_erc_clock = McgInfo::getErcClock();

   if ((mcg().C1&MCG_C1_IREFS_MASK) == 0) {
      // External reference clock is selected
      SystemMcgffClock = mcg_erc_clock/(1<<((mcg().C1&MCG_C1_FRDIV_MASK)>>MCG_C1_FRDIV_SHIFT));

      if (((mcg().C2&MCG_C2_RANGE0_MASK) != 0) &&
	     ((mcg().C7&MCG_C7_OSCSEL_MASK) !=  1)) {
         // High divisors - extra division
         if ((mcg().C1&MCG_C1_FRDIV_MASK) == MCG_C1_FRDIV(6)) {
            SystemMcgffClock /= 20;
         }
         else if ((mcg().C1&MCG_C1_FRDIV_MASK) == MCG_C1_FRDIV(7)) {
            SystemMcgffClock /= 12;
         }
         else {
            SystemMcgffClock /= 32;
         }
      }
   }
   else {
      // External reference clock is selected
      SystemMcgffClock = McgInfo::system_slow_irc_clock;
   }

   uint32_t systemFllClock = SystemMcgffClock * ((mcg().C4&MCG_C4_DMX32_MASK)?732:640) * (((mcg().C4&MCG_C4_DRST_DRS_MASK)>>MCG_C4_DRST_DRS_SHIFT)+1);

   uint32_t systemPllClock = 0;
   systemPllClock  = (mcg_erc_clock/10)*(((mcg().C6&MCG_C6_VDIV0_MASK)>>MCG_C6_VDIV0_SHIFT)+McgInfo::pll_vdiv_min);
   systemPllClock /= ((mcg().C5&MCG_C5_PRDIV0_MASK)>>MCG_C5_PRDIV0_SHIFT)+1;
   systemPllClock *= (10/McgInfo::pll_post_divider);

   SystemMcgPllClock = 0;
   SystemMcgFllClock = 0;
   switch (mcg().S&MCG_S_CLKST_MASK) {
      case MCG_S_CLKST(0) : // FLL
         SystemMcgOutClock = systemFllClock;
         SystemMcgFllClock = systemFllClock;
         break;
      case MCG_S_CLKST(1) : // Internal Reference Clock
         SystemMcgOutClock = McgInfo::getInternalIrcClock();
         break;
      case MCG_S_CLKST(2) : // External Reference Clock
         SystemMcgOutClock = McgInfo::getErcClock();
         break;
      case MCG_S_CLKST(3) : // PLL
         SystemMcgOutClock = systemPllClock;
         SystemMcgPllClock = systemPllClock;
         break;
   }
   if (mcg().C5&MCG_C5_PLLCLKEN0_MASK) {
      SystemMcgPllClock = systemPllClock;
   }
   ::SystemCoreClock   = SystemMcgOutClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV1_MASK)>>SIM_CLKDIV1_OUTDIV1_SHIFT)+1);
   ::SystemBusClock    = SystemMcgOutClock/(((SIM->CLKDIV1&SIM_CLKDIV1_OUTDIV2_MASK)>>SIM_CLKDIV1_OUTDIV2_SHIFT)+1);

   SystemLpoClock    = 1000;
}

/**
 * Initialise MCG to default settings.
 */
void Mcg::defaultConfigure() {

   currentClockMode = McgInfo::ClockMode::ClockMode_None;

   // Transition to desired clock mode
   clockTransition(clockInfo[ClockConfig_default]);

   SimInfo::initRegs();

   enableNvicInterrupts();

   SystemCoreClockUpdate();
}

} // end namespace USBDM

/**
 * Sets up the clock out of RESET
 */
extern "C"
void clock_initialise(void) {

#ifdef USBDM_OSC0_IS_DEFINED
   USBDM::Osc0::initialise();
#endif

#ifdef USBDM_RTC_IS_DEFINED
   USBDM::Rtc::initialise();
#endif

#ifdef USBDM_MCG_IS_DEFINED
   USBDM::Mcg::initialise();
#endif

}

