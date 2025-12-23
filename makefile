NAME    = rinth
CFLAGS += -Isrc -Ithird_party
CFLAGS += -O3 -flto -ggdb
LIBS   +=

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(CFLAGS) -o $(NAME) \
	src/main.c \
	src/misc.c \
	src/pddl_parse.c src/pddl_validate.c \
	$(LIBS)

for bench: LIBS += -lm
bench:
	gcc $(CFLAGS) -o benchmark \
	bench/main.c \
	bench/pddl_parse.c \
	src/misc.c \
	src/pddl_parse.c src/pddl_validate.c \
	$(LIBS)

test:
	gcc $(CFLAGS) -o testrunner \
	test/main.c \
	test/pddl_parse.c \
	src/misc.c \
	src/pddl_parse.c src/pddl_validate.c \
	$(LIBS)
