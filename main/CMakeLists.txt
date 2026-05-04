idf_component_register(
    SRCS "HTTP.c" "../ui/ui.c" "../ui/ui_helpers.c" "../ui/screens/ui_Screen1.c" "../ui/images/ui_img_1286710220.c" "HTTP.c" "WiFi.c" "main.c"
    "UART/fake_config.cpp" "UART/fake_system_status.cpp" "UART/fake_sensor.cpp" "UART/fake_leop.cpp" "UART/UART.cpp" "UART/uart_diag_shell.cpp"
    INCLUDE_DIRS "." "UART"
    WHOLE_ARCHIVE
    PRIV_REQUIRES can gpio rgb_lcd_port
    REQUIRES esp_lcd nvs_flash esp_event esp_wifi esp_http_client jansson
    )
    
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format) 