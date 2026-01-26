# rinth

rinth is a lifted planning engine.

It features:
- A 2 GB/s Parser
- SQL based successor generation with SQLite3
- And not much else

It is technically, probably, complete, in that it can find a path from the initial state to goal. However, it is very feature-limited.

## Building
To build the executable, simply run the following:
```
make
```
The resulting executable will be generated at ./rinth

## Experiments
Experiments are intended to be run with gnu parallel.
```
parallel -j 4 ./expr.sh ::: blocksworld{0..89} ::: cp ::: bfs gbfs
```
