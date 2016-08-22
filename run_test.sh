#!/bin/bash

root_dir=$(dirname "$0")
test_dir=$root_dir/test
tmp_out=$test_dir/.out.txt

function fail_msg {
    echo "failed: ${2}"
    echo "return status: ${1}"
}

function pass_msg {
    echo "passed: ${1}"
}

for i in $(ls $test_dir/invalid_*); do
    $root_dir/src/rattle < $i > $tmp_out 2>&1
    RET_STAT=$?
    if [[ $(cat $tmp_out) == "" ]]
    then
        fail_msg $RET_STAT $i
    else
        pass_msg $i
    fi
done

for i in $(ls $test_dir/valid_*); do
    $root_dir/src/rattle < $i > $tmp_out 2>&1
    RET_STAT=$?
    if [[ $(cat $tmp_out) != "" ]]
    then
        fail_msg $RET_STAT $i
    else
        pass_msg $i
    fi
done
