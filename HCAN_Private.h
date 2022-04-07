/*
 * HCAN_Private.h
 *
 *  Created on: Mar 22, 2022
 *      Author: Dell
 */

#ifndef HAL_HCAN_HCAN_PRIVATE_H_
#define HAL_HCAN_HCAN_PRIVATE_H_

/* CAN Control Resister*/
#define CNF1           0x2A
#define CNF2		   0x29
#define CNF3           0x28
#define CANSTAT        0x0E
#define CANCTRL        0x0F
#define CANINTE        0x2B
#define CANINTF        0x2C
#define CAN_TXB0CTRL   0x30
#define CAN_TXB1CTRL   0x40
#define CAN_TXB2CTRL   0x50
#define CAN_RXB0CTRL   0x60
#define CAN_RXB1CTRL   0x70
#define RXM0SIDH       0x20
#define RXM1SIDH       0x24

/*SPI Instructions*/
#define  CAN_INS_RESET			0xC0
#define  CAN_INS_READ			0x03
#define  CAN_INS_WRITE			0x02
#define  CAN_INS_RTS_TXB0       0x81
#define  CAN_INS_RTS_TXB1		0x82
#define  CAN_INS_RTS_TXB2		0x84
#define  CAN_INS_READSTATUS     0xA0
#define  CAN_INS_RXSTATUS       0xB0
#define  CAN_INS_BITMODIFY      0x05

#define 	RX0IE				0
#define 	BUKT				2

/*Operations Mode*/
#define  CAN_NORMALMODE    	0
#define  CAN_SLEEP		   	1
#define  CAN_LOOPBACK      	2
#define  CAN_LISTENONLY    	3
#define  CAN_CONGMODE      	4

/*Private Functions*/

void HCAN_VidReset(void);
void HCAN_VidSetOperationMode(uint8 copy_uint8Mode);
uint8 HCAN_uint8ReadRegister(uint8 copy_uint8Address);
void HCAN_VidReqToSend(uint8 copy_uint8BuffNumber);
void HCAN_VidWriteRegister(uint8 copy_uint8Address,uint8 copy_uint8DataByte);
void HCAN_VidSetMask(uint8 copy_uint8MaskAddress,uint32 copy_uint32MaskValue,uint8 copy_uint8Extended);
void HCAN_VidBitModify(uint8 copy_uint8Address ,uint8 copy_uint8MaskByte,uint8 copy_uint8DataByte);
void HCAN_VidBitTiming(uint8 copy_uint8CNF1Value,uint8 copy_uint8CNF2Value,uint8 copy_uint8CNF3Value);
void HCAN_VidSetRollOver(void);


#endif /* HAL_HCAN_HCAN_PRIVATE_H_ */
