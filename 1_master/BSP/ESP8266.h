#ifndef __ESP8266_H
#define __ESP8266_H	 

#include "stm32f10x.h"

#define SSID  "QueuingMaster"    //ssid
#define PASS  "123456789"           //密码
#define AP_ECN   WPA_WPA2_PSK       //WPA_WPA2_PSK加密方式

#define SERVER_IP    "192.168.1.1"    //服务器IP
#define SERVER_PORT   5050             //服务器端口号

#define OPEN            0
#define WEP             1
#define WPA_PSK         2
#define WPA2_PSK        3
#define WPA_WPA2_PSK    4

#define BUFF_LEN        1024
extern u8 Esp8266_DdtaBuff[1024];

u8 ESP8266_AP_TCPServer(void);
u8 ESP8266_SoftReset(u16 timeout);
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout);
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout);
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout);
u8 ESP8266_GetIP(u8 *ip, u16 timeout);
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout);
u8 ESP8266_Get_LinkSta(void);
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id);

u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout);
void ESP8266_Exit_Transfer(u16 timeout);

u16 Char_to_Hex(u8 *char_data, u16 len);
#endif



