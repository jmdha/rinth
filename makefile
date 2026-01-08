NAME    = rinth
CFLAGS += -Isrc -Ithird_party
CFLAGS += -O3 -flto -ggdb
CFLAGS += -Wall
#CFLAGS += -fsanitize=address
LIBS   +=

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(CFLAGS) -o $(NAME) \
	src/main.c \
	src/args.c src/bit.c src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/state_heap.c src/state_queue.c src/state_set.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/eval.c src/eval_goal_count.c \
	src/search.c src/search_bfs.c src/search_gbfs.c src/search_lazy.c \
	$(LIBS)

for bench: LIBS += -lm
bench:
	gcc $(CFLAGS) -o benchmark \
	bench/main.c \
	bench/pddl_parse.c \
	src/args.c src/bit.c src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/state_heap.c src/state_queue.c src/state_set.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/eval.c src/eval_goal_count.c \
	src/search.c src/search_bfs.c src/search_gbfs.c src/search_lazy.c \
	$(LIBS)

test:
	gcc $(CFLAGS) -o testrunner \
	test/main.c \
	test/algo.c test/pddl_parse.c test/translate_pddl.c test/expand.c \
	test/state.c test/state_heap.c \
	src/args.c src/bit.c src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/state_heap.c src/state_queue.c src/state_set.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/eval.c src/eval_goal_count.c \
	src/search.c src/search_bfs.c src/search_gbfs.c src/search_lazy.c \
	$(LIBS)
