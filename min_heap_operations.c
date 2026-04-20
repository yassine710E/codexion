#include "coders/codexion.h"

void push(min_heap *m_heap,t_coder coder)
{
    m_heap->queue[m_heap->size] = coder;

    unsigned int i = 0;
    m_heap->size++;
    while (i < m_heap->size)
    {
        unsigned int left_child_index = (i*2) + 1;
        unsigned int right_child_index = (i*2) + 2;
        t_coder tmp;
        if(left_child_index < m_heap->size && m_heap->queue[i].priorety > m_heap->queue[left_child_index].priorety)
        {
            tmp = m_heap->queue[i];
            m_heap->queue[i] = m_heap->queue[left_child_index];
            m_heap->queue[left_child_index] = tmp;
        }
        if(right_child_index < m_heap->size && m_heap->queue[i].priorety > m_heap->queue[right_child_index].priorety)
        {
            tmp = m_heap->queue[i];
            m_heap->queue[i] = m_heap->queue[right_child_index];
            m_heap->queue[right_child_index] = tmp;
        }
        i++;
    }
    
    
    
    

}


