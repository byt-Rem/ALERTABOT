#include <Servo.h>
#include <Wire.h>                                          // Librerias de servomotor, lcd y sensor ultrasonico. wire sirve para conectar lcd a la placa.
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);                        // Especificaciones del lcd
Servo miservo;                                             // Servomotor 1, eje de giro el X
Servo miservo2;                                            // Servomotor 2, eje de giro el Y

int DURACION;
int DISTANCIA;                                             // Variable encargada de tomar la función de medir distancia (mediante el uso de un sensor ultrasonico).
int medir_distancia();                                     // Función que mide distancia.

int direccion = 1;                                         // Intervalo de desplazamiento de un gradp (de izquierda a derecha) 
int pos = 0;                                               // Posicion inicial del primer servomotor
int flag = 0;                                              // Variable tipo interruptor,bandera baja.

int pos2 = 45;                                             // Posición inicial del segundo servomotor
int posiciones[] = {45, 90, 135};                          // Posiciones de movimiento del segundo servomotor
int direccion2 = 0;                                        // Direccion para el arreglo de posiciones del segundo servomotor

const int TRIG = 12;                                       // Variable que "lanza" la señal del ultrasonico
const int ECO = 11;                                        // Variable que almacena la señal de regreso del sensor ultrasonico

void setup() {                                             // Void setup, el lugar donde se da inicio (se ejecutara una vez)
  pinMode(TRIG, OUTPUT);                                   // Pin mode nos dice
  pinMode(ECO, INPUT);
  Serial.begin(9600);

  miservo.attach(13);                                      // Pin para el primer servo
  miservo2.attach(10);                                     // Pin para el segundo servo

  // display
  lcd.init();
  lcd.backlight();
}

void loop() {
  //                                                       // Movimiento del primer servo           
  if (flag == 0 && direccion == 1) {                       // Utilizamos el condicional if
    for (pos; pos < 180; pos += 1) {                       // Utilizamos un bucle for
      DISTANCIA = medir_distancia();
      if (DISTANCIA < 20) {
        flag = 1;
        lcd.setCursor(0, 0);                               // Posición donde se coloca el cursor para escribir en el lcd
        lcd.print("alejese!,get out!");                    // Impresión en el lcd
        break;
      }
      miservo.write(pos);
      delay(10);
      lcd.clear();                                         // Limpia lo escrito en el lcd
    }
    direccion = -1;
  }

  if (flag == 0 && direccion == -1) {
    for (pos; pos > 0; pos -= 1) {
      DISTANCIA = medir_distancia();
      if (DISTANCIA < 20) {
        flag = 1;
        lcd.setCursor(0, 0);
        lcd.print("alejese!,get out!");
        break;
      }
      miservo.write(pos);
      delay(10);
      lcd.clear();
    }
    direccion = 1;
  }

  //                                                       // Movimiento del segundo servo entre 45, 90 y 135 grados
  if (flag == 0) {
    miservo2.write(posiciones[direccion2]);                // Mueve el segundo servo a la posición actual
    delay(500);                                            // Pausa para que el cambio de posición sea notorio

      
    direccion2 += 1;                                       // Cambia el índice para la siguiente posición
    if (direccion2 > 2) {                                  // Regresa al inicio del arreglo
      direccion2 = 0;
    }
  }

  
  DISTANCIA = medir_distancia();                           // Verificar distancia y resetear flag si no hay obstáculo
  if (DISTANCIA > 20) {
    flag = 0;
  }

  delay(10);
}

int medir_distancia() {
  digitalWrite(TRIG, HIGH);
  delay(1);
  digitalWrite(TRIG, LOW);
  int DUR = pulseIn(ECO, HIGH);
  int DIST = DUR / 58.2;
  return DIST;
}
