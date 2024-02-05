/**
    @file usb.cpp
    @brief Simple USB Stack for Kinetis

    @version  V4.12.1.150
    @date     13 Nov 2016

\verbatim
    Kinetis USB Code

    Copyright (C) 2008-24  Peter O'Donoghue

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim

*/
/*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include <string.h>
#include "derivative.h"
#include "usb.h"
#include "stringFormatter.h"

namespace USBDM {

/** BDTs organised by endpoint, odd/even, tx/rx */
volatile EndpointBdtEntry endPointBdts[Usb0::NUMBER_OF_ENDPOINTS] __attribute__ ((aligned (512)));

/** End-points in use */
Endpoint *UsbBase::fEndPoints[UsbImplementation::NUMBER_OF_ENDPOINTS];

#ifdef MS_COMPATIBLE_ID_FEATURE

const uint8_t UsbBase::fMsOsStringDescriptor[] = {
      18, DT_STRING, 'M',0,'S',0,'F',0,'T',0,'1',0,'0',0,'0',0,GET_MS_FEATURE_DESCRIPTOR,0x00
};

// See https://github.com/pbatard/libwdi/wiki/WCID-Devices
//
const MS_CompatibleIdFeatureDescriptor msCompatibleIdFeatureDescriptor = {
      /* lLength;             */  nativeToLe32((uint32_t)sizeof(MS_CompatibleIdFeatureDescriptor)),
      /* wVersion;            */  nativeToLe16((uint16_t)0x0100),
      /* wIndex;              */  nativeToLe16((uint16_t)0x0004),
      /* bnumSections;        */  1,
      /* bReserved1[7];       */  {0},
      /*---------------------- Section 1 -----------------------------*/
      /* bInterfaceNum;       */  0,
      /* bReserved2;          */  1,
      /* bCompatibleId[8];    */  {'W','I','N','U','S','B','\0','\0'},
      /* bSubCompatibleId[8]; */  {0},
      /* bReserved3[6];       */  {0}
};

const MS_PropertiesFeatureDescriptor msPropertiesFeatureDescriptor = {
      /* uint32_t lLength;         */ (uint32_t)nativeToLe32((uint32_t)sizeof(MS_PropertiesFeatureDescriptor)),
      /* uint16_t wVersion;        */ (uint16_t)nativeToLe16(0x0100),
      /* uint16_t wIndex;          */ (uint16_t)nativeToLe16(5),
      /* uint16_t bnumSections;    */ (uint16_t)nativeToLe16(2),
      /*---------------------- Section 1 -----------------------------*/
      /* uint32_t lPropertySize0;  */ (uint32_t)nativeToLe32(
            sizeof(msPropertiesFeatureDescriptor.lPropertySize0)+
            sizeof(msPropertiesFeatureDescriptor.ldataType0)+
            sizeof(msPropertiesFeatureDescriptor.wNameLength0)+
            sizeof(msPropertiesFeatureDescriptor.bName0)+
            sizeof(msPropertiesFeatureDescriptor.wPropertyLength0)+
            sizeof(msPropertiesFeatureDescriptor.bData0)
      ),
      /* uint32_t ldataType0;       */ (uint32_t)nativeToLe32(7UL), // 7 == REG_MULTI_SZ
      /* uint16_t wNameLength0;     */ (uint16_t)nativeToLe16(sizeof(msPropertiesFeatureDescriptor.bName0)),
      /* char16_t  bName0[42];      */ MS_DEVICE_INTERFACE_GUIDs,
      /* uint32_t wPropertyLength0; */ (uint32_t)nativeToLe32(sizeof(msPropertiesFeatureDescriptor.bData0)),
      /* char16_t  bData0[78];      */ MS_DEVICE_GUID,
      /*---------------------- Section 2 -----------------------------*/
      /* uint32_t lPropertySize1;   */ (uint32_t)nativeToLe32(
            sizeof(msPropertiesFeatureDescriptor.lPropertySize1)+
            sizeof(msPropertiesFeatureDescriptor.ldataType1)+
            sizeof(msPropertiesFeatureDescriptor.wNameLength1)+
            sizeof(msPropertiesFeatureDescriptor.bName1)+
            sizeof(msPropertiesFeatureDescriptor.wPropertyLength1)+
            sizeof(msPropertiesFeatureDescriptor.bData1)
      ),
      /* uint32_t ldataType1;       */ (uint32_t)nativeToLe32(7UL), // 7 == REG_MULTI_SZ
      /* uint16_t wNameLength1;     */ (uint16_t)nativeToLe16(sizeof(msPropertiesFeatureDescriptor.bName1)),
      /* uint8_t  bName1[];         */ MS_ICONS,
      /* uint32_t wPropertyLength1; */ (uint32_t)nativeToLe32(sizeof(msPropertiesFeatureDescriptor.bData1)),
      /* uint8_t  bData1[];         */ MS_ICON_PATH,
};

