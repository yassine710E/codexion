#include "coders/codexion.h"

void push(min_heap *m_heap,t_coder *coder)
{
    t_coder *arr_coder = (t_coder *)(m_heap->queue);
    arr_coder[m_heap->size] = *coder;
    m_heap->size += 1;
}

void pop(min_heap *m_heap)
{
    if(m_heap->size > 1)
    {
        unsigned int i = 0;
        while (++i < m_heap->size){
            t_coder *queue = (t_coder *)m_heap->queue;
            queue[i-1] = queue[i];
        }
    }
    m_heap->size --;       
}