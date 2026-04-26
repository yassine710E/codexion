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

int ft_get_index (min_heap *m_heap,unsigned int coder_id)
{
    int i = -1;
    while (++i < (int)m_heap->size)
    {
        if (coder_id == m_heap->queue[i].coder_id)
            return i;
    }
    return -1;    
}

void move_coder(min_heap *m_heap,unsigned int index_from,int *index_ptr)
{
    t_coder tmp;
    if(index_from >= m_heap->size)
        return;
    int i = -1;
    while (++i < (int)index_from)
    {
        if (! m_heap->queue[i].state_coder)
        {
            tmp = m_heap->queue[i];
            m_heap->queue[i] = m_heap->queue[index_from];
            m_heap->queue[index_from] = tmp;
            *index_ptr = i;
            break;
        }        
    }
    
}


