#include "ESP_ClientMacaddress.h"

ClientMacaddress::ClientMacaddress(){
}

ClientMacaddress::ClientMacaddress(uint8_t (*_mac_list_)[6], size_t rows){
    this->Num_Macaddress = rows;

    for (size_t i=0; i<rows; ++i){
        for (size_t j = 0; j < 6; ++j){
            this->_macaddressList_[i][j] = _mac_list_[i][j];
        }
    }
}

uint8_t *ClientMacaddress::getAddr(WiFiClient _client_){
    IPAddress _client_ip_ = _client_.remoteIP();
    wifi_sta_list_t stationList;
    tcpip_adapter_sta_list_t adapter_sta_list;

    memset(&stationList, 0, sizeof(stationList));
    memset(&adapter_sta_list, 0, sizeof(adapter_sta_list));
  
    esp_wifi_ap_get_sta_list(&stationList);
    tcpip_adapter_get_sta_list(&stationList, &adapter_sta_list);  

    IPAddress _ip_from_list_;
    static uint8_t _mac_addr_[6];
    memset(_mac_addr_, 0, 6);

    for(int i = 0; i < stationList.num; i++){
        wifi_sta_info_t station = stationList.sta[i];
        tcpip_adapter_sta_info_t adapter = adapter_sta_list.sta[i];

        _ip_from_list_.fromString(ip4addr_ntoa(&(adapter.ip)));
        
        if (_client_ip_ == _ip_from_list_){
            memcpy(_mac_addr_, station.mac, 6);
            return _mac_addr_;
        }     
    }

    return _mac_addr_;

}

bool ClientMacaddress::isKnown(uint8_t *mac){
    uint8_t temp_mac[6];

    for (int i=0; i<this->Num_Macaddress;i++){
        for(int k = 0; k < 6; k++)
            temp_mac[k] = this->_macaddressList_[i][k]; 

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