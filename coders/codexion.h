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

typedef struct 
{
    void *queue;
    unsigned int size;

} min_heap;

typedef struct s_dongle
{
    unsigned int dongle_id;
    pthread_mutex_t *mutex_dongle;
    pthread_cond_t *cond_dongle;
    min_heap *m_heap; 

} t_dongle;

typedef struct s_coder
{
    unsigned int coder_id;
    unsigned int count_compiled;
    t_dongle *left;
    t_dongle *right;
} t_coder;


typedef struct
{
    t_coder *coder;
    t_args *args;
    pthread_cond_t *cond_display;
    pthread_mutex_t *mutex_display;
    pthread_mutex_t *main_mutex;
    pthread_cond_t *main_cond;
    struct timeval start;
} t_shared_data;




int parsing (int c,char **v,t_args *args);
min_heap *init_min_heap();
void set_dongle(t_dongle *dongle,unsigned int dongle_id,min_heap *m_heap,pthread_mutex_t *mutex,pthread_cond_t *cond);
void init_arr_mutex(pthread_mutex_t *ptr_mutex,unsigned int dongles_size);
void destroy_arr_mutex(pthread_mutex_t *ptr_mutex,unsigned int dongles_size);
void set_coders(t_coder *arr_coders , unsigned int size_coders ,pthread_mutex_t *ptr_mutex,pthread_cond_t *ptr_cond);
void init_arr_cond(pthread_cond_t *ptr_cond,unsigned int dongles_size);
void destroy_arr_cond(pthread_cond_t *ptr_cond,unsigned int dongles_size);
void set_shared_data (t_shared_data *arr_s_data,t_coder *arr_coders,t_args *args,pthread_mutex_t *d_mutex,pthread_cond_t *d_cond,pthread_mutex_t *main_mutex,pthread_cond_t *main_cond,struct timeval);
void *coder_routine (void *data);
void push(min_heap *m_heap,t_coder *coder);
void compile(TIME t_compile);
void debugging_hh(t_dongle *dongle);
void compile(TIME t_compile);
void cooldown(TIME t_cooldown);
void debugging(TIME t_debugging);
void refactoring(TIME t_refactoring);
void pop(min_heap *m_heap);
long get_timestamp_ms(struct timeval start);
#endif 