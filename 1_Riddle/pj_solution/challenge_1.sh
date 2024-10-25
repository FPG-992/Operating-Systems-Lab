#!/bin/bash

# Get the pid of the riddle process
processId=$(ps -ef | grep 'riddle' | grep -v 'grep' | awk '{ printf $2 }')

# Send signal to the riddle process
kill -SIGCONT $processId
