/**
 * @file cfn_svc_network.h
 * @brief Abstract Network (Socket-like) service interface.
 */

#ifndef CAFFEINE_SERVICES_DEVICES_CFN_SVC_NETWORK_H
#define CAFFEINE_SERVICES_DEVICES_CFN_SVC_NETWORK_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ---------------------------------------------------------*/
#include "cfn_svc.h"
#include "cfn_svc_types.h"

/* Defines ----------------------------------------------------------*/

#define CFN_SVC_TYPE_NETWORK CFN_SVC_TYPE('N', 'E', 'T')

/* Types Enums ------------------------------------------------------*/

typedef enum
{
    CFN_SVC_NETWORK_EVENT_NONE = 0,
    CFN_SVC_NETWORK_EVENT_CONNECTED = CFN_HAL_BIT(0),
    CFN_SVC_NETWORK_EVENT_DISCONNECTED = CFN_HAL_BIT(1),
    CFN_SVC_NETWORK_EVENT_DATA_READY = CFN_HAL_BIT(2),
} cfn_svc_network_event_t;

typedef enum
{
    CFN_SVC_NETWORK_SOCKET_TCP,
    CFN_SVC_NETWORK_SOCKET_UDP,
} cfn_svc_network_socket_type_t;

/* Types Structs ----------------------------------------------------*/

typedef struct
{
    void *instance; /*!< Mapping to Ethernet, WiFi, or Cellular driver */
    void *user_arg;
} cfn_svc_network_phy_t;

typedef struct
{
    const char *hostname;
    uint16_t    port;
    void       *custom;
} cfn_svc_network_config_t;

typedef struct cfn_svc_network_s     cfn_svc_network_t;
typedef struct cfn_svc_network_api_s cfn_svc_network_api_t;

typedef void (*cfn_svc_network_callback_t)(cfn_svc_network_t *driver, uint32_t event, uint32_t error, void *user_arg);

/**
 * @brief Network Virtual Method Table (VMT).
 */
struct cfn_svc_network_api_s
{
    cfn_hal_api_base_t base;

    /* Connection Management */
    cfn_hal_error_code_t (*connect)(cfn_svc_network_t *driver, const char *host, uint16_t port, cfn_svc_network_socket_type_t type);
    cfn_hal_error_code_t (*disconnect)(cfn_svc_network_t *driver);
    cfn_hal_error_code_t (*is_connected)(cfn_svc_network_t *driver, bool *connected_out);

    /* Data Transfer */
    cfn_hal_error_code_t (*send)(cfn_svc_network_t *driver, const uint8_t *data, size_t len, size_t *sent_len);
    cfn_hal_error_code_t (*receive)(cfn_svc_network_t *driver, uint8_t *buffer, size_t max_len, size_t *received_len);

    /* Info */
    cfn_hal_error_code_t (*get_ip_address)(cfn_svc_network_t *driver, char *ip_out, size_t max_len);
};

CFN_HAL_VMT_CHECK(struct cfn_svc_network_api_s);

CFN_SVC_CREATE_DRIVER_TYPE(svc_network, cfn_svc_network_config_t, cfn_svc_network_api_t, cfn_svc_network_phy_t, cfn_svc_network_callback_t);

#define CFN_SVC_NETWORK_INITIALIZER(api_ptr, phy_ptr, config_ptr)                                                   \
    CFN_SVC_DRIVER_INITIALIZER(CFN_SVC_TYPE_NETWORK, api_ptr, phy_ptr, config_ptr)

/* Functions inline ------------------------------------------------- */

CFN_HAL_INLINE cfn_hal_error_code_t cfn_svc_network_init(cfn_svc_network_t *driver)
{
    if (!driver) { return CFN_HAL_ERROR_BAD_PARAM; }
    driver->base.vmt = (const struct cfn_hal_api_base_s *) driver->api;
    return cfn_hal_base_init(&driver->base, CFN_SVC_TYPE_NETWORK);
}

CFN_HAL_INLINE cfn_hal_error_code_t cfn_svc_network_send(cfn_svc_network_t *driver, const uint8_t *data, size_t len, size_t *sent_len)
{
    cfn_hal_error_code_t error = CFN_HAL_ERROR_OK;
    CFN_HAL_CHECK_AND_CALL_FUNC_VARG(CFN_SVC_TYPE_NETWORK, send, driver, error, data, len, sent_len);
    return error;
}

#ifdef __cplusplus
}
#endif

#endif /* CAFFEINE_SERVICES_DEVICES_CFN_SVC_NETWORK_H */
