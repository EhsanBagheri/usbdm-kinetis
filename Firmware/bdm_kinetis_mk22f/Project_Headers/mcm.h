/**
 * @file     mcm.h (180.ARM_Peripherals/Project_Headers/mcm.h)
 * @brief    Miscellaneous Control Module
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */

#ifndef HEADER_MCM_H
#define HEADER_MCM_H
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
#include "pin_mapping.h"

namespace USBDM {

/**
 * @addtogroup MCM_Group MCM, Miscellaneous Control Module
 * @brief Abstraction for Miscellaneous Control Module
 * @{
 */
#if false // /MCM/enablePeripheralSupport
/**
 * Template class providing a base for Miscellaneous Control Module
 */
class McmBase : public McmInfo {

public:
// /MCM/DefaultFlashInitValue not found
// /MCM/DefaultSramAccessInitValue not found
   /**
    * Default value for Mcm::FloatingPointIrqInit
    * This value is created from Configure.usbdmProject settings (Peripheral Parameters->MCM)
    */
   static constexpr McmInfo::FloatingPointIrqInit DefaultFloatingPointIrqInitValue {
      McmFpuDenormalIrq_Unselect , // (mcm_iscr_fidce) FPU input de-normalized interrupt - Disabled
      McmFpuInexactIrq_Unselect , // (mcm_iscr_fixce) FPU inexact interrupt - Disabled
      McmFpuUnderflowIrq_Unselect , // (mcm_iscr_fufce) FPU underflow interrupt - Disabled
      McmFpuOverflowIrq_Unselect , // (mcm_iscr_fofce) FPU overflow interrupt - Disabled
      McmFpuDivideByZeroIrq_Unselect , // (mcm_iscr_fdzce) FPU divide-by-zero interrupt - Disabled
      McmFpuInvalidOperationIrq_Unselect,  // (mcm_iscr_fioce) FPU invalid-operation interrupt - Disabled
   };


#if false
   /**
    * Wrapper to allow the use of a class member as a callback function
    * @note Only usable with static objects.
    *
    * @tparam T         Type of the object containing the callback member function
    * @tparam callback  Member function pointer
    * @tparam object    Object containing the member function
    *
    * @return  Pointer to a function suitable for the use as a callback
    *
    * @code
    * class AClass {
    * public:
    *    int y;
    *
    *    // Member function used as callback
    *    // This function must match McmCallbackFunction
    *    void callback() {
    *       ...;
    *    }
    * };
    * ...
    * // Instance of class containing callback member function
    * static AClass aClass;
    * ...
    * // Wrap member function
    * auto fn = Mcm::wrapCallback<AClass, &AClass::callback, aClass>();
    * // Use as callback
    * Mcm::setCallback(fn);
    * @endcode
    */
   template<class T, void(T::*callback)(), T &object>
   static CallbackFunction wrapCallback() {
      static CallbackFunction fn = []() {
         (object.*callback)();
      };
      return fn;
   }

   /**
    * Wrapper to allow the use of a class member as a callback function
    * @note There is a considerable space and time overhead to using this method
    *
    * @tparam T         Type of the object containing the callback member function
    * @tparam callback  Member function pointer
    * @tparam object    Object containing the member function
    *
    * @return  Pointer to a function suitable for the use as a callback
    *
    * @code
    * class AClass {
    * public:
    *    int y;
    *
    *    // Member function used as callback
    *    // This function must match McmCallbackFunction
    *    void callback() {
    *       ...;
    *    }
    * };
    * ...
    * // Instance of class containing callback member function
    * AClass aClass;
    * ...
    * // Wrap member function
    * auto fn = Mcm::wrapCallback<AClass, &AClass::callback>(aClass);
    * // Use as callback
    * Mcm::setCallback(fn);
    * @endcode
    */
   template<class T, void(T::*callback)()>
   static CallbackFunction wrapCallback(T &object) {
      static T &obj = object;
      static CallbackFunction fn = []() {
         (obj.*callback)();
      };
      return fn;
   }
#endif

};

   /**
    * Class representing MCM
    */
   class Mcm : public McmBase{};

#endif // /MCM/enablePeripheralSupport
/**
 * End MCM_Group
 * @}
 */

} // End namespace USBDM

#endif /* HEADER_MCM_H */
