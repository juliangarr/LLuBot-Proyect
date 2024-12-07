//***********************************CONFIGURACIÓN*************************************************

// Se debe llamar a la función tipo "SU" que se desee

// VARIABLE QUE CONTROLA EL ID DEL LLUBOT
int id_llubot = 1; // Cambiar el ID según el LLUBot

// VARIABLES PARA EL CONTROL DEL PARKING DE LLUBOTS
int iteraciones = 0;  // Variable global para contar las iteraciones del parking de LluBots
bool initialize = false;  // Variable global para inicializar el WiFi y ESPNow (solo primera iteración)

void setup() {
  //----------------------------------------------------------------------------------------------
  //*********************************** NO TOCAR *************************************************
  //----------------------------------------------------------------------------------------------
  Serial.begin(115200);
  delay(10);
 
  pinMode(voltagePin, INPUT); 
  pinMode(ledPin, OUTPUT);

  sensorServo.attach(servoPin);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");


//************IMPORTANTE*************//
 
  // IMPRIMIR LA DIRECCIÓN IP
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

//***********************************//

  // Steppers speed
  motorR.setSpeed(chosenSpeed);
  motorL.setSpeed(chosenSpeed);   

  // Init servo at 90 degrees
  sensorServo.write(servoAngle); 

  Serial.print("Empiezo en la actividad: ");
  Serial.println(actividadSel);
  actividadSel = 0;
  valueA1 = 0; // initialization in 0 for Roomba in activity 1
  valueA2 = 0; // initialization in 0 for activity 2

  //----------------------------------------------------------------------------------------------
  //*********************************** FIN NO TOCAR *********************************************
  //----------------------------------------------------------------------------------------------

  iteraciones = 0;  // Inicializar la variable global para contar las iteraciones del parking de LluBots
  initialize = false;  // Inicializar la variable global para inicializar el WiFi y ESPNow (solo primera iteración)
}
 