#endif

/**
 * Get name of USB token
 *
 * @param[in]  token USB token
 *
 * @return Pointer to static string
 */
const char *UsbBase::getTokenName(unsigned token) {
   static const char *names[] = {
         "Unknown #0",
         "OUTToken",   //  (0x1) - Out token
         "ACKToken",   //  (0x2) - Acknowledge
         "DATA0Token", //  (0x3) - Data 0
         "Unknown #4",
         "SOFToken",   //  (0x5) - Start of Frame token
         "NYETToken",  //  (0x6) - No Response Yet
         "DATA2Token", //  (0x7) - Data 2
         "Unknown #8",
         "INToken",    //  (0x9) - In token
         "NAKToken",   //  (0xA) - Negative Acknowledge
         "DATA1Token", //  (0xB) - Data 1
         "PREToken",   //  (0xC) - Preamble
         "SETUPToken", //  (0xD) - Setup token
         "STALLToken", //  (0xE) - Stall
         "MDATAToken", //  (0xF) - M data
   };
   const char *rc = "Unknown";
   if (token<(sizeof(names)/sizeof(names[0]))) {
      rc = names[token];
   }
   return rc;
}
/**
 * Get name of USB end-point state
 *
 * @param[in]  state End-point state
 *
 * @return Pointer to static string
 */
const char *UsbBase::getStateName(EndpointState state) {
   return Endpoint::getStateName(state);
}

/**
 * Get name of User event
 *
 * @param[in]  userEvent
 *
 * @return Pointer to static string
 */
const char *UsbBase::getUserEventName(UserEvent userEvent) {
   static const char *names[] = {
         "Suspend",   //!< USB has been suspended
         "Resume",    //!< USB has been resumed
         "Reset",     //!< USB has been reset
         "Configure", //!< USB has been configured
   };
   const char *rc = "Unknown";
   if (userEvent<(sizeof(names)/sizeof(names[0]))) {
      rc = names[userEvent];
   }
   return rc;
}


/**
 * Get name of USB request
 *
 * @param[in]  reqType Request type
 *
 * @return Pointer to static string
 */
const char *UsbBase::getRequestName(uint8_t reqType){
   static const char *names[] = {
         "GET_STATUS",              /* 0x00 */
         "CLEAR_FEATURE",           /* 0x01 */
         "Unknown #2",
         "SET_FEATURE",             /* 0x03 */
         "Unknown #4",
         "SET_ADDRESS",             /* 0x05 */
         "GET_DESCRIPTOR",          /* 0x06 */
         "SET_DESCRIPTOR",          /* 0x07 */
         "GET_CONFIGURATION",       /* 0x08 */
         "SET_CONFIGURATION",       /* 0x09 */
         "GET_INTERFACE",           /* 0x0a */
         "SET_INTERFACE",           /* 0x0b */
         "SYNCH_FRAME",             /* 0x0c */
         "Unknown #D",
         "Unknown #E",
         "Unknown #F",
   };
   const char *rc = "Unknown";
   if (reqType<(sizeof(names)/sizeof(names[0]))) {
      rc = names[reqType];
   }
   return rc;
}

