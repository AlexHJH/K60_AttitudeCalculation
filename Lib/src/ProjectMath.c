/*********************************************************************************************************************
 * @file       		ProjectMath
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @Target core		MK60DNZ  MK60DN MK60FX/FN MK66FX/MK66FN
 * @date       		2017-11-9
 ********************************************************************************************************************/

#include "ProjectMath.h"

uint8 BmpHead[14 + 40 + 1024] = {0};

//////8λbmp��ʽ�ļ�ͷ��ֱ�ӷ���ͼ������֮ǰ���γ�һ��BMP�ļ�
void BmpHeadinit(uint16 H, uint16 W)
{
  uint16 size = H * W;
  uint16 m_lenH = 14 + 40 + 1024;
  BmpHead[0] = 0x42;
  BmpHead[1] = 0x4D;
  BmpHead[2] = (size & 0x00FF);           /////С�˴洢
  BmpHead[3] = (size & 0xFF00) >> 8;
  BmpHead[4] = 0;
  BmpHead[5] = 0;
  BmpHead[7] = 0x00;  
  BmpHead[8] = 0x00;                       //WORD �����֣�������  
  BmpHead[9] = 0x00;  
  BmpHead[10] = (m_lenH & 0x00FF);      //DWORD ʵ��λͼ���ݵ�ƫ����  
  BmpHead[11] = (m_lenH & 0xFF00) >> 8;      //����������������ͷ���ļ�ͷ��  
  BmpHead[12] = 0;                       //�ļ���Ϣͷ����ɫ��֮��    
  BmpHead[13] = 0;                       //14 + 40 + 1024 = 1078 
  
  ///////////////////////����Ϊ�ļ�ͷ
  
  BmpHead[14] = 40;                      ////�ļ���Ϣͷ��С
  BmpHead[15] = 0;
  BmpHead[16] = 0;
  BmpHead[17] = 0;
  BmpHead[18] = (W & 0x00FF);            ////ͼƬ���
  BmpHead[19] = (W & 0xFF00) >> 8;
  BmpHead[20] = 0;
  BmpHead[21] = 0;
  BmpHead[22] = (H & 0x00FF);           /////ͼƬ�߶�
  BmpHead[23] = (H & 0xFF00) >> 8;
  BmpHead[24] = 0;
  BmpHead[25] = 0;
  
  BmpHead[26] = 1;
  BmpHead[27] = 0;
  BmpHead[28] = 8;                      /////ͼ��λ��
  BmpHead[29] = 0;
  BmpHead[30] = 0;
  BmpHead[31] = 0;
  BmpHead[32] = 0;
  BmpHead[33] = 0;
  BmpHead[34] = 0;
  BmpHead[35] = 0;
  BmpHead[36] = 0;
  BmpHead[37] = 0;
  BmpHead[38] = 0x20;          ///X�ֱ���
  BmpHead[39] = 0x4E;
  BmpHead[40] = 0;
  BmpHead[41] = 0;
  BmpHead[42] = 0x20;         ////Y�ֱ���
  BmpHead[43] = 0x4E;
  BmpHead[44] = 0;
  BmpHead[45] = 0;
  
  BmpHead[46] = 0;
  BmpHead[47] = 0;
  BmpHead[48] = 0;
  BmpHead[49] = 0;
  BmpHead[50] = 0;
  BmpHead[51] = 0;
  BmpHead[52] = 0;
  BmpHead[53] = 0;
  ////////����Ϊ�ļ���Ϣͷ
  for(int16 i = 0; i < 256; i++)
  {
    BmpHead[i * 4 + 54] = i;
    BmpHead[i * 4 + 55] = i;
    BmpHead[i * 4 + 56] = i;
    BmpHead[i * 4 + 57] = 0xFF;
  }
  //////����Ϊ��ɫ��
}


void UnZip(uint8 *dst, uint8 *src, uint32 srclen)/////ͼ���ѹ��
{
	uint8_t colour[2] = { 0xff, 0x00 }; //0 �� 1 �ֱ��Ӧ����ɫ///ɽ������ͷ��ԭ����Ϊ0Ϊ��
	uint8_t * mdst = dst;
	uint8_t * msrc = src;
	uint8_t tmpsrc;
	while (srclen--)
	{
		tmpsrc = *msrc++;
		*mdst++ = colour[(tmpsrc >> 7) & 0x01];
		*mdst++ = colour[(tmpsrc >> 6) & 0x01];
		*mdst++ = colour[(tmpsrc >> 5) & 0x01];
		*mdst++ = colour[(tmpsrc >> 4) & 0x01];
		*mdst++ = colour[(tmpsrc >> 3) & 0x01];
		*mdst++ = colour[(tmpsrc >> 2) & 0x01];
		*mdst++ = colour[(tmpsrc >> 1) & 0x01];
		*mdst++ = colour[(tmpsrc >> 0) & 0x01];
	}
}

uint32 Float2U32(float data)    ///////����ת32λ
{
  uint8 * point;
  point = (uint8*)(&data);
  return (((uint32)point[3] << 24) | ((uint32)point[2] << 16) | ((uint32)point[1] << 8) | ((uint32)point[0]));
}

float U322Float(uint32 * data)   /////32λת����
{
  return (*((float *)data));
}

int ABS(int data)   ////////���;���ֵ
{
	return ((data >= 0) ? data : (-data));
}

float ABSF(float data)  ///////�������ֵ
{
	return ((data >= 0) ? data : (-data));
}

int ABSDet(int datai, int dataj)   ///////���Ͳ�ֵ����ֵ
{
	int temp = datai - dataj;
	return ((temp >= 0) ? temp : (-temp));
}

float ABSFDet(float datai, float dataj) //////�����ֵ����ֵ
{
        float temp = datai - dataj;
	return ((temp >= 0) ? temp : (-temp));
}
