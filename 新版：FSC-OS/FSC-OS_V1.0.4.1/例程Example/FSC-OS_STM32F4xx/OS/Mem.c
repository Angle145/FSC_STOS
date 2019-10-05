#include "mem.h"
/*-------------------------------------------内存管理(Page Up)----------------------------------------------*/
/*
内存管理设计：
将内存池视为一个线性区块，区块的开头4byte(32bit)*4=16byte用于记录该区块的信息,其中第二个数据用于标志该区块是否空闲，如下图：
Block:[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]--[byteN-2][byteN-1][byteN]
      {        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-16)byte(=struct+1)  }
                 size(N)            1(==1空闲标志)        last             next                     data
一、申请内存时，从内存池开始地址开始查找空闲内存区块，如查找到的空闲内存区块size大于所申请的内存size，则分割出所申请内存大小内存区块(从前面开始分)，
           分割出去的内存空间=区块信息长度+申请内存大小。如申请4*64byte=256byte内存，则分割出去的内存=16+256=272。例：

1.第一次申请4*64byte内存，分割出去的内存=16+256=272，得到的内存区块：
						[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]         --       [byte271]
						{        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
												256                0(==0使用标志)       last             next                      data
剩余内存区块：[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]   --   [byteN-(16+256)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit) } {  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
             size(N-(16+256))       1(==1空闲标志)            last               next                      data

2.第二次申请4*32byte内存，分割出去的内存=16+128=144，得到的内存区块：
						[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]         --       [byte415]
						{   4byte(32bit)    }{   4byte(32bit)   }{    4byte(32bit) }{   4byte(32bit)   }{   address:(N-16)byte(=struct+1)  }
								   256              0(==0使用标志)            last               next                     data
剩余内存区块：[byte416]--[byte419][byte420]--[byte423][byte424]--[byte427][byte428]--[byte431][byte432]--[byteN-(16+256)-(16+128)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit)  }{   4byte(32bit)   }{  address:(N-16)byte(=struct+1)  }
       size(N-(16+256)-(16+128))    1(==1空闲标志)            last                next                       data
			 
二、内存释放时，只是将该内存区块的标志位置1(空闲)，并没有整理把该区域与所有空闲区块合并在一起。释放内存后再申请内存容易造成内存碎片。
              OS有提供专门的内存碎片整理函数。

三、由于没有对内存池进行事先分区，当申请的内存过多时，申请的速度会越来越慢，不过对单片机小容量内存池来说影响不大，如果内存池比较大
             则必须事先对内存池进行分区，这样申请内存时高效快捷，也易于内存管理。
*/        
struct memblock{  
    os_u32 size;                  //内存区块大小  
    os_u32 free;                  //是否已使用 
    struct memblock *last;        //指向上一个内存区块	
    struct memblock *next;        //指向下一个内存区块  
};
os_mem_stack memorypool[OS_MEMORYPOOL_SIZE];
struct memblock *memblockList = (void *)memorypool; 
os_u32 os_mem_free_bytes_get(void)
{
	os_u32 addsize=0;
	struct memblock *curr=memblockList;
	os_thread_sched_lock();
	while(curr!=NULL)
	{
		if(curr->free) addsize+=curr->size;
		curr=curr->next;
	}
	os_thread_sched_unlock();
	return addsize;
}
void os_mem_init(void)  
{          
    memblockList->size=OS_MEMORYPOOL_SIZE-sizeof(struct memblock);//可用内存空间大小  
    memblockList->free=1;                                      //1：内存空闲 0：内存已使用 
    memblockList->last=NULL;                                   //指向空内存 	
    memblockList->next=NULL;                                   //指向空内存 
}

void os_mem_split(struct memblock *fitting_slot,size_t size)//内存分割(从大区块分割出一部分作为申请到的内存，实质是把空闲信息结构体往后移动,再将前面的信息结构体的标志改为使用)
{
    struct memblock *memnew=(void*)((os_u8*)fitting_slot+size+sizeof(struct memblock));  //重新定位剩余空闲的地址
    os_thread_sched_lock();
	  memnew->size=(fitting_slot->size)-size-sizeof(struct memblock);                   //重新定义剩余空闲size大小
    memnew->free=1;                                                        //设置空闲
    memnew->last=fitting_slot;
   	memnew->next=fitting_slot->next;                                       //插入到fitting_slot与fitting_slot->next中间
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=memnew;                                             //插入到fitting_slot与fitting_slot->next中间
    os_thread_sched_unlock();
}
void *os_mem_malloc(size_t noOfBytes)//内存申请(每个内存块的开始12个字节用来保存该块内存使用信息的结构体数据)
{   
	  struct memblock *curr;
    void *result;
	  os_thread_sched_lock();
    if(!(memblockList->size))
    {
        os_mem_init();
    }
    curr=memblockList;
    while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL))//从内存池开始地址查找空闲内存块
    {
        curr=curr->next;
    }
    if((curr->size)==noOfBytes) //剩余内存恰好相等
    {
        curr->free=0;
        result=(void*)(++curr);//返回指向数据(data区)起始地址
    }
    else if((curr->size)>(noOfBytes+sizeof(struct memblock)))         //剩余内存充足
    {
        os_mem_split(curr,noOfBytes);                                        //分割所需的内存出去
        result=(void*)(++curr);                                       //返回指向数据(data区)起始地址
    }
    else  //剩余内存不足
    {
        result=NULL;
    }
		os_thread_sched_unlock();
		return result;
}

