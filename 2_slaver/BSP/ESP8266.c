/******************** (C) COPYRIGHT  源地工作室 ********************************
 * 文件名  ：esp8266.c
 * 描述    ：完成对ESP8266的各种操作  
 * 作者    ：zhuoyingxingyu
 * 淘宝    ：源地工作室http://vcc-gnd.taobao.com/
 * 论坛地址：极客园地-嵌入式开发论坛http://vcc-gnd.com/
 * 版本更新: 2016-04-08
 * 硬件连接: 无
 * 调试方式：J-Link-OB
********************************************************************************/	
#include "ESP8266.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "string.h"

u8 Esp8266_DdtaBuff[BUFF_LEN];      //数据缓冲区
u8 IP_Buff[20];                     //存放8266的IP

 /**
  * @file   ESP8266_STA_TCPServer
  * @brief  ESP8266设置STA模式下TCP服务器
  * @param  无
  * @retval 0---7
  */
u8 ESP8266_STA_TCPServer(void)
{
  if(ESP8266_SoftReset(50))return 1;                         //软件复位8266，超时时间5s，失败就返回1
  else printf("软复位成功\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 2;          //设置8266为STA模式，超时时间5s，失败返回2
  else printf("设置STA模式成功\r\n");
  
  printf("连接AP中... ...\r\n");
  if(ESP8266_ConnectionAP(AP_SSID,AP_PASS,200))return 3;     //设置8266连接AP，超时时间20s，失败返回3
  else printf("连接AP成功\r\n");
  
  if(ESP8266_GetIP(IP_Buff,50))return 4;                     //查询8266的IP，超时时间5s，失败返回4
  else printf("查询IP成功\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 5;         //关闭透传，超时时间5s，失败返回5
  else printf("关闭透传成功\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPMUX=1",50))return 6;          //开启多路连接，超时时间5s，失败返回6
  else printf("开启多路连接成功\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050",50))return 7;  //开启TCP服务器，端口5050，超时时间5s，失败返回7	
  
  return 0;                                                  //成功设置STA_TCP服务器返回0
}

 /**
  * @file   ESP8266_STA_TCPServer
  * @brief  ESP8266设置STA模式下TCP客户端
  * @param  无
  * @retval 1---10
  */
u8 ESP8266_STA_TCPCLient(void)
{
  u8 res;
  if(ESP8266_SoftReset(50))return 1;                         //软件复位8266，超时时间5s，失败就返回1
  else printf("软复位成功\r\n");

  if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 2;          //设置8266为STA模式，超时时间5s，失败返回2
  else printf("设置STA成功\r\n");

  //printf("连接AP中... ...\r\n");
  if(ESP8266_ConnectionAP(AP_SSID,AP_PASS,200))return 3;     //设置8266连接AP，超时时间20s，失败返回3
  else printf("连接AP成功\r\n");

  if(ESP8266_GetIP(IP_Buff,50))return 4;                     //查询8266的IP，超时时间5s，失败返回4
  else //printf("查询IP成功\r\n");

  if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 5;         //开启透传，超时时间5s，失败返回5
  else printf("开启透传成功\r\n");

  if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 6;          //关闭多路连接，超时时间5s，失败返回6
  else //printf("关闭多路连接成功\r\n");

  //printf("连接服务器... ...\r\n");
  res=ESP8266_Connect_Server(SERVER_IP,SERVER_PORT,50);     //连接TCP服务器,并进入透传，超时时间5s，失败返回7	
  switch(res)
  {
    case 0 : break;
    case 1 : return 7;
    case 2 : return 8;
    case 3 : return 9;
    case 4 : return 10;
  }

  return 0;
}

 /**
  * @file   ESP8266_SoftReset
  * @brief  ESP8266软件复位
  * @param  timeout：超时时间
  * @retval 超时错误，返回1;无错，返回0
  */
