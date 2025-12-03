#include "task.h"

void task_free(
	struct task* task
) {
    state_free(task->init);
    state_free(task->goal);
}
