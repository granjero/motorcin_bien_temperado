#include <AccelStepper.h>
#include "musica.h"
#include "melodia.h"

#define paso 8
#define direccion 7

AccelStepper stepper(AccelStepper::DRIVER, paso, direccion);

// Tempo
int tempo = 66;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!


// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

unsigned long previousMillis = 0;

long interval = 0;

void setup() {

  Serial.begin(115200);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(5);
  stepper.setSpeed(0);

  Serial.println(" - : ");
}

void loop() {
  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;  // increases the duration in half for dotted notes
    }
    Serial.print(noteDuration);
    Serial.print(" ms - ");
    Serial.print(melody[thisNote]);
    Serial.println(" *");

    // we only play the note for 90% of the duration, leaving 10% as a pause
    // tone(buzzer, melody[thisNote], noteDuration * 0.9);
    stepper.setSpeed(melody[thisNote]);
    unsigned long startTime = millis();
    while (1) {
      stepper.runSpeed();
      if (millis() - startTime >= noteDuration) break;  // Exit the loop if we've exceeded the play duration
    }
  }
  stepper.disableOutputs();
  delay(60000);
  stepper.enableOutputs();
}