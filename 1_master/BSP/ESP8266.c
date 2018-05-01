/******************** (C) COPYRIGHT  Ô´µØ¹¤×÷ÊÒ ********************************
 * ÎÄ¼şÃû  £ºesp8266.c
 * ÃèÊö    £ºÍê³É¶ÔESP8266µÄ¸÷ÖÖ²Ù×÷  
 * ×÷Õß    £ºzhuoyingxingyu
 * ÌÔ±¦    £ºÔ´µØ¹¤×÷ÊÒhttp://vcc-gnd.taobao.com/
 * ÂÛÌ³µØÖ·£º¼«¿ÍÔ°µØ-Ç¶ÈëÊ½¿ª·¢ÂÛÌ³http://vcc-gnd.com/
 * °æ±¾¸üĞÂ: 2016-04-08
 * Ó²¼şÁ¬½Ó: ÎŞ
 * µ÷ÊÔ·½Ê½£ºJ-Link-OB
********************************************************************************/	
#include "ESP8266.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "string.h"

u8 Esp8266_DdtaBuff[BUFF_LEN];      //Êı¾İ»º³åÇø
u8 IP_Buff[20];                     //´æ·Å8266µÄIP

 /**
  * @file   ESP8266_STA_TCPServer
  * @brief  ESP8266ÉèÖÃAPÄ£Ê½ÏÂTCP·şÎñÆ÷
  * @param  ÎŞ
  * @retval 0---7
  */
u8 ESP8266_AP_TCPServer(void)
{
	if(ESP8266_SoftReset(50))return 1;                         //Èí¼ş¸´Î»8266£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü¾Í·µ»Ø1
	else printf("ESP8266Èí¸´Î»³É¹¦\r\n");
 	
	if(ESP8266_SendCmd_OK("AT+CWMODE=2",50))return 2;          //ÉèÖÃ8266ÎªAPÄ£Ê½£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø2
	else printf("ESP8266ÉèÖÃAPÄ£Ê½³É¹¦\r\n");
 	
	printf("ESP8266×¼±¸ÉèÖÃAP²ÎÊı\r\n");
	printf("SSID£ºQueuingMaster\r\n");
	printf("ÃÜÂë£º1234567890\r\n");
	printf("¼ÓÃÜ·½Ê½£ºWPA_WPA2_PSK\r\n");
	if(ESP8266_SET_AP(SSID,PASS,WPA_WPA2_PSK,50))return 3;     //ÉèÖÃ8266µÄAP£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø3
	else printf("ESP8266ÉèÖÃAP²ÎÊı³É¹¦\r\n");
		
	if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 4;         //¹Ø±ÕÍ¸´«£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø4
 	else printf("ESP8266¹Ø±ÕÍ¸´«³É¹¦\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPMUX=1",50))return 5;          //¿ªÆô¶àÂ·Á¬½Ó£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø5
 	else printf("ESP8266¿ªÆô¶àÂ·Á¬½Ó³É¹¦\r\n");
	
	printf("ÉèÖÃAPÄ£Ê½IPµØÖ·£º192.168.1.1\r\n");
	if(ESP8266_SendCmd_OK("AT+CIPAP=\"192.168.1.1\"",50))return 6; //ÉèÖÃAPµÄIPµØÖ·£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø6
 	else printf("ESP8266ÉèÖÃAPÄ£Ê½IP³É¹¦\r\n");
	
	if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050",50))return 7;  //¿ªÆôTCP·şÎñÆ÷£¬¶Ë¿Ú5050£¬³¬Ê±Ê±¼ä5s£¬Ê§°Ü·µ»Ø7	
	
	return 0;                                                  //³É¹¦ÉèÖÃSTA_TCP·şÎñÆ÷·µ»Ø0
}
 /**
  * @file   ESP8266_SoftReset
  * @brief  ESP8266Èí¼ş¸´Î»
  * @param  timeout£º³¬Ê±Ê±¼ä
  * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎŞ´í£¬·µ»Ø0
  */