u8 ESP8266_SoftReset(u16 timeout)
{  
  u16  time;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //清除串口2数据	
  u2_printf("AT+RST\r\n");                //发送指令
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"ready"))//如果接受到ready表示复位成功
              break;
    delay_ms(100);//延时100ms
    time++;      
  }
  if(time>=timeout)return 1;                //超时错误，返回1
  else return 0;	                          //无错，返回0
}
 /**
  * @file   ESP8266_SendCmd_OK
  * @brief  ESP8266发送指令，期待的回答是OK
	* @param  cmd: 指令 timeout：超时时间
  * @retval 超时错误，返回1;无错，返回0
  */
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);    //清除串口2数据	
  u2_printf("%s\r\n",cmd);                   //发送指令
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//如果接受到OK表示指令成功
              break;
    delay_ms(100);//延时100ms
    time++;      
  }
  if(time>=timeout)return 1;                //超时错误，返回1
  else return 0;		                      //无错，返回0
}

 /**
  * @file   ESP8266_ConnectionAP
  * @brief  连接到AP
	* @param  ssid：无线路由的ssid ; password：密码 ;timeout 超时时间
  * @retval 超时错误，返回1;无错，返回0
  */
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	  
  u2_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);//发送连接AP指令
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))//如果接受到OK表示指令成功
              break;
    delay_ms(100);//延时100ms
    time++;      
  }
  if(time>=timeout)return 1;                //超时错误，返回1
  else return 0;		                      //无错，返回0
}
 /**
  * @file   ESP8266_GetIP
  * @brief  获取当前模块的IP地址
	* @param  ip：保存IP的数组;timeout 超时时间
  * @retval 超时错误，返回1;无错，返回0
  */
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
  u16  time=0;
  char *presult1,*presult2;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	  
  u2_printf("AT+CIFSR\r\n");                            //发送查询IP指令
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//如果接受到OK表示指令成功
              break;
    delay_ms(100);//延时100ms
    time++;      
  }
  if(time>=timeout)return 1;                //超时错误，返回1
  else
  {
    presult1 = strstr( (const char *)USART2_RX_BUF , (const char *)"\"");
    if( presult1 != NULL )
    {
      presult2 = strstr( (const char *)presult1+1 ,    (const char *)"\"");
      if( presult2 != NULL )
      {
        memcpy(ip,presult1+1,presult2-presult1-1);
        printf("从机IP：%s\r\n",ip);
        return 0;
      }
      else return 3;  //未收到预期数据
    }
    else return 2;  //未收到预期数据
  }		
}
 /**
  * @file   ESP8266_Get_LinkSta
  * @brief  ESP8266获取连接状态
	* @param 无
	* @retval 1：有客户接入;2：有客户断开;0:无状态改变
  */
