# rinth

For now simply a parser of PDDL.

The goal is at some point to make it into a grounded planner, however, that is on a standstill.

## Building
To build the executable, simply run the following:
```
make build
```
The resulting executable will then be found in ./build/

## Tests
To run tests, simply run the following:
```
make test
```
Example Output
```
gcc -Wall -Wextra -Wshadow -Isrc -Ithird_party -ggdb -O0 -flto -std=gnu17 -march=native -o testrunner src/misc.c
 src/parse.c src/io.c src/expand.c src/log.c src/task.c src/state.c src/sql.c test/parse.c test/expand.c test/st
ate.c test/main.c -lm -lsqlite3
./testrunner
[==========] Running 29 test cases.
[ RUN      ] state.equal
[       OK ] state.equal (852ns)
...
[ RUN      ] parse.domain_empty
[       OK ] parse.domain_empty (421ns)
[==========] 29 test cases ran.
[  PASSED  ] 29 tests.
```
