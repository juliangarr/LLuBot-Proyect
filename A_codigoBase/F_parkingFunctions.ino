#include "MedianFilterLib.h" // Hay que instalar la libreria MedianFilterLib de Luis LLamas desde el gestor de librerias: https://github.com/luisllamasbinaburo/Arduino-MedianFilter

/*
Funcion que devuelve una medida: 
        - Si la medida es 0.0 significa hay un obstaculo en el scanner del LLUBot y NO se puede aparcar
        - Si la medida es mayor que 0.0, esta medida representa la distancia que debe recorrer el LLUBot para SI aparcar o volver a escanear
*/
float scannerLlubot(float ancho = 20){
  float avanceLL = 0.0;  // Variable para la distancia que debe recorrer el LLUBot para aparcar
  float readCm = 0;      // Variable para la lectura de la distancia en cm de la medida pasado un angulo al sensor
  int minDegFront = 45;  // Angulo minimo para el escaneo frontal
  int maxDegFront = 135; // Angulo maximo para el escaneo frontal
  float angulo;          // Variable hacer cálculos con el angulo (respecto a la perpendicular) en radianes
  float anchura;         // Variable para hacer calculos con la anchura disponible delante del LLUBot (la mitad de la anchura)
  float ancho2 = ancho/2 + 5; // +5 añadido como pequeño intervalo de confianza
  float cosenos[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};  // Array para guardar los catetos adyacentes de los angulos
  bool pasoLL = true;    // Variable para saber si el LLUBot puede pasar
  
  // Bucle que para cada angulo, calcula la distancia (hipotenusa) y la anchura (cateto opuesto) y el largo (cateto adyacente)
  for (int i=minDegFront; i<=maxDegFront; i+=10) {  // from 45º to 135º to make sure
    readCm = sensorDistanceRead(i);
    if (readCm < 60){ // Si la distancia es mayor a 60cm, no se tiene en cuenta debido a la curva de sensibilidad del sensor
      angulo = abs(i-90);  // Calculamos el angulo respecto a la perpendicular del LLUBot para aplicar trigonometria
      angulo = angulo*(M_PI/180); // Convertimos el angulo a radianes
      anchura = sin(angulo)*readCm; // Calculamos la anchura disponible (la mitad de la anchura) delante del LLUBot con el cateto opuesto

      // Para cada angulo en el que la anchura sea mayor que la mitad de la anchura del LLUBot (el LLUBot de momento puede pasar), se calcula el la distancia frontal (cateto adyacente)
      // Calculamos la distancia a la que se encuentra el objeto 
      if ( anchura > ancho2 ) {
        cosenos[(i-minDegFront)/10] = cos(angulo)*readCm;
      }
      else{
        pasoLL = false;
        break;  // Si no se cumple la condicion, el LLUBot no puede pasar salimos del bucle para devolver 0.0
      }

      // Debugging (Comentar para no imprimir en el monitor serie)
      Serial.print("Angulo: ");
      Serial.print(i);
      Serial.print(" Distancia: ");
      Serial.print(readCm);
      Serial.print(" Anchura: ");
      Serial.println(anchura);
      Serial.print(" Largo: ");
      Serial.println(cosenos[(i-minDegFront)/10]);
    }
  }

  // Recorre el array de catetos adyacentes y calculamos la distancia a moverse
  if (pasoLL){
    // Usamos un filtro de mediana para eliminar los valores atipicos
    MedianFilter<float> medianFilter(10);
    for (int i=0; i<10; i++) {
        if (cosenos[i] != -1) {
            // Añadir un nuevo valor al filtro
            medianFilter.AddValue(cosenos[i]);
        }
    }

    // Calculamos la mediana de los valores filtrados
    avanceLL = medianFilter.GetFiltered();;
  }

  return avanceLL;
}

/*
Una vez el LLUBot ha decidido que puede pasar, se mueve hacia delante, pero debe comprobar que realmente se ha aparcado entre dos LLUBots
Porque puede pasar que no haya obstaculos en el scanner y tampoco LLUBots En el rango de 60 cm, entonces una vez avance deberá volver a escanear

Funcion que devuelve un booleano:
        - Si devuelve true, significa que el LLUBot ha aparcado correctamente entre dos obstaculos (LLUBots) a ambos lados
        - Si devuelve false, significa que el LLUBot no ha aparcado correctamente y debe volver a escanear
*/
bool verificarParking(){
  // Verificamos si en los laterales hay LLUBots (obstaculos)
  if (sensorDistanceRead(0) < 60 && sensorDistanceRead(180) < 60){
    return true;
  }
  else{
    return false;
  }
}