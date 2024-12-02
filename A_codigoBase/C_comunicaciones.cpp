#include "comunicaciones.h"

IPAddress staticIP_AP(192,168,4,90);
IPAddress staticIP_llubot2(192,168,4,95);
IPAddress staticIP_llubot3(192,168,4,96);

// Direcciones MAC
uint8_t MAC_llubot_1[] = {0x5E, 0xCF, 0x7F, 0xB5, 0x58, 0x66};
uint8_t MAC_llubot_2[] = {0x1A, 0xFE, 0x34, 0x9D, 0xAD, 0xDE};
uint8_t MAC_llubot_3[] = {0x18, 0xFE, 0x34, 0x9D, 0xAD, 0xDE};

struct_emisor s_emisor;

void init_wifi(int llubot){
  //const char* ssid = "ESP32_AP";
  //const char* password = "pwordLlubot";

  // Configuración de IP estática para el punto de acceso (AP) y estaciones (STA)
  IPAddress gateway(192,168,4,1);   // Dirección del gateway del AP
  IPAddress subnet(255,255,255,0);  // Máscara de subred del AP
  if (llubot == 1){
    // Configurar el ESP8266 como punto de acceso
    WiFi.softAP(ssid, password);              // Configurar el AP con SSID y contraseña
    WiFi.softAPConfig(staticIP_AP, gateway, subnet); // Asignar IP, gateway y subred al AP
  }
  if (llubot == 2){
    // Configurar el ESP8266 como un cliente (modo estación)
    WiFi.mode(WIFI_STA);                // Cambiar el ESP8266 a modo estación (STA) para conectarse a una red WiFi
    WiFi.config(staticIP_llubot2, gateway, subnet);  // Configurar la IP, gateway y subred estáticos
    WiFi.begin(ssid, password);         // Conectarse a la red WiFi utilizando el SSID y la contraseña definidos
    // Esperar hasta que se establezca la conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {  // Verificar si el estado de la conexión es "Conectado"
      delay(500);                           // Esperar 500 ms antes de volver a verificar
      Serial.print(".");                    // Imprimir un punto como indicador de espera
    }
  }
  if (llubot == 3){
    // Configurar el ESP8266 como un cliente (modo estación)
    WiFi.mode(WIFI_STA);                // Cambiar el ESP8266 a modo estación (STA) para conectarse a una red WiFi
    WiFi.config(staticIP_llubot3, gateway, subnet);  // Configurar la IP, gateway y subred estáticos
    WiFi.begin(ssid, password);         // Conectarse a la red WiFi utilizando el SSID y la contraseña definidos
    // Esperar hasta que se establezca la conexión WiFi
    while (WiFi.status() != WL_CONNECTED) {  // Verificar si el estado de la conexión es "Conectado"
      delay(500);                           // Esperar 500 ms antes de volver a verificar
      Serial.print(".");                    // Imprimir un punto como indicador de espera
    }
  }
}

void init_esp_now(int llubot){
  
  // Inicializar ESP-NOW
  if (esp_now_init() == 0) {            // Intentar inicializar ESP-NOW y verificar si es exitoso
    Serial.println("ESP-NOW inicializado con éxito."); // Mensaje de éxito si ESP-NOW inicia correctamente
  } else {
    Serial.println("Error al inicializar ESP-NOW.");   // Mensaje de error si ESP-NOW falla al iniciar
    return;                             // Terminar la configuración si ESP-NOW no inicia
  }

  if (llubot == 1){
    
    // Configurar ESP-NOW como emisor
    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);  // Configurar el ESP8266 como controlador (emisor)

    // Añadir el receptor de ESP-NOW (dirección MAC del receptor)
    esp_now_add_peer(MAC_llubot_2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0); // Añadir el receptor con su dirección MAC
    esp_now_add_peer(MAC_llubot_3, ESP_NOW_ROLE_SLAVE, 1, NULL, 0); // Añadir el receptor con su dirección MAC
    
    // Registrar la función de callback para recibir datos
    esp_now_register_recv_cb(Recibir_datos_del_receptor);   // Registrar la función de callback para cuando se reciben datos del receptor
  }

  if (llubot == 2){
    // Registrar la función de callback para recibir datos
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);  // Configurar el ESP8266 como receptor de ESP-NOW
    esp_now_register_recv_cb(Recibir_datos_del_emisor);       // Registrar la función como callback al recibir datos del emisor

    // Añadir al emisor como peer para enviarle la confirmación
    esp_now_add_peer(MAC_llubot_1, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0); // Añadir el emisor con su MAC
  }

  if (llubot == 3){
    // Registrar la función de callback para recibir datos
    esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);  // Configurar el ESP8266 como receptor de ESP-NOW
    esp_now_register_recv_cb(Recibir_datos_del_emisor);       // Registrar la función como callback al recibir datos del emisor

    // Añadir al emisor como peer para enviarle la confirmación
    esp_now_add_peer(MAC_llubot_1, ESP_NOW_ROLE_CONTROLLER, 1, NULL, 0); // Añadir el emisor con su MAC
  }

}

