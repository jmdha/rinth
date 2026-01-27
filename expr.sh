#!/usr/bin/env bash
task=$1
expand=$2
search=$3

name=${task}_${expand}_${search}
log=logs/${name}

time_start=$(date +%s.%N)
ulimit -v $((1*1024*1024))
timeout 30m ./rinth \
	-d ~/Data/experiment/$task/domain.pddl \
	-p ~/Data/experiment/$task/problem.pddl \
	-e $expand \
	-s $search \
	> $log

ec=$?
elapsed=$( date +%s.%N --date="$time_start seconds ago" )

echo "EXPR_TASK $task" >> $log
echo "EXPR_EXPAND $expand" >> $log
echo "EXPR_SEARCH $search" >> $log
echo "EXPR_TIME $elapsed" >> $log

if [ $ec -ne 0 ]; then
	echo "EXPR_ERROR $ec" >> $log
fi
