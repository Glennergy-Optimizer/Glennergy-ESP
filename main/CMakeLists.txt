idf_component_register(
    SRCS "HTTP.c" "../ui/ui.c" "../ui/ui_helpers.c" "../ui/screens/ui_Screen1.c" "../ui/images/ui_img_1286710220.c" "HTTP.c" "WiFi.c" "main.c"
    "fake/fake_config.cpp" "fake/fake_system_status.cpp" 
    "leop/leop.cpp" "leop/fake_leop.cpp" 
    "sensor/sensor.cpp" "sensor/fake_sensor.cpp"
    "UART/UART.cpp" "UART/uart_diag_shell.cpp"
    "../ui/Tabs/Home/Sensor_UI.c" "../ui/Tabs/Settings/WiFi_UI.c"
    "../ui/Tabs/Electricity/Electricity_UI.c"
    "LEOP/Recommendation.c" "JSONParser/DataParser.c"
    "LEOP/LEOP_Fetcher.c"
    "hal/bme280_sensor.cpp"
    INCLUDE_DIRS "." "fake" "hal" "leop" "sensor" "UART"
    WHOLE_ARCHIVE
    PRIV_REQUIRES can gpio rgb_lcd_port
    REQUIRES esp_lcd nvs_flash esp_event esp_wifi esp_http_client jansson espressif__bme280 espressif__i2c_bus
    )
    
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format) 
