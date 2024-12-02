// Prueba básica de movimiento del LLUBOt
void LOOP_prueba_Movimiento(){
  goStraight('B', 7);
  delay(2000);
  goStraight('F', 5);
  delay(2000);
  turn('L', 90);
  delay(2000);
  turn('R', 90);
  goStraight('F', 2);
  Serial.println("Fin de programa");
  
  Serial.print("Mido distancia normal");
  Serial.println(sensorDistanceRead(0));
  delay(2000);
  Serial.print("Mido al frente");
  Serial.println(seeStraight());
  delay(2000);
}

// Prueba básica del sensor de distancias del LLUBOt
void LOOP_prueba_Sensor(){
  Serial.println(sensorDistanceRead(90));
  delay(5);
}