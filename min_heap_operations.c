#include "coders/codexion.h"

void update_queue()
{

}

void push(min_heap *m_heap,t_coder *coder,char *scheduler)
{
    int context_parent,context_left_child,context_right_child = -1;
    (void)scheduler;
    t_coder *arr_coder = (t_coder *)(m_heap->queue);
    arr_coder[m_heap->size] = *coder;
    m_heap->size += 1;
    
    int i = 0;
    while (i < m_heap->size && m_heap->size >= 2)
    {
        if(!strcmp("fifo",scheduler))
        {
            context_parent = arr_coder[i].priority_for_fifo;
            if((2*i + 1) < m_heap->size)
                context_left_child = arr_coder[(2*i + 1)].priority_for_fifo;
            if((2*i + 2) < m_heap->size)
                context_right_child = arr_coder[(2*i + 2)].priority_for_fifo;
        }
        if(context_parent >= 1 && context_left_child >= 1 && context_parent > context_left_child)
        {
            swap(arr_coder + i,arr_coder + (2*i + 1));
        }
        if(context_parent >= 1 && context_right_child >= 1 && context_parent > context_right_child)
        {
            swap(arr_coder + i,arr_coder + (2*i + 2));
        }
        context_parent = -1;
        context_left_child = -1;
        i++;
    }
}

void pop(min_heap *m_heap)
{
    if(m_heap->size > 1)
    {
        int i = 0;
        while (++i < m_heap->size){
            t_coder *queue = (t_coder *)m_heap->queue;
            queue[i-1] = queue[i];
        }
    }
    m_heap->size --;       
}

void swap (t_coder *item1,t_coder *item2)
{
    if(!item1 || !item2)
        return;
    t_coder tmp;
    tmp = *item1;
    *item1 = *item2;
    *item2 = tmp;
}