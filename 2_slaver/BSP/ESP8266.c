/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��esp8266.c
 * ����    ����ɶ�ESP8266�ĸ��ֲ���  
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������: ��
 * ���Է�ʽ��J-Link-OB
********************************************************************************/	
#include "ESP8266.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "string.h"

u8 Esp8266_DdtaBuff[BUFF_LEN];      //���ݻ�����
u8 IP_Buff[20];                     //���8266��IP

 /**
  * @file   ESP8266_STA_TCPServer
  * @brief  ESP8266����STAģʽ��TCP������
  * @param  ��
  * @retval 0---7
  */
u8 ESP8266_STA_TCPServer(void)
{
  if(ESP8266_SoftReset(50))return 1;                         //�����λ8266����ʱʱ��5s��ʧ�ܾͷ���1
  else printf("��λ�ɹ�\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 2;          //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���2
  else printf("����STAģʽ�ɹ�\r\n");
  
  printf("����AP��... ...\r\n");
  if(ESP8266_ConnectionAP(AP_SSID,AP_PASS,200))return 3;     //����8266����AP����ʱʱ��20s��ʧ�ܷ���3
  else printf("����AP�ɹ�\r\n");
  
  if(ESP8266_GetIP(IP_Buff,50))return 4;                     //��ѯ8266��IP����ʱʱ��5s��ʧ�ܷ���4
  else printf("��ѯIP�ɹ�\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPMODE=0",50))return 5;         //�ر�͸������ʱʱ��5s��ʧ�ܷ���5
  else printf("�ر�͸���ɹ�\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPMUX=1",50))return 6;          //������·���ӣ���ʱʱ��5s��ʧ�ܷ���6
  else printf("������·���ӳɹ�\r\n");
  
  if(ESP8266_SendCmd_OK("AT+CIPSERVER=1,5050",50))return 7;  //����TCP���������˿�5050����ʱʱ��5s��ʧ�ܷ���7	
  
  return 0;                                                  //�ɹ�����STA_TCP����������0
}

 /**
  * @file   ESP8266_STA_TCPServer
  * @brief  ESP8266����STAģʽ��TCP�ͻ���
  * @param  ��
  * @retval 1---10
  */
u8 ESP8266_STA_TCPCLient(void)
{
  u8 res;
  if(ESP8266_SoftReset(50))return 1;                         //�����λ8266����ʱʱ��5s��ʧ�ܾͷ���1
  else printf("��λ�ɹ�\r\n");

  if(ESP8266_SendCmd_OK("AT+CWMODE=1",50))return 2;          //����8266ΪSTAģʽ����ʱʱ��5s��ʧ�ܷ���2
  else printf("����STA�ɹ�\r\n");

  //printf("����AP��... ...\r\n");
  if(ESP8266_ConnectionAP(AP_SSID,AP_PASS,200))return 3;     //����8266����AP����ʱʱ��20s��ʧ�ܷ���3
  else printf("����AP�ɹ�\r\n");

  if(ESP8266_GetIP(IP_Buff,50))return 4;                     //��ѯ8266��IP����ʱʱ��5s��ʧ�ܷ���4
  else //printf("��ѯIP�ɹ�\r\n");

  if(ESP8266_SendCmd_OK("AT+CIPMODE=1",50))return 5;         //����͸������ʱʱ��5s��ʧ�ܷ���5
  else printf("����͸���ɹ�\r\n");

  if(ESP8266_SendCmd_OK("AT+CIPMUX=0",50))return 6;          //�رն�·���ӣ���ʱʱ��5s��ʧ�ܷ���6
  else //printf("�رն�·���ӳɹ�\r\n");

  //printf("���ӷ�����... ...\r\n");
  res=ESP8266_Connect_Server(SERVER_IP,SERVER_PORT,50);     //����TCP������,������͸������ʱʱ��5s��ʧ�ܷ���7	
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
  * @brief  ESP8266�����λ
  * @param  timeout����ʱʱ��
  * @retval ��ʱ���󣬷���1;�޴�����0
  */
u8 ESP8266_SoftReset(u16 timeout)
{  
  u16  time;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN); //�������2����	
  u2_printf("AT+RST\r\n");                //����ָ��
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"ready"))//������ܵ�ready��ʾ��λ�ɹ�
              break;
    delay_ms(100);//��ʱ100ms
    time++;      
  }
  if(time>=timeout)return 1;                //��ʱ���󣬷���1
  else return 0;	                          //�޴�����0
}
 /**
  * @file   ESP8266_SendCmd_OK
  * @brief  ESP8266����ָ��ڴ��Ļش���OK
	* @param  cmd: ָ�� timeout����ʱʱ��
  * @retval ��ʱ���󣬷���1;�޴�����0
  */
