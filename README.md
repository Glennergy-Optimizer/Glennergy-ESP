| Supported Targets | ESP32-S3 |
| ----------------- | -------- |

| Supported LCD Controller    | ST7262 |
| ----------------------------| -------|

| Supported TOUCH Controller    | GT911 |
| ----------------------------| -------|

## ESP-IDF Required
To build and run this software you are required to have the following ESP-IDF version
* ESP-IDF v5.3.5


### Hardware Required
* An Waveshare ESP32-S3-Touch-LCD-4.3 development board
* BME280 Sensor

### Hardware Connection

        BME280 Sensor                  ESP32-S3-Touch-LCD-4.3
      ┌─────────────────┐            ┌──────────────────────────┐
      │                 │            │                          │
      │ VIN   ──────────┼──────────► │ 3V3 (3.3V Power)        │
      │ GND   ──────────┼──────────► │ GND                     │
      │ SDA   ──────────┼──────────► │ GPIO (I2C SDA)         │
      │ SCL   ──────────┼──────────► │ GPIO (I2C SCL)         │
      │                 │            │                          │
      └─────────────────┘            └──────────────────────────┘

### Configure the Project

### Build and Flash

Run `idf.py set-target esp32s3` to select the target chip.

Run `idf.py -p PORT build flash monitor` to build, flash and monitor the project.

Finding the correct port:
-Windows: Go to device manager and scroll down to the "Ports(COM and LPT)" look for the port that appears/dissapears when you plug/unplug the Esp32s3.
-Linux: Run the following command "dmesg -w" after running the command, plug in the Esp32s3 and the port should appear.

The first time you run `idf.py` it will take extra time.

(To exit the serial monitor, type ``Ctrl-]``.)


