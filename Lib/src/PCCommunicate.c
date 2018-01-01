/*********************************************************************************************************************
 *
 * @file       		PCCommunicate
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @Target core		MK60DNZ  MK60DN MK60FX/FN MK66FX/MK66FN
 * @date       		2017-11-9
 ********************************************************************************************************************/

#include "PCCommunicate.h"

//////֡ͷ����
uint8 SendHead[4] = {0xFF, 0xFA, 0xFD, 0xF6};

//////���͸�����
/////Ĭ�Ϸ��͵�ʾ�������Ǹ�������
/////��������ʾ6�����㣬���ǲ����Ƶ��η��͸�������ݣ�6���Ժ�����ݲ����д�����ʾ
void SendWaveData(UARTn uartn, uint8 * data, uint16 size)
{
  uint8 psize[2] = {0};
  if(size >= 256)
    psize[0] = size / 256;
  psize[1] = size - 256 * (uint16)psize[0];
  
  uart_putbuff(uartn, SendHead, 4);
  uart_putchar(uartn, 0);
  uart_putbuff(uartn, psize, sizeof(psize));
  uart_putbuff(uartn, data, size);
  uart_putchar(uartn, 0x0A);  ////���һλУ��
}

/////����ͼ������
////�����Ʒ��͵���һλͼ���ǰ�λͼ��ͼ���ʽ����������λ��������
void SendPicData(UARTn uartn, uint8 * data, uint16 size)
{
  uint8 psize[2] = {0};
  if(size >= 256)
    psize[0] = size / 256;
  psize[1] = size - 256 * (uint16)psize[0];
  
  uart_putbuff(uartn, SendHead, 4);
  uart_putchar(uartn, 250);
  uart_putbuff(uartn, psize, sizeof(psize));
  uart_putbuff(uartn, data, size);
  uart_putchar(uartn, 0x0A);  ////���һλУ��
}

////����·����Ϣ
////Ĭ��Ϊ�������ݣ���λΪ����
////��ʽΪ  ����X   ����Y   .........�����������������ʾ��ͷ��λ����Point�㼯
void SendRoute(UARTn uartn, uint8 * data, uint16 size)
{
  uint8 psize[2] = {0};
  if(size >= 256)
    psize[0] = size / 256;
  psize[1] = size - 256 * (uint16)psize[0];
  
  uart_putbuff(uartn, SendHead, 4);
  uart_putchar(uartn, 10);
  uart_putbuff(uartn, psize, sizeof(psize));
  uart_putbuff(uartn, data, size);
  uart_putchar(uartn, 0x0A);  ////���һλУ��
}