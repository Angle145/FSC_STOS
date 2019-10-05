#include "app_01.h"

#include "led.h"  	//�û�����ͷ�ļ�

static void thread_01(void);//�̺߳�������
static void thread_02(void);
static void thread_03(void);

os_type_app_id* os_app_new_create__App_01(void) //����APP_01
{
	  os_type_app_id *app_id=os_app_new_create();//��ȡһ���µ�APP ID
		os_app_name_set(app_id,"APP_01");  //����APP�ַ�������
	  os_app_prio_set(app_id,1);					//����APP���ȼ�
	  os_app_state_set(app_id,os_app_state_creating);//����APP״̬
	
		//����APP�߳�
	  //                    (����)   (�̺߳�����)  (�߳�����)  (�̶߳�ջ��С)(ʱ��Ƭ)(���ȼ�)     (״̬)               (����)  //ʱ��Ƭ�Ͳ�����ʱû��
	  //                    app_id   app_thread app_thread_name stk_size slice  prio         state                para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //�����߳�1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //�����߳�2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //�����߳�3

	  os_app_new_init(app_id); //�������ϲ�����ʹ��APP����APP����ϵͳ
	  return app_id;
}
	
static void thread_01(void)  //APP_01���߳�01
{  
	while(1) 
	 {	 		 
     os_printf("APP_01  thread_01\r\n"); //ʾ�����룬ʹ��ʱɾ��
		 os_thread_delay(4000);  	//4000msִ��һ���������ʱ��ԽС��������ִ�еļ��ԽС�� //ʾ�����룬ʹ��ʱɾ��
   } 
}
static void thread_02(void)  //APP_01���߳�02
{	
	while(1)
	{
		//ʾ�����룬ʹ��ʱɾ��
		LED1_ON();							//LED��˸(������Ҫ��ֲ���ܹ���)
		os_thread_delay(500);		//��˸Ƶ��1s
		LED1_OFF();
		os_thread_delay(500);		
		os_printf("APP_01  thread_02\r\n");
	}
}
static void thread_03(void) //APP_01���߳�03
{
	while(1) 
	 {
		 os_printf("APP_01  thread_03\r\n");	//ʾ�����룬ʹ��ʱɾ��
		 os_thread_delay(2000);			//2000msִ��һ������		//ʾ�����룬ʹ��ʱɾ�� 
	 }			
}


