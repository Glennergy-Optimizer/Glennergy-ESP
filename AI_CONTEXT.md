# AI Context For Doxygen Automation

This repository is an ESP-IDF / embedded firmware project for Glennergy-ESP.

Important project characteristics:
- Runs on ESP32-S3 hardware
- Uses ESP-IDF APIs and conventions such as `esp_err_t`, `ESP_LOG*`, `ESP_ERROR_CHECK`, and component-based structure
- Uses FreeRTOS tasks, queues, delays, and event-driven background work
- Contains hardware-facing modules for Wi-Fi, HTTP, UART, sensors, I2C, GPIO, storage, display, touch, and UI integration
- Many modules depend on initialization order, task context, hardware availability, or blocking behavior

Documentation priorities for this repository:
- Preserve all code behavior exactly
- Preserve developer comments, TODOs, debug prints, commented-out code, and formatting intent
- Treat Doxygen as editable, but keep non-Doxygen comments intact
- Prefer concise public source-file docs and richer header docs
- Explicitly document embedded-specific concerns when relevant:
  - task vs ISR context
  - blocking vs non-blocking behavior
  - initialization order and dependencies
  - hardware/peripheral usage
  - concurrency and shared state assumptions
  - buffer limits, units, scaling, and timing assumptions
  - failure behavior and safe-state handling

When unsure:
- preserve existing code and ordinary comments
- prefer concise Doxygen over speculative verbosity
- document only real embedded constraints, not generic boilerplate
