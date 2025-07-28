#include "scheduler.h"
#include "logger.h"
#include "tim.h"
#include <stdint.h>

#define MAX_TASKS 4 // Max tasks that scheduler can accommodate. can be changed
#define STACK_SIZE 128 // Words not bytes
#define STACK_CANARY 0xDEADBEEF // Magic number placed at start of stack to detect overflows

typedef struct {
    task_func_t fn;            // Pointer to task function
    void *arg;                 // Argument to pass to task
    uint32_t stack[STACK_SIZE]; // Stack space for task
    int8_t is_ready;            // Whether task is ready to run
    uint32_t priority;         // Lower value = higher priority
} task_t;

static task_t tasks[MAX_TASKS];
static uint8_t num_tasks = 0;
static int8_t current_task = -1;

void scheduler_init(void) {
    num_tasks = 0;
}

void create_task(task_func_t fn, void *arg, uint32_t priority) {
    if (num_tasks >= MAX_TASKS) return;

    tasks[num_tasks].fn = fn;
    tasks[num_tasks].arg = arg;
    tasks[num_tasks].stack[0] = STACK_CANARY;
    tasks[num_tasks].is_ready = 1;
    tasks[num_tasks].priority = priority;
    num_tasks++;
}

extern TIM_HandleTypeDef htim2;
char msg_buf[64];

void scheduler_run(void) {
    int8_t best_index = -1; // Index of task selected to run
    uint32_t best_priority = 0xFFFFFFFF; //Smallest priority value

    for (int i = 0; i < num_tasks; ++i) {
        task_t *t = &tasks[i];

        if (t->is_ready) {
            if (t->stack[0] != STACK_CANARY) {
                logger_log("Stack overflow detected!");
                t->is_ready = 0;
                continue;
            }
            // Find task with highest priority (lowest value)
            if (t->priority < best_priority) {
                best_priority = t->priority;
                best_index = i;
            }
        }
    }
    if (best_index >= 0) {
    	current_task = best_index;
        task_t *t = &tasks[best_index];

        uint32_t start = __HAL_TIM_GET_COUNTER(&htim2); // start time
        t->fn(t->arg);  // Run the task
        uint32_t end = __HAL_TIM_GET_COUNTER(&htim2); // end time

        uint32_t elapsed = (end >= start) ? (end - start) : (0xFFFFFFFF - start + end); // to know the total time of the task. correction for overflow also there
        snprintf(msg_buf, sizeof(msg_buf), "Task %d ran for %lu us", best_index + 1, elapsed);
        logger_log(msg_buf);
    }
}
void task_yield(void) {
    if (current_task >= 0 && current_task < num_tasks) {
        tasks[current_task].is_ready = 0; // mark the current task as not ready
    }
}
int scheduler_all_tasks_done(void) {
    for (int i = 0; i < num_tasks; ++i) {
        if (tasks[i].is_ready) return 0;
    }
    return 1;
}

void scheduler_reset_all_tasks(void) {
    for (int i = 0; i < num_tasks; ++i) {
        tasks[i].is_ready = 1;
    }
}

//
//typedef struct {
//    task_func_t fn;
//    void *arg;
//    uint32_t stack[STACK_SIZE];
//    int is_ready;
//} task_t;
//
//static task_t tasks[MAX_TASKS];
//static int current_task = 0;
//static int num_tasks = 0;
//
//void scheduler_init(void) {
//    num_tasks = 0;
//    current_task = -1;
//}
//
//
//void create_task(task_func_t fn, void *arg) {
//    if (num_tasks >= MAX_TASKS) return;
//
//    tasks[num_tasks].fn = fn;
//    tasks[num_tasks].arg = arg;
//    tasks[num_tasks].stack[0] = STACK_CANARY;
//    tasks[num_tasks].is_ready = 1;
//    num_tasks++;
//}
//
//extern TIM_HandleTypeDef htim2;
//char msg_buf[64];
//
//void scheduler_run(void) {
//
//
//    for (int i = 0; i < num_tasks; ++i) {
//        current_task = (current_task + 1) % num_tasks; //round robin task selection. circular rotation
//        task_t *t = &tasks[current_task];
//
//        if (t->is_ready) {
//            if (t->stack[0] != STACK_CANARY) {
//                logger_log("Stack overflow detected!");
//                t->is_ready = 0;
//                continue;
//            }
//
//            uint32_t start = __HAL_TIM_GET_COUNTER(&htim2);
//
//            t->fn(t->arg);  // Run task
//
//            uint32_t end = __HAL_TIM_GET_COUNTER(&htim2);
//            uint32_t elapsed = (end >= start) ? (end - start) : (0xFFFFFFFF - start + end);
//
//
//            snprintf(msg_buf, sizeof(msg_buf), "Task %d ran for %lu us", current_task + 1, elapsed);
//            logger_log(msg_buf);
//
//            break;
//        }
//    }
//}
//
//void task_yield(void) {
//
//   Cooperative yield — just return to scheduler
//}
//
