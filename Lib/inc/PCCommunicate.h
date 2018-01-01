#ifndef PCCOMMUNICATE_H
#define PCCOMMUNICATE_H

#include "common.h"
#include "include.h"


void SendWaveData(UARTn uartn, uint8 * data, uint16 size);
void SendPicData(UARTn uartn, uint8 * data, uint16 size);
void SendRoute(UARTn uartn, uint8 * data, uint16 size);



#endif