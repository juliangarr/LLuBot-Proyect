#include "C_comunicaciones.h"
/*
IPAddress staticIP_llubot1_AP(192,168,4,0);
IPAddress staticIP_llubot2(192,168,4,95);
IPAddress staticIP_llubot3(192,168,4,96);
*/

// Configuración de la red externa
const char* ssid_external = "RAUL_LAPTOP";
const char* password_external = "12345678";

// Configuración de la red AP
const char* ssid_AP = "Llubot_Network";
const char* password_AP = "ClaveLlubot";
IPAddress staticIP_llubot1_AP(192, 168, 4, 0); // IP fija para AP
IPAddress gateway_AP(192, 168, 4, 1);
IPAddress subnet_AP(255, 255, 255, 0);

// Configuración de IP fija para este dispositivo
IPAddress staticIP_llubot2(192, 168, 4, 2); // Cambiar a .3 para el tercer dispositivo

// Configuración de IP fija para este dispositivo
IPAddress staticIP_llubot3(192, 168, 4, 3); // Cambiar a .3 para el tercer dispositivo


uint8_t MAC_llubot_1[] = {0xDC, 0x4F, 0x22, 0x5F, 0x2A, 0xBA};
uint8_t MAC_llubot_2[] = {0x5C, 0xCF, 0x7F, 0x01, 0x64, 0x68};
uint8_t MAC_llubot_3[] = {0x84, 0xF3, 0xEB, 0xB2, 0x2D, 0xB5};

struct_emisor s_emisor;

void init_wifi(int llubot){

  if (llubot == 1){
    // Configurar como STA (Conectarse a Red Externa)
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(ssid_external, password_external);

    // Esperar conexión STA
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println("\nConectado a red externa.");
    Serial.print("IP STA: ");
    Serial.println(WiFi.localIP());

    // Configurar como AP
    WiFi.softAPConfig(staticIP_llubot1_AP, gateway_AP, subnet_AP);
    WiFi.softAP(ssid_AP, password_AP);
    Serial.println("AP configurado.");
    Serial.print("IP AP: ");
    Serial.println(WiFi.softAPIP());
  }

  if (llubot == 2){
    // Configurar como STA con IP fija
    WiFi.mode(WIFI_STA);
    WiFi.config(staticIP_llubot2, gateway_AP, subnet_AP);
    WiFi.begin(ssid_AP, password_AP);

    // Esperar conexión STA
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println("\nLlubot 2 Conectado a AP.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }

  if (llubot == 3){
    // Configurar como STA con IP fija
    WiFi.mode(WIFI_STA);
    WiFi.config(staticIP_llubot3, gateway_AP, subnet_AP);
    WiFi.begin(ssid_AP, password_AP);

    // Esperar conexión STA
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
    }
    Serial.println("\nLlubot 3 Conectado a AP.");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
}

void init_esp_now(int llubot){
  if (esp_now_init() == 0) {
    Serial.println("ESP-NOW inicializado con éxito.");
  } else {
    Serial.println("Error al inicializar ESP-NOW.");
    return;
  }

  if (llubot == 1){
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(MAC_llubot_2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_add_peer(MAC_llubot_3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_register_recv_cb(Recibir_datos_del_receptor);
  }

  if (llubot == 2 || llubot == 3){
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
    esp_now_register_recv_cb(Recibir_datos_del_emisor);
    esp_now_add_peer(MAC_llubot_1, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0);
  }
}

void Recibir_datos_del_emisor(uint8_t *macAddr, uint8_t *data, uint8_t len) {
  Serial.print("Datos recibidos de: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.print("Longitud del mensaje: ");
  Serial.println(len);
  Serial.println("Mensaje recibido del emisor: ");
  memcpy(&s_emisor, data, sizeof(s_emisor));
  Serial.print("Distancia: ");
  Serial.println(s_emisor.dist_to_move);
  Serial.print("Actividad recibida: ");
  Serial.println(s_emisor.actividad);
  Serial.print("Actividad actual: ");
  Serial.println(actividadSel);
  actividadSel = s_emisor.actividad;
  Serial.print("Actividad actualizada: ");
  Serial.println(actividadSel);

  String confirmationMessage = "Mensaje recibido correctamente desde el receptor";
  uint8_t confirmationData[confirmationMessage.length() + 1];
  confirmationMessage.getBytes(confirmationData, sizeof(confirmationData));

  if (esp_now_send(MAC_llubot_1, confirmationData, sizeof(confirmationData)) == 0) {
    Serial.println("Confirmación enviada al emisor.");
  } else {
    Serial.println("Error al enviar la confirmación al emisor.");
  }
}

void Recibir_datos_del_receptor(uint8_t *macAddr, uint8_t *data, uint8_t len) {
  Serial.print("Datos recibidos de: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", macAddr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.print("Longitud del mensaje: ");
  Serial.println(len);
  Serial.print("Mensaje recibido del receptor: ");
  for (int i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  Serial.println();
}

void enviar_datos(int llubot, struct_emisor data){
  if (llubot == 2){
    Serial.print("Enviando mensaje a llubot con IP: ");
    Serial.print(staticIP_llubot2);
    Serial.print(" y dirección MAC: ");
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", MAC_llubot_2[i]);
      if (i < 5) Serial.print(":");
    }
    Serial.println("");
    if (Ping.ping(staticIP_llubot2)) {
      if (esp_now_send(MAC_llubot_2, (uint8_t *) &data, sizeof(data)) == 0) {
        Serial.println("Mensaje enviado exitosamente por ESP-NOW.");
      } else {
        Serial.println("Error al enviar el mensaje por ESP-NOW.");
      }
    } else {
      Serial.println("El llubot 2 no está en línea.");
    }
  }

  if (llubot == 3){
    Serial.print("Enviando mensaje a llubot con IP: ");
    Serial.print(staticIP_llubot3);
    Serial.print(" y dirección MAC: ");
    for (int i = 0; i < 6; i++) {
      Serial.printf("%02X", MAC_llubot_3[i]);
      if (i < 5) Serial.print(":");
    }
    if (Ping.ping(staticIP_llubot3)) {
      if (esp_now_send(MAC_llubot_3, (uint8_t *) &data, sizeof(data)) == 0) {
        Serial.println("Mensaje enviado exitosamente por ESP-NOW.");
      } else {
        Serial.println("Error al enviar el mensaje por ESP-NOW.");
      }
    } else {
      Serial.println("El llubot 3 no está en línea.");
    }
  }
}