#include "comunicaciones.h"
#include <ESP8266WiFi.h>

void SU_main_Emisor() {
    Serial.begin(115200);
    init_wifi(1);
    Serial.print("Dirección MAC emisor: ");       
    Serial.println(WiFi.softAPmacAddress());
    init_esp_now(1);
}

void LOOP_main_Emisor() {
    struct_emisor data = {23.5, 45.2, 1};   
    
    enviar_datos(3, data);
    
    delay(5000);
}