# LLubot-Proyect
 Código Arduino para el proyecto grupal final de la asignatura R.I.S.A.
 
## Tipos de fichero
Hay 4 clases de fichero:
- A: Archivos reutilizados del código previo. Incluyen el código para inicializar y mover el LLU-Bot.
- C: Archivos correspondientes a la parte de COMUNICACIÓN entre LLU-Bots (ESP8266).
- P: Archivos para realizar pruebas de los distintos módulos (comunicación, movimiento, sensorización, etc.)
- S: Archivos correspondientes al SERVIDOR WEB y al CONTROL REMOTO de los LLU-Bots.
- Z: Archivos correspondientes a las funciones de setup y loop propias de arduino.

## Creación y Uso de Código
Si quieres crear nuevas funcionalidades, crea un ARCHIVO NUEVO .ino según el código de nombres y desarrolla el código dentro de funciones que luego pueden ser llamadas desde el set-up y el loop.

Para probar código, llama a las funciones correspondientes en los archivos Z_setup.ino y Z_loop.ino y comenta las que no necesites. Nunca debe modificarse la parte "NO-TOCAR" del setup (parte inicial).

## USO DEL GIT
Pasos para trabajar de manera conjunta:
- 1) Instalación de Git y creación de cuenta GitHub. Descarga también GitHub Desktop para trabajar cómodamente con Git (opcional).
  2) Clonar el repositorio en tu PC.
  3) CADA VEZ QUE COMIENCES A TRABAJAR: Avisa por el grupo y EJECUTA PULL (actualizar los cambios en tu PC).
  4) CADA VEZ QUE TERMINES DE TRABAJAR: Avisa por el grupo y EJECUTA PUSH (actualizar tus cambios en el proyecto conjunto).