void os_mem_merge(void)//合并相邻空闲内存区块(小优化)
{
	  struct memblock *curr;//*prev;
    curr=memblockList;
	  os_thread_sched_lock();
	  if(curr!=NULL)
		{
			while((curr->next)!=NULL)
			{ 
					if((curr->free) && (curr->next->free)) 
					{                                     
							curr->size += (curr->next->size)+sizeof(struct memblock);
							curr->next = curr->next->next;
						  curr->next->next->last=curr;
					}
					curr=curr->next;
			}
		}
		os_thread_sched_unlock();
}
void os_mem_free(void* ptr)//释放内存(输入形参地址必须是FSC_Malloc函数返回的地址)
{   
    if(((void*)memorypool<=ptr)&&(ptr<=(void*)(memorypool+OS_MEMORYPOOL_SIZE)))//检查要释放的地址是否在内存池范围内
    {
        struct memblock* curr=ptr;
        --curr;   //从data区回退到区块信息区
        curr->free=1;//置空闲
       // os_mem_merge();//合并相邻空闲区块(小优化，大优化需要内存碎片整理)
     }
     else return;
}

os_bool os_mem_frag_check(void)
{
	os_bool result=os_true;
	struct memblock *curr;
	os_thread_sched_lock();
	curr=memblockList;
	while(curr->next!=NULL)
	{
		if(curr->free) 
		{
		  if(!(curr->next->free)) 
			{
				result=os_true;
				break;
			}
		}
		curr=curr->next;
	}
	os_thread_sched_unlock();
	return result;
}
void os_mem_frag_merge(void)//内存碎片整理(只能在空闲任务中使用，在其他地方调用会使系统崩溃。处理耗时非常久，请在足够空闲时使用)
{
	os_u64 i,devi,len=0;
	struct memblock *curr=memblockList;
	struct memblock *le=memblockList;
	struct memblock* mbk;
	os_mem *ptr_start;
	
	os_thread_sched_lock();
	os_mem_merge();//合并相邻空闲内存块
	
	while(curr->next!=NULL) //查找除最后一个内存块的所有内存块
	{
		if(curr->free)//查找空闲内存块
		{		
			le=memblockList;
			while(le!=curr)//统计内存池首地址到当前内存块的内存长度
			{
				 len+=le->size+sizeof(len);
				 le=le->next;
			}
			len=OS_MEMORYPOOL_SIZE-len;//计算剩余长度
			mbk=curr;//备份内存块信息
			curr->next->last=curr->last;
			(*curr)=(*(curr->next));		
			ptr_start=(void*)(++curr);
			devi=mbk->size+sizeof(mbk);
			for(i=0;i<len;i++)   //复制(移动)数据区(耗时非常久)
			{
				ptr_start[i]=ptr_start[i+devi];
			}
			le=curr;
			mbk=le;//备份内存块信息
			le->next=le->next-devi;
			le=mbk->next;
			while(le!=NULL)
			{
				mbk=le;//备份内存块信息
				le=(struct memblock*)((os_u8*)le-devi);
				le->last=le->last-devi;
				if(le->next!=NULL) le->next=le->next-devi;
			  le=mbk->next;
			}
		}
		curr=curr->next;
	}
	os_thread_sched_unlock();
}

os_u32 os_mem_fragment_rate_get(void)
{
  struct memblock *curr=memblockList;
	os_u32 frag_mem_sum=0;
	os_thread_sched_lock();
	while(curr->next!=NULL)//统计内存池中间空闲的内存
	{
		if(curr->free)
		{
		  frag_mem_sum+=curr->size+sizeof(curr);
		}
		curr=curr->next;
	}
	os_thread_sched_unlock();
	return frag_mem_sum;
}
/*-------------------------------------------内存管理(Page Down)----------------------------------------------*/

