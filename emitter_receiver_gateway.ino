/*****************************************************************************
* Model: emitter_receiver_gateway.qm
* File:  ./emitter_receiver_gateway.ino
*
* This code has been generated by QM tool (see state-machine.com/qm).
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*****************************************************************************/
/*${.::emitter_receiver_gateway.ino} .......................................*/
#include "erg.h"
#include "Radio433.h"

//#include <avr/sleep.h>

Q_DEFINE_THIS_MODULE("emmiter_receiver_gateway")


// define to println() many information
// #DEFINE TRACE 1

Led AO_RedLed;
Led AO_Led13;
Radio433 AO_Radio433;

// QF_active[] array defines all active object control blocks ----------------
static QEvt l_led13QSto[10]; // Event queue storage for Led
static QEvt l_radio433QSto[10]; // Event queue storage for Radio

QActiveCB const Q_ROM QF_active[] = {
  { (QActive *)0,            (QEvt *)0,        0U                    },
  { (QActive *)&AO_Led13,    l_led13QSto,      Q_DIM(l_led13QSto)    },
  { (QActive *)&AO_Radio433, l_radio433QSto,   Q_DIM(l_radio433QSto) }
};

void PrintInfo()
{
    Serial.println(F("---- 433Mhz Gateway ----"));

    //Serial.print(F("QP-nano version: "));
    //Serial.println(F(QP_VERSION_STR));

    Serial.print(F("* CPU Freq: "));
    Serial.print(F_CPU / 1000000);
    Serial.println(F("Mhz"));

    Serial.print(F("* State machine ticks/s: "));
    Serial.println(BSP_TICKS_PER_SEC);

    Serial.print(F("* 433Mhz baud rate: "));
    Serial.print(BAUDRATES[AO_Radio433.m_SpeedFactor]);
    Serial.println(F("bps"));

    Serial.print("* UART baud rate: ");
    Serial.print(UART_SPEED);
    Serial.println(F("bps"));

    if(digitalRead(UART_POWER_CTRL_PIN))
    {
        Serial.println(F("* UART power is active"));
    }
    else
    {
        Serial.println(F("* UART power is not active"));
    }

    Serial.println(F("Commands : "));

    Serial.println(F("h/H   : Help (this display"));
    Serial.println(F("r/R   : Switch to receiver mode"));
    Serial.println(F("i/I   : Switch to idle mode"));
    Serial.println(F("u/U   : Enable UART power (in case of BT module)"));
    Serial.println(F("a/A   : Disable UART power (in case of BT module)"));
    Serial.println(F("fx/Fx : Change 433Mhz baud rate. Values for x :"));
    Serial.println(F("        0 : 300 bps"));
    Serial.println(F("        1 : 600 bps"));
    Serial.println(F("        2 : 1200 bps"));
    Serial.println(F("        3 : 2400 bps"));
    Serial.println(F("        4 : 4800 bps"));
    Serial.println(F("        5 : 9600 bps"));
    Serial.println(F("        6 : 19200 bps"));
    Serial.println(F("        7 : 38400 bps"));
}

/*!
* Called when the state of MODE_SEL_PIN changes
*/
unsigned int debounceModeChange = 0;
void OnModeChange()
{
    if(millis()-debounceModeChange > 100)
    {
        // if we read 1, this is the pullup and thus
        // the jumper has been removed
        uint8_t isDeactivated = digitalRead(RADIO433_IDLE_PIN);
        if(isDeactivated)
        {
            QACTIVE_POST(&AO_Radio433, MODE_IDLE_SIG, 0U);
        }
        else
        {
            QACTIVE_POST(&AO_Radio433, MODE_RECEIVER_SIG, 0U);
        }
    }
}

//............................................................................
void setup()
{
   // initialize the QF-nano framework
    QF_init(Q_DIM(QF_active));

    QActive_ctor(&AO_Led13.super,    Q_STATE_CAST(&Led_initial));
    QActive_ctor(&AO_Radio433.super, Q_STATE_CAST(&Radio433_initial));

    AO_Led13.m_LedPin = LED13_PIN;

    pinMode(RADIO433_IDLE_PIN, INPUT);
    pinMode(5, OUTPUT);

    digitalWrite(RADIO433_IDLE_PIN, 1);
    attachInterrupt(0, OnModeChange, CHANGE);

    pinMode(UART_POWER_CTRL_PIN, OUTPUT);


    Serial.begin(UART_SPEED);
    Serial.setTimeout(5); // 5 ms waiting data on uart

    PrintInfo();
}

