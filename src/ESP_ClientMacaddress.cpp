#include "ESP_ClientMacaddress.h"

ClientMacaddress::ClientMacaddress(){
}

ClientMacaddress::ClientMacaddress(uint8_t (*predetermined_mac_list)[6], size_t rows){
    this->predetermined_list_size = rows;

    for (size_t i=0; i<rows; ++i){
        for (size_t j = 0; j < 6; ++j){
            this->predetermined_list[i][j] = predetermined_mac_list[i][j];
        }
    }
}

uint8_t *ClientMacaddress::getAddr(WiFiClient client){
    IPAddress client_ip = client.remoteIP();
    wifi_sta_list_t stationList;
    tcpip_adapter_sta_list_t adapter_sta_list;

    memset(&stationList, 0, sizeof(stationList));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
  
    esp_wifi_ap_get_sta_list(&stationList);
    tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);  

    IPAddress ip_from_list;
    static uint8_t mac_addr[6];
    memset(mac_addr, 0, 6);

    for(int i = 0; i < stationList.num; i++){
        wifi_sta_info_t station = stationList.sta[i];
        tcpip_adapter_sta_info_t adapter = adapter_sta_list.sta[i];

        ip_from_list.fromString(ip4addr_ntoa(&(adapter.ip)));
        
        if (client_ip == ip_from_list){
            memcpy(mac_addr, station.mac, 6);
            return mac_addr;
        }     
    }

    return mac_addr;
}

bool ClientMacaddress::isKnown(uint8_t *mac){
    uint8_t temp_mac[6];

    for (int i=0; i<this->predetermined_list_size;i++){
        for(int k = 0; k < 6; k++)
            temp_mac[k] = this->predetermined_list[i][k]; 

        if(this->compare(mac, temp_mac))
            return true;
    }
    return false;
}

bool ClientMacaddress::compare(uint8_t *mac1, uint8_t *mac2){
    bool match = true;
    for( int j = 0; ((j<6) && match); j++){
        if(mac1[j] != mac2[j]){
            match = false;
        }
    }
    return match;
}
