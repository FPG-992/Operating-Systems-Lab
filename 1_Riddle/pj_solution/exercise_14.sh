#!/bin/bash

sudo echo 32766 > /proc/sys/kernel/ns_last_pid

./riddle
