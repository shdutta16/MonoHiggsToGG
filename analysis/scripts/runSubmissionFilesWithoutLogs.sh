#! /bin/bash

DIR=$1
NAME=$2

FILES=$DIR/$NAME/logOld/*

rm test # file that will hold all log file numbers

for i in $FILES 
do
  number=$(ls $i | cut -d/ -f4 | cut -d_ -f2 | cut -d. -f1)
  echo $number >> test # make a list of all log file numbers
done

# make new list (test_log) with the numbers that need to be resubmitted
python whichFilesMissing.py $DIR $NAME

# setup submissions for those numbers in test_log
python resubmitNoLogFile.py $PWD $DIR $NAME
