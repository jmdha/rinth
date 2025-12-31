NAME    = rinth
CFLAGS += -Isrc -Ithird_party
CFLAGS += -O3 -flto -ggdb
LIBS   +=

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(CFLAGS) -o $(NAME) \
	src/main.c \
	src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/states.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/search.c \
	$(LIBS)

for bench: LIBS += -lm
bench:
	gcc $(CFLAGS) -o benchmark \
	bench/main.c \
	bench/pddl_parse.c \
	src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/states.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/search.c \
	$(LIBS)

test:
	gcc $(CFLAGS) -o testrunner \
	test/main.c \
	test/algo.c test/pddl_parse.c test/translate_pddl.c test/expand.c \
	src/misc.c src/io.c src/log.c src/algo.c \
	src/pddl_parse.c src/pddl_validate.c \
	src/task.c src/translate_pddl.c \
	src/state.c src/states.c \
	src/expand.c src/expand_cp.c src/expand_sqlite.c \
	src/search.c \
	$(LIBS)
