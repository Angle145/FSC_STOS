os_tcb* os_tcb_highest_prio_next_rdy_get(void)//��ȡ�����ȼ�����tcb�б����ҳ����Ѿ���������ȼ��е��¸�Ҫ���е�ͬ�����ȼ��߳�
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_highestrio;

	os_tcb_prio_sort_table_highestrio=os_tcb_prio_sort_table_highest_prio_rdy_get();
	if(os_tcb_prio_sort_table_highestrio->list_same_prio_cur->next!=NULL)  os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_prio_sort_table_highestrio->list_same_prio_cur->next;
	else os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_prio_sort_table_highestrio->list_same_prio_front;
	return os_tcb_prio_sort_table_highestrio->list_same_prio_cur;
}
os_tcb* os_tcb_same_prio_next_rdy_get(void)
{
	os_tcb_prio_sort_table *os_tcb_prio_sort_table_highestrio=os_p.os_tcb_highest_prio_sort_table;

	if(os_tcb_prio_sort_table_highestrio->list_same_prio_cur->next!=NULL)  os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_prio_sort_table_highestrio->list_same_prio_cur->next;
	else os_tcb_prio_sort_table_highestrio->list_same_prio_cur=os_tcb_prio_sort_table_highestrio->list_same_prio_front;
	return os_tcb_prio_sort_table_highestrio->list_same_prio_cur;
}