#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <stdint.h>

//void scheduler_init(void);
//void scheduler_run(void);
//void create_task(void (*task_fn)(void *), void *param);
//void task_yield(void);


typedef void (*task_func_t)(void *);
void scheduler_init(void);
void scheduler_run(void);
void create_task(task_func_t fn, void *arg, uint32_t priority);
//void create_task(task_func_t fn, void *arg);
void task_yield(void);              // Called by task to release control
int scheduler_all_tasks_done(void);
void scheduler_reset_all_tasks(void);


#endif
