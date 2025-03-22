idf_component_register(SRCS "main.c"
                    PRIV_REQUIRES spi_flash
                    INCLUDE_DIRS ""
                    REQUIRES
                        dht
                        mqtt_app
                        ssd1306
                        wifi
                        nvs_flash)
