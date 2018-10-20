#include "stm32f10x.h"
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Flag_Stop=0,delay_50,delay_flag,Flash_Send,PID_Send;                   //停止标志位 50ms精准延时标志位
int Encoder,Encoder_Key,Target_Position=10000,Target_Velocity=40; //编码器的脉冲计数
int Moto;                                             //电机PWM变量 应是Motor的 向Moto致敬	
int Voltage;                                          //电池电压采样相关的变量
float Position_KP=40,Position_KI=0.1,Position_KD=200,Velocity_KP=5,Velocity_KI=5;      //PID系数
float Amplitude_PKP=2,Amplitude_PKI=0.1,Amplitude_PKD=3,Amplitude_VKP=1,Amplitude_VKI=1; //PID调试相关参数
float Menu_MODE=1,Menu_PID=1;  //PID相关标志位
float Angle_Balance;  //角度
u8 Flag_MODE=1;   
int main(void)
  { 
		delay_init();	    	            //=====延时函数初始化	
		uart_init(128000);	            //=====串口初始化为
		JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
		JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
		LED_Init();                     //=====初始化与 LED 连接的硬件接口
	  KEY_Init();                     //=====按键初始化
		MY_NVIC_PriorityGroupConfig(2);	//=====设置中断分组
    MiniBalance_PWM_Init(7199,0);   //=====初始化PWM 10KHZ，用于驱动电机 如需初始化电调接口 
		uart3_init(9600);               //=====串口3初始化
    Encoder_Init_TIM4();            //=====初始化编码器
		Adc_Init();                     //=====adc初始化
    IIC_Init();                     //=====IIC初始化
    MPU6050_initialize();           //=====MPU6050初始化	
    DMP_Init();                     //=====初始化DMP 
    OLED_Init();                    //=====OLED初始化	    
		while(Flag_MODE)                //=====让用户选择  运行模式
		{
		  oled_show_once();               //=====临时显示OLED
			if(TIM4->CNT>10500)Menu_MODE=0; //速度模式  向后转动右轮
			if(TIM4->CNT<9500) Menu_MODE=1; //位置模式  向前转动右轮
		  if(TIM4->CNT>10500||TIM4->CNT<9500)Flag_MODE=0,OLED_Clear(),TIM4->CNT=0;
		}
		 Encoder_Init_TIM2();            //=====编码器接口
	  MiniBalance_EXTI_Init();        //=====MPU6050 5ms定时中断初始化
    while(1) // 死循环
	   {
				delay_flag=1;	              //===50ms中断精准延时标志位
				oled_show();                //===显示屏打开	  	
        DataScope();			           //===上位机
				while(delay_flag);          //===50ms中断精准延时  主要是波形显示上位机需要严格的50ms传输周期   	
	   } 
}

