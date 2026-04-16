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
    int state_dongle;

} t_dongle;

typedef struct s_coder
{
    unsigned int coder_id;
    unsigned int count_compiled;
    t_dongle *left;
    t_dongle *right;
    struct s_coder *next;
} t_coder;


typedef struct
{
    unsigned int id_thread;
    t_coder *queue_coders;
    t_args *args;
} t_shared_data;


int parsing (int c,char **v,t_args *args);
void set_coders(t_coder *coder,unsigned int n_coders);
void set_dongles (t_coder *coder);
void set_shared_data(unsigned int id,t_shared_data *data,t_coder *coders,t_args *args);
void free_pointer(void *ptr);
t_coder *get_coder(t_coder *coder,unsigned int id_coder);
#endif 