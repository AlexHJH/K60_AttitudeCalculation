/*********************************************************************************************************************
 *
 * @file       		main
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @Target core		MK60DNZ  MK60DN MK60FX/FN MK66FX/MK66FN
 * @date       		2017-11-9
 ********************************************************************************************************************/
#include "include.h"
#include "isr.h"
#include "Attitude_Calculation.h"


//float   SendFloat[6] = {0};
//extern volatile uint8 IsGyroOffsetReset;
//extern volatile bool IsSend;
void main()
{
  IIC_init();
  Common_delay(10);
  unsigned char FalseCount = 0;
  while(false == BMX055_init())
  {
    FalseCount++;
    if(FalseCount > 5)
    {
      ////加入失败回显   
    }
  }
  PIT_IRQ_init(PIT0, 2, PIT0_IRQHandler);
  NVIC_EnableIRQ(PIT0_IRQn);
  EnableInterrupts;
  while(1)
  {
    
  }
}
