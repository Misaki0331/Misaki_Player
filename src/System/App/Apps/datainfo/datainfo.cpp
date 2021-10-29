#include"datainfo.h"
#include"../../../Fonts/FastFont.h"
using namespace Core::Draw;
using namespace App::Info;
void InfoData::Begin(){
    tohome=0;
}
void InfoData::Loop(){

}
void InfoData::ButtonType(int Type){
    tohome=1;
}
void InfoData::Draw(){
    char *text=new char[64];
 
  uint64_t chipid;
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  sprintf(text,"ESP32 Chip ID = %04X\r\n",(uint16_t)(chipid>>32));//print High 2 bytes
    FastFont::printConsole(text,0,0);
  sprintf(text,"Chip Revision %d\r\n", ESP.getChipRevision());
  FastFont::printConsole(text,0,8);
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  sprintf(text,"Number of Core: %d\r\n", chip_info.cores);
  FastFont::printConsole(text,0,16);
  sprintf(text,"CPU Frequency: %d MHz\r\n", ESP.getCpuFreqMHz());  
  FastFont::printConsole(text,0,24);
  sprintf(text,"Flash Chip Size = %d byte\r\n", ESP.getFlashChipSize());
  sprintf(text,"Flash Frequency = %d Hz\r\n", ESP.getFlashChipSpeed());
  sprintf(text,"ESP-IDF version = %s\r\n", esp_get_idf_version());
  sprintf(text,"Free Heap Size = %d\r\n", esp_get_free_heap_size());
  sprintf(text,"System Free Heap Size = %d\r\n", system_get_free_heap_size());
  sprintf(text,"Minimum Free Heap Size = %d\r\n", esp_get_minimum_free_heap_size());
 
  Serial.println();
 
  uint8_t mac0[6];
  esp_efuse_mac_get_default(mac0);
  sprintf(text,"Default Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
 
  uint8_t mac1[6];
  esp_efuse_read_mac(mac1);
  sprintf(text,"EFuse Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac1[0], mac1[1], mac1[2], mac1[3], mac1[4], mac1[5]);
 
  uint8_t mac2[6];
  system_efuse_read_mac(mac2);
  sprintf(text,"System Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);
 
  uint8_t mac3[6];
  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  sprintf(text,"[Wi-Fi Station] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5]);
 
  uint8_t mac4[7];
  esp_read_mac(mac4, ESP_MAC_WIFI_SOFTAP);
  sprintf(text,"[Wi-Fi SoftAP] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac4[0], mac4[1], mac4[2], mac4[3], mac4[4], mac4[5]);
 
  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  sprintf(text,"[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5]);
 
  uint8_t mac6[6];
  esp_read_mac(mac6, ESP_MAC_ETH);
  sprintf(text,"[Ethernet] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac6[0], mac6[1], mac6[2], mac6[3], mac6[4], mac6[5]);
}
 

