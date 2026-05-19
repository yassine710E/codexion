#include "codexion.h"


void push(min_heap *m_heap,t_coder *coder)
{
    int i = 0;
    t_coder *arr_coder = (t_coder *)(m_heap->queue);
    arr_coder[m_heap->size] = *coder;
    m_heap->size += 1;
    while (i < m_heap->size && m_heap->size >= 2)
    {
        if(2*i + 1 < m_heap->size && arr_coder[i].deadline > arr_coder[2*i + 1].deadline)
            swap(arr_coder + i,arr_coder + (2*i + 1));
        if(2*i + 2 < m_heap->size && arr_coder[i].deadline > arr_coder[2*i + 2].deadline)
            swap(arr_coder + i,arr_coder + (2*i + 2));
        i++;
    }
}

void pop(min_heap *m_heap)
{
    if(m_heap->size >= 1)
    {
        int i = 0;
        t_coder *queue = (t_coder *)m_heap->queue;
        while (++i < m_heap->size)
            queue[i-1] = queue[i];
        m_heap->size --;

    }

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