/******************** (C) COPYRIGHT  Դ�ع����� ********************************
 * �ļ���  ��OLED.c
 * ����    ������OLED�ĳ�ʼ����������  
 * ����    ��zhuoyingxingyu
 * �Ա�    ��Դ�ع�����http://vcc-gnd.taobao.com/
 * ��̳��ַ������԰��-Ƕ��ʽ������̳http://vcc-gnd.com/
 * �汾����: 2016-04-08
 * Ӳ������:
          //oledƬѡ����   PB0
            TP_NCS   PA4
            TP_SCK   PA5
						TP_MISO  PA6	
						TP_MOSI  PA7						
 * ���Է�ʽ��J-Link-OB
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include "OLED.h"
#include "fonts.h"
#include <math.h>
#include "usart.h"
FONT_T tFont12, tFont16;

//#define TRUN_180_EN			/* ��ת180�� */
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);
static void OLED_ConfigGPIO(void);
static void OLED_BufToPanel(void);
/* 12864 OLED���Դ澵��ռ��1K�ֽ�. ��8�У�ÿ��128���� */
/* Ϊ�˱���ˢ����Ļ��̫ǿ������ˢ����־��
0 ��ʾ��ʾ����ֻ��д����������д����1 ��ʾֱ��д����ͬʱд�������� */
static uint8_t s_ucUpdateEn = 1;
static uint8_t s_ucGRAM[8][128];
void  OLEDDelay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

 /**
  * @file   OLED_Refresh_Gram
  * @brief  �����Դ浽LCD	
  * @param  ��
  * @retval ��
  */
void OLED_Refresh_Gram(void)
{
  uint8_t i,n;		    
  for(i=0;i<8;i++)  
  {  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ    
		for(n=0;n<128;n++)OLED_WR_Byte(s_ucGRAM[i][n],OLED_DATA); 
  }   
}
 /**
  * @file   OLED_ConfigGPIO
  * @brief  ����OLED���ƿ��ߣ�����Ϊ8λ80XX���߿���ģʽ��SPIģʽ
  * @param  ��
  * @retval ��
  */
