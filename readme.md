# rinth

rinth is a lifted planning engine.

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

## Results

The following section describes the most recent experiment run, and parameters thereof.

Engines tested [rinth](https://github.com/jmdha/rinth), [powerlifted](https://github.com/abcorrea/powerlifted), and [fast downward](https://github.com/aibasel/downward). 

Problems [ipc2023 - learning](https://github.com/ipc2023-learning/benchmarks)

```
Time limit: 5M
Memory limit: 1GB
rinth -e cp -s beam -h goal_count -d ${domain} -p ${problem}
powerlifted -s alt-bfws1 -e ff -g yannakis -d ${domain} -i ${problem}
donward --alias lama-first ${domain} ${problem}
```

Do note: the limits are below that of the standard ipc 2023, namely 30 min time limit and 8 gb memory limit, this is intentional ... i can't be bothered to wait that long.

### Coverage - How many problems were solved (Higher is better)

| Domain        | rinth         | powerlifted   | downward      |
| ------------- | ------------- | ------------- | ------------- |
| Blocksworld   | 32            | 35            | 53            |
| ------------- | ------------- | ------------- | ------------- |
| Total         | 32            | 35            | 53            |

### Score - IPC score (Higher is better)

| Domain        | rinth         | powerlifted   | downward      |
| ------------- | ------------- | ------------- | ------------- |
| Blocksworld   | 27            | 23            | 41            |
| ------------- | ------------- | ------------- | ------------- |
| Total         | 27            | 23            | 41            |