u8 ESP8266_SendCmd_OK(u8 *cmd, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);    //�������2����	
  u2_printf("%s\r\n",cmd);                   //����ָ��
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//������ܵ�OK��ʾָ��ɹ�
              break;
    delay_ms(100);//��ʱ100ms
    time++;      
  }
  if(time>=timeout)return 1;                //��ʱ���󣬷���1
  else return 0;		                      //�޴�����0
}

 /**
  * @file   ESP8266_ConnectionAP
  * @brief  ���ӵ�AP
	* @param  ssid������·�ɵ�ssid ; password������ ;timeout ��ʱʱ��
  * @retval ��ʱ���󣬷���1;�޴�����0
  */
u8 ESP8266_ConnectionAP(u8 *ssid, u8 *password, u16 timeout)
{
  u16 time=0;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	  
  u2_printf("AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,password);//��������APָ��
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"WIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n"))//������ܵ�OK��ʾָ��ɹ�
              break;
    delay_ms(100);//��ʱ100ms
    time++;      
  }
  if(time>=timeout)return 1;                //��ʱ���󣬷���1
  else return 0;		                      //�޴�����0
}
 /**
  * @file   ESP8266_GetIP
  * @brief  ��ȡ��ǰģ���IP��ַ
	* @param  ip������IP������;timeout ��ʱʱ��
  * @retval ��ʱ���󣬷���1;�޴�����0
  */
u8 ESP8266_GetIP(u8 *ip, u16 timeout)
{
  u16  time=0;
  char *presult1,*presult2;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	  
  u2_printf("AT+CIFSR\r\n");                            //���Ͳ�ѯIPָ��
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//������ܵ�OK��ʾָ��ɹ�
              break;
    delay_ms(100);//��ʱ100ms
    time++;      
  }
  if(time>=timeout)return 1;                //��ʱ���󣬷���1
  else
  {
    presult1 = strstr( (const char *)USART2_RX_BUF , (const char *)"\"");
    if( presult1 != NULL )
    {
      presult2 = strstr( (const char *)presult1+1 ,    (const char *)"\"");
      if( presult2 != NULL )
      {
        memcpy(ip,presult1+1,presult2-presult1-1);
        printf("�ӻ�IP��%s\r\n",ip);
        return 0;
      }
      else return 3;  //δ�յ�Ԥ������
    }
    else return 2;  //δ�յ�Ԥ������
  }		
}
 /**
  * @file   ESP8266_Get_LinkSta
  * @brief  ESP8266��ȡ����״̬
	* @param ��
	* @retval 1���пͻ�����;2���пͻ��Ͽ�;0:��״̬�ı�
  */
u8 ESP8266_Get_LinkSta(void)
{
  u8 id_temp[10]={0}; //�����������ID
  u8 sta_temp[10]={0};//�����������״̬

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//������ܵ�CONNECT��ʾ�����ӳɹ�
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("�пͻ��˽��룬ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	
    return 1;  //�пͻ��˽���
  }
  if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//������ܵ�CLOSED��ʾ�����ӶϿ�
  {
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,]",id_temp,sta_temp);
    printf("�пͻ��˶Ͽ���ID=%s\r\n",id_temp);
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	
    return 2;  //�пͻ��˶Ͽ�
  }
  else return 0; //��״̬�ı�	
}
 /**
  * @file   ESP8266_Get_Data
  * @brief  ESP8266��ȡ�ͻ�������
	* @param  data���ͻ������ݴ洢��ַ;len:���ݳ���;id:���ӵ�ID��
	* @retval 1�������ݵ���;0:�����ݵ���
  */
u8 ESP8266_Get_Data(u8 *data, u8 *len, u8 *id)
{
  u8 temp[10]={0};      //��������
  char *presult;

  if(strstr( (const char *)USART2_RX_BUF , (const char *)"+IPD"))//������ܵ�+IPD��ʾ������
  {
    delay_ms(5);
    sscanf((const char *)USART2_RX_BUF,"%[^,],%[^,],%[^:]",temp,id,len);//��ȡ��������	
    presult = strstr( (const char *)USART2_RX_BUF , (const char *)":");
    if( presult != NULL )
    {
      sprintf((char *)data,"%s",(presult+1));
    }
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	
    return 1;  //�����ݵ���
  }
  else return 0; //�����ݵ���
}
 /**
  * @file   ESP8266_SendData
  * @brief  TCP��������������
	* @param  id�����ӵ�ID�ţ�databuff�����ݻ�����<2048�data_len�����ݳ��Ȼ
	* @retval 1�������ݵ���;0:�����ݵ���
  */
