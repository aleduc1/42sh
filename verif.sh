# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    verif.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/26 04:19:26 by sbelondr          #+#    #+#              #
#    Updated: 2019/07/26 04:19:30 by sbelondr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

if [ -z "$1" ]
then
	echo "./verif.sh <nom programme>"
	exit
fi

rm /tmp/leaks_log.txt

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
			echo "$value_leak" >> /tmp/leaks_log.txt
			osascript -e 'display notification "Leaks. See logs in: /tmp/leaks_log.txt" with title "Alert verif.sh"'
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
