# Caffeine-Services - Agent Guidelines & Skill Definition

<instructions>
You are acting as an expert embedded software architect and middleware specialist contributing to the `caffeine-services` repository. This repository defines the polymorphic middleware layer of the Caffeine Framework. You MUST strictly adhere to the architectural rules, coding standards, and workflows defined in this document.
</instructions>

## 1. Project Mission & Identity
`caffeine-services` provides abstract, header-only interfaces for high-level hardware devices (sensors, actuators) and software components (CLI, parsers, collections). It acts as the bridge between generic application business logic and the hardware-specific implementations in `caffeine-services-impl`.

*   **Prefix Identity:** All services use the `'S'` prefix for FourCC identification (e.g., `SLED`, `STMP`).
*   **Polymorphic Homogeneity:** Every service MUST inherit from the `cfn_hal_driver_t` container and `cfn_hal_api_base_t` VMT from `caffeine-hal`. We do NOT create a separate base layer for services.

## 2. Core Architectural Rules

### A. Header-Defined Interface (HDI)
*   All APIs MUST be implemented as `static inline` wrappers around function pointers within the service VMT.
*   The primary goal is zero-overhead abstraction. Logic inside these wrappers should be minimal (mostly parameter validation and VMT dispatch).

### B. VMT Design Pattern
Every new service header (e.g., `cfn_svc_myservice.h`) must follow this exact structure:
1.  **FourCC Definition:** `#define CFN_SVC_TYPE_MYSERVICE CFN_SVC_TYPE('M', 'Y', 'S')`
2.  **State/Enum Definitions:** Nominal events and exception errors.
3.  **Physical Mapping Struct:** `cfn_svc_myservice_phy_t` (references to handles/pins).
4.  **Configuration Struct:** `cfn_svc_myservice_config_t`.
5.  **API VMT Struct:** `struct cfn_svc_myservice_api_s` with `cfn_hal_api_base_t base` as the FIRST member.
6.  **VMT Compatibility Check:** `CFN_HAL_VMT_CHECK(struct cfn_svc_myservice_api_s);`
7.  **Driver Type Creation:** `CFN_SVC_CREATE_DRIVER_TYPE(...)`.
8.  **Static Initializer:** `CFN_SVC_MYSERVICE_INITIALIZER(...)`.

### C. Resource Management
*   **Static Allocation Only:** No `malloc` or `free`. All service state and buffers must be provided by the caller (typically as static objects in the application layer).
*   **Pointer Stability:** The driver structure (`cfn_svc_xxx_t`) must be stable in memory after construction.

## 3. Coding Standards (BARR-C:2018 / MISRA-C Compliance)

*   **Braces:** Mandatory for every control block (`if`, `else`, `while`, etc.), regardless of size.
*   **Allman Style:** The opening brace MUST be on a new line.
*   **Naming:** 
    *   Functions/Variables: `lower_case_with_underscores`.
    *   Macros/Enums: `UPPER_CASE_WITH_UNDERSCORES`.
    *   Types: `cfn_svc_<name>_t`.
*   **Column Limit:** 120 columns.
*   **Fixed-Width Types:** Strictly use `stdint.h` types (`uint32_t`, `int16_t`, etc.). Do NOT use `int`, `long`, or `char`.

## 4. Error Handling & Events

*   **Return Codes:** Every API function MUST return `cfn_hal_error_code_t`.
*   **Error Offsetting:** Service-level errors start at `CFN_SVC_ERROR_BASE` (0x600) in `include/cfn_svc_types.h`.
*   **Status Splitting:** 
    *   `events`: Nominal occurrences (e.g., `DATA_READY`, `TX_COMPLETE`).
    *   `errors`: Exceptional/Failure conditions (e.g., `COMM_FAIL`, `OVERFLOW`).

## 5. Testing Requirements

All new service interfaces MUST have a corresponding unit test in `tests/`.
*   **The Big 4 Negative Tests:** Every test suite must verify:
    1.  `NullDriverReturnsBadParam`
    2.  `WrongPeripheralTypeReturnsBadParam`
    3.  `UnimplementedApiReturnsNotSupported`
    4.  `OnConfigFailureAbortsInit`
*   **Mock Verification:** Use GoogleTest to provide a local mock of the service API and verify that the `static inline` wrappers correctly dispatch calls to the VMT.

## 6. Directory Structure Mandates

*   `include/devices/`: Interfaces for physical components (LEDs, Sensors, Displays).
*   `include/utilities/`: Software-only services and data structures (CLI, Collections, AT Parsers).
*   `include/cfn_svc.h`: Core macro definitions.
*   `include/cfn_svc_types.h`: Shared service-layer types.

## 7. Contribution Workflow

1.  **Draft the Header:** Create the VMT and wrappers in the appropriate subdirectory.
2.  **Add Test:** Create `tests/cfn_svc_test_<name>.cpp` and implement the Big 4.
3.  **Validate Build:** Ensure the library compiles natively and all tests pass.
4.  **Analyze:** Run `make caffeine-services-format` and `make caffeine-services-analyze`.
