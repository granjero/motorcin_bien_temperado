#include <AccelStepper.h>
#include <Button.h>
#include "musica.h"
#include "melodia.h"

#define direccion 9
#define paso 10
#define habilita 11
#define boton 12

Button boton_inicio(boton, 1000);
AccelStepper stepper(AccelStepper::DRIVER, paso, direccion);

int tempo = 66;                                            // Tempo
int cantNotas = sizeof(melodia) / sizeof(melodia[0]) / 2;  // se divide por 2 porque el array tiene frecuencia - tempo (dos valores)
int negra = (60000 * 4) / tempo;                           // calcula el tiempo de una negra en ms (60s/tempo)*4 tiempos
int divisor = 0, duracionNota = 0;

//bool cantar = false;

void setup() {
  Serial.begin(115200);
  Serial.println(F("jm. Buenos Aires, 2024."));
  Serial.println(F("Diversos materiales, motor paso a paso, código, electricidad."));
  Serial.println(F(""));
  Serial.println(F("El motorcín bien temperado."));
  Serial.println(F("Preludio nro.1 en DO major, BWV 846 Johann Sebastian Bach"));

  stepper.setPinsInverted(false, false, true);
  stepper.setEnablePin(habilita);
  stepper.setMaxSpeed(NOTE_DS8);
  stepper.setAcceleration(150);
  stepper.disableOutputs();
}
  
void loop() {
  if (boton_inicio.pressed()) {
    delay(500);
    stepper.enableOutputs();
    // itera sobre el array de notas de la melodia, tiene dos por coso
    for (int thisNote = 0; thisNote < cantNotas * 2; thisNote += 2) {
      divisor = pgm_read_word(&melodia[thisNote + 1]);
      duracionNota = divisor > 0 ? (negra / divisor) : (negra / abs(divisor) * 1.5);  // duracion negativa es como el punto en la nota

/*      Serial.print(" Duracion en ms: ");
      Serial.print(duracionNota);
      Serial.print(" Frecuencia de esta nota: ");
      Serial.print(pgm_read_word(&melodia[thisNote]));
      Serial.println(""); */

      stepper.setSpeed(pgm_read_word(&melodia[thisNote]));
      unsigned long tiempoNotaComienzo = millis();

      while (millis() - tiempoNotaComienzo <= duracionNota) stepper.runSpeed();
    } 
    stepper.disableOutputs();
  }
}