/**********************************************************************************
 * �ļ���  ��incap.c
 * ����    ����ͨ�����벶��	PC6-PC9	TIM3_CH1-CH4

 * ʵ��ƽ̨�����������STM33F4���ذ�
 * ��汾  ��
 * ����    ��
 * ʱ��    ��
**********************************************************************************/	
#include "pwm_in.h"


u8  TIM3CH1_CAPTURE_STA=0;	//ͨ��1���벶��״̬		  �ø���λ�������־������λ���������  				
u16	TIM3CH1_CAPTURE_UPVAL;	//ͨ��1���벶��ֵ
u16	TIM3CH1_CAPTURE_DOWNVAL;	//ͨ��1���벶��ֵ

u8  TIM3CH2_CAPTURE_STA=0;	//ͨ��2���벶��״̬		    				
u16	TIM3CH2_CAPTURE_UPVAL;	//ͨ��2���벶��ֵ
u16	TIM3CH2_CAPTURE_DOWNVAL;	//ͨ��2���벶��ֵ

u8  TIM3CH3_CAPTURE_STA=0;	//ͨ��3���벶��״̬		    				
u16	TIM3CH3_CAPTURE_UPVAL;	//ͨ��3���벶��ֵ
u16	TIM3CH3_CAPTURE_DOWNVAL;	//ͨ��3���벶��ֵ

u8  TIM3CH4_CAPTURE_STA=0;	//ͨ��4���벶��״̬		    				
u16	TIM3CH4_CAPTURE_UPVAL;	//ͨ��4���벶��ֵ
u16	TIM3CH4_CAPTURE_DOWNVAL;	//ͨ��4���벶��ֵ


u32 tempup1=0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tempup2=0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tempup3=0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tempup4=0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tim3_T;

int pwmout1,pwmout2,pwmout3,pwmout4; 				//���ռ�ձ�


void pwm_in_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;

   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);  
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
		
