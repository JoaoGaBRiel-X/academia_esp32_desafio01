idf_component_register(SRCS "wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES 
                        esp_wifi
                        esp_http_client
                        )