/**
 * Report contents of BDT
 *
 * @param[in] name    Descriptive name to use
 * @param[in] bdt     BDT to report
 */
void UsbBase::reportBdt(const char *name, BdtEntry *bdt) {
   (void)name;
   (void)bdt;
#if defined(DEBUG_BUILD) && USE_CONSOLE
   if (bdt->own) {
      console.writeln(name,
            " addr=0x", bdt->addr, Radix_16, ", bc=", bdt->bc, ", ",
            bdt->data0_1?"DATA1":"DATA0", ", ", bdt->setup.bdt_stall?"STALL":"OK", "USB");
   }
   else {
      console.writeln(name,
            " addr=0x", bdt->addr, Radix_16, ", bc=", bdt->bc, ", ",
            getTokenName(bdt->result.tok_pid), "PROC");
   }
#endif
}

/**
 * Report line code structure values
 *
 * @param[in] lineCodingStructure
 */
void UsbBase::reportLineCoding(const LineCodingStructure *lineCodingStructure) {
   (void)lineCodingStructure;
#if defined(DEBUG_BUILD) && USE_CONSOLE
   console.writeln("rate   = ", lineCodingStructure->dwDTERate);
   console.writeln("format = ", lineCodingStructure->bCharFormat);
   console.writeln("parity = ", lineCodingStructure->bParityType);
   console.writeln("bits   = ", lineCodingStructure->bDataBits);
#endif
}

/**
 * Format SETUP packet as string
 *
 * @param[in]  p SETUP packet
 *
 * @return Pointer to static buffer
 */
const char *UsbBase::getSetupPacketDescription(SetupPacket *p) {
#ifdef DEBUG_BUILD
   static StringFormatter_T<100> sf;
   sf.clear().
         write("[",p->bmRequestType, Radix_2, ",",
               getRequestName(p->bRequest), ",",
               p->wValue, Radix_16, ",",
               p->wIndex, ",",
               p->wLength).write("]");
   return sf.toString();
#else
   (void)p;
   return "";
#endif
}

/**
 * Report line state value to stdout
 *
 * @param[in] value
 */
void UsbBase::reportLineState(uint8_t value) {
   (void)value;
#if defined(DEBUG_BUILD) && USE_CONSOLE
   console.
      writeln("Line state: RTS=", (value&(1<<1))?1:0, ", DTR=", (value&(1<<0))?1:0);
#endif
}

/**
 *  Creates a valid string descriptor in UTF-16-LE from a limited UTF-8 string
 *
 *  @param[in] to       Where to place descriptor
 *  @param[in] from     Zero terminated UTF-8 C string
 *  @param[in] maxSize  Size of destination
 *
 *  @note Only handles UTF-8 characters that fit in a single UTF-16 value.
 */
void UsbBase::utf8ToStringDescriptor(volatile uint8_t *to, volatile const uint8_t *from, unsigned maxSize) {
   volatile uint8_t *size = to; // 1st byte is where to place descriptor size

   *to++ = 2;         // 1st byte = descriptor size (2 bytes so far including DT_STRING)
   *to++ = DT_STRING; // 2nd byte = descriptor type, DT_STRING;

   while (*from != '\0') {
      // Buffer for converted character
      uint16_t utf16Char=0;

      // Update size
      *size = *size + 2;
      if (*from < 0x80) {
         // 1-byte UTF-8
         utf16Char = *from++;
      }
      else if ((*from &0xE0) == 0xC0){
         // 2-byte UTF-8
         utf16Char  = (0x1F&*from++)<<6;
         utf16Char += (0x3F&*from++);
      }
      else if ((*from &0xF0) == 0xE0){
         // 3-byte UTF-8
         utf16Char  = (0x0F&*from++)<<12;
         utf16Char += (0x3F&*from++)<<6;
         utf16Char += (0x3F&*from++);
      }
      // Write UTF-16LE value
      *to++ = (char)utf16Char;
      *to++ = (char)(utf16Char>>8);
      if (*size>=maxSize) {
         // Truncate value
         break;
      }
   }
}

} // End namespace USBDM