/*--------------------------------------------------���ö�ʱ��5���벶��--------------------------------------------*/	
	//��ʼ����ʱ��3 TIM3	 
	TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =84-1; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	//��ʼ��TIM3���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
			
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
									  
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	 
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);//��������ж� ,����CC1IE�����ж�	
					 
   	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��5
}

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{   
 	if((TIM3CH1_CAPTURE_STA&0X80)==0)//CH1 ��δ�ɹ�����	
	{  		
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//����1���������¼�
		{	
			 TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);

			if(TIM3CH1_CAPTURE_STA&0X40)		//����һ���½���Ϊ�� 		   �����������жϺ��ٴη����ж�Ϊ�½���
			{	  			
				TIM3CH1_CAPTURE_DOWNVAL=TIM_GetCapture1(TIM3);

				if(TIM3CH1_CAPTURE_DOWNVAL<TIM3CH1_CAPTURE_UPVAL)
				{
					 tim3_T=65535;
			    }
			    else tim3_T=0;	

			   tempup1=TIM3CH1_CAPTURE_DOWNVAL-TIM3CH1_CAPTURE_UPVAL+tim3_T;//�õ��ܵĸߵ�ƽʱ��

			   pwmout1=tempup1;

			   TIM3CH1_CAPTURE_STA=0;
					
			   TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���			  
			}
			else  								//���������¼��������½��أ���һ�β��������أ����㣬��ʱ����ʼ����
			{
				TIM3CH1_CAPTURE_UPVAL=TIM_GetCapture1(TIM3);			//��ȡ�����ص�����
				TIM3CH1_CAPTURE_STA|=0X40;		//����Բ�����������
			   	TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			
			}		    
		}			     	    					   
 	}
	
  
	if((TIM3CH2_CAPTURE_STA&0X80)==0)//CH1 ��δ�ɹ�����	
	{  
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)//����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM3,TIM_IT_CC2);
			if(TIM3CH2_CAPTURE_STA&0X40)		//����һ���½���Ϊ�� 		   �����������жϺ��ٴη����ж�Ϊ�½���
			{	  			

			   TIM3CH2_CAPTURE_DOWNVAL=TIM_GetCapture2(TIM3);

				if(TIM3CH2_CAPTURE_DOWNVAL<TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T=65535;
				}
				else tim3_T=0;	

			   tempup2=TIM3CH2_CAPTURE_DOWNVAL-TIM3CH2_CAPTURE_UPVAL+tim3_T;//�õ��ܵĸߵ�ƽʱ��			

			   pwmout2=tempup2;
	  
			   TIM3CH2_CAPTURE_STA=0;	

			   TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			   
			}
			else  								//���������¼��������½��أ���һ�β��������أ����㣬��ʱ����ʼ����
			{
				TIM3CH2_CAPTURE_UPVAL=TIM_GetCapture2(TIM3);			//��ȡ�����ص�����
				TIM3CH2_CAPTURE_STA|=0X40;		//����Բ�����������
			   	TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���	
			}		    
		}			     	    					   
 	}		 
	

	if((TIM3CH3_CAPTURE_STA&0X80)==0)//CH3 ��δ�ɹ�����	
	{	
	  
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)//����3���������¼�
		{	
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

			if(TIM3CH3_CAPTURE_STA&0X40)		//����һ���½���Ϊ�� 		   �����������жϺ��ٴη����ж�Ϊ�½���
			{	  			
				TIM3CH3_CAPTURE_DOWNVAL=TIM_GetCapture3(TIM3);

				if(TIM3CH3_CAPTURE_DOWNVAL<TIM3CH3_CAPTURE_UPVAL)
				{
					tim3_T=65535;
				}
				else tim3_T=0;	

				tempup3=TIM3CH3_CAPTURE_DOWNVAL-TIM3CH3_CAPTURE_UPVAL+tim3_T;//�õ��ܵĸߵ�ƽʱ��

			   pwmout3=tempup3;

			   TIM3CH3_CAPTURE_STA=0;
				
			   TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC3P=0 ����Ϊ�����ز���
			
			}
			else  								//���������¼��������½��أ���һ�β��������أ����㣬��ʱ����ʼ����
			{
				TIM3CH3_CAPTURE_UPVAL=TIM_GetCapture3(TIM3);			//��ȡ�����ص�����
				TIM3CH3_CAPTURE_STA|=0X40;		//����Բ�����������
			   	TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
			}		    
		}			     	    					   
 	}

	if((TIM3CH4_CAPTURE_STA&0X80)==0)//CH4 ��δ�ɹ�����	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)//����1���������¼�
		{	
			TIM_ClearITPendingBit(TIM3,TIM_IT_CC4);

			if(TIM3CH4_CAPTURE_STA&0X40)		//����һ���½���Ϊ�� 		   �����������жϺ��ٴη����ж�Ϊ�½���
			{	  			

				TIM3CH4_CAPTURE_DOWNVAL=TIM_GetCapture4(TIM3);
				if(TIM3CH4_CAPTURE_DOWNVAL<TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T=65535;
				}
				else tim3_T=0;	

				tempup4=TIM3CH4_CAPTURE_DOWNVAL-TIM3CH4_CAPTURE_UPVAL+tim3_T;//�õ��ܵĸߵ�ƽʱ��

			   	pwmout4=tempup4;

			    TIM3CH4_CAPTURE_STA=0;	
   				
			   	TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���				
			}
			else  								//���������¼��������½��أ���һ�β��������أ����㣬��ʱ����ʼ����
			{
				TIM3CH4_CAPTURE_UPVAL=TIM_GetCapture4(TIM3);			//��ȡ�����ص�����
				TIM3CH4_CAPTURE_STA|=0X40;		//����Բ�����������
			   	TIM_OC4PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC4P=1 ����Ϊ�½��ز���
			
			}		    
		}			     	    					   
 	}							  			 		 
}


