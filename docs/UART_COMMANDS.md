# UART Command Documentation

This document describes the UART diagnostics interface used by the Glennergy ESP32 terminal.

The UART shell is intended for development, debugging and simple runtime inspection of the terminal state.

## Interface Settings

| Setting | Value |
| --- | --- |
| UART port | `UART_NUM_0` |
| Baud rate | `115200` |
| Data bits | `8` |
| Parity | None |
| Stop bits | `1` |
| Flow control | Disabled |
| Pins | Default UART0 pins, `UART_PIN_NO_CHANGE` |
| Input ending | `Enter`, `\n` or `\r` |

Defined in:

```c
#define UART_PORT UART_NUM_0
#define UART_BAUD 115200
```

The code does not assign custom TX/RX pins. UART0 is kept on the default serial connection used for the ESP terminal.

## Input Handling

The UART task reads one byte at a time and builds a line buffer.

When the user presses Enter:

1. The current line is terminated.
2. Leading and trailing whitespace is removed.
3. The command is converted to lowercase.
4. The command is split on spaces.
5. The first token is used as the command name.

Example:

```text
  SeNsOr
```

is handled as:

```text
sensor
```

The current line buffer is 128 characters long. Longer input is ignored after the buffer is full.

## Command Summary

| Command | Purpose |
| --- | --- |
| `help` | Prints available commands. |
| `status` | Prints basic system health. |
| `sensor` | Prints latest BME280 sensor readings. |
| `leop` | Prints latest LEOP recommendation data. |
| `config <name> <value>` | Updates a runtime config value. |
| `pconfig` | Prints current config values. |
| `diag` | Prints runtime diagnostics. |

Unknown commands return:

```text
Unknown command: <input>
```

## `help`

Prints the available UART commands.

Request:

```text
help
```

Output includes:

```text
STATUS
LEOP
SENSOR
CONFIG <param> <value>
PCONFIG
DIAG
HELP
```

There is also a `help immersive` variant in the code. It prints a longer interactive help sequence and waits for Enter between messages. This is mostly an Easter egg/debug feature and is not needed for normal diagnostics.

## `status`

Prints high-level system status from `app_state_t.system_status`.

Request:

```text
status
```

Output fields:

| Field | Meaning |
| --- | --- |
| `Wifi` | Whether system status says WiFi is connected. |
| `LEOP` | Whether system status says LEOP is connected. |
| `Sensor` | Whether system status says sensor is OK. |
| `Update counter` | Current system update counter. |

Example output:

```text
Wifi: Connected
LEOP: Connected
Sensor: OK
Update counter: 123
```

## `sensor`

Prints the latest sensor snapshot from `app_state_t.sensor_data`.

Request:

```text
sensor
```

If no valid sensor data exists yet:

```text
Debug valid: 0
Temperature raw: 0
Sensor: No valid data yet.
```

If valid data exists:

```text
Debug valid: 1
Temperature raw: 22.5
Last updated monotinic time: 183
Last updated local time: 2026-06-12 14:05:22
Temperature - 22.5
Pressure    - 1012.8
Humidity    - 44.1
```

Notes:

- Monotonic time is seconds since boot.
- Local time is printed only when wall time has been synced and validated.
- Sensor values come from the BME280 task through `app.sensor_data`.

## `leop`

Prints the latest LEOP recommendation data from `app_state_t.leop_data`.

Request:

```text
leop
```

Output format:

```text
--- Latest leop data ---
Number of entries: <count>
Now printing entries  recommendation and timestamp
<recommendation>, <timestamp>
<recommendation>, <timestamp>
...
```

The current command prints recommendation values only. Weather and price data are fetched by the LEOP task, but this UART command does not currently print them.

## `pconfig`

Prints current runtime configuration values.

Request:

```text
pconfig
```

Output fields:

| Field | Meaning |
| --- | --- |
| `fetch_interval_minutes` | LEOP fetch interval in minutes. |
| `sensor_interval_ms` | Sensor read interval in milliseconds. |
| `test_mode` | Whether test mode is enabled. |

Example output:

```text
fetch_interval_minutes: 5
sensor_interval_ms: 1000
test_mode: Disabled
```

## `config`

Updates a runtime configuration value.

Syntax:

```text
config <config_name> <value>
```

Supported fields:

| Config name | Type | Accepted values |
| --- | --- | --- |
| `fetch_interval_minutes` | integer | `2` to `1440` |
| `sensor_interval_ms` | integer | `1000` to `60000` |
| `test_mode` | boolean string | `true` or `false` |

Examples:

```text
config fetch_interval_minutes 15
config sensor_interval_ms 5000
config test_mode true
```

Important behavior:

- `fetch_interval_minutes` must currently be greater than `1`, so `1` is rejected even though the error text says "between 1 and 1440".
- `sensor_interval_ms` accepts values from 1 second to 60 seconds.
- `test_mode` accepts only lowercase `true` or `false`, but command input is lowercased before parsing.
- Values are changed in RAM through `app_state_t.config_data`.
- The command does not currently persist the config to NVS.

Invalid syntax prints:

```text
syntax: "config <config_name> <value>".
Fields (name) (value):
        fetch_interval_minutes uint32_t
        test_mode bool
```

## `diag`

Prints runtime diagnostics.

Request:

```text
diag
```

Output fields:

| Field | Meaning |
| --- | --- |
| `Uptime` | Seconds since boot. |
| `Free Heap` | Current heap available. |
| `Minimum free heap` | Lowest heap level since boot. |
| `Task count` | Number of FreeRTOS tasks. |
| Task stack lines | Stack usage estimate for WiFi, UI, Sensor, UART and LEOP tasks. |

Example output:

```text
Diagnostics:
Uptime: 120 seconds.
Free Heap: 123456 bytes.
Minimum free heap: 100000 bytes.
Task count: 12
WIFI_Work: used 512/4096 {12%), min free 3584
UI_Update: used 9000/16384 {54%), min free 7384
Sensor: used 480/4096 {11%), min free 3616
UART: used 700/4096 {17%), min free 3396
LEOP: used 1200/4096 {29%), min free 2896
```

Note: the printed stack usage is based on `uxTaskGetStackHighWaterMark()`. It is useful for debugging but should be treated as an estimate.

## Typical Debug Session

```text
help
pconfig
status
sensor
leop
diag
```

Recommended workflow:

1. Run `status` to check overall state.
2. Run `sensor` to confirm BME280 data.
3. Run `leop` to confirm LEOP recommendation data.
4. Run `pconfig` to inspect intervals.
5. Run `diag` if memory or task behavior looks suspicious.

## Limitations

- Commands read shared `app_state_t` values directly.
- The UART shell is intended as a diagnostics tool, not a user-facing API.
- Config changes are runtime-only and are not persisted.
- `leop` currently prints recommendation data, not all LEOP data types.
- UART output uses `std::cout`, so formatting is simple text rather than structured JSON.
