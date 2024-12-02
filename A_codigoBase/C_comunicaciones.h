#ifndef COMUNICACIONES_H
#define COMUNICACIONES_H

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <espnow.h>

extern IPAddress staticIP_AP;
extern IPAddress staticIP_llubot2;
extern IPAddress staticIP_llubot3;

typedef struct struct_emisor {    /// TO-DO: Cambiar estructura de DATOS
    float temperature;    
    float humidity;
    int numero;
} struct_emisor;

extern struct_emisor s_emisor;

void init_wifi(int llubot);
void init_esp_now(int llubot);
void enviar_datos(int llubot, struct_emisor data);
void Recibir_datos_del_emisor(uint8_t *macAddr, uint8_t *data, uint8_t len);
void Recibir_datos_del_receptor(uint8_t *macAddr, uint8_t *data, uint8_t len);

#endif