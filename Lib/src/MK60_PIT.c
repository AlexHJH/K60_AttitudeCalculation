#include "MK60_PIT.h"


void PIT_init(PITn pitn, float ms)
{
    float cnt = 0;
    cnt = (float)SysClock.bus_khz * ms;
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
    PIT_BASE_PTR->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = (uint32)cnt - 1;              			//��������ж�ʱ��
    PIT_FlAG_CLR(pitn);										//����жϱ�־λ
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
				| PIT_TCTRL_TIE_MASK        //��PITn�ж�
			        );
}

void PIT_IRQ_init(PITn pitn, float ms, void pfunc_handler(void))
{
  set_vector_handler((VECTORn_t)((int)pitn + PIT0_VECTORn), pfunc_handler);
  PIT_init(pitn, ms);
}

void PIT_delay(PITn pitn, float ms)
{
    float cnt = 0;
    cnt = (float)SysClock.bus_khz * ms;
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
    PIT_BASE_PTR->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = (uint32)cnt;              			//��������ж�ʱ��
    PIT_FlAG_CLR(pitn);										//����жϱ�־λ
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
				//| PIT_TCTRL_TIE_MASK        //��PITn�ж�
				);
    while( !((PIT_BASE_PTR->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) );//�ȴ�ʱ�䵽
    PIT_FlAG_CLR(pitn);                                     //���жϱ�־λ
}

void PIT_time_start(PITn pitn)
{
    SIM_SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
    PIT_BASE_PTR->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
    PIT_BASE_PTR->CHANNEL[pitn].LDVAL  = ~0;              			//��������ж�ʱ��
    PIT_FlAG_CLR(pitn);										//����жϱ�־λ
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT_BASE_PTR->CHANNEL[pitn].TCTRL  = ( 0
				| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
			        //| PIT_TCTRL_TIE_MASK        //��PITn�ж�
				);
}

//////���ص�λΪ����
float PIT_time_get(PITn pitn)
{
  uint32 val;
  float temp;
  val = (uint32)(~0) - PIT_BASE_PTR->CHANNEL[pitn].CVAL;
  if(PIT_BASE_PTR->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//�ж�ʱ���Ƿ�ʱ
  {
	PIT_FlAG_CLR(pitn);									//����жϱ�־λ
	PIT_BASE_PTR->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //��ֹPITn��ʱ����������ռ���ֵ��
	return 0XFFFFFFFF;									//��ʱ�������ֵ
  }
  temp = val / (SysClock.bus_khz / 1000);
  temp /= 1000;
  return temp;
}
