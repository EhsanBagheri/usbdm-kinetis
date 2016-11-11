/**
 * @file      hardware.h
 *
 * @version  V4.12.1.80
 * @date     13 April 2016
 */

#ifndef PROJECT_HEADERS_HARDWARE_H_
#define PROJECT_HEADERS_HARDWARE_H_
 /*
 * *****************************
 * *** DO NOT EDIT THIS FILE ***
 * *****************************
 *
 * This file is generated automatically.
 * Any manual changes will be lost.
 */
namespace USBDM {

static constexpr float us = 1E-6;
static constexpr float ms = 1E-3;

enum ErrorCode {
   E_NO_ERROR = 0,      // No error
   E_ERROR,             // General error
   E_TOO_SMALL,         // Value too small
   E_TOO_LARGE,         // Value too large
   E_ILLEGAL_PARAM,     // Parameter has illegal value
   E_NO_HANDLER,        // No handler installed
   E_FLASH_INIT_FAILED, // Flash initialisation failed
};

/** Last error set by USBDM code */
extern volatile ErrorCode errorCode;

/**
 * Get USBDM error code
 *
 * @return  Error code
 */
ErrorCode getError();

/**
 * Get error message from error code or last error if not provided
 *
 * @param  err Error code
 *
 * @return Pointer to static string
 */
const char *getErrorMessage(ErrorCode err = errorCode);

/**
 * Check for error code being set (drastically!)
 * This routine does not return if there is an error
 */
#ifndef DEBUG_BUILD
inline static ErrorCode checkError() {
   while (errorCode != E_NO_ERROR) {
   }
   return errorCode;
}
#else
extern ErrorCode checkError();
#endif

/**
 * Set error code
 *
 * @param err Error code to set
 *
 * @return Error code
 */
inline static ErrorCode setErrorCode(ErrorCode err) {
   errorCode = err;
   return errorCode;
}

/**
 * Set error code and check for error
 *
 * @param err Error code to set
 *
 * @return Error code
 */
inline static ErrorCode setAndCheckErrorCode(ErrorCode err) {
   errorCode = err;
   return checkError();
}

/**
 * Clear error code
 */
inline void clearError() {
   errorCode = E_NO_ERROR;
}

} // End namespace USBDM

#include "pin_mapping.h"

#endif /* PROJECT_HEADERS_HARDWARE_H_ */
