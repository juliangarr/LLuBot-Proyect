#ifndef COMUNICACIONES_H
#define COMUNICACIONES_H

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>    // Hay que instalar la libreria ESP8266Ping desde un archivo .zip descargado aquí: https://github.com/dancol90/ESP8266Ping/tree/master
#include <espnow.h>

extern IPAddress staticIP_AP;
extern IPAddress staticIP_llubot2;
extern IPAddress staticIP_llubot3;

typedef struct struct_emisor {    /// TO-DO: Cambiar estructura de DATOS
    //int id_llubot;
    float dist_to_move;  // dist = 0 means parking is possible
} struct_emisor;

extern struct_emisor s_emisor;

void init_wifi(int llubot);
void init_esp_now(int llubot);
void enviar_datos(int llubot, struct_emisor data);
void Recibir_datos_del_emisor(uint8_t *macAddr, uint8_t *data, uint8_t len);
void Recibir_datos_del_receptor(uint8_t *macAddr, uint8_t *data, uint8_t len);

#endif