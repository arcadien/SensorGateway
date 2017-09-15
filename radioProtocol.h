/*****************************************************************************
* Model: emitter_receiver_gateway.qm
* File:  ./radioProtocol.h
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
/*${.::radioProtocol.h} ....................................................*/
#ifndef RADIO_PROTOCOL_H
#define RADIO_PROTOCOL_H

// protocol definition, should
// be shared with receiver/emitter

enum {
    MESSAGE_SIZE_POS,
    DEV_ID_POS,
    SENSOR_ID_POS,
    SENSOR_BYTE_1_POS,
    SENSOR_BYTE_2_POS
};

//uint8_t TEMP_SENSOR_ID= 0x0;

#endif
