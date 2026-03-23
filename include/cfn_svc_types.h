/**
 * @file cfn_svc_types.h
 * @brief Common type definitions and error codes for Caffeine Services.
 */

#ifndef CAFFEINE_SERVICES_CFN_SVC_TYPES_H
#define CAFFEINE_SERVICES_CFN_SVC_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ---------------------------------------------------------*/
#include "cfn_hal_types.h"
#include "cfn_svc.h"

/* Defines ----------------------------------------------------------*/

/* Types Enums ------------------------------------------------------*/

/**
 * @brief Service-level error codes.
 * Extends the cfn_hal_error_code_t space starting from CFN_SVC_ERROR_BASE (0x600).
 */
typedef enum
{
    CFN_SVC_ERROR_OK = CFN_HAL_ERROR_OK,

    /* Collection Specific Errors */
    CFN_SVC_ERROR_COLLECTION_FULL = CFN_SVC_ERROR_BASE,
    CFN_SVC_ERROR_COLLECTION_EMPTY,
    CFN_SVC_ERROR_COLLECTION_NOT_FOUND,

    /* Sensor Specific Errors */
    CFN_SVC_ERROR_SENSOR_COMM_FAIL = 0x700,
    CFN_SVC_ERROR_SENSOR_DATA_INVALID,

    /* Communication Specific Errors */
    CFN_SVC_ERROR_AT_RESPONSE_TIMEOUT = 0x800,
    CFN_SVC_ERROR_AT_RESPONSE_ERROR,
    CFN_SVC_ERROR_COMM_NOT_REGISTERED,

    /* File System Specific Errors */
    CFN_SVC_ERROR_FS_MOUNT_FAILED = 0x900,
    CFN_SVC_ERROR_FS_NO_MEDIA,
    CFN_SVC_ERROR_FS_NOT_FOUND,
    CFN_SVC_ERROR_FS_ALREADY_EXISTS,
    CFN_SVC_ERROR_FS_FULL,

    /* Connection Specific Errors */
    CFN_SVC_ERROR_CON_NOT_CONNECTED = 0xA00,
    CFN_SVC_ERROR_CON_DNS_FAILED,
    CFN_SVC_ERROR_CON_TIMEOUT,
    CFN_SVC_ERROR_CON_REFUSED,
    CFN_SVC_ERROR_CON_AUTH_FAILED,

    /* Transport Specific Errors */
    CFN_SVC_ERROR_TPT_BUSY = 0xB00,
    CFN_SVC_ERROR_TPT_OVERFLOW,
    CFN_SVC_ERROR_TPT_FRAMING,

    /* Serialization Specific Errors */
    CFN_SVC_ERROR_SER_BUFFER_OVERFLOW = 0xC00,
    CFN_SVC_ERROR_SER_SCHEMA_MISMATCH,
    CFN_SVC_ERROR_SER_INVALID_TYPE,
    CFN_SVC_ERROR_SER_UNSUPPORTED_FORMAT,

} cfn_svc_error_code_t;

/* Types Structs ----------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* CAFFEINE_SERVICES_CFN_SVC_TYPES_H */
