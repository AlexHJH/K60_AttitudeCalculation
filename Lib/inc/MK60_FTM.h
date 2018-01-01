#ifndef MK60_FTM_H
#define MK60_FTM_H


#include "common.h"
#include "MK60_GPIO.h"
typedef enum
{
    ftm0,
    ftm1,
    ftm2,
#if (defined(MK60F15) || defined(MK66F18))
    ftm3,
#endif
    ftmmax,
} FTMn;


typedef enum
{
    ftm_ch0,
    ftm_ch1,
    ftm_ch2,
    ftm_ch3,
    ftm_ch4,
    ftm_ch5,
    ftm_ch6,
    ftm_ch7,
} FTM_CHn;

//------------------------ PWM ------------------------

#define FTM0_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM1_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM2_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM3_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON

void ftm_pwm_init(FTMn ftmn, FTM_CHn ch, uint32 freq, uint32 duty);
void ftm_pwm_duty(FTMn ftmn, FTM_CHn ch, uint32 duty);
void ftm_pwm_freq(FTMn ftmn, uint32 freq);
void ftm_quad_init(FTMn ftmn);
int16 ftm_quad_get(FTMn ftmn);
void ftm_quad_clean(FTMn ftmn);

#endif