/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
0.  ���ߣ�������ˮ

1.  ϵͳ�ṩ��ȷС��ʱ����:
        delay_us(); ΢�뼶��ʱ,��λ:us΢�� 
	    delay_ms(); ���뼶��ʱ,��λ:ms���� 		
    ������ʱ������
        OS_delayMs(); �����������ʱ,��λ:ms����  �����ú������л�����������ֱ����ʱ�䵽������ϵͳ�л�������
		
		PS:delay_ms()��OS_delayMs()����ǰ����ԭ�صȴ�����������ʱ�ڼ��õ�ǰ������ͣ���У�������ʱ����������������ʱʱ��
		                                 ����ʱ������������У��Ӷ��������Ч�ʡ�
		
		��Ҫ������c�ļ���ʹ����ʱ��������������������c��h�ļ��Ŀ�ͷ��,����c�ļ������FSC_STOS.hͷ�ļ�
		extern void delay_us(unsigned int nus);
		extern void delay_ms(unsigned int nms);		
		
2.  ϵͳ�ṩ�����л�����������(��Ҫ)
        OSSchedLock();   �����л����� (ĳ��������������󣬻�������ʱ�������Ȩֱ������Ϊֹ)
		OSSchedUnlock(); �����л�����	

    ���ã�ĳЩ����ִ�й��̲��ܱ���ϣ���Ҫһ�������ʱ��
		���磺��ȡDS18B20�¶�ֵʱ����ȡDS3231ʱ������ʱ��ˢ��LCDҳ��ʱ�ȵ�������ڲ���֮ǰ����OSSchedLock()������ǰ�����ڲ���
		      ��ɺ����OSSchedUnlock()�������������ܱ�֤�����������ԡ�
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/	

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
�й�ϵͳ���ܣ�
									 
1.ʹ�ò���Driver�ļ����ڵ�����ʱ��ɾ����������ֱ�����Ӧ����.c��.h���ֱ��Ҽ�ѡ��Remove File "xxx"

2.������񣺣��������֮ǰ��鿴fsc_stos.h����������������Ƿ����㵱ǰ����������Ҫ�������������޸ģ�
  
	*��Ӳ��裺(�����Task13Ϊ��,�����������⣬ֻҪ��Ӧ����������һ�¼��ɡ�����Ϊ���㣬ȡ��ΪTask13)
	*1.void Task13(void);//����13���� 
    *2.#define Task13_StkSize 128  //����13��ջ��С��>=64,(����һ����ģ�Ȼ�󴮿ڲ鿴ʹ�������޸��趨��һ���ʵ��ʹ�ô�20%�������)
    *3.OSSTK Task13_Stk[Task13_StkSize];//����13��ջ����
	*4.OSTaskCreate("Task13",Task13,Task13_Stk,Task13_StkSize,TASK_RUNNING,1);//��main()�����ﴴ������Task13(����OS)
	*5.void Task13(void) //����13ʵ�庯��
		{
			 while(1) 
			 {		
				 //printf("Task13 is running\r\n");//ʾ�����룬ʹ��ʱɾ��	
				 //OS_delayMs(1000); 				       //ʾ�����룬ʹ��ʱɾ��	
			 }
		}
		
	������   ������   ��������    �����ջ      �����ջ��С      ����״̬     �������ȼ�(����Խ�����ȼ�Խ�ߣ����65535,���1)
		    "Task13",  Task13,     Task13_Stk,   Task13_StkSize,   TASK_RUNNING,       1
		
3.ϵͳ����ָ��: ��Ĭ�ϴ���1��
    �Ѽ���ָ�����ʽ����//Ϊ��������
		<1> cmd/Task1/close//             �ر�Task1����Task1��Ϊ����������
		<2> cmd/Task1/open//              ��Task1����Task1��Ϊ����������
		<3> cmd/Task1/prio=%d//           �����������ȼ�,��cmd/Task1/prio=8//
		<4> cmd/osmanage//                ����״̬��Ϣ
		<5> cmd/runmode/order//           �����ϵ�˳������ģʽ  
        <6> cmd/runmode/prio//            ����ʽ���ȼ�����ģʽ 

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



ͨ�ð�FSC_STOS֧��STM32F4 FPU���ã�

STM32F4 FPU����������Ϊ֧��FPU�������㣬��ͨ�ð��FSC_STOS�������޸�

һ��MDK��������(���ħ����)

1.Target�� Floating Point Hardware: ��Ϊ  Use Single Precision

2.C/C++�� Define:(���������Define��)
    USE_STDPERIPH_DRIVER,STM32F429_439xx,ARM_MATH_CM4,__CC_ARM,ARM_MATH_MATRIX_CHECK,ARM_MATH_ROUNDING,__FPU_PRESENT=1


�����޸������ļ�

1.startup_stm32f429_439x.s�����ļ�(����Ϊ�����ͺ�F4�������ļ�)

ԭ�ļ�����
; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
                 IMPORT  SystemInit
                 IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

�޸ģ������������滻��������
; Reset handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main

                IF {FPU} != "SoftVFP"
                                                ; Enable Floating Point Support at reset for FPU
                LDR.W   R0, =0xE000ED88         ; Load address of CPACR register
                LDR     R1, [R0]                ; Read value at CPACR
                ORR     R1,  R1, #(0xF <<20)    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                                                ; Write back the modified CPACR value
                STR     R1, [R0]                ; Wait for store to complete
                DSB
                
                                                ; Disable automatic FP register content
                                                ; Disable lazy context switch
                LDR.W   R0, =0xE000EF34         ; Load address to FPCCR register
                LDR     R1, [R0]
                AND     R1,  R1, #(0x3FFFFFFF)  ; Clear the LSPEN and ASPEN bits
                STR     R1, [R0]
                ISB                             ; Reset pipeline now the FPU is enabled
                ENDIF
				 
				 
				 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP
				 				 