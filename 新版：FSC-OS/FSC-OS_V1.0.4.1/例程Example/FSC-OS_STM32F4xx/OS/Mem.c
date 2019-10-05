#include "mem.h"
/*-------------------------------------------�ڴ����(Page Up)----------------------------------------------*/
/*
�ڴ������ƣ�
���ڴ����Ϊһ���������飬����Ŀ�ͷ4byte(32bit)*4=16byte���ڼ�¼���������Ϣ,���еڶ����������ڱ�־�������Ƿ���У�����ͼ��
Block:[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]--[byteN-2][byteN-1][byteN]
      {        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-16)byte(=struct+1)  }
                 size(N)            1(==1���б�־)        last             next                     data
һ�������ڴ�ʱ�����ڴ�ؿ�ʼ��ַ��ʼ���ҿ����ڴ����飬����ҵ��Ŀ����ڴ�����size������������ڴ�size����ָ���������ڴ��С�ڴ�����(��ǰ�濪ʼ��)��
           �ָ��ȥ���ڴ�ռ�=������Ϣ����+�����ڴ��С��������4*64byte=256byte�ڴ棬��ָ��ȥ���ڴ�=16+256=272������

1.��һ������4*64byte�ڴ棬�ָ��ȥ���ڴ�=16+256=272���õ����ڴ����飺
						[byte0][byte1][byte2][byte3][byte4]--[byte7][byte8]--[byte11][byte12]--[byte15][byte16]         --       [byte271]
						{        4byte(32bit)      }{ 4byte(32bit) }{  4byte(32bit) }{  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
												256                0(==0ʹ�ñ�־)       last             next                      data
ʣ���ڴ����飺[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]   --   [byteN-(16+256)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit) } {  4byte(32bit)  }{  address:(N-12)byte(=struct+1)  }
             size(N-(16+256))       1(==1���б�־)            last               next                      data

2.�ڶ�������4*32byte�ڴ棬�ָ��ȥ���ڴ�=16+128=144���õ����ڴ����飺
						[byte272]--[byte275][byte276]--[byte279][byte280]--[byte283][byte284]--[byte287][byte288]         --       [byte415]
						{   4byte(32bit)    }{   4byte(32bit)   }{    4byte(32bit) }{   4byte(32bit)   }{   address:(N-16)byte(=struct+1)  }
								   256              0(==0ʹ�ñ�־)            last               next                     data
ʣ���ڴ����飺[byte416]--[byte419][byte420]--[byte423][byte424]--[byte427][byte428]--[byte431][byte432]--[byteN-(16+256)-(16+128)]
            {   4byte(32bit)   }{   4byte(32bit)    }{    4byte(32bit)  }{   4byte(32bit)   }{  address:(N-16)byte(=struct+1)  }
       size(N-(16+256)-(16+128))    1(==1���б�־)            last                next                       data
			 
�����ڴ��ͷ�ʱ��ֻ�ǽ����ڴ�����ı�־λ��1(����)����û������Ѹ����������п�������ϲ���һ���ͷ��ڴ���������ڴ���������ڴ���Ƭ��
              OS���ṩר�ŵ��ڴ���Ƭ��������

��������û�ж��ڴ�ؽ������ȷ�������������ڴ����ʱ��������ٶȻ�Խ��Խ���������Ե�Ƭ��С�����ڴ����˵Ӱ�첻������ڴ�رȽϴ�
             ��������ȶ��ڴ�ؽ��з��������������ڴ�ʱ��Ч��ݣ�Ҳ�����ڴ����
*/        
struct memblock{  
    os_u32 size;                  //�ڴ������С  
    os_u32 free;                  //�Ƿ���ʹ�� 
    struct memblock *last;        //ָ����һ���ڴ�����	
    struct memblock *next;        //ָ����һ���ڴ�����  
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
    memblockList->size=OS_MEMORYPOOL_SIZE-sizeof(struct memblock);//�����ڴ�ռ��С  
    memblockList->free=1;                                      //1���ڴ���� 0���ڴ���ʹ�� 
    memblockList->last=NULL;                                   //ָ����ڴ� 	
    memblockList->next=NULL;                                   //ָ����ڴ� 
}

