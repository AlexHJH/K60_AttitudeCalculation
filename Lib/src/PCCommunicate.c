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

//////帧头定义
uint8 SendHead[4] = {0xFF, 0xFA, 0xFD, 0xF6};

//////发送浮点数
/////默认发送到示波器的是浮点数据
/////最多可以显示6个浮点，但是不限制单次发送更多的数据，6个以后的数据不进行处理及显示
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
  uart_putchar(uartn, 0x0A);  ////最后一位校验
}

/////发送图像数据
////不限制发送的是一位图还是八位图，图像格式的设置在上位机端设置
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
  uart_putchar(uartn, 0x0A);  ////最后一位校验
}

////发送路径信息
////默认为浮点数据，单位为厘米
////格式为  坐标X   坐标Y   .........往后的数据以数字显示，头两位才是Point点集
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
  uart_putchar(uartn, 0x0A);  ////最后一位校验
}