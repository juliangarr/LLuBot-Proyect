#include "C_comunicaciones.h"

// VARIABLES PARA EL CONTROL REMOTO
int angle1 = 0;
int angle2 = 0;
int cmAv = 0;
int cmRe = 0;
int movimiento = 0;

// FUNCTION DEMO (Debugging):
int veces_demo = 0;

void demo(){
  veces_demo++;
  Serial.print("Hola soy Demo : -----  ");
  Serial.println(veces_demo);
}

// FUNCTION 10: CONTROL REMOTO - Menú Inicial LluBots: ------------------------------------------------------------------------------------------//
void controlRemoto(){

  // Stay within the Remote Control until the order is told
  while (actividadSel == 0) {
    // --------------------- CHECK WIFI -----------------------------
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    //------------------------ DO NOT CHANGE --------------------------//
    WiFiClient client = server.available();
    while (!client) {
      // Check if a client has connected
      client = server.available();
      actividadSel = s_emisor.actividad;
      if (actividadSel != 0){
        break;
      }
    }
    
    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available()) {
      delay(1);
      if (actividadSel != 0){
        break;
      } 
    }
    
    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    //---------------------- END OF DO NOT CHANGE ---------------------//
   
    // Match the request
    int pos = request.lastIndexOf('-');

    next_order=0;

    //-------------------------- ALGORITHMS ---------------------------//
    // Verifica si la solicitud contiene cada comando y extrae el parámetro correspondiente
    if (request.indexOf("/MD") != -1) {
      value = 0;  // Identificador para Girar Derecha
      int angle1Index = request.indexOf("angle1=");
      if (angle1Index != -1) {
        angle1 = request.substring(angle1Index + 7, request.indexOf('&', angle1Index)).toInt();
      }
    }
    else if (request.indexOf("/MI") != -1) {
      value = 1;  // Identificador para Girar Izquierda
      int angle2Index = request.indexOf("angle2=");
      if (angle2Index != -1) {
        angle2 = request.substring(angle2Index + 7, request.indexOf('&', angle2Index)).toInt();
      }
    }
    else if (request.indexOf("/MF") != -1) {
      value = 2;  // Identificador para Avanzar
      int cmAvIndex = request.indexOf("cmAv=");
      if (cmAvIndex != -1) {
        cmAv = request.substring(cmAvIndex + 5, request.indexOf('&', cmAvIndex)).toInt();
      }
    }
    else if (request.indexOf("/MB") != -1) {
      value = 3;  // Identificador para Retroceder
      int cmReIndex = request.indexOf("cmRe=");
      if (cmReIndex != -1) {
        cmRe = request.substring(cmReIndex + 5, request.indexOf('&', cmReIndex)).toInt();
      }
    }
    else if (request.indexOf("/SA") != -1) {
      value = 4;  // Identificador para START
      struct_emisor mensaje3;
      mensaje3.actividad = 1;
      mensaje3.dist_to_move = 0.0;
      enviar_datos(2, mensaje3);
      enviar_datos(3, mensaje3);
    }
    //---------------------- END OF ALGORITHMS ------------------------//

   
    //***************************A PARTIR DE AQUÍ VAMOS A TRABAJAR**********************************************************************************************************************************************************************************  
    // Return the response
    client.println("HTTP/1.1 200 OK\
    Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<!DOCTYPE html>\
    <html>\
      <head>\
        <title> LLUBot </title>");

    //************************************************************** CSS ************************************************************************************************************************************************************//
    client.println("<style>\
      body {background-color: #7c05a3; text-align: center; color: white; font-family: Verdana}\
        .main button {  height: 150px; width: 80%; font-size: 25px; }\
        .num {padding: 30px; text-decoration: none; display: inline-block; width: 60px; font-size: 25px}\
        .button {background-color: #7c05a3; border-radius: 12px; border: none; color: white; padding: 20px; text-align: center; text-decoration: none; display: inline-block; font-size: 40px}\
      h1 {font-size: 50px;}\
      a {color: white; text-decoration: none}\
      body {font-size: 30px; font-family: Verdana}\
    </style>");

    client.println("<style>\
      .button-green {\
        background-color: green;\
        color: white;\
        border-radius: 12px;\
        padding: 20px;\
        font-size: 40px;\
        border: none;\
      }\
    </style>");

    //************************************************************* HTLM ************************************************************************************************************************************************************//
    client.println("<head><meta charset=\"UTF-8\"></head>");
    
    client.println("<body>\
      <div class='main'>\
        <h1>LLUBot</h1>\
        <h2>Control REMOTO</h2>\
        <form action=\"/MD\" method=\"get\">\
          <label for='angle1'>Girar DERECHA(grados):</label>\
          <input type='number' name='angle1' min='0' max='180' value='" + String(angle1) + "'>\
          <input type='submit' value='Enviar giro DCHA' class='button-green'>\
        </form>\
        <form action=\"/MI\" method=\"get\">\
          <label for='angle2'>Girar IZQUIERDA(grados):</label>\
          <input type='number' name='angle2' min='0' max='180' value='" + String(angle2) + "'>\
          <input type='submit' value='Enviar giro IZQ' class='button-green'>\
        </form>\
        <form action=\"/MF\" method=\"get\">\
          <label for='angle2'>AVANZAR(centimetros):</label>\
          <input type='number' name='cmAv' min='0' value='" + String(cmAv) + "'>\
          <input type='submit' value='Enviar avance' class='button-green'>\
        </form>\
        <form action=\"/MB\" method=\"get\">\
          <label for='angle2'>RETROCEDER(centimetros):</label>\
          <input type='number' name='cmRe' min='0' value='" + String(cmRe) + "'>\
          <input type='submit' value='Enviar retroceso' class='button-green'>\
        </form>");

        // Añade el nuevo botón en el Server del LLuBot Estacion
        if (id_llubot == 1) {
          client.println("        <form action=\"/SA\" method=\"get\">\
              <input type='submit' value='START Demo' class='button-green'>\
            </form>");
        }

    client.println("<br><br><br><br><br><br><br><br><br><br><br><br>\
          <p>Hola Iván</p>\
          </div>\
        </body>\
      </html>");



    //**************ÓRDENES RECIBIDAS**********************//  
    /*
    actividadSel=value;   // esto es del codigo anterior, debería ser distinto para usarlo en nuestro codigo
    Serial.println("Actividad actual: 0");
    Serial.print("Actividad seleccionada: ");
    Serial.println(actividadSel);
    */

    // Bool para intentar evitar solicitudes dobles (Checkear si es realmente necesario)
    bool solicitudProcesada = false;

    // Dentro del ciclo principal de cliente
    if (!solicitudProcesada) {
      solicitudProcesada = true;
      movimiento = value;         // Creo que movimiento no es necesario
      switch(movimiento){
        case 0:
          turn('R', angle1);
          break;
        
        case 1:
          turn('L', angle2);
          break;
        
        case 2:
          goStraight('F', cmAv);
          break;

        case 3:
          goStraight('B', cmRe);
          break;
          
        case 4:
          actividadSel = 1;
        
        /*  
        case 4:   // Caso para debuggear con Demo y ver si la solicitud se envia más de una vez (Santi sabe)
          demo();
          break;
        */

        case 5:   // Caso para evitar que si se procesan solicitudes dobles, al poner value a 5 a continuación, no haga efecto.
                  // Creo que es INUTIL porque lo que ocurre es que se procesa la orden de request y la de fo... y entonces vuelve a setear "value" a un valor distinto de 5
                  // Lo que hay que hacer es EVITAR PROCESAR LA SOLICITUD "F...."
          break;

        default:
          Serial.println("ERROR en SWITCH movimiento");
      }
      value = 5;
    }

    //************************* FIN DEL SERVIDOR ***************************************************/

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
    client.flush();
  }
}