static void OLED_ConfigGPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB2PeriphClockCmd(OLED_SCLK_GPIO_RCC|OLED_MISO_GPIO_RCC|OLED_MOSI_GPIO_RCC|
	                         OLED_SCS_GPIO_RCC , ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = OLED_SCS;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(OLED_SCS_GPIO_PORT, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = OLED_SCLK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(OLED_SCLK_GPIO_PORT, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = OLED_MISO;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(OLED_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = OLED_MOSI;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(OLED_MOSI_GPIO_PORT, &GPIO_InitStructure);
    SSD_CS_1();
   
}

 /**
  * @file   OLED_InitHard
  * @brief  OLED��Ӳ����ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_InitHard(void)
{
  OLED_ConfigGPIO();

  /* �ϵ��ӳ� */
  OLEDDelay(0xfffff);
   /* ģ�鳧���ṩ��ʼ������ */
  OLED_WR_Byte(0xAE,OLED_CMD);	/* �ر�OLED�����ʾ(����) */
  OLED_WR_Byte(0x00,OLED_CMD);	/* �����е�ַ��4bit */
  OLED_WR_Byte(0x10,OLED_CMD);	/* �����е�ַ��4bit */
  OLED_WR_Byte(0x40,OLED_CMD);	/* ������ʼ�е�ַ����5bit 0-63���� Ӳ�����*/

  OLED_WR_Byte(0x81,OLED_CMD);	/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
  OLED_WR_Byte(0xCF,OLED_CMD);	/* ���öԱȶȲ��� */

  OLED_WR_Byte(0xA1,OLED_CMD);	/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
  OLED_WR_Byte(0xA6,OLED_CMD);	/* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */

  OLED_WR_Byte(0xA8,OLED_CMD);	/* ����COM·�� */
  OLED_WR_Byte(0x3F,OLED_CMD);	/* 1 ->��63+1��· */

  OLED_WR_Byte(0xD3,OLED_CMD);	/* ������ʾƫ�ƣ�˫�ֽ����*/
  OLED_WR_Byte(0x00,OLED_CMD);	/* ��ƫ�� */

  OLED_WR_Byte(0xD5,OLED_CMD);	/* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ�� */
  OLED_WR_Byte(0x80,OLED_CMD);	/* ���÷�Ƶϵ��,��4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */

  OLED_WR_Byte(0xD9,OLED_CMD);	/* ����Ԥ������� */
  OLED_WR_Byte(0xF1,OLED_CMD);	/* [3:0],PHASE 1; [7:4],PHASE 2; */

  OLED_WR_Byte(0xDA,OLED_CMD);	/* ����COM��Ӳ�����߷�ʽ */
  OLED_WR_Byte(0x12,OLED_CMD);

  OLED_WR_Byte(0xDB,OLED_CMD);	/* ���� vcomh ��ѹ���� */
  OLED_WR_Byte(0x40,OLED_CMD);	/* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */

  OLED_WR_Byte(0x8D,OLED_CMD);	/* ���ó��ã����¸�������ʹ�ã� */
  OLED_WR_Byte(0x14,OLED_CMD);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
  OLED_WR_Byte(0xAF,OLED_CMD);	/* ��OLED��� */
}

 /**
  * @file   OLED_Init
  * @brief  ��ʼ��OLED��
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
     OLED_InitHard();	 /* ��ʼ��OLEDӲ�� */
     OLED_ClrScr(0x00);	 /* ������0x00��ʾ�ڵף� 0xFF ��ʾ�׵� */
    /* ����������� */
    tFont16.usFontCode = FC_ST_16;	/* ������� 16���� */
    tFont16.usTextColor = 1;		/* ������ɫ 0 �� 1 */
    tFont16.usBackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
    tFont16.usSpace = 0;			/* ���ּ�࣬��λ = ���� */

    tFont12.usFontCode = FC_ST_12;	/* ������� 12���� */
    tFont12.usTextColor = 1;		/* ������ɫ 0 �� 1 */
    tFont12.usBackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
    tFont12.usSpace = 1;			/* ���ּ�࣬��λ = ���� */
}

 /**
  * @file   OLED_WR_Byte
  * @brief  ��OLED����һ�ֽ�
  * @param  dat:Ҫд�������/����
  *         cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
  * @retval ��
  */
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
  uint8_t i;
  SSD_CS_0();
  SSD_SCK_0();
  if (cmd==1)/* 0 ��ʾ���洫�͵������� 1��ʾ���洫�͵����� */
  {
    SSD_SDIN_1();//����
  }
  else
  {
    SSD_SDIN_0();//����
  }
//	SSD_SDIN_0();	/* 0 ��ʾ���洫�͵�������,1��ʾ���洫�͵����� */
  SSD_SCK_1();
  for (i = 0; i < 8; i++)
  {
    if (dat & 0x80)
    {
			SSD_SDIN_1();
    }
    else
    {
			SSD_SDIN_0();
    }
    SSD_SCK_0();
    dat <<= 1;
    SSD_SCK_1();
  }
  SSD_CS_1();
}	

 /**
  * @file   OLED_DispOn
  * @brief  ����ʾ
  * @param  ��
  * @retval ��
  */
void OLED_DispOn(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD); 	/* ���ó��ã����¸�������ʹ�ã� */
  OLED_WR_Byte(0X14,OLED_CMD);	/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
  OLED_WR_Byte(0XAF,OLED_CMD);	/* ��OLED��� */
}
 /**
  * @file   OLED_DispOff
  * @brief  �ر���ʾ
  * @param  ��
  * @retval ��
  */
void OLED_DispOff(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);	/* ���ó��ã����¸�������ʹ�ã�*/
  OLED_WR_Byte(0X10,OLED_CMD);	/* 0x14 ʹ�ܳ��ã�0x10 �ǹر� */
  OLED_WR_Byte(0XAE,OLED_CMD);	/* ��OLED��� */
}
 /**
  * @file   OLED_StartDraw
  * @brief  ��ʼ��ͼ���Ժ��ͼ����ֻ��д������������д����Դ�
  * @param  ��
  * @retval ��
  */
