#include <math.h>
bool initialize = false;
bool paso;

void start() {
  if (initialize == false) {
    // Inicializar Wifi de los 3 llubots
    initWiFi(1);
    initWiFi(2);
    initWiFi(3);
    Serial.println("Se ha inicializado el WIFI");

    // Inicializar ESPNow de los 3 llubots
    initESPNow(1);
    initESPNow(2);
    initESPNow(3);
    Serial.println("Se ha inicializado la comunicacion por ESPNow");
    
    initialize = true;
  }
  
  //paso 1 
  paso = pasaLLUbot();

  if (paso){
    //LLubot tendria que comunicar que va a pasar y hacer 
    Serial.println("Caminando hacia delante");
    goStraight('F', 10);
    
  }
  else{
    //comunicar a los LLUbot esclavos que han de moverse hacia los lados y que hagan
    Serial.println("Caminando hacia atras");
    goStraight('B', 10);
  }
}