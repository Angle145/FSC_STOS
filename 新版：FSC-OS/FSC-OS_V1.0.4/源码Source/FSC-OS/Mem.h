#ifndef _MEM_H_
#define _MEM_H_
#include "os.h"
/*-----------=--动态内存管理函数--------------*/
void os_mem_frag_merge(void);
os_bool os_mem_frag_check(void);
void *os_mem_malloc(size_t noOfBytes);             //内存申请(单位:byte)
void os_mem_free(void* ptr);                       //内存释放(输入地址必须是申请得到的地址)
os_u32 os_mem_free_bytes_get(void);                //获取内存池剩余内存(单位：Byte)
os_u32 os_mem_fragment_rate_get(void);             //获取内存池内存碎片(单位：Byte)
#endif
