#include "C_comunicaciones.h"
#include <ESP8266WiFi.h>

void SU_main_Receptor() {
    Serial.begin(115200); // Inicializar la comunicación serial
    Serial.println("Configurando receptor ESP-NOW...");

    // Inicializar WiFi y ESP-NOW como receptor (por ejemplo, llubot 2)
    init_wifi(3); 
    Serial.print("Dirección MAC receptor: ");       
    Serial.println(WiFi.macAddress()); 
    init_esp_now(3);

    Serial.println("Receptor listo para recibir datos.");
}

void LOOP_main_Receptor() {
  // No necesitas manejar los datos aquí, ya que el *callback* se ocupa de ellos.
  delay(5000); // Mantén un retardo para evitar bucles rápidos
  Serial.print("Datos recibidos - Tamaño: ");
  Serial.println(sizeof(s_emisor));
  //Comentado para compilacion
  /*
  Serial.print("Temperatura: ");
  Serial.println(s_emisor.temperature);
  Serial.print("Humedad: ");
  Serial.println(s_emisor.humidity);
  Serial.print("Número: ");
  Serial.println(s_emisor.numero);
  */
}
