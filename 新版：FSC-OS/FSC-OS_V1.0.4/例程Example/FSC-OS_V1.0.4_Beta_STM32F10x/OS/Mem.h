#ifndef _MEM_H_
#define _MEM_H_
#include "os.h"
/*-----------=--��̬�ڴ������--------------*/
void os_mem_frag_merge(void);
os_bool os_mem_frag_check(void);
void *os_mem_malloc(size_t noOfBytes);             //�ڴ�����(��λ:byte)
void os_mem_free(void* ptr);                       //�ڴ��ͷ�(�����ַ����������õ��ĵ�ַ)
os_u32 os_mem_free_bytes_get(void);                //��ȡ�ڴ��ʣ���ڴ�(��λ��Byte)
os_u32 os_mem_fragment_rate_get(void);             //��ȡ�ڴ���ڴ���Ƭ(��λ��Byte)
#endif
