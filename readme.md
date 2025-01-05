# rinth

rinth is a lifted planning engine.

It features:
- A 2 GB/s Parser
- SQL based successor generation with SQLite3
- And not much else

It is technically, probably, complete, in that it can find a path from the initial state to goal. However, it is very feature-limited. As it can not handle most anything more complex than Gripper, one of the simplest domains.

## Building
To build the executable, simply run the following:
```
make
```
The resulting executable will be generated at ./rinth

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
