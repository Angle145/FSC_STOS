/*
ע��stm32f1xxϵ��оƬ���뱣֤ÿ���߳������ͷ�>=2ms��ʱ����ܱ�֤�߳�������ת����
*/
#include "os.h"  //ʹ���ں�

#include "app_01.h" //���ڵ��ô���APP_01����
#include "usart.h"  //���ڵ��ó�ʹ�����ں���
#include "led.h"  	//���ڵ��ó�ʹ��LED����
	
static void os_thread_idle(void);

os_type_app_id* os_app_new_create__App_System(void) //����App_System ��ϵͳAPP��
{
	  os_type_app_id *app_id=os_app_new_create();
		os_app_name_set(app_id,"APP_SYSTEM");
	  os_app_prio_set(app_id,0);
	  os_app_state_set(app_id,os_app_state_creating);
	
    //                                (����)   (�̺߳�����)  (�߳�����)  (�̶߳�ջ��С)(ʱ��Ƭ)(���ȼ�)    (״̬)       (����)  //ʱ��Ƭ�Ͳ�����ʱû��
	  //                                app_id   app_thread app_thread_name stk_size slice  prio         state       para
    thread_idle=os_thread_new_create( app_id, os_thread_idle, "thread_idle", 128, 1, 0, os_thread_state_readying, (void*)0 ); //���������߳� 
	  thread_manager=os_thread_new_create( app_id, os_thread_manager, "thread_manager", 256, 1, (os_u32)-1, os_thread_state_readying, (void*)0 ); //����ϵͳ�����߳� 
	
	  os_app_new_init(app_id);
	  return app_id;
}
static void os_thread_idle(void) //�����������ڷ�ֹ�û����񶼴�����ͣ��Ϊ����Ϊ0ʱ����(��ֹ����os_thread_delay())
{
	os_printf("app_idle is running\r\n");
	while(1) 
	{
    //��ֹ����os_thread_delay()
	}
}
void os_user_init(void) //�û������ʹ���������û����г�ʹ�����������ڴ˺�����,��ʹ��������ֹ����os_thread_delay()�ͽ�ֹ���ȣ�
{
		LED_Init();//LED��ʹ��
	  Usart1_Config(115200); //����1��ʹ��
	  printf("-@FSC-OS Inside\r\n");  //��ʹ����ϵͳ��û������������ʹ��os_printf(ϵͳ������ֻ��ʹ��os_printf)
		printf(">>>>>STM32F4xx Test Demo\r\n\r\n");
}	
void os_main(void)
{      	
	  App_System=os_app_new_create__App_System(); //����ϵͳAPP��ϵͳ�Դ����û����ɸ��ģ�
    App_01=os_app_new_create__App_01();         //�����û�APP_01(����ֵ��APP�������global.c/.h�ﴴ��)
	
    os_init_and_startup();//ϵͳ��ʹ��������
}
