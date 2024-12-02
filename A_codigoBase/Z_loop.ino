//*********************************** BUCLE PRINCIPAL *************************************************

// Se debe llamar a la función tipo "LOOP" que se desee


// Variables para el control del tiempo
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void loop() {
  controlRemoto();
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