void OLED_StartDraw(void)
{
  s_ucUpdateEn = 0;
}

 /**
  * @file   OLED_EndDraw
  * @brief  ������ͼ��������������ˢ�µ�����Դ档 OLED_StartDraw() �� OLED_EndDraw() ����ɶ�ʹ��
  * @param  ��
  * @retval ��
  */
void OLED_EndDraw(void)
{
  s_ucUpdateEn = 1;
  OLED_BufToPanel();
}


 /**
  * @file   OLED_ClrScr
  * @brief  ����
  * @param  ucMode : 0 ��ʾȫ�ڣ� 0xFF��ʾȫ��
  * @retval ��
  */
void OLED_ClrScr(uint8_t _ucMode)
{
  uint8_t i,j;

  for (i = 0 ; i < 8; i++)
  {
    for (j = 0 ; j < 128; j++)
    {
			s_ucGRAM[i][j] = _ucMode;
    }
  }

  if (s_ucUpdateEn == 1)
  {
    OLED_BufToPanel();
  }
}

 /**
  * @file   OLED_BufToPanel
  * @brief  ���������еĵ�������д�����
  * @param  ��
  * @retval ��
  */
static void OLED_BufToPanel(void)
{
  uint8_t i,j;

  for (i = 0 ; i< 8; i++)
  {
    //OLED_WR_Byte(0X8D,OLED_CMD); 
    OLED_WR_Byte ((0xB0 + i),OLED_CMD);	/* ����ҳ��ַ��0~7�� */
    OLED_WR_Byte (0x00,OLED_CMD);		/* �����е�ַ�ĵ͵�ַ */
    OLED_WR_Byte (0x10,OLED_CMD);		/* �����е�ַ�ĸߵ�ַ */
    for (j = 0 ; j < 128; j++)
    {
      OLED_WR_Byte(s_ucGRAM[i][j],OLED_DATA);
    }
  }
}


 /**
  * @file   OLED_DispStr
  * @brief  ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ���ַ���
  * @param 
	*		_usX : X���꣬����12864������ΧΪ��0 - 127��
	*		_usY : Y���꣬����12864������ΧΪ��0 - 63��
	*		_ptr  : �ַ���ָ��
	*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
  * @retval ��
  */
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
  uint32_t i;
  uint8_t code1;
  uint8_t code2;
  uint32_t address;
  uint8_t buf[24 * 24 / 8];	/* ���֧��24������ */
  uint8_t m, width;
  uint8_t font_width,font_height, font_bytes;
  uint16_t x, y;
  const uint8_t *pAscDot;
  const uint8_t *pHzDot;

  /* �������ṹΪ��ָ�룬��ȱʡ��16���� */	
  if (_tFont->usFontCode == FC_ST_12)
	{
			font_height = 12;
			font_width = 12;
			font_bytes = 24;
			pAscDot = g_Ascii12;
			pHzDot = g_Hz12;
  }
  else
  {
		/* ȱʡ��16���� */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		pHzDot = g_Hz16;
  }

  /* ��ʼѭ�������ַ� */
  while (*_ptr != 0)
  {
      code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
      if (code1 < 0x80)
      {
                /* ��ascii�ַ������Ƶ�buf */
					memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
					width = font_width / 2;
      }
      else
      {
        code2 = *++_ptr;
        if (code2 == 0)
        {
						break;
        }

        /* ����16�����ֵ����ַ
                  ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
                  ;
        */
        #ifdef USE_SMALL_FONT
        m = 0;
        while(1)
        {         
          address = m * (font_bytes + 2);
          m++;
          if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
          {
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
          }
          else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
          {
								/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
						memset(buf, 0xFF, font_bytes);
						break;
          }
        }
        #else	/* ��ȫ�ֿ� */
          /* �˴���Ҫ�����ֿ��ļ����λ�ý����޸� */
          address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
          memcpy(buf, (const uint8_t *)address, font_bytes);
        #endif

        width = font_width;
      }

    y = _usY;
    /* ��ʼˢLCD */
    for (m = 0; m < font_height; m++)	/* �ַ��߶� */
    {
      x = _usX;															 
      for (i = 0; i < width; i++)	/* �ַ���� */
      {
          if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
          {
                    OLED_PutPixel(x, y, _tFont->usTextColor);	/* ����������ɫΪ����ɫ */
          }
          else
          {
                    OLED_PutPixel(x, y, _tFont->usBackColor);	/* ����������ɫΪ���ֱ���ɫ */
          }

          x++;
      }
      y++;
    }

    if (_tFont->usSpace > 0)
    {
    /* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
    }
    _usX += width + _tFont->usSpace;	/* �е�ַ���� */
    _ptr++;			/* ָ����һ���ַ� */
  }
}

 /**
  * @file  OLED_PutPixel
  * @brief  ��1������
  * @param  
	*			_usX,_usY : ��������
	*			_ucColor  ��������ɫ
  * @retval ��
  */