void os_mem_split(struct memblock *fitting_slot,size_t size)//�ڴ�ָ�(�Ӵ�����ָ��һ������Ϊ���뵽���ڴ棬ʵ���ǰѿ�����Ϣ�ṹ�������ƶ�,�ٽ�ǰ�����Ϣ�ṹ��ı�־��Ϊʹ��)
{
    struct memblock *memnew=(void*)((os_u8*)fitting_slot+size+sizeof(struct memblock));  //���¶�λʣ����еĵ�ַ
    os_thread_sched_lock();
	  memnew->size=(fitting_slot->size)-size-sizeof(struct memblock);                   //���¶���ʣ�����size��С
    memnew->free=1;                                                        //���ÿ���
    memnew->last=fitting_slot;
   	memnew->next=fitting_slot->next;                                       //���뵽fitting_slot��fitting_slot->next�м�
    fitting_slot->size=size;
    fitting_slot->free=0;
    fitting_slot->next=memnew;                                             //���뵽fitting_slot��fitting_slot->next�м�
    os_thread_sched_unlock();
}
void *os_mem_malloc(size_t noOfBytes)//�ڴ�����(ÿ���ڴ��Ŀ�ʼ12���ֽ���������ÿ��ڴ�ʹ����Ϣ�Ľṹ������)
{   
	  struct memblock *curr;
    void *result;
	  os_thread_sched_lock();
    if(!(memblockList->size))
    {
        os_mem_init();
    }
    curr=memblockList;
    while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL))//���ڴ�ؿ�ʼ��ַ���ҿ����ڴ��
    {
        curr=curr->next;
    }
    if((curr->size)==noOfBytes) //ʣ���ڴ�ǡ�����
    {
        curr->free=0;
        result=(void*)(++curr);//����ָ������(data��)��ʼ��ַ
    }
    else if((curr->size)>(noOfBytes+sizeof(struct memblock)))         //ʣ���ڴ����
    {
        os_mem_split(curr,noOfBytes);                                        //�ָ�������ڴ��ȥ
        result=(void*)(++curr);                                       //����ָ������(data��)��ʼ��ַ
    }
    else  //ʣ���ڴ治��
    {
        result=NULL;
    }
		os_thread_sched_unlock();
		return result;
}

void os_mem_merge(void)//�ϲ����ڿ����ڴ�����(С�Ż�)
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
void os_mem_free(void* ptr)//�ͷ��ڴ�(�����βε�ַ������FSC_Malloc�������صĵ�ַ)
{   
    if(((void*)memorypool<=ptr)&&(ptr<=(void*)(memorypool+OS_MEMORYPOOL_SIZE)))//���Ҫ�ͷŵĵ�ַ�Ƿ����ڴ�ط�Χ��
    {
        struct memblock* curr=ptr;
        --curr;   //��data�����˵�������Ϣ��
        curr->free=1;//�ÿ���
       // os_mem_merge();//�ϲ����ڿ�������(С�Ż������Ż���Ҫ�ڴ���Ƭ����)
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
void os_mem_frag_merge(void)//�ڴ���Ƭ����(ֻ���ڿ���������ʹ�ã��������ط����û�ʹϵͳ�����������ʱ�ǳ��ã������㹻����ʱʹ��)
{
	os_u64 i,devi,len=0;
	struct memblock *curr=memblockList;
	struct memblock *le=memblockList;
	struct memblock* mbk;
	os_mem *ptr_start;
	
	os_thread_sched_lock();
	os_mem_merge();//�ϲ����ڿ����ڴ��
	
	while(curr->next!=NULL) //���ҳ����һ���ڴ��������ڴ��
	{
		if(curr->free)//���ҿ����ڴ��
		{		
			le=memblockList;
			while(le!=curr)//ͳ���ڴ���׵�ַ����ǰ�ڴ����ڴ泤��
			{
				 len+=le->size+sizeof(len);
				 le=le->next;
			}
			len=OS_MEMORYPOOL_SIZE-len;//����ʣ�೤��
			mbk=curr;//�����ڴ����Ϣ
			curr->next->last=curr->last;
			(*curr)=(*(curr->next));		
			ptr_start=(void*)(++curr);
			devi=mbk->size+sizeof(mbk);
			for(i=0;i<len;i++)   //����(�ƶ�)������(��ʱ�ǳ���)
			{
				ptr_start[i]=ptr_start[i+devi];
			}
			le=curr;
			mbk=le;//�����ڴ����Ϣ
			le->next=le->next-devi;
			le=mbk->next;
			while(le!=NULL)
			{
				mbk=le;//�����ڴ����Ϣ
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
	while(curr->next!=NULL)//ͳ���ڴ���м���е��ڴ�
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
/*-------------------------------------------�ڴ����(Page Down)----------------------------------------------*/

