idf_component_register(
    SRCS "HTTP.c" "../ui/ui.c" "../ui/ui_helpers.c" "../ui/screens/ui_Screen1.c" "../ui/images/ui_img_1286710220.c" "HTTP.c" "WiFi.c" "main.c"
    INCLUDE_DIRS "."
    WHOLE_ARCHIVE
    PRIV_REQUIRES can gpio rgb_lcd_port
    REQUIRES esp_lcd nvs_flash esp_event esp_wifi esp_http_client jansson
    )
    
idf_component_get_property(lvgl_lib lvgl__lvgl COMPONENT_LIB)
target_compile_options(${lvgl_lib} PRIVATE -Wno-format) 