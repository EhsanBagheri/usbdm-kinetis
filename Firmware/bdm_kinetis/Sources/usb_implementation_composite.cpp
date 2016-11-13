/**
 * @file     usb_implementation_composite.cpp
 * @brief    USB Kinetis implementation
 *
 * @version  V4.12.1.150
 * @date     13 Nov 2016
 *
 *  This file provides the implementation specific code for the USB interface.
 *  It will need to be modified to suit an application.
 */
#include <string.h>

#include "usb.h"
#include "usb_cdc_uart.h"

namespace USBDM {

/**
 * Interface numbers for USB descriptors
 */
enum InterfaceNumbers {
   /** Interface number for BDM channel */
   BULK_INTF_ID,

   /** Interface number for CDC Control channel */
   CDC_COMM_INTF_ID,
   /** Interface number for CDC Data channel */
   CDC_DATA_INTF_ID,
   /*
    * TODO Add additional Interface numbers here
    */
   /** Total number of interfaces */
   NUMBER_OF_INTERFACES,
};

/** Force command handler to exit and restart */
bool Usb0::forceCommandHandlerInitialise = false;

/*
 * String descriptors
 */
static const uint8_t s_language[]        = {4, DT_STRING, 0x09, 0x0C};  //!< Language IDs
static const uint8_t s_manufacturer[]    = MANUFACTURER;                //!< Manufacturer
static const uint8_t s_product[]         = PRODUCT_DESCRIPTION;         //!< Product Description
static const uint8_t s_serial[]          = SERIAL_NO;                   //!< Serial Number
static const uint8_t s_config[]          = "Default configuration";     //!< Configuration name

static const uint8_t s_bulk_interface[]  = "Bulk Interface";           //!< Bulk Interface

static const uint8_t s_cdc_interface[]   = "CDC Interface";             //!< Interface Association #2
static const uint8_t s_cdc_control[]     = "CDC Control Interface";     //!< CDC Control Interface
static const uint8_t s_cdc_data[]        = "CDC Data Interface";        //!< CDC Data Interface
/*
 * Add additional String descriptors here
 */

/**
 * String descriptor table
 */
const uint8_t *const Usb0::stringDescriptors[] = {
      s_language,
      s_manufacturer,
      s_product,
      s_serial,
      s_config,

      s_bulk_interface,
      
      s_cdc_interface,
      s_cdc_control,
      s_cdc_data
      /*
       * Add additional String descriptors here
       */
};

/**
 * Device Descriptor
 */
const DeviceDescriptor Usb0::deviceDescriptor = {
      /* bLength             */ sizeof(DeviceDescriptor),
      /* bDescriptorType     */ DT_DEVICE,
      /* bcdUSB              */ nativeToLe16(0x0200),           // USB specification release No. [BCD = 2.00]
      /* bDeviceClass        */ 0xEF,                    // Device Class code [Miscellaneous Device Class]
      /* bDeviceSubClass     */ 0x02,                    // Sub Class code    [Common Class]
      /* bDeviceProtocol     */ 0x01,                    // Protocol          [Interface Association Descriptor]
      /* bMaxPacketSize0     */ CONTROL_EP_MAXSIZE,             // EndPt 0 max packet size
      /* idVendor            */ nativeToLe16(VENDOR_ID),        // Vendor ID
      /* idProduct           */ nativeToLe16(PRODUCT_ID),       // Product ID
      /* bcdDevice           */ nativeToLe16(VERSION_ID),       // Device Release    [BCD = 4.10]
      /* iManufacturer       */ s_manufacturer_index,           // String index of Manufacturer name
      /* iProduct            */ s_product_index,                // String index of product description
      /* iSerialNumber       */ s_serial_index,                 // String index of serial number
      /* bNumConfigurations  */ NUMBER_OF_CONFIGURATIONS        // Number of configurations
};

/**
 * All other descriptors
 */
const Usb0::Descriptors Usb0::otherDescriptors = {
      { // configDescriptor
            /* bLength                 */ sizeof(ConfigurationDescriptor),
            /* bDescriptorType         */ DT_CONFIGURATION,
            /* wTotalLength            */ nativeToLe16(sizeof(otherDescriptors)),
            /* bNumInterfaces          */ NUMBER_OF_INTERFACES,
            /* bConfigurationValue     */ CONFIGURATION_NUM,
            /* iConfiguration          */ s_config_index,
            /* bmAttributes            */ 0x80,     //  = Bus powered, no wake-up
            /* bMaxPower               */ USBMilliamps(500)
      },
      /**
       * Bulk interface, 2 end-points
       */
      { // bulk_interface
            /* bLength                 */ sizeof(InterfaceDescriptor),
            /* bDescriptorType         */ DT_INTERFACE,
            /* bInterfaceNumber        */ BULK_INTF_ID,
            /* bAlternateSetting       */ 0,
            /* bNumEndpoints           */ 2,
            /* bInterfaceClass         */ 0xFF,                         // (Vendor specific)
            /* bInterfaceSubClass      */ 0xFF,                         // (Vendor specific)
            /* bInterfaceProtocol      */ 0xFF,                         // (Vendor specific)
            /* iInterface desc         */ s_bulk_interface_index,
      },
      { // bulk_out_endpoint - OUT, Bulk
            /* bLength                 */ sizeof(EndpointDescriptor),
            /* bDescriptorType         */ DT_ENDPOINT,
            /* bEndpointAddress        */ EP_OUT|BULK_OUT_ENDPOINT,
            /* bmAttributes            */ ATTR_BULK,
            /* wMaxPacketSize          */ nativeToLe16(BULK_OUT_EP_MAXSIZE),
            /* bInterval               */ USBMilliseconds(1)
      },
      { // bulk_in_endpoint - IN, Bulk
            /* bLength                 */ sizeof(EndpointDescriptor),
            /* bDescriptorType         */ DT_ENDPOINT,
            /* bEndpointAddress        */ EP_IN|BULK_IN_ENDPOINT,
            /* bmAttributes            */ ATTR_BULK,
            /* wMaxPacketSize          */ nativeToLe16(BULK_IN_EP_MAXSIZE),
            /* bInterval               */ USBMilliseconds(1)
      },
      { // interfaceAssociationDescriptorCDC
            /* bLength                 */ sizeof(InterfaceAssociationDescriptor),
            /* bDescriptorType         */ DT_INTERFACEASSOCIATION,
            /* bFirstInterface         */ CDC_COMM_INTF_ID,
            /* bInterfaceCount         */ 2,
            /* bFunctionClass          */ 0x02,                                   //  CDC Control
            /* bFunctionSubClass       */ 0x02,                                   //  Abstract Control Model
            /* bFunctionProtocol       */ 0x01,                                   //  AT CommandL V.250
            /* iFunction = ""          */ s_cdc_interface_index,
      },
      /**
       * CDC Control/Communication Interface, 1 end-point
       */
      { // cdc_CCI_Interface
            /* bLength                 */ sizeof(InterfaceDescriptor),
            /* bDescriptorType         */ DT_INTERFACE,
            /* bInterfaceNumber        */ CDC_COMM_INTF_ID,
            /* bAlternateSetting       */ 0,
            /* bNumEndpoints           */ 1,
            /* bInterfaceClass         */ 0x02,      //  CDC Communication
            /* bInterfaceSubClass      */ 0x02,      //  Abstract Control Model
            /* bInterfaceProtocol      */ 0x01,      //  V.25ter, AT Command V.250
            /* iInterface description  */ s_cdc_control_interface_index
      },
      { // cdc_Functional_Header
            /* bFunctionalLength       */ sizeof(CDCHeaderFunctionalDescriptor),
            /* bDescriptorType         */ CS_INTERFACE,
            /* bDescriptorSubtype      */ DST_HEADER,
            /* bcdCDC                  */ nativeToLe16(0x0110),
      },
      { // cdc_CallManagement
            /* bFunctionalLength       */ sizeof(CDCCallManagementFunctionalDescriptor),
            /* bDescriptorType         */ CS_INTERFACE,
            /* bDescriptorSubtype      */ DST_CALL_MANAGEMENT,
            /* bmCapabilities          */ 1,
            /* bDataInterface          */ CDC_DATA_INTF_ID,
      },
      { // cdc_Functional_ACM
            /* bFunctionalLength       */ sizeof(CDCAbstractControlManagementDescriptor),
            /* bDescriptorType         */ CS_INTERFACE,
            /* bDescriptorSubtype      */ DST_ABSTRACT_CONTROL_MANAGEMENT,
            /* bmCapabilities          */ 0x06,
      },
      { // cdc_Functional_Union
            /* bFunctionalLength       */ sizeof(CDCUnionFunctionalDescriptor),
            /* bDescriptorType         */ CS_INTERFACE,
            /* bDescriptorSubtype      */ DST_UNION_MANAGEMENT,
            /* bmControlInterface      */ CDC_COMM_INTF_ID,
            /* bSubordinateInterface0  */ {CDC_DATA_INTF_ID},
      },
      { // cdc_notification_Endpoint - IN,interrupt
            /* bLength                 */ sizeof(EndpointDescriptor),
            /* bDescriptorType         */ DT_ENDPOINT,
            /* bEndpointAddress        */ EP_IN|CDC_NOTIFICATION_ENDPOINT,
            /* bmAttributes            */ ATTR_INTERRUPT,
            /* wMaxPacketSize          */ nativeToLe16(CDC_NOTIFICATION_EP_MAXSIZE),
            /* bInterval               */ USBMilliseconds(255)
      },
      /**
       * CDC Data Interface, 2 end-points
       */
      { // cdc_DCI_Interface
            /* bLength                 */ sizeof(InterfaceDescriptor),
            /* bDescriptorType         */ DT_INTERFACE,
            /* bInterfaceNumber        */ CDC_DATA_INTF_ID,
            /* bAlternateSetting       */ 0,
            /* bNumEndpoints           */ 2,
            /* bInterfaceClass         */ 0x0A,                         //  CDC DATA
            /* bInterfaceSubClass      */ 0x00,                         //  -
            /* bInterfaceProtocol      */ 0x00,                         //  -
            /* iInterface description  */ s_cdc_data_Interface_index
      },
      { // cdc_dataOut_Endpoint - OUT, Bulk
            /* bLength                 */ sizeof(EndpointDescriptor),
            /* bDescriptorType         */ DT_ENDPOINT,
            /* bEndpointAddress        */ EP_OUT|CDC_DATA_OUT_ENDPOINT,
            /* bmAttributes            */ ATTR_BULK,
            /* wMaxPacketSize          */ nativeToLe16(CDC_DATA_OUT_EP_MAXSIZE),
            /* bInterval               */ USBMilliseconds(1)
      },
      { // cdc_dataIn_Endpoint - IN, Bulk
            /* bLength                 */ sizeof(EndpointDescriptor),
            /* bDescriptorType         */ DT_ENDPOINT,
            /* bEndpointAddress        */ EP_IN|CDC_DATA_IN_ENDPOINT,
            /* bmAttributes            */ ATTR_BULK,
            /* wMaxPacketSize          */ nativeToLe16(2*CDC_DATA_IN_EP_MAXSIZE), // x2 so all packets are terminating (short))
            /* bInterval               */ USBMilliseconds(1)
      },
      /*
       * TODO Add additional Descriptors here
       */
};

/*
 * TODO Add additional end-points here
 */
OutEndpoint <Usb0Info, Usb0::BULK_OUT_ENDPOINT, BULK_OUT_EP_MAXSIZE> Usb0::epBulkOut;
InEndpoint  <Usb0Info, Usb0::BULK_IN_ENDPOINT,  BULK_IN_EP_MAXSIZE>  Usb0::epBulkIn;

InEndpoint  <Usb0Info, Usb0::CDC_NOTIFICATION_ENDPOINT, CDC_NOTIFICATION_EP_MAXSIZE>  Usb0::epCdcNotification;
OutEndpoint <Usb0Info, Usb0::CDC_DATA_OUT_ENDPOINT,     CDC_DATA_OUT_EP_MAXSIZE>      Usb0::epCdcDataOut;
InEndpoint  <Usb0Info, Usb0::CDC_DATA_IN_ENDPOINT,      CDC_DATA_IN_EP_MAXSIZE>       Usb0::epCdcDataIn;

/**
 * Handler for Start of Frame Token interrupt (~1ms interval)
 */
void Usb0::sofCallback() {
   // Activity LED
   // Off                     - no USB activity, not connected
   // On                      - no USB activity, connected
   // Off, flash briefly on   - USB activity, not connected
   // On,  flash briefly off  - USB activity, connected
   if (usb->FRMNUML==0) { // Every ~256 ms
      switch (usb->FRMNUMH&0x03) {
         case 0:
            if (connectionState == USBconfigured) {
               // Activity LED on when USB connection established
               UsbLed::on();
            }
            else {
               // Activity LED off when no USB connection
               UsbLed::off();
            }
            break;
         case 1:
         case 2:
            break;
         case 3:
         default :
            if (activityFlag) {
               // Activity LED flashes
               UsbLed::toggle();
               setActive(false);
            }
            break;
      }
   }
   // Check CDC status
   epCdcSendNotification();
}

/**
 * Configure epCdcNotification for an IN status transaction [Tx, device -> host, DATA0/1]\n
 * A packet is only sent if there has been a change in status
 */
void Usb0::epCdcSendNotification() {
   const CDCNotification cdcNotification= {CDC_NOTIFICATION, SERIAL_STATE, 0, RT_INTERFACE, nativeToLe16(2)};
   uint8_t status = Uart::getSerialState().bits;

   if ((status & Uart::CDC_STATE_CHANGE_MASK) == 0) {
      return;
   }
   if (epCdcNotification.getState() != EPIdle) {
      // Busy with previous
      return;
   }
   static_assert(epCdcNotification.BUFFER_SIZE>=sizeof(CDCNotification), "Buffer size insufficient");

   // Copy the data to Tx buffer
   (void)memcpy(epCdcNotification.getBuffer(), &cdcNotification, sizeof(cdcNotification));
   epCdcNotification.getBuffer()[sizeof(cdcNotification)+0] = status&~Uart::CDC_STATE_CHANGE_MASK;
   epCdcNotification.getBuffer()[sizeof(cdcNotification)+1] = 0;

   // Set up to Tx packet
   PRINTF("epCdcSendNotification(0x%2X)\n", epCdcNotification.getBuffer()[sizeof(cdcNotification)+0]);
   epCdcNotification.startTxTransaction(EPDataIn, sizeof(cdcNotification)+2);
}

static uint8_t cdcOutBuff[10] = "Welcome\n";
static int cdcOutByteCount    = 8;

/**
 * Start CDC IN transaction\n
 * A packet is only sent if data is available
 */
void Usb0::startCdcIn() {
   if ((epCdcDataIn.getState() == EPIdle) && (cdcOutByteCount>0)) {
      static_assert(epCdcDataIn.BUFFER_SIZE>sizeof(cdcOutBuff), "Buffer too small");
      memcpy(epCdcDataIn.getBuffer(), cdcOutBuff, cdcOutByteCount);
      //TODO check this
      epCdcDataIn.setNeedZLP();
      epCdcDataIn.startTxTransaction(EPDataIn, cdcOutByteCount);
      cdcOutByteCount = 0;
   }
}
/**
 * Handler for Token Complete USB interrupts for
 * end-points other than EP0
 */
void Usb0::handleTokenComplete() {

   // Status from Token
   uint8_t   usbStat  = usb->STAT;

   // Endpoint number
   uint8_t   endPoint = ((uint8_t)usbStat)>>4;

   endPoints[endPoint]->flipOddEven(usbStat);
   switch (endPoint) {
      case BULK_OUT_ENDPOINT: // Accept OUT token
         setActive();
         epBulkOut.handleOutToken();
         return;
      case BULK_IN_ENDPOINT: // Accept IN token
         epBulkIn.handleInToken();
         return;

      case CDC_NOTIFICATION_ENDPOINT: // Accept IN token
//         PRINTF("CDC_NOTIFICATION_ENDPOINT\n");
         epCdcSendNotification();
         return;
      case CDC_DATA_OUT_ENDPOINT: // Accept OUT token
//         PRINTF("CDC_DATA_OUT_ENDPOINT\n");
         epCdcDataOut.handleOutToken();
         return;
      case CDC_DATA_IN_ENDPOINT:  // Accept IN token
//         PRINTF("CDC_DATA_IN_ENDPOINT\n");
         epCdcDataIn.handleInToken();
         return;
      /*
       * TODO Add additional End-point handling here
       */
   }
}

/**
 * Call-back handling CDC-OUT transaction complete\n
 * Data received is passed to the UART
 *
 * @param state Current end-point state
 */
void Usb0::cdcOutTransactionCallback(EndpointState state) {
   PRINTF("cdc_out\n");
   if (state == EPDataOut) {
      uint8_t *buff = epCdcDataOut.getBuffer();
      for (int i=epCdcDataOut.getDataTransferredSize(); i>0; i--) {
         if (!Uart::putChar(*buff++)) {
            // Discard further data in this transfer
            break;
         }
      }
      // Set up for next transfer
      epCdcDataOut.startRxTransaction(EPDataOut, epCdcDataOut.BUFFER_SIZE);
   }
}

static Queue<100> inQueue;

/**
 * Call-back handling CDC-IN transaction complete\n
 * Checks for data from UART and schedules transfer as necessary
 *
 * @param state Current end-point state
 */
void Usb0::cdcInTransactionCallback(EndpointState state) {
   if (state == EPDataIn) {
      int     charCount = 0;
      uint8_t *buff     = epCdcDataIn.getBuffer();

      // Copy characters from UART to end-point buffer
      while(!inQueue.isEmpty()) {
         if (charCount>=epCdcDataIn.BUFFER_SIZE) {
            // Buffer full. Leave rest for next transfer.
            break;
         }
         *buff++ = inQueue.deQueue();
         charCount++;
      }
      if (charCount>0) {
         PRINTF("Sending %d\n", charCount);
         // Schedules transfer if data available
         epCdcDataIn.startTxTransaction(EPDataIn, charCount);
      }
   }
}

/**
 * Add character to CDC OUT buffer.
 *
 * @param ch Character to send
 *
 * @return true  Character added
 * @return false Overrun, character not added
 */
bool Usb0::putCdcChar(uint8_t ch) {
   if (inQueue.isFull()) {
      return false;
   }
   inQueue.enQueue(ch);
   if (epCdcDataIn.getState() == EPIdle) {
      IrqProtect ip;
      // Restart IN transfer
      cdcInTransactionCallback(EPDataIn);
   }
   return true;
}

/**
 * Call-back handling BULK-OUT transaction complete
 *
 * @param state Current end-point state
 */
void Usb0::bulkOutTransactionCallback(EndpointState state) {
   (void)state;
   // No actions - End-point is polled
}

/**
 * Call-back handling BULK-IN transaction complete
 *
 * @param state Current end-point state
 */
void Usb0::bulkInTransactionCallback(EndpointState state) {
   (void)state;
   // No actions - End-point is polled
}

/**
 * Initialise the USB0 interface
 *
 *  @note Assumes clock set up for USB operation (48MHz)
 */
void Usb0::initialise() {
   UsbBase_T::initialise();

   forceCommandHandlerInitialise = false;

   // Add extra handling of CDC packets directed to EP0
   setUnhandledSetupCallback(handleUserEp0SetupRequests);

   setSOFCallback(sofCallback);

   CdcUart<Uart0Info>::setInCallback(putCdcChar);
   /*
    * TODO Additional initialisation
    */
}

/**
 *  Blocking reception of data over bulk OUT end-point
 *
 *   @param maxSize  = max # of bytes to receive
 *   @param buffer   = ptr to buffer for bytes received
 *
 *   @return Number of bytes received
 *
 *   @note Doesn't return until command has been received.
 */
int Usb0::receiveBulkData(uint8_t maxSize, uint8_t *buffer) {
   epBulkOut.startRxTransaction(EPDataOut, maxSize, buffer);
   while(epBulkOut.getState() != EPIdle) {
      __WFI();
   }
   setActive();
   return epBulkOut.getDataTransferredSize();
}

/**
 *  Blocking transmission of data over bulk IN end-point
 *
 *  @param size   Number of bytes to send
 *  @param buffer Pointer to bytes to send
 *
 *   @note : Waits for idle BEFORE transmission but\n
 *   returns before data has been transmitted
 *
 */
void Usb0::sendBulkData(uint8_t size, const uint8_t *buffer) {
//   commandBusyFlag = false;
   //   enableUSBIrq();
   while (epBulkIn.getState() != EPIdle) {
      __WFI();
   }
   epBulkIn.startTxTransaction(EPDataIn, size, buffer);
}

/**
 * CDC Set line coding handler
 */
void Usb0::handleSetLineCoding() {
//   PRINTF("handleSetLineCoding()\n");

   // Call-back to do after transaction complete
   static auto callback = []() {
      // The controlEndpoint buffer will contain the LineCodingStructure data at call-back time
      Uart::setLineCoding((LineCodingStructure * const)controlEndpoint.getBuffer());
      setSetupCompleteCallback(nullptr);
   };
   setSetupCompleteCallback(callback);

   // Don't use external buffer - this requires response to fit in internal EP buffer
   static_assert(sizeof(LineCodingStructure) < controlEndpoint.BUFFER_SIZE, "Buffer insufficient size");
   controlEndpoint.startRxTransaction(EPDataOut, sizeof(LineCodingStructure));
}

/**
 * CDC Get line coding handler
 */
void Usb0::handleGetLineCoding() {
//   PRINTF("handleGetLineCoding()\n");
   // Send packet
   ep0StartTxTransaction( sizeof(LineCodingStructure), (const uint8_t*)Uart::getLineCoding());
}

/**
 * CDC Set line state handler
 */
void Usb0::handleSetControlLineState() {
//   PRINTF("handleSetControlLineState(%X)\n", ep0SetupBuffer.wValue.lo());
   Uart::setControlLineState(ep0SetupBuffer.wValue.lo());
   // Tx empty Status packet
   ep0StartTxTransaction( 0, nullptr );
}

/**
 * CDC Send break handler
 */
void Usb0::handleSendBreak() {
//   PRINTF("handleSendBreak()\n");
   Uart::sendBreak(ep0SetupBuffer.wValue);
   // Tx empty Status packet
   ep0StartTxTransaction( 0, nullptr );
}

/**
 * Handle SETUP requests not handled by base handler
 *
 * @param setup SETUP packet received from host
 *
 * @note Provides CDC extensions
 * @note Provides BDM extensions
 */
void Usb0::handleUserEp0SetupRequests(const SetupPacket &setup) {
   //PRINTF("handleUserEp0SetupRequests()\n");
   switch(REQ_TYPE(setup.bmRequestType)) {
      case REQ_TYPE_CLASS :
         // Class requests
         switch (setup.bRequest) {
            case SET_LINE_CODING :       handleSetLineCoding();       break;
            case GET_LINE_CODING :       handleGetLineCoding();       break;
            case SET_CONTROL_LINE_STATE: handleSetControlLineState(); break;
            case SEND_BREAK:             handleSendBreak();           break;
            default :                    controlEndpoint.stall();     break;
         }
         break;
      case REQ_TYPE_VENDOR :
         PRINTF("REQ_TYPE_VENDOR\n");
         switch (setup.bRequest) {
            case ICP_GET_VER : {
               // Tell command handler to re-initialise
               forceCommandHandlerInitialise = true;

               // Version response
               static const uint8_t versionResponse[5] = {
                     BDM_RC_OK,
                     VERSION_SW,      // BDM SW version
                     VERSION_HW,      // BDM HW version
                     0,               // ICP_Version_SW;
                     VERSION_HW,      // ICP_Version_HW;
               };
               ep0StartTxTransaction( sizeof(versionResponse),  versionResponse );
               }
               break;
            default :
               controlEndpoint.stall();
               break;
         }
         break;
      default:
         controlEndpoint.stall();
         break;
   }
}

void idleLoop() {
   for(;;) {
      __asm__("nop");
   }
}

} // End namespace USBDM