// Función de callback del receptor que se llama cuando se reciben datos por ESP-NOW del emisor
void Recibir_datos_del_emisor(uint8_t *macAddr, uint8_t *data, uint8_t len) {

  Serial.print("Datos recibidos de: ");  // Mensaje de cabecera al recibir datos
  for (int i = 0; i < 6; i++) {          // Bucle para mostrar la dirección MAC del remitente
    Serial.printf("%02X", macAddr[i]);   // Imprimir cada byte de la MAC en formato hexadecimal
    if (i < 5) Serial.print(":");        // Añadir ":" entre los bytes de la MAC, excepto al final
  }
  Serial.println();                      // Imprimir una nueva línea después de la MAC

  Serial.print("Longitud del mensaje: "); // Mensaje para mostrar la longitud del mensaje recibido
  Serial.println(len);                   // Imprimir la longitud del mensaje
  
  Serial.println("Mensaje recibido del emisor: ");    // Cabecera para el contenido del mensaje
  
  memcpy(&s_emisor, data, sizeof(s_emisor));

  // Enviar mensaje de confirmación de vuelta al emisor
  String confirmationMessage = "Mensaje recibido correctamente desde el receptor";  // Crear un mensaje de confirmación
  uint8_t confirmationData[confirmationMessage.length() + 1];  // Crear un array de bytes para el mensaje de confirmación
  confirmationMessage.getBytes(confirmationData, sizeof(confirmationData)); // Convertir el mensaje a bytes

  // Enviar el mensaje de confirmación al emisor usando ESP-NOW
  if (esp_now_send(MAC_llubot_1, confirmationData, sizeof(confirmationData)) == 0) { // Enviar el mensaje de confirmación
    Serial.println("Confirmación enviada al emisor."); // Imprimir mensaje de éxito al enviar la confirmación
  } else {
    Serial.println("Error al enviar la confirmación al emisor.");  // Imprimir mensaje de error si el envío falla
  }

}

// Función de callback del emisor que se llama cuando se reciben datos por ESP-NOW del receptor
void Recibir_datos_del_receptor(uint8_t *macAddr, uint8_t *data, uint8_t len) {
  Serial.print("Datos recibidos de: ");   // Imprimir la dirección MAC del receptor
  for (int i = 0; i < 6; i++) {           // Bucle para mostrar la dirección MAC
    Serial.printf("%02X", macAddr[i]);    // Imprimir cada byte de la dirección MAC
    if (i < 5) Serial.print(":");         // Agregar ":" entre los bytes de la dirección MAC
  }
  Serial.println();                       // Imprimir una línea en blanco después de la MAC

  Serial.print("Longitud del mensaje: "); // Imprimir la longitud del mensaje recibido
  Serial.println(len);                    // Imprimir la longitud del mensaje
  
  Serial.print("Mensaje recibido del receptor: ");     // Imprimir el contenido del mensaje recibido
  for (int i = 0; i < len; i++) {         // Imprimir cada byte del mensaje recibido
    Serial.print((char)data[i]);          // Convertir el byte a carácter ASCII y mostrarlo
  }
  Serial.println();                       // Imprimir una línea en blanco después del mensaje
}

void enviar_datos(int llubot, struct_emisor data){
  if (llubot == 2){
    // Escanear la IP específica para verificar si el dispositivo está en línea
    if (Ping.ping(staticIP_llubot2)) {
      // Enviar el mensaje a través de ESP-NOW al receptor
      if (esp_now_send(MAC_llubot_2, (uint8_t *) &data, sizeof(data)) == 0) { // Enviar datos al receptor y verificar éxito
        Serial.println("Mensaje enviado exitosamente por ESP-NOW."); // Mensaje de éxito en el envío
      } else {
        Serial.println("Error al enviar el mensaje por ESP-NOW.");   // Mensaje de error si falla el envío
      }
    }
  }

  if (llubot == 3){
    // Escanear la IP específica para verificar si el dispositivo está en línea
    if (Ping.ping(staticIP_llubot3)) {
      // Enviar el mensaje a través de ESP-NOW al receptor
      if (esp_now_send(MAC_llubot_3, (uint8_t *) &data, sizeof(data)) == 0) { // Enviar datos al receptor y verificar éxito
        Serial.println("Mensaje enviado exitosamente por ESP-NOW."); // Mensaje de éxito en el envío
      } else {
        Serial.println("Error al enviar el mensaje por ESP-NOW.");   // Mensaje de error si falla el envío
      }
    }
  }
}
