/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychabane <ychabane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 16:16:05 by ychabane          #+#    #+#             */
/*   Updated: 2026/05/22 01:29:45 by ychabane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define TIME unsigned int

typedef struct s_args
{
	int				number_of_coders;
	TIME			time_to_burnout;
	TIME			time_to_compile;
	TIME			time_to_debug;
	TIME			time_to_refactor;
	int				number_of_compiles_required;
	TIME			dongle_cooldown;
	char			*scheduler;

}					t_args;

typedef struct s_min_heap
{
	void			*queue;
	int				size;

}					t_min_heap;

typedef struct s_dongle
{
	int				dongle_id;
	pthread_mutex_t	*mutex_dongle;
	pthread_cond_t	*cond_dongle;
	long			last_compile_start_saver;
	t_min_heap		*m_heap;

}					t_dongle;

typedef struct s_coder
{
	int				coder_id;
	int				count_compiled;
	long			last_compile_start;
	int				is_waiting;
	long			deadline;
	t_dongle		*left;
	t_dongle		*right;
}					t_coder;

typedef struct s_shared_data
{
	t_coder			*coder;
	t_args			*args;
	int				*flag_burnout;
	int				*how_many_coders_finished;
	pthread_cond_t	*cond_display;
	pthread_mutex_t	*mutex_display;
	pthread_mutex_t	*main_mutex;
	pthread_cond_t	*main_cond;
	struct timeval	start;
}					t_shared_data;

typedef struct s_sync
{
	pthread_mutex_t	main_mutex;
	pthread_mutex_t	display_mutex;
	pthread_cond_t	main_cond;
	pthread_cond_t	display_cond;
}					t_sync;

typedef struct s_resources
{
	t_coder			*arr_coders;
	t_shared_data	*s_data;
	pthread_mutex_t	*mutex_arr;
	pthread_t		*arr_threads;
	pthread_t		thread_monitor;
}					t_resources;

int					parsing(int c, char **v, t_args *args);
t_min_heap			*init_min_heap(void);
void				set_dongle(t_dongle *dongle, unsigned int dongle_id,
						t_min_heap *m_heap, pthread_mutex_t *mutex);
void				init_arr_mutex(pthread_mutex_t *ptr_mutex,
						unsigned int dongles_size);
void				destroy_arr_mutex(pthread_mutex_t *ptr_mutex,
						unsigned int dongles_size);
void				set_coders(t_coder *arr_coders, unsigned int size_coders,
						pthread_mutex_t *ptr_mutex);
void				init_arr_cond(pthread_cond_t *ptr_cond,
						unsigned int dongles_size);
void				destroy_arr_cond(pthread_cond_t *ptr_cond,
						unsigned int dongles_size);

void				set_shared_data(t_shared_data *arr_s_data,
						t_coder *arr_coders, t_args *args, int *flag);
void				*coder_routine(void *data);
void				push(t_min_heap *m_heap, t_coder *coder);
void				debugging_hh(t_dongle *dongle);
int					sleep_for_operation(TIME t_operation, t_shared_data *s_data,
						long last_operation_start, char *msg);
void				pop(t_min_heap *m_heap);
long				get_timestamp_ms(struct timeval start);
void				swap(t_coder *item1, t_coder *item2);
int					get_min_context(char *scheduler, t_min_heap *m_heap);
void				debugging_hh(t_dongle *dongle);
void				request_dongle(t_coder *coder, t_dongle *dongle);
void				leave_dongle(t_dongle *dongle);
void				logs(pthread_mutex_t *display_mutex, char *message,
						long current_time, int coder_id);
void				set_deadline(char *scheduler, t_coder *coder,
						struct timeval start, TIME t_to_burnout);
int					is_coder_exist_in_queue(t_min_heap *m_heap, int coder_id,
						pthread_mutex_t *dongle_mutex);
void				*routine_monitor(void *data);
void				broadcast_other_coders(t_shared_data *s_data);
void				set_mutexes(t_shared_data *arr_s_data,
						pthread_mutex_t *d_mutex, pthread_mutex_t *main_mutex,
						t_args *args);
void				set_conditions(t_shared_data *arr_s_data,
						pthread_cond_t *d_cond, pthread_cond_t *main_cond,
						t_args *args);
void				set_start_time(t_shared_data *arr_s_data,
						struct timeval start, t_args *args);
void				set_pointer_to_counter_coders(t_shared_data *arr_s_data,
						int *coders_counter, t_args *args);
int					alloc_resources(t_resources *r, t_args *args);
void				cleanup(t_resources *r, t_sync *s, t_args *args);

void				free_coders_and_dongles_and_m_heap(t_args args,
						t_coder *arr_coders);
void				free_shared_data__mutexes_threads(t_shared_data *s_data,
						pthread_mutex_t *mutex_arr, pthread_t *arr_threads);
int					ft_atoi(char *str, int *is_overflow);

int					is_coder_exist_in_queue(t_min_heap *m_heap, int coder_id,
						pthread_mutex_t *dongle_mutex);
void				broadcast_other_coders(t_shared_data *s_data);
int					wait_for_waiter_coders(t_shared_data *s_data);
void				release_dongle(t_shared_data *s_data);
void				coder_loop(t_shared_data *s_data, t_coder *coder);

#endif