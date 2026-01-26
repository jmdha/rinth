#!/usr/bin/env bash
task=$1
expand=$2
search=$3

log=logs/${task}_${expand}_${search}

ulimit -v $((8*1024*1024))
timeout 30m ./rinth \
	-d ~/Data/experiment/$task/domain.pddl \
	-p ~/Data/experiment/$task/problem.pddl \
	-e $expand \
	-s $search \
	> $log

ec=$?

if [ $ec -eq 124 ]; then
	echo "TIME LIMIT" >> $log
elif [ $ec -eq 12 ]; then
	echo "MEMORY LIMIT" >> $log
elif [ $ec -ne 0 ]; then
	echo "ERROR ${ec}" >> $log
fi
