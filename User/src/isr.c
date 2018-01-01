#include "include.h"
#include "isr.h"
#include "Attitude_Calculation.h"
#include "DataStore.h"


vuint8                 IsGyroOffsetReset = 0;     /////�����Ҫ������������Ʈ�����򽫸ı�����Ϊ   1
/////��1�ķ�ʽ����ͨ�� �������߱�Ĳ���
BMX055Datatypedef      BMX055_data;
EulerAngleTypedef      SystemAttitude;            /////��̬��
EulerAngleTypedef      SystemAttitudeRate;        /////��̬���ٶ�
AttitudeDatatypedef    GyroOffset;


void GyroOffset_init(void)      /////////��������Ʈ��ʼ��
{
  static uint16 Count = 0;
  if(Count == 0)
  {
    GyroOffset.Xdata = 0;
    GyroOffset.Ydata = 0;
    GyroOffset.Zdata = 0;
  }
  
  if(Count == 1000)
  {
    GyroOffset.Xdata /= 1000;
    GyroOffset.Ydata /= 1000;
    GyroOffset.Zdata /= 1000;
    IsGyroOffsetReset = 0;
    DataSave();
    Count = 0;
  }
  else
  {
    BMX055_DataRead(&BMX055_data, 0);
    GyroOffset.Xdata += BMX055_data.GYROXdata;
    GyroOffset.Ydata += BMX055_data.GYROYdata;
    GyroOffset.Zdata += BMX055_data.GYROZdata;
    Count++;
  }
}


/**************************************************/
/*��ȫ���Ŀ��Ƽ��ɼ��������ڸ��ж�*/
/*************************************************/
//float TestUsing = 0;   //��K66 210M  ��PFU������£���Ԫ����̬�������̬���ݲɼ�����Ҫ 280ns
volatile bool IsSend = false;
float AccZAngle = 0;
void PIT0_IRQHandler(void)
{
  static uint8 MAGWaitCount = 0;
  static bool IsAttitudeinit = false;
  static uint8 SendCount = 0;
  PIT_FlAG_CLR(PIT0);
  
  PIT_time_start(PIT2);
  if(IsSend == false)
  {
    SendCount++;
    if(SendCount == 5)
    {
      IsSend = true;
      SendCount = 0;
    }
  }
  if(IsGyroOffsetReset)
  {
    GyroOffset_init();             ///////��ƫ���������в�������̬���㣬����֤��̬ģ�龲ֹ
    return;
  }
  /*****************************************************************/
  /*���Ƿֽ��ߣ���������ƫ����*/
  /******************************************************************/
  
  MAGWaitCount++;
  if(MAGWaitCount < 15)
    BMX055_DataRead(&BMX055_data, 0);
  else
  {
    BMX055_DataRead(&BMX055_data, 1);     //////ÿ30ms��ȡһ�δ�����
    MAGWaitCount = 0;
  }
  BMX055_data.GYROXdata = (BMX055_data.GYROXdata - GyroOffset.Xdata) * 0.030517578;   
  BMX055_data.GYROYdata = (BMX055_data.GYROYdata - GyroOffset.Ydata) * 0.030517578;
  BMX055_data.GYROZdata = (BMX055_data.GYROZdata - GyroOffset.Zdata) * 0.030517578;
  ///////1000 / 32768     //////BMX055������Ʈ�������Ժ��Բ��ƣ����ǰ�ȫ������ǽ���һ��
  BMX055_data.ACCXdata *= 0.001953125;    ///////4 / 2048
  BMX055_data.ACCYdata *= 0.001953125;
  BMX055_data.ACCZdata *= 0.001953125;
  
  
  Acc.Xdata = BMX055_data.ACCXdata;
  Acc.Ydata = BMX055_data.ACCYdata;
  Acc.Zdata = BMX055_data.ACCZdata;
  Gyro.Xdata = BMX055_data.GYROXdata;
  Gyro.Ydata = BMX055_data.GYROYdata;
  Gyro.Zdata = BMX055_data.GYROZdata;

  
  if(IsAttitudeinit == false)
  {
    Quaternion_init();                    ////��̬�����ʼ��        
    IsAttitudeinit = true;
  }
  else
  {
    Attitude_UpdateGyro();                /////���ٸ���
    Attitude_UpdateAcc();                 /////����ںϸ���
    
    
    SystemAttitude.Pitch = -EulerAngle.Roll / PI * 180;            ////תΪ�Ƕ�   ������
    SystemAttitude.Roll = EulerAngle.Pitch / PI * 180;             ////������
    SystemAttitude.Yaw = EulerAngle.Yaw / PI * 180;                ////ƫ����
    SystemAttitudeRate.Pitch = -EulerAngleRate.Roll / PI * 180;   ////�������ٶ�
    SystemAttitudeRate.Yaw = EulerAngleRate.Yaw / PI * 180;       ////ƫ�����ٶ�
    
    float AccZ, AccZAdjust;    
    AccZ = -Acc.Zdata;
    if (AccZ > 1)
	AccZ = 1;
    if (AccZ < -1)
	AccZ = -1;            
    AccZAngle = asinf(AccZ) * 180 / PI;
    AccZAdjust = (AccZAngle - SystemAttitude.Pitch);
    SystemAttitude.Pitch += (-Gyro.Xdata + AccZAdjust) * PERIODS;
  }
  //TestUsing = PIT_time_get(PIT2);
  ///////////////���ڴ�����ֻ���������߼����������и������㣬ֻҪ�ж���ֵ��С����
  
  
  /*****************************************************************/
  /*���Ƿֽ��ߣ���������̬����*/
  /******************************************************************/
  
  
  
  /*****************************************************************/
  /*���Ƿֽ��ߣ���������̬����*/
  /******************************************************************/
}