void OLED_PutPixel(uint16_t _usX, uint16_t _usY, uint8_t _ucColor)
{
  uint8_t ucValue;
  uint8_t ucPageAddr;
  uint8_t ucColAddr;
  #ifdef TRUN_180_EN
  const uint8_t aOrTab[8]  = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
  const uint8_t aAndTab[8] = {0xFE, 0xFD, 0xFB, 0xF7,0xEF,0xDF,0xBF,0x7F};

  ucPageAddr = _usY / 8;
  ucColAddr = 127 - _usX;
  #else
  const uint8_t aOrTab[8]  = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
  const uint8_t aAndTab[8] = {0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE};

  ucPageAddr = 7 - _usY / 8;
  ucColAddr = _usX;
  #endif

  ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
  if (_ucColor == 0)
  {
    ucValue &= aAndTab[_usY % 8];
  }
  else
  {
    ucValue |= aOrTab[_usY % 8];
  }
  s_ucGRAM[ucPageAddr][ucColAddr] = ucValue;

  if (s_ucUpdateEn == 1)
  {
    OLED_WR_Byte (0xB0 + ucPageAddr,OLED_CMD);					/* ����ҳ��ַ��0~7�� */
    OLED_WR_Byte (0x00 + (ucColAddr & 0x0F),OLED_CMD);			/* �����е�ַ�ĵ͵�ַ */
    OLED_WR_Byte (0x10 + ((ucColAddr >> 4) & 0x0F),OLED_CMD);	/* �����е�ַ�ĸߵ�ַ */
    OLED_WR_Byte(ucValue,OLED_DATA);
  }
}

 /**
  * @file   OLED_GetPixel
  * @brief ��ȡ1������
  * @param  _usX,_usY : ��������
  * @retval ��ɫֵ (0, 1)
  */
uint8_t OLED_GetPixel(uint16_t _usX, uint16_t _usY)
  {
  uint8_t ucValue;
  uint8_t ucPageAddr;
  uint8_t ucColAddr;
  #ifdef TRUN_180_EN
  ucPageAddr = _usY / 8;
  ucColAddr = 127 - _usX;
  #else
  ucPageAddr = 7 - _usY / 8;
  ucColAddr = _usX;
  #endif

  ucValue = s_ucGRAM[ucPageAddr][ucColAddr];
  if (ucValue & (_usY % 8))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}


 /**
  * @file  OLED_DrawLine
  * @brief ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
  * @param 
	*					_usX1, _usY1 ����ʼ������
	*					_usX2, _usY2 ����ֹ��Y����
	*					_ucColor     ����ɫ
  * @retval ��
  */
