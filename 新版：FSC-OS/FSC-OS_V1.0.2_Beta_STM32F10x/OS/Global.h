#ifndef _OS_GLOBAL_
#define _OS_GLOBAL_
#include "type.h"
/*---------------------------------------------用户全局变量声明区------------------------------------------------*/
extern os_type_app_id *App_System; 
extern os_type_app_id *App_01;
extern os_type_app_id *App_02;

extern os_type_thread_id *thread_idle;
extern os_type_thread_id *thread_manager;


extern os_mbox *os_mbox_01;
extern os_mbox *os_mbox_02;
extern os_mbox *os_mbox_03;

#endif
