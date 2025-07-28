#include "tasks.h"
#include "scheduler.h"
#include "logger.h"

void task1(void *param) {
    logger_log("Task 1 running");
    HAL_Delay(1500);
    task_yield();
}

void task2(void *param) {
    logger_log("Task 2 running");
    HAL_Delay(1500);
    task_yield();
}

void task3(void *param) {
    logger_log("Task 3 running");
    HAL_Delay(1500);
    task_yield();
}
