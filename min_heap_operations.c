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
    if(!m_heap)
        return;
    if(m_heap->size > 1)
    {
        unsigned int i = 0;
        while (++i < m_heap->size)
            m_heap->queue[i - 1] = m_heap->queue[i]; 
        
    }
    m_heap->size--;
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
        if (!m_heap->queue[i].state_coder)
        {
            tmp = m_heap->queue[i];
            m_heap->queue[i] = m_heap->queue[index_from];
            m_heap->queue[index_from] = tmp;
            *index_ptr = i;
            break;
        }        
    }
    
}


void update_index(int *ptr_to_index,min_heap *m_heap,unsigned int coder_id)
{
    *ptr_to_index = ft_get_index(m_heap,coder_id);
}

//[1,2,3]
//[2,3,3]
void rotate_queue(min_heap *m_heap)
{
    if(m_heap->size > 1)
    {
        t_coder coder_tmp = m_heap->queue[0];
        unsigned int i = 0;
        while (++i < m_heap->size)
        {
            m_heap->queue[i-1] = m_heap->queue[i];
            if(i == m_heap->size -1)
                m_heap->queue[i] = coder_tmp;
        }
        

    }
    if(m_heap->size > 0)
        m_heap->size --;
}