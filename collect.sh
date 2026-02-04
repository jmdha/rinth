#!/usr/bin/env bash

coverage=0
echo "name,task,expand,search,time,error" > out.csv
for file in "$@"
do
	name=$(grep -Po '\bEXPR_NAME\s+\K\S+' $file)
	task=$(grep -Po '\bEXPR_TASK\s+\K\S+' $file)
	expand=$(grep -Po '\bEXPR_EXPAND\s+\K\S+' $file)
	search=$(grep -Po '\bEXPR_SEARCH\s+\K\S+' $file)
	time=$(grep -Po '\bEXPR_TIME\s+\K\S+' $file)
	error=$(grep -Po '\bEXPR_ERROR\s+\K\S+' $file)
	echo "$name,$task,$expand,$search,$time,$error" >> out.csv
	if [[ -z $error ]]; then
		coverage=$(($coverage+1))
	fi
done
echo "coverage: $coverage"
