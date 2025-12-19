#include "task.h"

void task_free(
	task_t* task
) {
    state_free(task->init);
    state_free(task->goal);
}
