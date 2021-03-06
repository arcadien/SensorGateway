/*****************************************************************************
* Model: emitter_receiver_gateway.qm
* File:  ./radio433.h
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
/*${.::radio433.h} .........................................................*/
#ifndef RADIO433_H
#define RADIO433_H

#include "erg.h"
#include "manchester.h"
#include "led.h"
#include "radioProtocol.h"

#define RADIO433_BUFFER_SIZE 20

// TODO : EEPROM
const uint16_t BAUDRATES[] = {300,600,1200,2400,4800,9600,19200,38400};

enum Radio433Signals{
    MODE_RECEIVER_SIG = Q_USER_SIG,
    MODE_IDLE_SIG,
    NEW_SPEED_FACTOR_SIG
};


#if ((QP_VERSION < 591) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8)))
#error qpn version 5.9.1 or higher required
#endif

/*${AOs::Radio433} .........................................................*/
typedef struct Radio433 {
/* protected: */
    QActive super;

/* public: */
    uint8_t m_SpeedFactor;
    uint8_t RADIO433_BUFFER_[RADIO433_BUFFER_SIZE];
} Radio433;

/* protected: */
QState Radio433_initial(Radio433 * const me);
QState Radio433_IDLE(Radio433 * const me);
QState Radio433_RECEIVING(Radio433 * const me);
QState Radio433_INITIAL(Radio433 * const me);


#endif
