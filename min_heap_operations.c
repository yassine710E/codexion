#include "coders/codexion.h"

void push(min_heap *m_heap,t_coder coder)
{
    m_heap->queue[m_heap->size] = coder;
    int current_index = m_heap->size;
    int parent;
    t_coder tmp;
    while (current_index)
    {
        parent = (current_index - 1)/2;
        if (m_heap->queue[current_index].priorety < m_heap->queue[parent].priorety)
        {
             tmp = m_heap->queue[current_index];
             m_heap->queue[current_index] = m_heap->queue[parent];
             m_heap->queue[parent] = tmp;
        }
        else
            break;
        
        current_index = parent;        
    }
    
    m_heap->size++;

}

void pop(min_heap *m_heap)
{
    t_coder tmp;
    if(m_heap->size > 0)
    {
        tmp = m_heap->queue[m_heap->size - 1];
        m_heap->queue[m_heap->size - 1] = m_heap->queue[0];
        m_heap->queue[0] = tmp;
        m_heap->size--;
        unsigned int i = 0;
        while (i < m_heap->size)
        {
            unsigned int left_child = (2 * i)+1;
            unsigned int right_child = (2 * i)+2;
            if (left_child < m_heap->size && m_heap->queue[left_child].priorety < m_heap->queue[i].priorety)
            {
                tmp = m_heap->queue[left_child];
                m_heap->queue[left_child] = m_heap->queue[i];
                m_heap->queue[i] = tmp;
            }
            if (right_child < m_heap->size && m_heap->queue[right_child].priorety < m_heap->queue[i].priorety)
            {
                tmp = m_heap->queue[right_child];
                m_heap->queue[right_child] = m_heap->queue[i];
                m_heap->queue[i] = tmp;
            }


            i++;
        }
        
    }
}   


