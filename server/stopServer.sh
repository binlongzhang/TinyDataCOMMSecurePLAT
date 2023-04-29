#!/bin/bash
PID=`ps -ef | grep serverMain | grep -v grep | awk '{print $2}'`
echo $PID
if [ -n $PID ]
then
	kill -USR1 $PID
fi
