#include "MK60_Systick.h"

vuint32 SysTicks = 0;

void SysTick_IRQHandler(void)
{
  SysTicks++;
}

void Systick_init(uint32 time)
{
    SYST_RVR = time;                        //������ʱʱ��
    NVIC_SetPriority (SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1); /* set Priority for Cortex-M4 System Interrupts */
    SYST_CVR = 0x00;                        //��ռ�����
    SYST_CSR = ( 0
                 | SysTick_CSR_ENABLE_MASK       //ʹ�� systick
                 | SysTick_CSR_TICKINT_MASK      //ʹ���ж� (ע���˱�ʾ�ر��ж�)
                 | SysTick_CSR_CLKSOURCE_MASK    //ʱ��Դѡ�� ( core clk)
               );
    set_vector_handler(SysTick_VECTORn, SysTick_IRQHandler);
}

