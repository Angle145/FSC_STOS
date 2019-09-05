/*
ע��stm32f1xxϵ��оƬ���뱣֤ÿ���߳������ͷ�>=2ms��ʱ����ܱ�֤�߳�������ת����
*/
#include "os.h"  //ʹ���ں�
#include "usart.h" 
#include "app_01.h" 
#include "app_02.h" 
	
static void os_thread_idle(void);

os_type_app_id* os_app_create__App_System(void) //����App_System ��ϵͳAPP��
{
	  os_type_app_id *app_id=os_app_new_init();
		os_app_name_set(app_id,"APP_SYSTEM");
	  os_app_prio_set(app_id,0);
	  os_app_state_set(app_id,os_app_state_creating);
	
    //                                (����)   (�̺߳�����)  (�߳�����)  (�̶߳�ջ��С)(ʱ��Ƭ)(���ȼ�)    (״̬)       (����)  //ʱ��Ƭ�Ͳ�����ʱû��
	  //                                app_id   app_thread app_thread_name stk_size slice  prio         state       para
    thread_idle=os_thread_new_create( app_id, os_thread_idle, "thread_idle", 128, 1, 0, os_thread_state_readying, (void*)0 ); //���������߳� 
	  thread_manager=os_thread_new_create( app_id, os_thread_manager, "thread_manager", 256, 1, (os_u32)-1, os_thread_state_readying, (void*)0 ); //����ϵͳ�����߳� 
	
	  os_app_new_create(app_id);
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
	  USART1_Config(115200); //����1��ʹ��
	  printf("-@OS Inside\r\n");
	
	  os_mbox_01=os_mbox_create();
}	
void os_main(void)
{                     
	  App_System=os_app_create__App_System(); //����ϵͳAPP��ϵͳ�Դ����û����ɸ��ģ�
    App_01=os_app_create__App_01();         //�����û�APP_01
	  App_02=os_app_create__App_02();         //�����û�APP_02
	
    os_init_and_startup();//ϵͳ��ʹ��������
}
