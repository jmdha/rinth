#!/usr/bin/env bash
task=$1
expand=$2
search=$3

log=logs/${task}_${expand}_${search}

ulimit -v 1048576 # 1 GB 1024*1024
timeout 120s ./rinth \
	-d ~/Data/experiment/$task/domain.pddl \
	-p ~/Data/experiment/$task/problem.pddl \
	-e $expand \
	-s $search \
	> $log 2>$1

ec=$?

if [ $ec -eq 124 ]; then
	echo "TIME LIMIT" >> $log
elif [ $ec -eq 137 ]; then
	echo "MEMORY LIMIT" >> $log
elif [ $ec -ne 0 ]; then
	echo "ERROR ${ec}" >> $log
fi