u8 ESP8266_SoftReset(u16 timeout)
{  
  u16  time;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //Çå³ı´®¿Ú2Êı¾İ	
  u2_printf("AT+RST\r\n");                //·¢ËÍÖ¸Áî
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"ready"))//Èç¹û½ÓÊÜµ½ready±íÊ¾¸´Î»³É¹¦
              break;
    delay_ms(100);//ÑÓÊ±100ms
    time++;      
  }
  if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
  else return 0;	                          //ÎŞ´í£¬·µ»Ø0
}
 /**
  * @file   ESP8266_SendCmd_OK
  * @brief  ESP8266·¢ËÍÖ¸Áî£¬ÆÚ´ıµÄ»Ø´ğÊÇOK
	* @param  cmd: Ö¸Áî timeout£º³¬Ê±Ê±¼ä
  * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎŞ´í£¬·µ»Ø0
  */
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);    //Çå³ı´®¿Ú2Êı¾İ	
  u2_printf("%s\r\n",cmd);                   //·¢ËÍÖ¸Áî
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
              break;
    delay_ms(100);//ÑÓÊ±100ms
    time++;      
  }
  if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
  else return 0;		                      //ÎŞ´í£¬·µ»Ø0
}
//ÉèÖÃAP
//ssid£ºssid
//password£ºÃÜÂë
//mode:¼ÓÃÜ·½Ê½
//timeout ³¬Ê±Ê±¼ä
 /**
  * @file   ESP8266_SendCmd_OK
  * @brief  ESP8266·¢ËÍÖ¸Áî£¬ÆÚ´ıµÄ»Ø´ğÊÇOK
	* @param  cmd: Ö¸Áî timeout£º³¬Ê±Ê±¼ä
  * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎŞ´í£¬·µ»Ø0
  */
u8 ESP8266_SET_AP(u8 *ssid, u8 *password, u8 mode, u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                           //Çå³ı´®¿Ú2Êı¾İ	  
	u2_printf("AT+CWSAP=\"%s\",\"%s\",4,%d\r\n",ssid,password,mode);  //·¢ËÍÉèÖÃAPÖ¸Áî
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"OK"))//Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
			break;
		delay_ms(100);//ÑÓÊ±100ms
		time++;      
	}
	if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
	else return 0;		                      //ÎŞ´í£¬·µ»Ø0
}
 /**
  * @file   ESP8266_ConnectionAP
  * @brief  Á¬½Óµ½AP
	* @param  ssid£ºÎŞÏßÂ·ÓÉµÄssid ; password£ºÃÜÂë ;timeout ³¬Ê±Ê±¼ä
  * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎŞ´í£¬·µ»Ø0
  */
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	  
  u2_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);//·¢ËÍÁ¬½ÓAPÖ¸Áî
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))//Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
              break;
    delay_ms(100);//ÑÓÊ±100ms
    time++;      
  }
  if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
  else return 0;		                      //ÎŞ´í£¬·µ»Ø0
}
 /**
  * @file   ESP8266_GetIP
  * @brief  »ñÈ¡µ±Ç°Ä£¿éµÄIPµØÖ·
	* @param  ip£º±£´æIPµÄÊı×é;timeout ³¬Ê±Ê±¼ä
  * @retval ³¬Ê±´íÎó£¬·µ»Ø1;ÎŞ´í£¬·µ»Ø0
  */
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
  u16  time=0;
  char *presult1,*presult2;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	  
  u2_printf("AT+CIFSR\r\n");                            //·¢ËÍ²éÑ¯IPÖ¸Áî
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//Èç¹û½ÓÊÜµ½OK±íÊ¾Ö¸Áî³É¹¦
              break;
    delay_ms(100);//ÑÓÊ±100ms
    time++;      
  }
  if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
  else
  {
    presult1 = strstr( (const char *)USART2_RX_BUF , (const char *)"\"");
    if( presult1 != NULL )
    {
      presult2 = strstr( (const char *)presult1+1 ,    (const char *)"\"");
      if( presult2 != NULL )
      {
        memcpy(ip,presult1+1,presult2-presult1-1);
        printf("ESP8266µÄIPµØÖ·£º%s\r\n",ip);
        return 0;
      }
      else return 3;  //Î´ÊÕµ½Ô¤ÆÚÊı¾İ
    }
    else return 2;  //Î´ÊÕµ½Ô¤ÆÚÊı¾İ
  }		
}
 /**
  * @file   ESP8266_Get_LinkSta
  * @brief  ESP8266»ñÈ¡Á¬½Ó×´Ì¬
	* @param ÎŞ
	* @retval 1£ºÓĞ¿Í»§½ÓÈë;2£ºÓĞ¿Í»§¶Ï¿ª;0:ÎŞ×´Ì¬¸Ä±ä
  */
