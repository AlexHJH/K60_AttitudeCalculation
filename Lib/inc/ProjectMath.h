#ifndef PROJECTMATH_H
#define PROJECTMATH_H


#include "common.h"

void UnZip(uint8 *dst, uint8 *src, uint32 srclen);/////ͼ���ѹ��
uint32 Float2U32(float data);                     ///////����ת32λ
float U322Float(uint32 * data);                   /////32λת����
int ABS(int data);                          ////////���;���ֵ
float ABSF(float data);                  ///////�������ֵ
int ABSDet(int datai, int dataj);               ///////��ֵ����ֵ
float ABSFDet(float datai, float dataj);

#endif