void loop()
{
    // run the QP-nano application
    QF_run();
}

volatile unsigned long _lastMillis = 0;
volatile unsigned long _CurrMillis = 0;
ISR(TIMER0_COMPA_vect)
{
 _CurrMillis = millis();
 uint8_t duration = _CurrMillis - _lastMillis;
 if(duration > BSP_TICK_DURATION_MS)
 {
    //Serial.print("Tick :");
    //Serial.print(duration);
    //Serial.println(" ms");

    QF_tickXISR(0);

    if (Serial.available() > 0)
    {

        if (Serial.available() > 0)
        {
            switch (Serial.read())
            {
                case 'H':
                case 'h':
                {
                   PrintInfo();
                   break;
                }

                case 'r':
                case 'R':
                {
                   QACTIVE_POST_ISR((QMActive *)&AO_Led13,    LED_BLINK_SIG,     4U);
                   QACTIVE_POST_ISR((QMActive *)&AO_Radio433, MODE_RECEIVER_SIG, 0U);
                   break;
                }

                case 'i':
                case 'I':
                {
                   QACTIVE_POST_ISR((QMActive *)&AO_Led13,    LED_BLINK_SIG,    1U);
                   QACTIVE_POST_ISR((QMActive *)&AO_Radio433, MODE_IDLE_SIG,    0U);
                   break;
                }

                case 'u':
                case 'U':
                {
                   digitalWrite(UART_POWER_CTRL_PIN,  0);
                   break;
                }

                case 'a':
                case 'A':
                {
                   digitalWrite(UART_POWER_CTRL_PIN,  0);
                   break;
                }

                case 'b':
                case 'B':
                {
                   digitalWrite(UART_POWER_CTRL_PIN,  1);
                   break;
                }

                case 'f':
                case 'F':
                {
                    uint8_t newSpeedFactor = Serial.read();
                    QACTIVE_POST_ISR((QMActive *)&AO_Radio433, NEW_SPEED_FACTOR_SIG, newSpeedFactor);
                    break;
                }

                default:
                {}

            }
        }
    }
    _lastMillis = _CurrMillis;
 }
}

//============================================================================

// QF callbacks...
void QF_onStartup(void)
{

#ifdef TRACE
    Serial.println(F("QF_onStartup"));
#endif

    // Do not paralyse TIMER2 here. Use TIMER0, used for millis().
    // see https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers

    // Timer0 is an 8-bit that counts from 0 to 255 and generates an interrupt
    // whenever it overflows. It uses a clock divisor of 64 by default to give us
    // an interrupt rate of 976.5625 Hz (close enough to a 1KHz for our purposes).
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);

    // set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking...
    //TCCR2A = (1U << WGM21) | (0U << WGM20);
    //TCCR2B = (1U << CS22 ) | (1U << CS21) | (1U << CS20); // 1/2^10
    //ASSR  &= ~(1U << AS2);
    //TIMSK2 = (1U << OCIE2A); // enable TIMER2 compare Interrupt
    //TCNT2  = 0U;

    // set the output-compare register based on the desired tick frequency
    //OCR2A  = (F_CPU / BSP_TICKS_PER_SEC / 1024U) - 1U;
}

//............................................................................
void QV_onIdle(void)
{   // called with interrupts DISABLED

#ifdef TRACE
    Serial.println(F("QV_onIdle"));
#endif

    // Put the CPU and peripherals to the low-power mode. You might
    // need to customize the clock management for your application,
    // see the datasheet for your particular AVR MCU.
    SMCR = (0 << SM0) | (1 << SE); // idle mode, adjust to your project
    QV_CPU_SLEEP();  // atomically go to sleep and enable interrupts
}

//............................................................................
void Q_onAssert(char const Q_ROM * const file, int line)
{
#ifdef TRACE
    // implement the error-handling policy for your application!!!
    Serial.print(F("Assertion raised in '"));
    Serial.print(file);
    Serial.print(F("' at line"));
    Serial.print(line);
#endif
    QF_INT_DISABLE(); // disable all interrupts
    QF_RESET();  // reset the CPU
}
