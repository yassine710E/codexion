#ifndef CODEXION
#define CODEXION

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#define TIME unsigned int 
typedef struct
{   
    unsigned int number_of_coders; 
    TIME time_to_burnout;
    TIME time_to_compile;
    TIME time_to_debug;
    TIME time_to_refactor;
    unsigned int number_of_compiles_required;
    TIME dongle_cooldown;
    char *scheduler;

} t_args;

typedef struct s_dongle
{
    unsigned int dongle_id;

} t_dongle;

typedef struct s_coder
{
    unsigned int coder_id;
    unsigned int priorety;
    unsigned int count_compiled;
    t_dongle *left;
    t_dongle *right;
} t_coder;

typedef struct 
{
    t_coder *queue;
    unsigned int size;
} min_heap;

typedef struct
{
    min_heap *m_heap;
    t_coder coder;
    t_args *args;
    pthread_mutex_t *mutex_1;
    pthread_cond_t *cond_1;
    unsigned int *c_compiling_at_the_sametime;
} t_shared_data;





int parsing (int c,char **v,t_args *args);
t_dongle *ft_create_dongle(unsigned int dongle_id);
t_coder *ft_create_coder(unsigned int coder_id,unsigned int priorety,t_dongle *left,t_dongle *right);
void push(min_heap *m_heap,t_coder coder);
void pop(min_heap *m_heap);
#endif 