#ifndef WIFI_H
#define WIFI_H

#include "esp_system.h"

esp_err_t wifi_init_sta(void);
void wifi_connect_ap(const char *ssid, const char *pass);
void wifi_disconnect(void);

#endif