u8 ESP8266_Get_LinkSta(void)
{
  u8 id_temp[10]={0}; //»º³åÇø£¬´æ·ÅID
  u8 sta_temp[10]={0};//»º³åÇø£¬´æ·Å×´Ì¬

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//Èç¹û½ÓÊÜµ½CONNECT±íÊ¾ÓĞÁ´½Ó³É¹¦
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("ÓĞ¿Í»§¶Ë½ÓÈë£¬ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	
    return 1;  //ÓĞ¿Í»§¶Ë½ÓÈë
  }
  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//Èç¹û½ÓÊÜµ½CLOSED±íÊ¾ÓĞÁ´½Ó¶Ï¿ª
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("ÓĞ¿Í»§¶Ë¶Ï¿ª£¬ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	
    return 2;  //ÓĞ¿Í»§¶Ë¶Ï¿ª
  }
  else return 0; //ÎŞ×´Ì¬¸Ä±ä	
}
 /**
  * @file   ESP8266_Get_Data
  * @brief  ESP8266»ñÈ¡¿Í»§¶ËÊı¾İ
	* @param  data£º¿Í»§¶ËÊı¾İ´æ´¢µØÖ·;len:Êı¾İ³¤¶È;id:Á¬½ÓµÄIDºÅ
	* @retval 1£ºÓĞÊı¾İµ½À´;0:ÎŞÊı¾İµ½À´
  */
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
  u8 temp[10]={0};      //»º³åÇø£¬
  char *presult;

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"+IPD"))//Èç¹û½ÓÊÜµ½+IPD±íÊ¾ÓĞÊı¾İ
  {
    delay_ms(5);
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//½ØÈ¡¸÷¶ÎÊı¾İ	
    presult = strstr( (const char *)USART2_RX_BUF , (const char *)":");
    if( presult != NULL )
    {
      sprintf((char *)data,"%s",(presult+1));
    }
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	
    return 1;  //ÓĞÊı¾İµ½À´
  }
  else return 0; //ÎŞÊı¾İµ½À´
}
 /**
  * @file   ESP8266_SendData
  * @brief  TCP·şÎñÆ÷·¢ËÍÊı¾İ
	* @param  id£ºÁ¬½ÓµÄIDºÅ£»databuff£ºÊı¾İ»º³åÇø<2048£data_len£ºÊı¾İ³¤¶È»
	* @retval 1£ºÓĞÊı¾İµ½À´;0:ÎŞÊı¾İµ½À´
  */
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
  u16 time=0;
  u16 i;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	        
  u2_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);                //·¢ËÍ·¢ËÍÊı¾İÖ¸Áî
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)">"))//Èç¹û½ÓÊÜµ½>±íÊ¾Ö¸Áî³É¹¦,×¼±¸·¢ËÍÊı¾İ
              break;
    delay_ms(100);//ÑÓÊ±100ms
    time++;      
  }
  if(time>=timeout)return 1;                //³¬Ê±´íÎó£¬·µ»Ø1
  else
  {
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //Çå³ı´®¿Ú2Êı¾İ	
    for(i=0;i<data_len;i++)u2_printf("%c",databuff[i]);      //·¢ËÍÊı¾İ	
    time=0;
    while(time<timeout)
    {
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"SEND OK"))//Èç¹û½ÓÊÜSEND OK£¬±íÊ¾·¢ËÍ³É¹¦
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //Çå³ı´®¿Ú2Êı¾İ	
        break;
      }
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"link is not valid"))//Èç¹û½ÓÊÜlink is not valid£¬±íÊ¾Á¬½Ó¶Ï¿ª
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //Çå³ı´®¿Ú2Êı¾İ	
        return 2;
      }
      delay_ms(100);  //ÑÓÊ±100ms
      time++;      
    }
    if(time>=timeout)return 3;                //³¬Ê±´íÎó£¬·µ»Ø3
    else return 0;	
  }	
}

 /**
  * @file  ESP8266_Connect_Server
  * @brief Á¬½ÓTCP·şÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½
	* @param 
	*					ip£º·şÎñÆ÷ip
	*					port£º·şÎñÆ÷¶Ë¿ÚºÅ
	* @retval 1£º·şÎñÆ÷Î´¿ªÆô
	*					2£ºÒÑÓë·şÎñÆ÷½¨Á¢Á´½Ó
	*					3£º³¬Ê±´íÎó
	*					4£ºÍ¸´«³¬Ê±
	*					0£ºÍ¸´«³É¹¦
  */
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //Çå³ı´®¿Ú2Êı¾İ  
	u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);//·¢ËÍÁ¬½Ó·şÎñÆ÷Ö¸Áî
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//Èç¹û½ÓÊÜµ½CONNECT±íÊ¾Á¬½Ó³É¹¦
			break;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//Èç¹û½ÓÊÜµ½CLOSED±íÊ¾·şÎñÆ÷Î´¿ªÆô
			return 1;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"ALREADY CONNECTED"))//Èç¹û½ÓÊÜµ½ALREADY CONNECTEDÒÑ¾­½¨Á¢Á¬½Ó
			return 2;
		delay_ms(100);//ÑÓÊ±100ms
		time++;      
	}
	if(time>=timeout)return 3;                //³¬Ê±´íÎó£¬·µ»Ø3
	else  //×¼±¸½øÈëÍ¸´«
	{
		printf("ESP8266Á¬½Ó·şÎñÆ÷³É¹¦£¬×¼±¸½øÈëÍ¸´«\r\n");
		USART2_RX_STA=0;                        
		memset(USART2_RX_BUF,0,USART2_REC_LEN);               //Çå³ı´®¿Ú2Êı¾İ	  
		u2_printf("AT+CIPSEND\r\n");                          //·¢ËÍ½øÈëÍ¸´«Ö¸Áî
		time=0;
		while(time<timeout)
		{
			if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//Èç¹û³ÉÁ¢±íÊ¾½øÈëÍ¸´«³É¹¦
				break;
			delay_ms(100);//ÑÓÊ±100ms
			time++;      
		}
		if(time>=timeout)return 4;                //Í¸´«³¬Ê±´íÎó£¬·µ»Ø4
		else return 0;	                          //³É¹¦·µ»Ø0		
	}
}
//ÏÈ¼ì²â£¬Èç¹ûÍ¸´«Ä£Ê½ÔòÍË³öÍ¸´«
 /**
  * @file  ESP8266_Connect_Server
  * @brief Á¬½ÓTCP·şÎñÆ÷£¬²¢½øÈëÍ¸´«Ä£Ê½
	* @param timeout£º³¬Ê±Ê±¼ä
	* @retval ÎŞ
  */
void ESP8266_Exit_Transfer(u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //Çå³ı´®¿Ú2Êı¾İ  
	u2_printf("AT\r\n");//·¢ËÍAT²âÊÔÖ¸Áî
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//Èç¹û³ÉÁ¢±íÊ¾½øÈëÍ¸´«³É¹¦
			break;
		delay_ms(100);//ÑÓÊ±100ms
		time++;      
	}
	if(time>=timeout)
	{
		printf("ESP8266´¦ÓÚÍ¸´«×´Ì¬£¬×¼±¸ÍË³öÍ¸´«\r\n");
		u2_printf("+++");//·¢ËÍÍË³öÍ¸´«Ö¸Áî°ü	
		delay_ms(1000);
	}
}

 /**
  * @file   Char_to_Hex
  * @brief  ½«×Ö·ûĞÍÊı×Ö×ª»»³ÉÊı×Ö£¬´¦ÀíËÄÎ»ÊıÒÔÏÂ0-9999
	* @param  char_data£º×Ö·ûĞÍÊı×Ö£»lenÒª×ª»»µÄ³¤¶È
	* @retval temp£º·µ»¹µÄÊı×Ö
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


