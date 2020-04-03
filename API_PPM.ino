#include "PPM.h"

extern volatile unsigned int signals[NumberOfSignals];
extern volatile unsigned int numberOfNonComplete;
extern volatile unsigned int numberOfOverflow;

void setup()
{
    Serial.begin(9600);
    setupPPM();
}

void loop()
{
    Serial.print("Non-complete: ");
    Serial.println(numberOfNonComplete);
    Serial.print("Overflows: ");
    Serial.println(numberOfOverflow);

    for (byte signalNumber = 0; signalNumber < NumberOfSignals; signalNumber++)
    {
        Serial.print("CH");
        Serial.print(signalNumber + 1);
        Serial.print(": ");
        Serial.println(signals[signalNumber]);
    }
    Serial.println();

    delay(1000);
}
