<p align="center">
  <a href="https://whileone.me">
    <img src="https://whileone.me/images/caffeine-small.png" alt="Caffeine Logo" width="384" height="384">
  </a>
</p>

# Caffeine-Services

<p align="center">
  <img src="https://img.shields.io/badge/C-11-blue.svg?style=flat-square&logo=c" alt="C11">
  <img src="https://img.shields.io/badge/CMake-%23008FBA.svg?style=flat-square&logo=cmake&logoColor=white" alt="CMake">
  <a href="https://github.com/while-one/caffeine-services/tags">
    <img src="https://img.shields.io/github/v/tag/while-one/caffeine-services?style=flat-square&label=Release" alt="Latest Release">
  </a>
  <a href="https://github.com/while-one/caffeine-services/actions/workflows/ci.yml">
    <img src="https://img.shields.io/github/actions/workflow/status/while-one/caffeine-services/ci.yml?style=flat-square&branch=main" alt="CI Status">
  </a>
  <a href="https://github.com/while-one/caffeine-services/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/while-one/caffeine-services?style=flat-square&color=blue" alt="License: MIT">
  </a>
</p>

**Caffeine-Services** is the polymorphic middleware layer of the **Caffeine Framework**. It provides a strictly C11-compliant, header-only, Virtual Method Table (VMT) architecture to define abstract interfaces for high-level hardware devices and software services.

Built directly on top of [**Caffeine-HAL**](https://github.com/while-one/caffeine-hal), this library ensures that business logic remains completely portable across different hardware implementations.

---

## Overview

The library decouples the high-level API from the concrete implementation (e.g., a specific sensor model or a specific CLI parser). This allows you to write application code that interacts with a "Temperature Sensor" or an "AT Parser" without caring about the specific hardware or software implementation underneath.

### Key Features
*   **Header-Only Interface:** Zero-overhead abstractions using `static inline` wrappers.
*   **Homogeneous Architecture:** Inherits from the standard `cfn_hal_driver_t` container, ensuring a consistent lifecycle (`init`, `deinit`, `config`, `callback`) across the entire framework.
*   **Layered Identification:** Uses FourCC codes with the `S` prefix (e.g., `SLED`, `STMP`) to uniquely identify services.
*   **Unified Collections:** A single polymorphic interface for Ring Buffers, Linked Lists, and Queues.
*   **Thread-Safe:** Directly utilizes the `CFN_HAL_WITH_LOCK` mechanism for multi-threaded safety.

---

## Directory Structure

*   `include/cfn_svc.h`: Core macros and FourCC definitions.
*   `include/devices/`: Hardware-agnostic interfaces for physical components.
    *   `cfn_svc_led.h`, `cfn_svc_button.h`, `cfn_svc_accel.h`, `cfn_svc_gsm.h`, etc.
*   `include/utilities/`: High-level software services and data structures.
    *   `cfn_svc_cli.h`, `cfn_svc_at_parser.h`, `cfn_svc_collection.h`.

---

## Integration

### 1. CMake (FetchContent)

```cmake
include(FetchContent)
FetchContent_Declare(
    caffeine-services
    GIT_REPOSITORY https://github.com/while-one/caffeine-services.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(caffeine-services)

target_link_libraries(your_app PRIVATE caffeine::services)
```

### 2. Usage Example (LED)

```c
#include "devices/cfn_svc_led.h"

// Define configuration
cfn_svc_led_config_t led_cfg = { .active_low = false };

// Physical mapping (provided by implementation layer)
extern cfn_svc_led_api_t my_specific_led_impl;
cfn_svc_led_phy_t led_phy = { .instance = (void*)&GPIO_INSTANCE_PORT_A };

// Initialize
cfn_svc_led_t status_led = CFN_SVC_LED_INITIALIZER(&my_specific_led_impl, &led_phy, &led_cfg);
cfn_svc_led_init(&status_led);

// Use
cfn_svc_led_set_state(&status_led, CFN_SVC_LED_STATE_ON);
```

---

## Development & Contribution

All contributions must adhere to the [**SKILL.md**](SKILL.md) guidelines:
*   Strict C11 (No C++ or GNU extensions).
*   No dynamic memory allocation.
*   Allman-style braces and 120-column limit.
*   Polymorphic VMT pattern for all complex interfaces.

---

## License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.
