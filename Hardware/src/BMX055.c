/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * All rights reserved.
 *���ϴ�ѧ�ƾ���
 *
 * @file       		BMX055
 * @author     		Alex
 * @version    		v1.0
 * @Software 		IAR 8.1
 * @date       		2017-11-9
 ********************************************************************************************************************/

#include "BMX055.h"

bool BMX055_init(void)
{
  /************************************/
  /*���ٶ�����*/
  /************************************/
  uint8 ErrCount = 0;
  while(IIC_Read_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_ID) != 0xFA)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMURANGE, 0x05) == false)return false;   ///4G      3 2G  5 4G  8 8G  
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMUBW, 0x0F) == false)return false;     ///1000HZ      
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_ACC_ADR, BMX055_ACC_PMULPM, 0x00) == false)return false;   ///Normal MODE      
  Common_delay(10); 
  
  /************************************/
  /*����������*/
  /************************************/
  ErrCount = 0;
  while(IIC_Read_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_ID) != 0x0F)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RANGE, 0x01) == false)return false;   ///+-1000      
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_BW, 0x02) == false)return false;     ///1000HZ      
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_LPM, 0x00) == false)return false;   ///Normal MODE      
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_GYRO_ADR, BMX055_GYRO_RATEHBW, 0x08) == false)return false;   ///��ͨ�˲� �����Բ�Ҫ      
  Common_delay(10);
  
  /************************************/
  /*����������*/
  /************************************/
  ErrCount = 0;
  IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_POM, 0x81);
  Common_delay(10);
  while(IIC_Read_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_ID) != 0x32)   /////ȷ��оƬID
  {
    ErrCount++;
    if(ErrCount > 5)
      return false;
  }
  if(IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_DATARATE, 0x38) == false)return false;   ///�������30HZ       
  Common_delay(10);
  if(IIC_Write_Reg(IIC_BMX055_MAG_ADR, BMX055_MAG_INTEN, 0x00) == false)return false;      ///��ʹ���ж�      
  Common_delay(10);
  return true;
}


bool BMX055_DataRead(BMX055Datatypedef *Q, uint8 type)
{
  uint8 datatemp[6] = {0};
  if(IIC_Read_Buff(IIC_BMX055_GYRO_ADR, BMX055_GYRO_XDATALSB, datatemp, 6) == false)return false;
  Q->GYROXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]));
  Q->GYROYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]));
  Q->GYROZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]));
  if(IIC_Read_Buff(IIC_BMX055_ACC_ADR, BMX055_ACC_XDATALSB, datatemp, 6) == false)return false;
  Q->ACCXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 4);
  Q->ACCYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 4);
  Q->ACCZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 4);
  /************************************/
  /*���������ݶ�ȡ*/
  /************************************/
  if(type)
  {
    if(IIC_Read_Buff(IIC_BMX055_MAG_ADR, BMX055_MAG_XDATALSB, datatemp, 6) == false)return false;
    Q->MAGXdata = (float)((int16)((datatemp[1] << 8) | datatemp[0]) >> 3);
    Q->MAGYdata = (float)((int16)((datatemp[3] << 8) | datatemp[2]) >> 3);
    Q->MAGZdata = (float)((int16)((datatemp[5] << 8) | datatemp[4]) >> 1);
  }
  return true;
}