u8 ESP8266_Get_LinkSta(void)
{
  u8 id_temp[10]={0}; //缓冲区，存放ID
  u8 sta_temp[10]={0};//缓冲区，存放状态

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//如果接受到CONNECT表示有链接成功
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("有客户端接入，ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	
    return 1;  //有客户端接入
  }
  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//如果接受到CLOSED表示有链接断开
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("有客户端断开，ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	
    return 2;  //有客户端断开
  }
  else return 0; //无状态改变	
}
 /**
  * @file   ESP8266_Get_Data
  * @brief  ESP8266获取客户端数据
	* @param  data：客户端数据存储地址;len:数据长度;id:连接的ID号
	* @retval 1：有数据到来;0:无数据到来
  */
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
  u8 temp[10]={0};      //缓冲区，
  char *presult;

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"+IPD"))//如果接受到+IPD表示有数据
  {
    delay_ms(5);
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//截取各段数据	
    presult = strstr( (const char *)USART2_RX_BUF , (const char *)":");
    if( presult != NULL )
    {
      sprintf((char *)data,"%s",(presult+1));
    }
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	
    return 1;  //有数据到来
  }
  else return 0; //无数据到来
}
 /**
  * @file   ESP8266_SendData
  * @brief  TCP服务器发送数据
	* @param  id：连接的ID号；databuff：数据缓冲区<2048ata_len：数据长度�
	* @retval 1：有数据到来;0:无数据到来
  */
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
  u16 time=0;
  u16 i;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	        
  u2_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);                //发送发送数据指令
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)">"))//如果接受到>表示指令成功,准备发送数据
              break;
    delay_ms(100);//延时100ms
    time++;      
  }
  if(time>=timeout)return 1;                //超时错误，返回1
  else
  {
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //清除串口2数据	
    for(i=0;i<data_len;i++)u2_printf("%c",databuff[i]);      //发送数据	
    time=0;
    while(time<timeout)
    {
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"SEND OK"))//如果接受SEND OK，表示发送成功
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //清除串口2数据	
        break;
      }
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"link is not valid"))//如果接受link is not valid，表示连接断开
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //清除串口2数据	
        return 2;
      }
      delay_ms(100);  //延时100ms
      time++;      
    }
    if(time>=timeout)return 3;                //超时错误，返回3
    else return 0;	
  }	
}

 /**
  * @file  ESP8266_Connect_Server
  * @brief 连接TCP服务器，并进入透传模式
	* @param 
	*					ip：服务器ip
	*					port：服务器端口号
	* @retval 1：服务器未开启
	*					2：已与服务器建立链接
	*					3：超时错误
	*					4：透传超时
	*					0：透传成功
  */
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //清除串口2数据  
	u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);//发送连接服务器指令
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//如果接受到CONNECT表示连接成功
			break;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//如果接受到CLOSED表示服务器未开启
			return 1;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"ALREADY CONNECTED"))//如果接受到ALREADY CONNECTED已经建立连接
			return 2;
		delay_ms(100);//延时100ms
		time++;      
	}
	if(time>=timeout)return 3;                //超时错误，返回3
	else  //准备进入透传
	{
		printf("连接服务器成功\r\n");//，准备进入透传
		USART2_RX_STA=0;                        
		memset(USART2_RX_BUF,0,USART2_REC_LEN);               //清除串口2数据	  
		u2_printf("AT+CIPSEND\r\n");                          //发送进入透传指令
		time=0;
		while(time<timeout)
		{
			if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//如果成立表示进入透传成功
				break;
			delay_ms(100);//延时100ms
			time++;      
		}
		if(time>=timeout)return 4;                //透传超时错误，返回4
		else return 0;	                          //成功返回0		
	}
}
//先检测，如果透传模式则退出透传
 /**
  * @file  ESP8266_Connect_Server
  * @brief 连接TCP服务器，并进入透传模式
	* @param timeout：超时时间
	* @retval 无
  */
void ESP8266_Exit_Transfer(u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //清除串口2数据  
	u2_printf("AT\r\n");//发送AT测试指令
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//如果成立表示进入透传成功
			break;
		delay_ms(100);//延时100ms
		time++;      
	}
	if(time>=timeout)
	{
		printf("处于透传，准备退出\r\n");
		u2_printf("+++");//发送退出透传指令包	
		delay_ms(1000);
	}
}

 /**
  * @file   Char_to_Hex
  * @brief  将字符型数字转换成数字，处理四位数以下0-9999
	* @param  char_data：字符型数字；len要转换的长度
	* @retval temp：返还的数字
  */
u16 Char_to_Hex(u8 *char_data, u16 len)
{
  u16 temp;

  switch(len)
  {
    case 1 : temp=(char_data[0]-0x30)*1;
    break;
    case 2 : temp=(char_data[0]-0x30)*10;
    temp+=(char_data[1]-0x30)*1;
    break;
    case 3 : temp=(char_data[0]-0x30)*100;
    temp+=(char_data[2]-0x30)*10;
    temp+=(char_data[3]-0x30)*1;
    break;
    case 4 : temp=(char_data[0]-0x30)*1000;
    temp+=(char_data[1]-0x30)*100;
    temp+=(char_data[2]-0x30)*10;
    temp+=(char_data[3]-0x30)*1;
    break;
  }
  return temp;
}


