#include "app_01.h"
#include "STM32F10x_C51Mode.h"

static void thread_01(void);
static void thread_02(void);
static void thread_03(void);

os_type_app_id* os_app_new_create__App_01(void) //����APP_01
{
	  os_type_app_id *app_id=os_app_new_create();
		os_app_name_set(app_id,"APP_01");
	  os_app_prio_set(app_id,1);
	  os_app_state_set(app_id,os_app_state_creating);
	
	  //                    (����)   (�̺߳�����)  (�߳�����)  (�̶߳�ջ��С)(ʱ��Ƭ)(���ȼ�)     (״̬)               (����)  //ʱ��Ƭ�Ͳ�����ʱû��
	  //                    app_id   app_thread app_thread_name stk_size slice  prio         state                para
    os_thread_new_create( app_id,   thread_01,  "thread_01",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //�����߳�1 
    os_thread_new_create( app_id,   thread_02,  "thread_02",   128,     10,    2,  os_thread_state_readying,  (void*)0  ); //�����߳�2
    os_thread_new_create( app_id,   thread_03,  "thread_03",   128,     10,    3,  os_thread_state_readying,  (void*)0  ); //�����߳�3

	  os_app_new_init(app_id); 
	  return app_id;
}
	
static void thread_01(void)  //APP_01���߳�01
{  
	while(1) 
	 {	 		 
     os_printf("APP_01  thread_01\r\n");
		 os_thread_delay(25000);  	//1000msִ��һ���������ʱ��ԽС��������ִ�еļ��ԽС��
   } 
}

static void timer_print_callback(void)	//timer_print��ʱ��ɻص�����
{
	os_printf("You are so good!---- \r\n");
}
static void timer_delte_callback(void)	//timer_delte��ʱ��ɻص�����
{
	os_timer_delete(timer_print);//ɾ����ʱ��timer_print
	os_timer_delete(timer_delte);//ɾ����ʱ��timer_delte����
}
static void thread_02(void)  //APP_01���߳�02
{	
	//���̣������ʱ��
	//���ܣ�������ʱ����һ����ʱ��ӡ�����һ����ʱɾ����һ����ʱ��
	timer_print=os_timer_new_create(timer_print_callback,timer_mode_type__cycle,5000);//��ʹ��������ʱ��timer_print����ʱ5000ms
	timer_delte=os_timer_new_create(timer_delte_callback,timer_mode_type__once,25000);//��ʹ��������ʱ��timer_delte����ʱ25000ms
	while(1)
	{
		os_printf("APP_01  thread_02\r\n");
		os_thread_delay(1000);			//1000msִ��һ������
	}
}

static void thread_03(void) //APP_01���߳�03
{
	os_u8 arr[]="hello world!";
	while(1) 
	 {
		 os_printf("APP_01  thread_03\r\n");	
		 os_thread_delay(3000);			//2000msִ��һ������	
     os_mbox_post(os_mbox_01,arr,sizeof(arr));		 
	 }			
}


