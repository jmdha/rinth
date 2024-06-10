[![C/C++ CI](https://github.com/jamadaha/rinth/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/jamadaha/rinth/actions/workflows/c-cpp.yml)
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
> [!IMPORTANT]
> The tests require [Criterion](https://github.com/Snaipe/Criterion). As such, it is required to be installed on the system. This is not automatic.

Example Output
```
mkdir -p build
gcc -Isrc -g -O3 -Wall -Wextra -pedantic -flto -std=c17 -march=native -O0 -o build/test tests/*.c src/log.c src/lexer.c src/token.c -lcriterion
build/test
[       lexing.action       ] [ 0,000233 s ] [   OK   ]
[       lexing.carriage     ] [ 0,000239 s ] [   OK   ]
[       lexing.eof          ] [ 0,000239 s ] [   OK   ]
[       lexing.id           ] [ 0,000235 s ] [   OK   ]
[       lexing.lparen       ] [ 0,000242 s ] [   OK   ]
[       lexing.effect       ] [ 0,000267 s ] [   OK   ]
[       lexing.newline      ] [ 0,000245 s ] [   OK   ]
[       lexing.parameters   ] [ 0,000226 s ] [   OK   ]
[       lexing.precondition ] [ 0,000255 s ] [   OK   ]
[       lexing.predicate    ] [ 0,000264 s ] [   OK   ]
[       lexing.rparen       ] [ 0,000115 s ] [   OK   ]
[       lexing.space        ] [ 0,000080 s ] [   OK   ]
[       lexing.tab          ] [ 0,000073 s ] [   OK   ]
[====] Synthesis: Tested: 13 | Passing: 13 | Failing: 0 | Crashing: 0 
```