void OLED_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint8_t _ucColor)
{
  int32_t dx , dy ;
  int32_t tx , ty ;
  int32_t inc1 , inc2 ;
  int32_t d , iTag ;
  int32_t x , y ;

  /* ���� Bresenham �㷨����2��仭һ��ֱ�� */

  OLED_PutPixel(_usX1 , _usY1 , _ucColor);

  /* ��������غϣ���������Ķ�����*/
  if ( _usX1 == _usX2 && _usY1 == _usY2 )
  {
    return;
  }

  iTag = 0 ;
  /* dx = abs ( _usX2 - _usX1 ); */
  if (_usX2 >= _usX1)
  {
      dx = _usX2 - _usX1;
  }
  else
  {
      dx = _usX1 - _usX2;
  }

  /* dy = abs ( _usY2 - _usY1 ); */
  if (_usY2 >= _usY1)
  {
      dy = _usY2 - _usY1;
  }
  else
  {
      dy = _usY1 - _usY2;
  }

  if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
  {
      uint16_t temp;

      iTag = 1 ;
      temp = _usX1; _usX1 = _usY1; _usY1 = temp;
      temp = _usX2; _usX2 = _usY2; _usY2 = temp;
      temp = dx; dx = dy; dy = temp;
  }
  tx = _usX2 > _usX1 ? 1 : -1 ;    /* ȷ������1���Ǽ�1 */
  ty = _usY2 > _usY1 ? 1 : -1 ;
  x = _usX1 ;
  y = _usY1 ;
  inc1 = 2 * dy ;
  inc2 = 2 * ( dy - dx );
  d = inc1 - dx ;
  while ( x != _usX2 )     /* ѭ������ */
  {
      if ( d < 0 )
      {
        d += inc1 ;
      }
      else
      {
        y += ty ;
        d += inc2 ;
      }
      if ( iTag )
      {
        OLED_PutPixel ( y , x , _ucColor) ;
      }
      else
      {
        OLED_PutPixel ( x , y , _ucColor) ;
      }
      x += tx ;
  }
}

 /**
  * @file  OLED_DrawPoints
  * @brief ���� Bresenham �㷨������һ��㣬������Щ�����������������ڲ�����ʾ��
  * @param 
	*             x, y     ����������
	*	   	      	_ucColor ����ɫ
  * @retval ��
  */
void OLED_DrawPoints(uint16_t *x, uint16_t *y, uint16_t _usSize, uint8_t _ucColor)
{
    uint16_t i;

    for (i = 0 ; i < _usSize - 1; i++)
    {
      OLED_DrawLine(x[i], y[i], x[i + 1], y[i + 1], _ucColor);
    }
}

 /**
  * @file  OLED_DrawRect
  * @brief ���ƾ��Ρ�
  * @param 
	*			    _usX,_usY���������Ͻǵ�����
	*			    _usHeight �����εĸ߶�
	*			    _usWidth  �����εĿ��
  * @retval ��
  */
void OLED_DrawRect(uint16_t _usX, uint16_t _usY, uint8_t _usHeight, uint16_t _usWidth, uint8_t _ucColor)
{
    /*
     ---------------->---
    |(_usX��_usY)        |
    V                    V  _usHeight
    |                    |
     ---------------->---
                _usWidth
    */

    OLED_DrawLine(_usX, _usY, _usX + _usWidth - 1, _usY, _ucColor);	/* �� */
    OLED_DrawLine(_usX, _usY + _usHeight - 1, _usX + _usWidth - 1, _usY + _usHeight - 1, _ucColor);	/* �� */

    OLED_DrawLine(_usX, _usY, _usX, _usY + _usHeight - 1, _ucColor);	/* �� */
    OLED_DrawLine(_usX + _usWidth - 1, _usY, _usX + _usWidth - 1, _usY + _usHeight, _ucColor);	/* �� */
}

 /**
  * @file  OLED_DrawCircle
  * @brief ����һ��Բ���ʿ�Ϊ1������
  * @param 
	*			_usX,_usY  ��Բ�ĵ�����
	*			_usRadius  ��Բ�İ뾶
  * @retval ��
  */
