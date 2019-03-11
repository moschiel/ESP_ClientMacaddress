#ifndef ESP_ClientMacaddress_h
#define ESP_ClientMacaddress_h

#ifndef MAX_MACADDRESS
#define MAX_MACADDRESS 30
#endif

#include "Arduino.h"
#include <stdint.h>
#include <WiFi.h>
#include "esp_wifi.h"

class ClientMacaddress{
    public:
        ClientMacaddress();
        ClientMacaddress(uint8_t (*predetermined_mac_list)[6], size_t rows);
        uint8_t *getAddr(WiFiClient client); 
        bool isKnown(uint8_t *mac); 
        bool compare(uint8_t *mac1, uint8_t *mac2);

    private:
        int predetermined_list[MAX_MACADDRESS][6];
        int predetermined_list_size = 0; 
};
#endif
