// FUNCTION 4: DISTANCE READING: ------------------------------------------------------------------------------------------//
// Modificada para incluir delay mas largo también en el ángulo de 180º
float sensorDistanceRead(float servoAngle_) { 

  // First you read the angle you are given and move the servo motor:
  float chosenServoAngle=servoAngle_;
  sensorServo.write(chosenServoAngle);

  // Then choose the delay for the required angle, for 0º, 90º and 180º, the required time is longer:
  if ((chosenServoAngle == 90) || (chosenServoAngle == 0) || (chosenServoAngle == 45) || (chosenServoAngle == 135) || (chosenServoAngle == 180)) {
    delay(500); // servo movement large pause
  }
  else {
    delay(50);  // servo movement small pause
  }

  float referenceMv = 1240; // after testing the sensor, 1240 mV seem to fit better

  // The distance in cm are calculated through the analog pin and the getsensorDistance function:
  float val = analogRead(sensorPin);
  
  // Then choose the delay for the required angle, for 0º, 90º and 180º, the required time is longer:
  if ((chosenServoAngle == 90) || (chosenServoAngle == 0) || (chosenServoAngle == 45) || (chosenServoAngle == 135) || (chosenServoAngle == 180)) {
    delay(500); // sensor reading large pause
  }
  else {
    delay(50);  // sensor reading small pause
  }
  delay(50); //  pause
  float mV = (val * referenceMv) / 1023;   // using the voltage reference and the resolution of A0
  float cm = getsensorDistance(mV) - 13; // the distance calculated with the sensor with offset

  return cm;
}
//-------------------------------------------------------------------------------------------------------------------------//

// FUNCTION 5: GET sensorDistance: ----------------------------------------------------------------------------------------//
float getsensorDistance(int mV) {
   if (mV > mVinterval * tableEntries - 1) {

      // To check the correct distance calculations, uncomment the following two code lines:
      //Serial.print("distancia predefinida:");
      //Serial.println(sensorDistance[tableEntries - 1]);

      return sensorDistance[tableEntries - 1];
   }
   else {
      int index = mV / mVinterval;
      float frac = (mV % 100) / (float)mVinterval; // 100 mV for the interval, but in integer
      
      // To check the correct distance calculations, uncomment the following 15 code lines:
      /*
      Serial.print(index);
      Serial.print("=");
      Serial.print(mV);
      Serial.print("/");
      Serial.println(mVinterval);
      Serial.print("distancia calculada:");
      Serial.print(sensorDistance[index]);
      Serial.print("-((");
      Serial.print(sensorDistance[index]);
      Serial.print("-");
      Serial.print(sensorDistance[index+1]);
      Serial.print(")*");
      Serial.print(frac);
      Serial.print(")=");
      Serial.println(sensorDistance[index] - ((sensorDistance[index] - sensorDistance[index + 1]) * frac));
      */
      return sensorDistance[index] - ((sensorDistance[index] - sensorDistance[index + 1]) * frac);
   }
}
//-------------------------------------------------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------------------------------------------------//


//------------------------------ SPECIAL FUNCTIONS FOR DIDACTIC USE -------------------------------------------------------//

// FUNCTION 6: SEE STRAIGHT: ----------------------------------------------------------------------------------------------//
float seeStraight() {

  // First the variables are created:
  float minCm = 23.0;    // 23 cm is the minimum distance to run, as it's the LLUBot's larger distance (center in the sensor)
  float readCm = 0;      // for the reading of the distance
  float readCm90 = 0;    // it has to run forward, so the main distance will be the one in 90 degrees front
  int minDegFront = 53;  // for the 
  int maxDegFront = 127; // 

  // It makes the LLUBot watch if the front path is clear for the entire width of the LLUBot 
  for (int i=minDegFront; i<maxDegFront; i++) {  // from 45º to 136º to make sure
    readCm = sensorDistanceRead(i);
    if (i == 90) {
      readCm90 = sensorDistanceRead(i);
    }
    if (readCm > minCm) {
      minCm = readCm;
    }
  }

  if (readCm < readCm90) {
    return readCm;
  }
  else {
    return readCm90;
  }
}
