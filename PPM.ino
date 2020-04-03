#include "PPM.h"

volatile unsigned int signals[NumberOfSignals];
volatile unsigned int numberOfNonComplete = 0;
volatile unsigned int numberOfOverflow = 0;

void setupPPM()
{
    for (byte signalNumber = 0; signalNumber < NumberOfSignals; signalNumber++)
    {
        signals[signalNumber] = 0;
    }

    DDRB &= ~(1 << PORTB0);

    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
}

volatile byte numberOfNextSignal = 0;
volatile unsigned long lastTimtstamp = 0;
volatile unsigned int temp[NumberOfSignals];

ISR(PCINT0_vect)
{
    int mask = PINB;
    unsigned long timestamp = micros();

    if ((mask & (1 << PINB0)) == 0)
    {
        unsigned long timeDelta = timestamp - lastTimtstamp;
        if ((timeDelta) < 3000)
        {
            if (numberOfNextSignal < NumberOfSignals)
            {
                temp[numberOfNextSignal] = timeDelta;
                numberOfNextSignal++;
            }
            else
            {
                numberOfOverflow++;
            }
        }
        else
        {
            if (numberOfNextSignal == NumberOfSignals)
            {
                for (byte signalNumber = 0; signalNumber < numberOfNextSignal; signalNumber++)
                {
                    unsigned int signalValue = temp[signalNumber];
                    if (signalValue < 1000)
                    {
                        signalValue = 1000;
                    }
                    else if (signalValue > 2000)
                    {
                        signalValue = 2000;
                    }
                    signals[signalNumber] = signalValue - 1000;
                }
            }
            else
            {
                numberOfNonComplete++;
            }
            numberOfNextSignal = 0;
        }
        lastTimtstamp = timestamp;
    }
}
