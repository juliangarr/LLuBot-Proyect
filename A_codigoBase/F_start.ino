#include <math.h>
#include "comunicaciones.h"

void parkingLlubots() {
  bool estacion = (id_llubot == 1)   // Variable para saber si el LLUBot es la estación

  if (initialize == false) {
    // Inicializar Wifi de los 3 llubots (segun el ID)
    initWiFi(id_llubot);
    Serial.println("Se ha inicializado el WIFI");

    // Inicializar ESPNow de los 3 llubots (segun el ID)
    initESPNow(id_llubot);
    Serial.println("Se ha inicializado la comunicacion por ESPNow");
    
    initialize = true;
  }
  
  // CODIGO PARA LLUBOT ESTACION (Emisor/Movimiento)
  if(estacion){
    //PASO 1: Escanear el entorno para el LLUBot Estación
    float avanceLL = scannerLlubot();

    //PASO 2: Comprobar si se puede aparcar
    if(avanceLL > 0.0){
      //PASO 3.1: PUEDE PASAR. Avanzar
      goStraight('B', avanceLL);        // Avaanzar hacia delante. 
                                        // ¡¡¡¡¡ IMP !!!!!! USAMOS 'B' PARA AVANZAR HACIA DELANTE PORQUE LA CONFIGURACION DE LOS LLUBOTS ESTA DADO INVERTIDA

      //PASO 4: VERIFICAR APARCAMIENTO. Comprobar si el LLUBot está entre dos LLUBots (obstaculos)
      if(verificarParking()){
        //PASO 5: APARCAMIENTO CORRECTO. Girar el LluBot Estación para finalizar aparcamiento
        goStraight('L', 90.0);    // Girar 90 grados a la izquierda

        // Devolver al control remoto
        actividadSel = 0;
      }
    }

    //PASO 3.2: NO PUEDE PASAR. Comunicar a los LLUbot esclavos que han de moverse hacia los lados para ampliar el espacio
    else{
      struct_emisor mensaje;
      mensaje.dist_to_move = 5.0;    // Distancia a moverse hacia los lados. Predefinida a 5.0 cm
      enviar_datos(2, mensaje);
      enviar_datos(3, mensaje);
    }
  }

  // CODIGO PARA LLUBOT ESCLAVO (Receptor/Estático)
  else{
    //PASO 1: Recibir mensaje del LLUBot Estación (se supone que con callback)
    // Recibir_datos_del_emisor(); no necesario en principio

    //PASO 2: Comprobar si tiene que moverse hacia los lados
    if (s_emisor.dist_to_move != 0.0) {   // DUDA: Como sabe que es s_emisor? si la estacion todavía no ha enviado datos
      //PASO 3: Moverse hacia los lados
      goStraight('B', s_emisor.dist_to_move);    // Desplazarse (SE SUPONE YA ORIENTADO CON EL CONTROL REMOTO)
    }
  }
}