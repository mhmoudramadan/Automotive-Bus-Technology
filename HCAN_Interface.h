/*
 * HCAN_Interface.h
 *
 *  Created on: Mar 22, 2022
 *      Author: Dell
 */

#ifndef HAL_HCAN_HCAN_INTERFACE_H_
#define HAL_HCAN_HCAN_INTERFACE_H_

/*CAN DRIVER MCP_2515 MODULE*/
void HCAN_VidReceiveMsg(void);
void HCAN_VidReceiveInit(void);
void HCAN_VidTransmitInit(void);
void HCAN_VidSendMSG(uint8 copy_uint8IDX,uint32 copy_uint32MSGID, uint8 copy_uint8DLC,uint8* copy_uint8Data);

#endif /* HAL_HCAN_HCAN_INTERFACE_H_ */
