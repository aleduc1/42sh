#!/bin/bash

if [ -z "$1" ]
then
	echo "./verif.sh <nom programme>"
	exit
fi

while [ true ]
do
	clear
	i=`tput cols`

	pid=`ps | grep $1 | grep -v 'grep' | grep -v 'bash' |  cut -d ' ' -f 1`
	if [ -z pid -o "$pid" == "" ]
	then 
		osascript -e 'display notification "Script verif.sh is stopped" with title "Alert verif.sh"'
		exit
	fi
	
	echo ""
	cnt=0
	while [ "$cnt" != "$i" ]
	do
		printf "="
		cnt=$((cnt+1))
	done
	echo ""
	echo ""

	pstree -g 2 "$pid"
	
	echo ""
	cnt=0
	while [ "$cnt" != "$i" ]
	do
		printf "_"
		cnt=$((cnt+1))
	done
	echo ""
	echo ""
	
	lsof -c $1 | grep -v "USER" | grep -v "0u" | grep -v "1u" | grep -v "2u" | cut -d ' ' -f 7- | sort | uniq

	for j in ${pid[*]}
	do
	
		echo ""
		cnt=0
		while [ "$cnt" != "$i" ]
		do
			printf "_"
			cnt=$((cnt+1))
		done
		echo ""
		echo ""
	
		value_leak=`leaks $j`
		test_leak=`echo $value_leak | grep size`
		if [ ! -z "$test_leak" ]
		then
			osascript -e 'display notification "Leaks" with title "Alert verif.sh"'
		fi
		echo "$value_leak"
	done
	
	echo ""
	cnt=0
	while [ "$cnt" != "$i" ]
	do
		printf "="
		cnt=$((cnt+1))
	done
	
	sleep 5
done
