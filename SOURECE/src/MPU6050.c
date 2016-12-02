#include "MPU6050.h"
#include "usart.h"
#include "IIC.h"
#include  <math.h> 

char TX_DATA[10];  	 //��ʾ�ݻ�����
char BUF[15];       //�������ݻ�����
short GTRO_X,GTRO_Y,GTRO_Z,ACCEL_X,ACCEL_Y,ACCEL_Z,T_T;		 //X,Y,Z�ᣬ�¶�

/*
void DATA_printf(char *s,short temp_data)    //ת�ַ���
{
	*s='\0';
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
	  *++s =temp_data/1000+0x30;
	  temp_data=temp_data%1000; 
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    *++s =temp_data+0x30; 
    	
}

void MPU6050_TEST(void)
{
	READ_MPU6050();	         //��ȡMPU6050����
	Send_String("GTRO:");
  DATA_printf(TX_DATA,GTRO_X);//ת��X�����ݵ�����
	Send_data('X');			        //����X����
	DATA_printf(TX_DATA,GTRO_Y);//ת��Y�����ݵ�����
	Send_data('Y');			        //����Y����
	DATA_printf(TX_DATA,GTRO_Z);//ת��Z�����ݵ�����
	Send_data('Z');			        //����Z����
	Send_String("ACCEL:");
  DATA_printf(TX_DATA,ACCEL_X);//ת��X�����ݵ�����
	Send_data('X');			        //����X����
	DATA_printf(TX_DATA,ACCEL_Y);//ת��Y�����ݵ�����
	Send_data('Y');			        //����Y����
	DATA_printf(TX_DATA,ACCEL_Z);//ת��Z�����ݵ�����
	Send_data('Z');			        //����Z����
	DATA_printf(TX_DATA,T_T);   //ת���¶����ݵ�����
	Send_data('T');			        //�����¶�����
	USART3_SendData(0X0D);	 //����
	USART3_SendData(0X0A);	 //�س�
}
*/
void Init_MPU6050(void)
{
/*
   Single_Write(MPU6050_Addr,PWR_M, 0x80);   //
   Single_Write(MPU6050_Addr,SMPL, 0x07);    //
   Single_Write(MPU6050_Addr,DLPF, 0x1E);    //��2000��
   Single_Write(MPU6050_Addr,INT_C, 0x00 );  //
   Single_Write(MPU6050_Addr,PWR_M, 0x00);   //
*/

  Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x02);	//ȡ����2��Ƶ��1k/2=500hz
	Single_Write(MPU6050_Addr,CONFIG, 0x02);	  	//��ͨ�˲�94hz
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);	//2000dps
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x11); 	//+-8g,5Hz��ͨ�˲�	 
/*	
	  Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
	  Single_Write(MPU6050_Addr,CONFIG, 0x06);
    Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
	  Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);				  		
*/
}
	
//******��ȡMPU6050����****************************************
void READ_MPU6050(void)
{
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   GTRO_X=	(BUF[1]<<8)|BUF[0];
   GTRO_X/=16.4; 						   //��ȡ����X������

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   GTRO_Y=	(BUF[3]<<8)|BUF[2];
   GTRO_Y/=16.4; 						   //��ȡ����Y������
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   GTRO_Z=	(BUF[5]<<8)|BUF[4];
   GTRO_Z/=16.4; 					       //��ȡ����Z������
   
	 BUF[6]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
   ACCEL_X=	(BUF[7]<<8)|BUF[6];
   ACCEL_X/=163.84; 						   //��ȡ����X������

   BUF[8]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
   BUF[9]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
   ACCEL_Y=	(BUF[9]<<8)|BUF[8];
   ACCEL_Y/=163.84; 						   //��ȡ����Y������
   BUF[10]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);
   BUF[11]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   ACCEL_Z=	(BUF[11]<<8)|BUF[10];
   ACCEL_Z/=163.84; 					       //��ȡ����Z������
	 
   BUF[12]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[13]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[13]<<8)|BUF[12];
   T_T = (-5)+((double) (T_T + 13200)) / 280;// ��ȡ������¶�
}
/*
 *******���ڷ�������***************************************
void Send_data(unsigned char axis)
{
  USART3_SendData(axis);
  USART3_SendData(':');
  Send_String(TX_DATA);
  USART3_SendData(' ');
  USART3_SendData(' ');
}
 
void Send_String(char *data)
 {
	 while(*data)
	 {
		 USART3_SendData(*data);
		 data++;
	 }
 }
*/