#ifndef ESP_ClientMacaddress_h
#define ESP_ClientMacaddress_h

#include "Arduino.h"
#include <stdint.h>
#include <WiFi.h>
#include "esp_wifi.h"

#define MAX_MACADDRESS 50

class ClientMacaddress{
    public:
        ClientMacaddress();
        ClientMacaddress(uint8_t (*_mac_list_)[6], size_t rows);
        uint8_t *getAddr(WiFiClient client); 
        bool isKnown(uint8_t *mac); 
        bool compare(uint8_t *mac1, uint8_t *mac2);

    private:
        int _macaddressList_[MAX_MACADDRESS][6];
        int Num_Macaddress = 0;    
};
#endif