void OLED_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint8_t _ucColor)
{
  int32_t  D;			/* Decision Variable */
  uint32_t  CurX;		/* ��ǰ X ֵ */
  uint32_t  CurY;		/* ��ǰ Y ֵ */

  D = 3 - (_usRadius << 1);
  CurX = 0;
  CurY = _usRadius;

  while (CurX <= CurY)
  {
      OLED_PutPixel(_usX + CurX, _usY + CurY, _ucColor);
      OLED_PutPixel(_usX + CurX, _usY - CurY, _ucColor);
      OLED_PutPixel(_usX - CurX, _usY + CurY, _ucColor);
      OLED_PutPixel(_usX - CurX, _usY - CurY, _ucColor);
      OLED_PutPixel(_usX + CurY, _usY + CurX, _ucColor);
      OLED_PutPixel(_usX + CurY, _usY - CurX, _ucColor);
      OLED_PutPixel(_usX - CurY, _usY + CurX, _ucColor);
      OLED_PutPixel(_usX - CurY, _usY - CurX, _ucColor);

      if (D < 0)
      {
        D += (CurX << 2) + 6;
      }
      else
      {
        D += ((CurX - CurY) << 2) + 10;
        CurY--;
      }
      CurX++;
  }
}

 /**
  * @file  OLED_DrawBMP
  * @brief ��LCD����ʾһ��BMPλͼ��λͼ����ɨ����򣺴����ң����ϵ���
  * @param 
	*			_usX, _usY : ͼƬ������
	*			_usHeight  ��ͼƬ�߶�
	*			_usWidth   ��ͼƬ���
	*			_ptr       ����ɫͼƬ����ָ�룬ÿ������ռ��1���ֽ�
  * @retval ��
  */
void OLED_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t *_ptr)
{
  uint16_t x, y;

  for (x = 0; x < _usWidth; x++)
  {
      for (y = 0; y < _usHeight; y++)
      {
          OLED_PutPixel(_usX + x, _usY + y, *_ptr);
      }
  }
}
 /**
  * @file  OLED_DisDigital
  * @brief ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ��������
  * @param 
	*		_usX : X���꣬����12864������ΧΪ��0 - 127��
	*		_usY : Y���꣬����12864������ΧΪ��0 - 63��
	*		Dig_Data  : ʮ��������
	*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ����
  * @retval ��
  */
void OLED_DisDigital(uint16_t _usX, uint16_t _usY, uint32_t Dig_Data, FONT_T *_tFont)
{
  char dd[11];
  uint8_t i,j=9;
  uint32_t temp;
  temp = Dig_Data;
  dd[10]=0;
  for(i=10;(i>j)&(i>0);i--) 
   { 
     dd[i-1] = temp%10 + '0';
     temp=temp/10;
     if(temp) j--; 
   }
  OLED_DispStr(_usX, _usY,&dd[i],&(*_tFont));
}
 /**
  * @file  oled_pow
  * @brief  m��n�η�
  * @param 
	*		    m : 
	*		    n :
  * @retval ��
  */
uint32_t oled_pow(uint8_t m,uint8_t n)
{
  uint32_t result=1;	 
  while(n--)result*=m;    
  return result;
}	

 /**
  * @file  OLED_DisDecimal
  * @brief  ����Ļָ�����꣨���Ͻ�Ϊ0��0����ʾһ��С����������С����������λ��
  * @param 
	*		_usX : X���꣬����12864������ΧΪ��0 - 127��
	*		_usY : Y���꣬����12864������ΧΪ��0 - 63��
	*		Dig_Data  : С��
	*   DecimalNum: С������
	*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
  * @retval ��
  */
void OLED_DisDecimal(uint16_t _usX, uint16_t _usY, double Dig_Data,uint8_t DecimalNum, FONT_T *_tFont)
{
  int NumTemp;
  char dd[11];
  uint8_t i,j=9;
  int temp;
  NumTemp=(uint32_t)Dig_Data;
  temp =(int)((Dig_Data-(double)NumTemp)*oled_pow(10,DecimalNum));
  dd[10]=0;
  for(i=0;i<DecimalNum;i++)
  {
    dd[9-i]=0 + '0';//С������ʼֵ0
  }
  for(i=10;(i>j)&(i>0);i--) 
  {    
    dd[i-1] = temp%10 + '0';
    temp=temp/10; 
    if(temp) 
    {
      j--;
    }				 
  } 
  dd[9-DecimalNum]='.';
  i=9-DecimalNum;
  j=9-DecimalNum-1;
  temp =(uint32_t)Dig_Data;//����������
  for(;(i>j)&(i>0);i--) //12345
  { 
    dd[i-1] = temp%10 + '0';//i=7
    temp=temp/10;// 333 33 3  i=10 9 8 j=9 8 7  
    if(temp) 
    {
      j--;
    }				 
  }
  OLED_DispStr(_usX, _usY,&dd[i],&(*_tFont));
}






