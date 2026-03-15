/**
 * @file cfn_svc_ble.h
 * @brief Abstract Bluetooth Low Energy (BLE) service interface.
 */

#ifndef CAFFEINE_SERVICES_DEVICES_CFN_SVC_BLE_H
#define CAFFEINE_SERVICES_DEVICES_CFN_SVC_BLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ---------------------------------------------------------*/
#include "cfn_svc.h"
#include "cfn_svc_types.h"

/* Defines ----------------------------------------------------------*/

#define CFN_SVC_TYPE_BLE CFN_SVC_TYPE('B', 'L', 'E')

/* Types Enums ------------------------------------------------------*/

typedef enum
{
    CFN_SVC_BLE_EVENT_NONE = 0,
    CFN_SVC_BLE_EVENT_CONNECTED = CFN_HAL_BIT(0),
    CFN_SVC_BLE_EVENT_DISCONNECTED = CFN_HAL_BIT(1),
    CFN_SVC_BLE_EVENT_MTU_CHANGED = CFN_HAL_BIT(2),
    CFN_SVC_BLE_EVENT_DATA_RECEIVED = CFN_HAL_BIT(3),
} cfn_svc_ble_event_t;

/* Types Structs ----------------------------------------------------*/

typedef struct
{
    void *instance; /*!< Mapping to BLE controller driver */
    void *user_arg;
} cfn_svc_ble_phy_t;

typedef struct
{
    const char *device_name;
    uint16_t    adv_interval_ms;
    void       *custom;
} cfn_svc_ble_config_t;

typedef struct cfn_svc_ble_s     cfn_svc_ble_t;
typedef struct cfn_svc_ble_api_s cfn_svc_ble_api_t;

typedef void (*cfn_svc_ble_callback_t)(cfn_svc_ble_t *driver, uint32_t event, uint32_t error, void *user_arg);

/**
 * @brief BLE Virtual Method Table (VMT).
 */
struct cfn_svc_ble_api_s
{
    cfn_hal_api_base_t base;

    /* Advertising */
    cfn_hal_error_code_t (*adv_start)(cfn_svc_ble_t *driver);
    cfn_hal_error_code_t (*adv_stop)(cfn_svc_ble_t *driver);

    /* Connection */
    cfn_hal_error_code_t (*disconnect)(cfn_svc_ble_t *driver);

    /* GATT Operations */
    cfn_hal_error_code_t (*gatt_write)(cfn_svc_ble_t *driver, uint16_t handle, const uint8_t *data, size_t len);
    cfn_hal_error_code_t (*gatt_notify)(cfn_svc_ble_t *driver, uint16_t handle, const uint8_t *data, size_t len);

    /* Info */
    cfn_hal_error_code_t (*get_mac_address)(cfn_svc_ble_t *driver, uint8_t *mac_out);
};

CFN_HAL_VMT_CHECK(struct cfn_svc_ble_api_s);

CFN_SVC_CREATE_DRIVER_TYPE(svc_ble, cfn_svc_ble_config_t, cfn_svc_ble_api_t, cfn_svc_ble_phy_t, cfn_svc_ble_callback_t);

#define CFN_SVC_BLE_INITIALIZER(api_ptr, phy_ptr, config_ptr)                                                   \
    CFN_SVC_DRIVER_INITIALIZER(CFN_SVC_TYPE_BLE, api_ptr, phy_ptr, config_ptr)

/* Functions inline ------------------------------------------------- */

CFN_HAL_INLINE cfn_hal_error_code_t cfn_svc_ble_init(cfn_svc_ble_t *driver)
{
    if (!driver) { return CFN_HAL_ERROR_BAD_PARAM; }
    driver->base.vmt = (const struct cfn_hal_api_base_s *) driver->api;
    return cfn_hal_base_init(&driver->base, CFN_SVC_TYPE_BLE);
}

CFN_HAL_INLINE cfn_hal_error_code_t cfn_svc_ble_adv_start(cfn_svc_ble_t *driver)
{
    cfn_hal_error_code_t error = CFN_HAL_ERROR_OK;
    CFN_HAL_CHECK_AND_CALL_FUNC(CFN_SVC_TYPE_BLE, adv_start, driver, error);
    return error;
}

#ifdef __cplusplus
}
#endif

#endif /* CAFFEINE_SERVICES_DEVICES_CFN_SVC_BLE_H */
