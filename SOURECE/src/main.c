#include "stm32f4xx.h"

#include "delay.h"
#include "led.h"
#include "timer.h"
#include "pwm_output.h"
#include "usart.h"
#include "pwm_in.h"
#include "MPU6050.h"
#include "IIC.h"
#include "imu.h"
#include "control.h"

/*------------------------------------------------------------------------------------------*/
/*               						global varibles                    				    */
/*------------------------------------------------------------------------------------------*/
unsigned char tim2flag=0;				   //��ʱ���ж�����ִ�б�־
unsigned char readyflag=0;				   //����ִ�б�־		0:��ʼ��kp,kd;   1:���ִ����ɣ�   2:ִ�е��ִ����ɺͷ��ͽǶ���Ϣ
/*------------------------------------------------------------------------------------------*/
/*                                     function code	                                    */
/*------------------------------------------------------------------------------------------*/


void TIM2_IRQHandler(void);
void NVIC_Configuration(void);
void USART3_IRQHandler(void);



int main(void)
 {
		float Roll=0;
		float Pitch=0;
		int i=0;

		LED_Init();

		LED1_OFF;                                                                                                                                                                                                                       
		LED2_OFF;
		LED3_OFF;
		LED4_OFF;

		delay_init(168);		//��ʱ��ʼ��  ����������ʱ�䡣

		NVIC_Configuration();
		usart_x_init(115200);
		Timer2_Init(40,8399);
		pwm_in_init();
		
		TIM5_PWM_Init();		//PWM�����ʼ��250hz
		TIM4_PWM_Init();		//50hz
		delay_ms(100);
		
		for(i=0;i<10000;i++)								//���Գ�ʼ��������̵�ʱ��
		{
			//TIM5_PWM_OUTPUT(1000,1000,1000,1000);
		 	TIM5_PWM_OUTPUT(pwmout2,pwmout2,pwmout2,pwmout2);
			delay_ms(1);
		}


		delay_ms(100);
		I2C_GPIO_Config(); 
		delay_ms(100);
		Init_MPU6050();
		
		Acc_Correct();
	  Gyro_Correct();

		PID_controllerInit();
		controlmiddleinit(pwmout1,pwmout2,pwmout3,pwmout4);	 //	  pwmout1:���	  pwmout2:����
															 //	  pwmout3:����	  pwmout4:����	
																	


		delay_ms(500);
		delay_ms(500);

		LED1_OFF;
		LED2_ON;
		LED3_ON;
		LED4_OFF;
		
		while(1)
		{	
			  //TIM5_PWM_OUTPUT(1200,1200,1200,1200);

		     if(tim2flag!=0)
				{
							 tim2flag=0;			 

						//	 READ_MPU6050();  	
						//	 MPU6050_TEST();

							 IMUdataprepare();
							 IMUupdate(GyroFinal.X,GyroFinal.Y,GyroFinal.Z,AccFinal.X,AccFinal.Y,AccFinal.Z);
					
							 Roll=(float)atan2(AccFinal.Y,AccFinal.Z)*57.295779513;    //X��Ƕ�ֵ 
							 Pitch=-(float)atan2(AccFinal.X,AccFinal.Z)*57.295779513;  //Y��Ƕ�ֵ

					
						//	 SendData(Q_ANGLE.Yaw*10,Q_ANGLE.Pitch*10,Q_ANGLE.Roll*10,0);	 //���diIMU��λ������λ��*10  �� �� �� ��
						//	 SendData(0,Pitch*10,0,Roll*10);		
						//	 SendData(0,GyroFinal.X*10,0,GyroFinal.Y*10);		

							 Getdesireddata(pwmout1,pwmout2,pwmout3,pwmout4);


							 PID_CAL();
							 TIM5_PWM_OUTPUT(MOTOR1,MOTOR2,MOTOR3,MOTOR4);		//MOTOR1  REAR_R  ���ҵ��
																				                        //MOTOR2	FRONT_R ǰ�ҵ��
																				                        //MOTOR3	REAR_L  ������
																				                        //MOTOR4	FRONT_L ǰ����	
						//	 TIM5_PWM_OUTPUT(pwmout2,pwmout2,pwmout2,pwmout2);
				 //	   platform_control(Q_ANGLE.Roll,Q_ANGLE.Pitch,0,0);
				 //	   TIM4_PWM_OUTPUT(servo_Roll,servo_Pitch);
						//	 SendData(Q_ANGLE.Roll*10,(&pidRoll)->outP,(&pidRoll)->outD,0);
						//	 SendData(MOTOR1*10,MOTOR2*10,MOTOR3*10,MOTOR4*10);
							 LED1_Tog;
				
			}
		}	
}

void NVIC_Configuration(void)
{ 
  	NVIC_InitTypeDef NVIC_InitStructure;  

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);					//�жϷ���0		��ռ���ȼ�4λ�������ȼ�0λ
//Usart1 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1

//TIM2��ʱ�ж�����

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
				
//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	
		
}


void TIM2_IRQHandler(void)   //TIM2�ж�
{	

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		}
//	LED4_Tog;
	tim2flag++;
	
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
		char ch;

		LED1_OFF;
		LED3_ON;
		
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
		{
			ch =USART_ReceiveData(USART3);//(USART3->DR);	//��ȡ���յ�������			
			USART3_SendData(ch);
				 
     	} 	 		
				
}

