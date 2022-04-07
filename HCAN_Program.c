/*
 * HCAN_Program.c
 *
 *  Created on: Mar 22, 2022
 *      Author: Dell
 */
#include"../../LIB/LSTD_TYPES.h"
#include"../../LIB/LBIT_MATH.h"
#include"../../MCAL/MSPI/MSPI_Interface.h"
#include"HCAN_Private.h"
#include"HCAN_Config.h"
#include"HCAN_Interface.h"

uint8 Global_uintt8ReadBuff[14]= { 0 };

void HCAN_VidTransmitInit(void)
{
	MSPI_VidMasterInit();
	HCAN_VidReset();
	HCAN_VidWriteRegister(CANCTRL,CAN_INS_RTS_TXB2);
	while((HCAN_uint8ReadRegister(CANSTAT)>>5) != CAN_CONGMODE);
	/*To Run at 250KHZ --> solve it by 16MHZ*/
	HCAN_VidBitTiming((2<<6),(1<<7)|(6<<3)|(1),5);
	HCAN_VidSetOperationMode(CAN_NORMALMODE);
}
void HCAN_VidReceiveInit(void)
{
	MSPI_VidMasterInit();
	HCAN_VidReset();
	HCAN_VidWriteRegister(CANCTRL,CAN_INS_RTS_TXB2);
	while((HCAN_uint8ReadRegister(CANSTAT)>>5) != CAN_CONGMODE);
	/*To Run at 250KHZ --> solve it by 16MHZ*/
	HCAN_VidBitTiming((2<<6),(1<<7)|(6<<3)|(1),5);
	/*Accept Message*/
	HCAN_VidSetMask(RXM0SIDH,0x00000000,1);
	HCAN_VidSetMask(RXM1SIDH,0x00000000,1);
	/*Enable interrupt*/
	HCAN_VidWriteRegister(CANINTE,0);
	HCAN_VidSetOperationMode(CAN_NORMALMODE);
}
void HCAN_VidReceiveMsg(void)
{
	uint8 Loc_uint8Counter;
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_READ);
	MSPI_uint8ExchangeDataBlock(CAN_RXB0CTRL);
	for(Loc_uint8Counter=0;Loc_uint8Counter<14;Loc_uint8Counter++)
	{
		Global_uintt8ReadBuff[Loc_uint8Counter]=MSPI_uint8ExchangeDataBlock(0);
	}
	MSPI_VidChip_SS_Select(1);
	/*For Stand For Next Message*/
	HCAN_VidWriteRegister(CANINTF,0);
}
void HCAN_VidSendMSG(uint8 copy_uint8IDX,uint32 copy_uint32MSGID, uint8 copy_uint8DLC,uint8* copy_uint8Data)
{
	uint8 Loc_uint8Counter;
	MSPI_VidChip_SS_Select(0);
	/*Send Header*/
	MSPI_uint8SendReceiveChar(CAN_INS_WRITE);
	MSPI_uint8ExchangeDataBlock(CAN_TXB0CTRL);
	/*Set Priority*/
	MSPI_uint8ExchangeDataBlock(copy_uint8DLC>>6);
	/* Setup Standard Identifier for MSG ID 11 */
	MSPI_uint8ExchangeDataBlock((uint8)(copy_uint32MSGID>>3));
	MSPI_uint8ExchangeDataBlock((uint8)(copy_uint32MSGID<<5));
	MSPI_uint8ExchangeDataBlock(0);
	MSPI_uint8ExchangeDataBlock(0);
	/*Setup Length And RTR*/
	MSPI_uint8ExchangeDataBlock(copy_uint8DLC & 0x0F);
	/*Store Message*/
	for(Loc_uint8Counter=0;Loc_uint8Counter<(copy_uint8DLC & 0x0F);Loc_uint8Counter++)
	{
		MSPI_uint8ExchangeDataBlock(copy_uint8Data[Loc_uint8Counter]);
	}
	/*Release Bus*/
	MSPI_VidChip_SS_Select(1);
	/*Send RTS*/
	HCAN_VidReqToSend(CAN_INS_RTS_TXB0);

}
void HCAN_VidReset(void)
{
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_RESET);
	MSPI_VidChip_SS_Select(1);
}
void HCAN_VidSetOperationMode(uint8 copy_uint8Mode)
{
  HCAN_VidBitModify(CANCTRL,0x0E,(copy_uint8Mode<<5));
  while((HCAN_uint8ReadRegister(CANSTAT)>>5) != copy_uint8Mode);
}
uint8 HCAN_uint8ReadRegister(uint8 copy_uint8Address)
{
	uint8 Loc_uint8ReadValue=0;
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_READ);
	MSPI_uint8ExchangeDataBlock(copy_uint8Address);
	Loc_uint8ReadValue=MSPI_uint8ExchangeDataBlock(0);
	MSPI_VidChip_SS_Select(1);
	return Loc_uint8ReadValue;
}
void HCAN_VidReqToSend(uint8 copy_uint8BuffNumber)
{
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(copy_uint8BuffNumber);
	MSPI_VidChip_SS_Select(1);
}
void HCAN_VidWriteRegister(uint8 copy_uint8Address,uint8 copy_uint8DataByte)
{
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_WRITE);
	MSPI_uint8ExchangeDataBlock(copy_uint8Address);
	MSPI_uint8ExchangeDataBlock(copy_uint8DataByte);
	MSPI_VidChip_SS_Select(1);
}
void HCAN_VidSetMask(uint8 copy_uint8MaskAddress,uint32 copy_uint32MaskValue,uint8 copy_uint8Extended)
{
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_WRITE);
	MSPI_uint8ExchangeDataBlock(copy_uint8MaskAddress);
	if(copy_uint8Extended)
		{
			/*Extend Identifier */
			MSPI_uint8SendReceiveChar((uint8)(copy_uint32MaskValue>>3));
			MSPI_uint8ExchangeDataBlock((uint8)(copy_uint32MaskValue << 5 )|(1<<3)|(uint8)(copy_uint32MaskValue >> 27));
			MSPI_uint8ExchangeDataBlock((uint8)(copy_uint32MaskValue >> 19));
			MSPI_uint8ExchangeDataBlock((uint8)(copy_uint32MaskValue<<11));
		}
	else
		{
			/*Standard Identifier*/
			MSPI_uint8SendReceiveChar((uint8)(copy_uint32MaskValue>>3));
			MSPI_uint8SendReceiveChar((uint8)(copy_uint32MaskValue<<5));
		}

	MSPI_VidChip_SS_Select(1);
}
void HCAN_VidBitModify(uint8 copy_uint8Address ,uint8 copy_uint8MaskByte,uint8 copy_uint8DataByte)
{
	MSPI_VidChip_SS_Select(0);
	MSPI_uint8SendReceiveChar(CAN_INS_BITMODIFY);
	MSPI_uint8ExchangeDataBlock(copy_uint8Address);
	MSPI_uint8ExchangeDataBlock(copy_uint8MaskByte);
	MSPI_uint8ExchangeDataBlock(copy_uint8DataByte);
	MSPI_VidChip_SS_Select(1);
}
void HCAN_VidBitTiming(uint8 copy_uint8CNF1Value,uint8 copy_uint8CNF2Value,uint8 copy_uint8CNF3Value)
{
	HCAN_VidWriteRegister(CNF1,copy_uint8CNF1Value);
	HCAN_VidWriteRegister(CNF2,copy_uint8CNF2Value);
	HCAN_VidWriteRegister(CNF3,copy_uint8CNF3Value);
}
void HCAN_VidSetRollOver(void)
{
	HCAN_VidBitModify(CAN_RXB0CTRL,(1<<BUKT),(1<<BUKT));
}
