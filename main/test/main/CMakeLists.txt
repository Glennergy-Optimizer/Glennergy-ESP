idf_component_register(SRCS "test.c" "test_dataparser_price.c" "test_dataparser_recommendation.c" "test_probe.c"
"test_dataparser_weather.c" "test_recommendation.c" "../../JSONParser/DataParser.c" 
                    PRIV_REQUIRES unity spiffs jansson 
                    REQUIRES 
                    INCLUDE_DIRS "." )
