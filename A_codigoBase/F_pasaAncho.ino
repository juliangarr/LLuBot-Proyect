//Funcion que da true si el LLubot puede pasar(debido la curva de la sensibilidad del sensor ninguna medida deberia tomarse menor a 23 cm)
float pasaLLUbot(float ancho = 20){
  float pasoLL = 0.0;
  float readCm = 0;      // for the reading of the distance
  int minDegFront = 45;  // for the 
  int maxDegFront = 135; // 
  float angulo;
  float anchura;
  float ancho2 = ancho/2 + 5; // +5 añadido como pequeño intervalo de confianza
  float largos[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
  
  // It makes the LLUBot watch if the front path is clear for the entire width of the LLUBot 
  for (int i=minDegFront; i<=maxDegFront; i+=10) {  // from 45º to 136º to make sure
    readCm = sensorDistanceRead(i);
    if (readCm < 60){
      angulo = abs(i-90);
      angulo = angulo*(M_PI/180);
      anchura = sin(angulo)*readCm; // Calculamos la anchura disponible delante del LLUBot

      // Calculamos la distancia a la que se encuentra el objeto
      if ( anchura > ancho2 ) {
        largos[(i-minDegFront)/10] = cos(angulo)*readCm;
      }
      else
        break;

      Serial.print("Angulo: ");
      Serial.print(i);
      Serial.print(" Distancia: ");
      Serial.print(readCm);
      Serial.print(" Anchura: ");
      Serial.println(anchura);
    }
  }

  // Recorre el array de largos y calculamos la distancia a moverse
    for (int i=0; i<10; i++) {
        if (largos[i] != -1) {
            // Calculate de median
            
        }
    }

  return pasoLL;
}