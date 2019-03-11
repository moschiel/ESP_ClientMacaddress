#ifndef ESP_ClientMacaddress_h
#define ESP_ClientMacaddress_h

#ifndef EEPROM_ADDR_MAC_LIST
#define EEPROM_ADDR_MAC_LIST 0
#endif

#ifndef MAX_LIST
#define MAX_LIST 10
#endif

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
        void putAddr(int list_index, uint8_t *mac);
        uint8_t *getAddr(WiFiClient client); 
        bool isKnown(uint8_t *mac); 
        bool compare(uint8_t *mac1, uint8_t *mac2);

    private:
        int predetermined_list[MAX_MACADDRESS][6];
        int predetermined_list_size = 0;
        struct list{
            int size;
            int macaddress[MAX_MACADDRESS][6];
        }; 
        struct list lists[MAX_LIST];    
};
#endif
