//*********************************** BUCLE PRINCIPAL *************************************************

// Se debe llamar a la función tipo "LOOP" que se desee

// Variables para el control del tiempo
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

int iteraciones = 1;  // Variable global para contar las iteraciones del parking de LluBots

void loop() {
  LOOP_parkingLlubots();
}

void LOOP_parkingLlubots(){
  // First read the voltage value for the correct functioning of the LLUBot.
  digitalWrite(ledPin, LOW);
  sensorVal=1;//digitalRead(voltagePin);
              // La batería se pone a 1 para ejecutar el código si o si pero debería comprobarse

  // Si NO tenemos suficiente batería:
  if (sensorVal == 0){
    // Turn on the warning LED and show the Low Battery Server:
    digitalWrite(ledPin, HIGH);
    lowBatteryServer();
  }
  
  // Si SÍ tenemos suficiente batería:
  else {
    delay(100);
    /*
    if (id_llubot == 2 || id_llubot == 3){
      actividadSel = 1;
    }
    */
    switch (actividadSel){
      case 0:
        Serial.println("Hey, estoy en el CONTROL REMOTO");
        controlRemoto();
        break;
      default:
        Serial.println("Hey, entré en MODO PARKING LLUBOTS");
        parkingLlubots();
        Serial.print("Iteración NÚMERO ---- : ");
        Serial.print(iteraciones);
        Serial.println(" ---- completada.");
        iteraciones++;
        break;
    }
  }
}

void LOOP_original_predeterminado(){
  // First read the voltage value for the correct functioning of the LLUBot.
  digitalWrite(ledPin, LOW);
  sensorVal=1;//digitalRead(voltagePin);

  // Si NO tenemos suficiente batería:
  if (sensorVal == 0){
    // Turn on the warning LED and show the Low Battery Server:
    digitalWrite(ledPin, HIGH);
    lowBatteryServer();
  }
  
  // Si SÍ tenemos suficiente batería:
  else {
    delay(100);
    switch (actividadSel){
      case 1:
        Serial.println("Hey, entré en la actividad 1");
        //juegoRoomba();
        Serial.println("Hey, salí de la actividad 1");
        break;
      case 2:
        Serial.println("Hey, entré en la actividad 2");
        //maths();
        Serial.println("Hey, salí de la actividad 2");
        break;
      default:
        Serial.println("Hey, estoy en el menú principal");
        //principalMenu();
        Serial.println("Hey, me voy del menú principal");
        break;
    }
  }
}
