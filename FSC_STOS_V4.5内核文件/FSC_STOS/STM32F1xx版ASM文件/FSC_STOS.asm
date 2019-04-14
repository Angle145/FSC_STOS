;asm for M0 ( stm32f1 ) 

    IMPORT    OSTCBRun
    IMPORT    OSTCBCur            
	IMPORT    OSPendSVPulse 
    EXPORT    OS_INT_ENTER        
    EXPORT    OS_INT_EXIT
    EXPORT    PendSV_Handler
	EXPORT	  OSStartUp	
    PRESERVE8 
    AREA |.text|, CODE, READONLY
    THUMB         
OS_INT_ENTER     
    CPSID    I   
    BX    LR    
OS_INT_EXIT      
    CPSIE    I   
    BX    LR                                 
OSStartUp                            
    CPSID    I                                    
    LDR R4,=0x0                  
    MSR    PSP, R4                 
    LDR     R0,=OSPendSVPulse      
	BLX     R0
    CPSIE    I                     
OSStartPending                        
    B    OSStartPending  
PendSV_Handler
    CPSID    I                     
    MRS     R0, PSP                
    CBZ     R0, OSFirstEnter      
    SUB     R0, R0, #0x20          
    STM     R0, {R4-R11}                                         
    LDR     R1, =OSTCBRun          
    LDR     R1, [R1]               
    STR     R0, [R1]               
OSFirstEnter    
    LDR     R0, =OSTCBRun          
    LDR     R1, =OSTCBCur          
    LDR     R2, [R1]               
    STR     R2, [R0]               	                               
    LDR     R0, [R2]               
	LDM     R0, {R4-R11}           
	ADD     R0, R0, #0x20                                           
    MSR     PSP, R0                
    ORR     LR, LR, #0x04          
    CPSIE     I                    
    BX    LR                       
    align 4   
    end     ;by Angle_145    
		