u8 ESP8266_SendData(u8 id, u8 *databuff, u16 data_len, u16 timeout)
{
  u16 time=0;
  u16 i;

  USART2_RX_STA=0;                        
  memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	        
  u2_printf("AT+CIPSEND=%d,%d\r\n",id,data_len);                //���ͷ�������ָ��
  time=0;
  while(time<timeout)
  {
    if(strstr( (const char *)USART2_RX_BUF , (const char *)">"))//������ܵ�>��ʾָ��ɹ�,׼����������
              break;
    delay_ms(100);//��ʱ100ms
    time++;      
  }
  if(time>=timeout)return 1;                //��ʱ���󣬷���1
  else
  {
    USART2_RX_STA=0;                        
    memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //�������2����	
    for(i=0;i<data_len;i++)u2_printf("%c",databuff[i]);      //��������	
    time=0;
    while(time<timeout)
    {
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"SEND OK"))//�������SEND OK����ʾ���ͳɹ�
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //�������2����	
        break;
      }
      if(strstr( (const char *)USART2_RX_BUF , (const char *)"link is not valid"))//�������link is not valid����ʾ���ӶϿ�
      {
        USART2_RX_STA=0;                        
        memset(USART2_RX_BUF,0,USART2_REC_LEN);                  //�������2����	
        return 2;
      }
      delay_ms(100);  //��ʱ100ms
      time++;      
    }
    if(time>=timeout)return 3;                //��ʱ���󣬷���3
    else return 0;	
  }	
}

 /**
  * @file  ESP8266_Connect_Server
  * @brief ����TCP��������������͸��ģʽ
	* @param 
	*					ip��������ip
	*					port���������˿ں�
	* @retval 1��������δ����
	*					2�������������������
	*					3����ʱ����
	*					4��͸����ʱ
	*					0��͸���ɹ�
  */
u8 ESP8266_Connect_Server(u8 *ip, u16 port,u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //�������2����  
	u2_printf("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",ip,port);//�������ӷ�����ָ��
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CONNECT"))//������ܵ�CONNECT��ʾ���ӳɹ�
			break;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"CLOSED"))//������ܵ�CLOSED��ʾ������δ����
			return 1;
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"ALREADY CONNECTED"))//������ܵ�ALREADY CONNECTED�Ѿ���������
			return 2;
		delay_ms(100);//��ʱ100ms
		time++;      
	}
	if(time>=timeout)return 3;                //��ʱ���󣬷���3
	else  //׼������͸��
	{
		printf("���ӷ������ɹ�\r\n");//��׼������͸��
		USART2_RX_STA=0;                        
		memset(USART2_RX_BUF,0,USART2_REC_LEN);               //�������2����	  
		u2_printf("AT+CIPSEND\r\n");                          //���ͽ���͸��ָ��
		time=0;
		while(time<timeout)
		{
			if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n\r\n>"))//���������ʾ����͸���ɹ�
				break;
			delay_ms(100);//��ʱ100ms
			time++;      
		}
		if(time>=timeout)return 4;                //͸����ʱ���󣬷���4
		else return 0;	                          //�ɹ�����0		
	}
}
//�ȼ�⣬���͸��ģʽ���˳�͸��
 /**
  * @file  ESP8266_Connect_Server
  * @brief ����TCP��������������͸��ģʽ
	* @param timeout����ʱʱ��
	* @retval ��
  */
void ESP8266_Exit_Transfer(u16 timeout)
{
	u16 time=0;
	
	USART2_RX_STA=0;                        
	memset(USART2_RX_BUF,0,USART2_REC_LEN);                //�������2����  
	u2_printf("AT\r\n");//����AT����ָ��
	time=0;
	while(time<timeout)
	{
		if(strstr( (const char *)USART2_RX_BUF , (const char *)"\r\nOK\r\n"))//���������ʾ����͸���ɹ�
			break;
		delay_ms(100);//��ʱ100ms
		time++;      
	}
	if(time>=timeout)
	{
		printf("����͸����׼���˳�\r\n");
		u2_printf("+++");//�����˳�͸��ָ���	
		delay_ms(1000);
	}
}

 /**
  * @file   Char_to_Hex
  * @brief  ���ַ�������ת�������֣�������λ������0-9999
	* @param  char_data���ַ������֣�lenҪת���ĳ���
	* @retval temp������������
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


