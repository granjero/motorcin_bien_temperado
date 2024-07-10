#include <AccelStepper.h>
#include "musica.h"
#include "melodia.h"

#define paso 8
#define direccion 7

AccelStepper stepper(AccelStepper::DRIVER, paso, direccion);

int tempo = 66; // Tempo
int cantNotas = sizeof(melodia) / sizeof(melodia[0]) / 2; // se divide por 2 porque el array tiene frecuencia - tempo (dos valores)
int negra = (60000 * 4) / tempo; // calcula el tiempo de una negra en ms (60s/tempo)*4 tiempos
int divisor = 0, duracionNota = 0;
long intervalo = 0;

void setup() {
  stepper.setMaxSpeed(5000);
  stepper.setAcceleration(150);
  stepper.setSpeed(0);

  Serial.begin(115200);
   Serial.println("Inicio : ");
}

void loop() {
  // itera sobre el array de notas de la melodia, tiene dos por coso
  for (int thisNote = 0; thisNote < cantNotas * 2; thisNote += 2) {
    // calcula la duracion de cada nota
    divisor = pgm_read_word(&melodia[thisNote + 1]);
    duracionNota = divisor > 0 ? (negra / divisor) : (negra / abs(divisor) * 1.5); // duracion negativa es como el punto en la nota

    Serial.print(" Duracion en ms: ");
    Serial.print(duracionNota);
    Serial.print(" Frecuencia de esta nota: ");
    Serial.print(pgm_read_word(&melodia[thisNote]));
    Serial.println("");

    stepper.setSpeed(pgm_read_word(&melodia[thisNote]));
    unsigned long tiempoNotaComienzo = millis();

    while (1) {
      stepper.runSpeed();
      if (millis() - tiempoNotaComienzo >= duracionNota) break;  // suena la nota durante el tiempo de duracion
    }
  }
  stepper.setSpeed(pgm_read_word(&melodia[cantNotas - 1]));
  stepper.runToNewPosition(stepper.currentPosition() + 5000);
 
  stepper.disableOutputs();
  delay(300000); // 5 minutos
  stepper.enableOutputs();
}