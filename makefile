NAME   = rinth
CFLAGS += -Isrc -Ithird_party
CFLAGS += -O3 -flto
CFLAGS += -lsqlite3

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(CFLAGS) -o $(NAME) \
	src/main.c \
	src/db.c src/eval.c src/expand.c src/io.c src/log.c src/misc.c src/parse.c \
	src/search.c src/sql.c src/state.c src/statespace.c src/task.c src/translate.c

for bench: CFLAGS += -lm
bench:
	gcc $(CFLAGS) -o benchmark \
	bench/main.c \
	bench/parse.c bench/state.c \
	src/db.c src/eval.c src/expand.c src/io.c src/log.c src/misc.c src/parse.c \
	src/search.c src/sql.c src/state.c src/statespace.c src/task.c src/translate.c

test:
	gcc $(CFLAGS) -o testrunner \
	test/main.c \
	test/expand.c test/parse.c test/state.c test/translate.c \
	src/db.c src/eval.c src/expand.c src/io.c src/log.c src/misc.c src/parse.c \
	src/search.c src/sql.c src/state.c src/statespace.c src/task.c src/translate.c
