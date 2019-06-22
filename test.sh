# ************************************************************************** #
#                                                                            #
#                                                        :::      ::::::::   #
#   test.sh                                            :+:      :+:    :+:   #
#                                                    +:+ +:+         +:+     #
#   By: sbelondr <sbelondr@student.42.fr>          +#+  +:+       +#+        #
#                                                +#+#+#+#+#+   +#+           #
#   Created: 2019/06/16 10:59:41 by sbelondr          #+#    #+#             #
#   Updated: 2019/06/17 01:55:42 by sbelondr         ###   ########.fr       #
#                                                                            #
# ************************************************************************** #

#!/bin/bash

#	Pour le lancer:
#		./test.sh 21sh

CGR='\033[0;32m'
CRE='\033[0;31m'
NO='\033[0m'
n=0
error=0
name=$1
valgrind_launch=$2
dossier="/tmp/test"


ft_gest_error()
{
	#	Gestion des erreurs		#

	#	Verif si shell posix
	printf "Verif si le shell est posix: "
	test_shell="/tmp/test_shell"
	set > $test_shell
	shell_bash=`cat $test_shell | grep bash`
	shell_posix=`cat $test_shell | grep posix`
	if [ -z "$shell_bash" -a -z "$shell_posix" ]; then
		printf "\n${CRE}Le shell n'est pas posix voulez-vous continuer [Y/n]? $NO"
		read inputuser
		if [ "$inputuser" == "n" ]; then exit; fi
	fi
	printf "${CGR}Ok$NO\n\n"

	#	Verif si le nom de l'executable est present et executable
	if [ -z $name ]; then
		printf "Lancer avec le nom de l'excutable\n"
		printf "./test.sh [executable name]\n"
		exit
	fi
	if [ ! -x $name ]; then
		printf "Le fichier $name n'est pas executable\n"
		exit
	fi
}

ft_valgrind()
{
	#	Utiliser valgrind ?
	if [ -z "$valgrind_launch" -o "$valgrind_launch" != "1" ]; then
		rm -rf $dossier
		mkdir -p $dossier
		printf "Utilisez valgrind [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then
			valgrind --log-file="$dossier/valgrind.log" ./test.sh $name 1 2> $dossier/valgrind_warning.log
			exit
		fi
	fi
}

ft_make()
{
	#	Make
	printf "Make [y/N]? "
	read inputuser
	if [ "$inputuser" == "y" ]; then make re; fi
	clear
}

ft_test_basic()
{
	printf "\n\n========================\n"
	printf "Test basic\n"
	printf "========================\n"

	# Test ls
	test_name="ls"
	./$name "ls" > $dossier/${n}a 2> $dossier/${n}ae
	ls > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test command error
	test_name="Test command erro"
	./$name "qwere" > $dossier/${n}a 2> $dossier/${n}ae
	touch $dossier/${n}b
	printf "21sh: command not found: qwere\n" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test option invalid: ls -z
	test_name="Test ls -z"
	./$name "ls -z" > $dossier/${n}a 2> $dossier/${n}ae
	ls -z > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test /bin/ls
	test_name="Test /bin/ls"
	./$name "/bin/ls -la" > $dossier/${n}a 2> $dossier/${n}ae
	/bin/ls -la > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test ls -la
	test_name="Test ls -la"
	./$name "ls -la" > $dossier/${n}a 2> $dossier/${n}ae
	ls -la > $dossier/${n}b 2> $dossier/${n}ae
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test ls -l                -a                -f
	test_name="Test ls -l                -a                -f"
	./$name "ls -l                -a                -f" > $dossier/${n}a 2> $dossier/${n}ae
	ls -l                -a                -f > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test touch riri;rm riri; cat riri 2>&-
	test_name="Test touch riri;rm riri; cat riri 2>&-"
	./$name "touch riri;rm riri; cat riri 2>&-" > $dossier/${n}a 2> $dossier/${n}ae
	touch $dossier/${n}b
	touch $dossier/${n}ae
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
}

ft_test_builtin()
{
	printf "\n\n========================\n"
	printf "Test builtin\n"
	printf "========================\n"

	# Test cd
	test_name="Test cd"
	./$name "cd" "pwd" > $dossier/${n}a 2> $dossier/${n}ae
	cd && pwd > $dossier/${n}b 2> $dossier/${n}be
	cd - > /dev/null
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test cd libft"
	./$name "cd libft" "pwd" > $dossier/${n}a 2> $dossier/${n}ae
	cd libft && pwd > $dossier/${n}b 2> $dossier/${n}be
	cd - > /dev/null
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test cd ~
	test_name="Test cd ~"
	./$name "cd ~" "pwd" > $dossier/${n}a 2> $dossier/${n}ae
	cd ~ && pwd > $dossier/${n}b 2> $dossier/${n}be
	cd - > /dev/null
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test cd -
	test_name="Test cd -"
	./$name "cd ~" "cd -" "pwd" > $dossier/${n}a 2> $dossier/${n}ae
	cd ~ && cd -  > $dossier/${n}b 2> $dossier/${n}be && pwd >> $dossier/${n}b 2>> $dossier/${n}be
	# cd - > /dev/null
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# A faire permission

	# Test echo
	test_name="Test echo"
	./$name "echo salut" "echo \"\$TERM\"" "echo '\$TERM'" "echo \"salut: '\$TERM'\"" > $dossier/${n}a 2> $dossier/${n}ae
	printf "salut\n" > $dossier/${n}b 2> $dossier/${n}be
	printf "$TERM\n" >> $dossier/${n}b 2>> $dossier/${n}be
	printf '$TERM\n' >> $dossier/${n}b 2>> $dossier/${n}be
	printf "salut: '$TERM'\n" >> $dossier/${n}b 2>> $dossier/${n}be
	# cd - > /dev/null
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test exit"
	./$name "exit" > $dossier/${n}a 2> $dossier/${n}ae
	echo "$?" >> $dossier/${n}a
	echo "exit" > $dossier/${n}be
	echo "0" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test exit 2"
	./$name "exit 2" > $dossier/${n}a 2> $dossier/${n}ae
	echo "$?" >> $dossier/${n}a
	echo "exit" > $dossier/${n}be
	echo "2" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test exit 278"
	./$name "exit 278" > $dossier/${n}a 2> $dossier/${n}ae
	echo "$?" >> $dossier/${n}a
	echo "exit" > $dossier/${n}be
	echo "22" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test exit 34a"
	./$name "exit 34a" > $dossier/${n}a 2> $dossier/${n}ae
	echo "$?" >> $dossier/${n}a
	echo "exit" > $dossier/${n}be
	echo "21sh: exit: 34a: numeric argument required" >> $dossier/${n}be
	echo "255" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test exit 34 22"
	./$name "exit 34 22" > $dossier/${n}a 2> $dossier/${n}ae
	touch $dossier/${n}a
	echo "exit" > $dossier/${n}be
	echo "21sh: exit: too many arguments" >> $dossier/${n}be
	touch $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	printf "${CRE}Test type\n$NO"

}

ft_test_redirection()
{
	printf "\n\n========================\n"
	printf "Test redirection\n"
	printf "========================\n"

	# Test >
	test_name="Test >"
	./$name "echo \"Testing redirections,\" > $dossier/${n}a"
	printf "Testing redirections,\n" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}a -a -z "$first" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi

	# Test >>
	test_name="Test >>"
	./$name "echo \"with multiple lines\" >> $dossier/${n}a"
	printf "with multiple lines\n" >> $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}a -a -z "$first" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test <
	test_name="Test <"
	./$name "wc -c < $dossier/$((n-1))a" > $dossier/${n}a  2> $dossier/${n}ae
	wc -c < $dossier/$((n-1))b > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf :"$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test cd /tmp; echo abc 10>&-; echo def 11>&-; echo ghi 10>fd_above_limit; cat -e fd_above_limit; rm -f fd_above_limit"
	./$name "cd /tmp; echo abc 10>&-; echo def 11>&-; echo ghi 10>fd_above_limit; cat -e fd_above_limit; rm -f fd_above_limit" > $dossier/${n}a 2> $dossier/${n}ae
	echo "abc" > $dossier/${n}b
	echo "def" >> $dossier/${n}b
	echo "ghi" >> $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf :"$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"


	printf "\nTester heredoc [y/N]? "
	read inputuser
	if [ "$inputuser" == "y" ]; then

		printf "***** Heredoc *****\n"
		test_name="Test heredoc"
		printf "${CGR}Ecrire ce texte pour tester l'heredoc:${NO}\n"
		printf "Roses are red\nViolets are blue\n"
		printf "All my base are belong to you\n"
		printf "And so are you\n"
		printf "EOF\n\n"

		./$name "cat -e << EOF > $dossier/${n}a"
		printf "Roses are red$\nViolets are blue$\nAll my base are belong to you$\nAnd so are you$\n" > $dossier/${n}b
		if [ -f $dossier/${n}a ]; then
			first=`diff $dossier/${n}a $dossier/${n}b`
		else
			first=""
		fi
		if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
			printf "$test_name: ${CGR}Ok$NO\n"
		else
			printf "$test_name: ${CRE}No$NO\n"
			printf "$first\n"
			printf "$sec\n"
			error=$((error+1))
			printf "\nQuitter [y/N]? "
			read inputuser
			if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
		fi
		n=$((n+1))
	fi
	# printf "\n"

	# Test &>
	test_name="Test &>"
	./$name "ls . dfghjkl &> $dossier/${n}a"
	ls . dfghjkl &> $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}a -a -z "$first" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test >&
	test_name="Test >&"
	./$name "ls . dfghjkl >& $dossier/${n}a"
	ls . dfghjkl >& $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}a -a -z "$first" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		# printf "\n"
		cat $dossier/${n}b
		#printf "$first\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test 2>&-
	test_name="Test 2>&-"
	./$name "rm nosuchfile 2>&-" > $dossier/${n}a 2> $dossier/${n}ae
	touch $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test 2>&1
	test_name="Test 2>&1"
	./$name "rm nosuchfile 2>&1 | cat -e" > $dossier/${n}a
	printf "rm: nosuchfile: No such file or directory$\n" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}a -a -z "$first" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"


	printf "${CRE}<& a gerer et &< n'existe pas$NO\n"
}

ft_test_multiple_command()
{
	printf "\n\n========================\n"
	printf "Test multiple command\n"
	printf "========================\n"

	# Test multiple command
	test_name="Test multiple command"
	./$name "ls -1; touch newfile; ls -1" > $dossier/${n}a  2> $dossier/${n}ae
	rm newfile
	ls -1 > $dossier/${n}b 2> $dossier/${n}be
	touch newfile
	ls -1 >> $dossier/${n}b 2>> $dossier/${n}be
	rm newfile
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"
}

ft_test_pipe()
{
	printf "\n\n========================\n"
	printf "Test pipe\n"
	printf "========================\n"

	# Test simple pipe
	test_name="Test simple command"
	./$name "ls | wc" > $dossier/${n}a  2> $dossier/${n}ae
	ls | wc > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test pipe error
	test_name="Test pipe error"
	./$name "/dev/null | wc" > $dossier/${n}a  2> $dossier/${n}ae
	echo "       0       0       0" > $dossier/${n}b
	echo "21sh: /dev/null: Permission denied" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test multiple pipe
	test_name="Test multiple pipe"
	./$name "ls -r | sort | cat -e" > $dossier/${n}a  2> $dossier/${n}ae
	ls -r | sort | cat -e > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test more complicate multiple pipe"
	rm -rf test
	./$name "mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi" > $dossier/${n}a  2> $dossier/${n}ae
	rm -rf test
	mkdir test
	cd test
	ls -a  > $dossier/${n}b  2> $dossier/${n}be
	ls | cat | wc -c > fifi
	cat fifi >> $dossier/${n}b  2>> $dossier/${n}be
	cd .. && rm -rf test
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	# Test base64 /dev/urandom | head -c 1000 | wc -c
	test_name="Test base64 /dev/urandom | head -c 1000 | wc -c"
	./$name "base64 /dev/urandom | head -c 1000 | wc -c" > $dossier/${n}a  2> $dossier/${n}ae
	base64 /dev/urandom | head -c 1000 | wc -c > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"
}

# cd /tmp; sort << EOF 
# Roses are red
# Violets are blue
# All my base are belong to you
# I love you
# EOF | cat -e > sorted_poem ; sed -e 's/Roses/Turnips/' < sorted_poem > better_poem; cd -; printf "I prefer turnips anyway" >> /tmp/better_poem; cat /tmp/better_poem


ft_env()
{
	printf "\n\n========================\n"
	printf "Test env\n"
	printf "========================\n"

	# Test env
	test_name="Test env vide ls"
	env -i ./$name "ls" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	printf "21sh: command not found: ls\n" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test env vide /bin/ls"
	env -i ./$name "/bin/ls" > $dossier/${n}a  2> $dossier/${n}ae
	/bin/ls > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test env vide ls | wc"
	env -i ./$name "ls | wc" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	printf "21sh: command not found: ls\n" > $dossier/${n}be
	printf "21sh: command not found: wc\n" >> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test env vide /bin/ls | /usr/bin/wc"
	env -i ./$name "/bin/ls | /usr/bin/wc" > $dossier/${n}a  2> $dossier/${n}ae
	/bin/ls | /usr/bin/wc > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test setenv sans argument"
	./$name "setenv" > $dossier/${n}a 2> $dossier/${n}ae
	if [ -s $dossier/${n}ae -o -s $dossier/${n}a ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		printf "$test_name: ${CGR}Ok$NO\n"
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test unsetenv sans argument"
	./$name "unsetenv" > $dossier/${n}a  2> $dossier/${n}ae
	if [ -s $dossier/${n}ae -o -s $dossier/${n}a ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		printf "$test_name: ${CGR}Ok$NO\n"
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test env"
	./$name "env" > $dossier/${n}a  2> $dossier/${n}ae
	if [ -f $dossier/${n}ae -a -s "$dossier/${n}ae" ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		if [ -f $dossier/${n}a -a -s $dossier/${n}a ]; then
			printf "$test_name: ${CGR}Ok$NO\n"
		else
			printf "$test_name: ${CRE}No$NO\n"
			cat $dossier/${n}ae
			error=$((error+1))
			printf "\nQuitter [y/N]? "
			read inputuser
			if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
		fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test env vide"
	env -i ./$name "env" > $dossier/${n}a  2> $dossier/${n}ae
	if [ -f $dossier/${n}ae -a -s "$dossier/${n}ae" ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		if [ ! -s $dossier/${n}a ]; then
			printf "$test_name: ${CGR}Ok$NO\n"
		else
			printf "$test_name: ${CRE}No$NO\n"
			cat $dossier/${n}a
			cat $dossier/${n}ae
			error=$((error+1))
			printf "\nQuitter [y/N]? "
			read inputuser
			if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
		fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test setenv add key"
	env -i ./$name "setenv toto tata" "env"> $dossier/${n}a  2> $dossier/${n}ae
	printf "toto=tata\n" > $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}ae -a -s "$dossier/${n}ae" ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		if [ -f $dossier/${n}a -a -z "$first" ]; then
			printf "$test_name: ${CGR}Ok$NO\n"
		else
			printf "$test_name: ${CRE}No$NO\n"
			echo "$first"
			error=$((error+1))
			printf "\nQuitter [y/N]? "
			read inputuser
			if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
		fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test setenv add key and remove with unsetenv"
	env -i ./$name "setenv toto tata" "unsetenv toto" "env"> $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	if [ -f $dossier/${n}ae -a -s "$dossier/${n}ae" ]; then
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}ae
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	else
		if [ -f $dossier/${n}a -a -z "$first" ]; then
			printf "$test_name: ${CGR}Ok$NO\n"
		else
			printf "$test_name: ${CRE}No$NO\n"
			cat $dossier/${n}a
			cat $dossier/${n}ae
			cat $dossier/${n}b
			cat $dossier/${n}be
			error=$((error+1))
			printf "\nQuitter [y/N]? "
			read inputuser
			if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
		fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test binaire env"
	./$name "setenv ok gg" "env"> $dossier/${n}a 2> $dossier/${n}ae
	./$name "setenv ok gg" "/usr/bin/env"> $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

}

ft_path()
{
	printf "\n\n========================\n"
	printf "Test PATH\n"
	printf "========================\n"

	test_name="Test env empty and add PATH with setenv and test command"
	env -i ./$name "setenv PATH /usr/bin:/bin" "ls" "wc < author" > $dossier/${n}a  2> $dossier/${n}ae
	ls > $dossier/${n}b  2> $dossier/${n}be
	wc < author >> $dossier/${n}b  2>> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"
}

ft_test_variable()
{
	printf "\n\n========================\n"
	printf "Test variable\n"
	printf "========================\n"

	test_name="Test var \$TERM"
	./$name "echo \$TERM" > $dossier/${n}a  2> $dossier/${n}ae
	echo $TERM > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$TERM\$TERM"
	./$name "echo \$TERM\$TERM" > $dossier/${n}a  2> $dossier/${n}ae
	echo $TERM$TERM > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$TERM/\$TERM"
	./$name "echo \$TERM/\$TERM" > $dossier/${n}a  2> $dossier/${n}ae
	echo $TERM/$TERM > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$TERM@\$TERM"
	./$name "echo \$TERM@\$TERM" > $dossier/${n}a  2> $dossier/${n}ae
	echo $TERM@$TERM > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM}"
	./$name "echo \${TERM}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM#xt}"
	./$name "echo \${TERM#xt}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM#xt} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM#\$TERM}"
	./$name "echo \${TERM#\$TERM}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM#$TERM} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM#nimp}"
	./$name "echo \${TERM#nimp}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM#nimp} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM#}"
	./$name "echo \${TERM#}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM#} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM%%color}"
	./$name "echo \${TERM%color}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM%color} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM%%\$TERM}"
	./$name "echo \${TERM%\$TERM}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM%$TERM} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM%%nimp}"
	./$name "echo \${TERM%nimp}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM%nimp} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM%%}"
	./$name "echo \${TERM%}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM%} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM%%xterm}"
	./$name "echo \${TERM%xterm}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM%xterm} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM#\${TERM%%\$TERM}}"
	./$name "echo \${TERM#\${TERM%\$TERM}}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM#${TERM%$TERM}} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${#TERM}"
	./$name "echo \${#TERM}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${#TERM} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${#notexist}"
	./$name "echo \${#notexist}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${#notexist} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${#}"
	./$name "echo \${#}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${#} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM:-salut}"
	./$name "echo \${TERM:-salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM:-salut} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TEddRM:-salut}"
	./$name "echo \${TEddRM:-salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TEddRM:-salut} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:-salut}"
	./$name "echo \${:-salut}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:-salut}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TEddRM:-}"
	./$name "echo \${TEddRM:-}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TEddRM:-} > $dossier/${n}b  2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:-}"
	./$name "echo \${:-}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:-}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:=}"
	./$name "echo \${:=}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:=}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:=coucou}"
	./$name "echo \${:=coucou}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:=coucou}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM:=salut}"
	./$name "echo \${TERM:=salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM:=salut} > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TEARM:=salut}"
	./$name "echo \${TEARM:=salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TEARM:=salut} > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:?}"
	./$name "echo \${:?}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:?}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:?coucou}"
	./$name "echo \${:?coucou}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:?coucou}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TERM:?salut}"
	./$name "echo \${TERM:?salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TERM:?salut} > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TEARME:?salut}"
	./$name "echo \${TEARME:?salut}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: TEARME: salut" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	unset $TEARME

	test_name="Test var \${TEARME:?}"
	./$name "echo \${TEARME:?}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: TEARME: parameter null or not set" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"


	test_name="Test var \${:+}"
	./$name "echo \${:+}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:+}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${:+coucou}"
	./$name "echo \${:+coucou}" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}b
	echo "21sh: \${:+coucou}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	export SALUT=dd

	test_name="Test var \${SALUT:+salut}"
	./$name "echo \${SALUT:+salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${SALUT:+salut} > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \${TEARME:+salut}"
	./$name "echo \${TEARME:+salut}" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${TEARME:+salut} > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	unset $TEARME
	FFG=
	export FFG
	test_name="Test var \${FFG:+salut}"
	./$name "echo \${FFG:+salut}" "echo \$FFG" > $dossier/${n}a  2> $dossier/${n}ae
	echo ${FFG:+salut} > $dossier/${n}b 2> $dossier/${n}be
	echo $FFG >> $dossier/${n}b 2>> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$#"
	./$name "echo \$#" > $dossier/${n}a  2> $dossier/${n}ae
	echo $# > $dossier/${n}b 2> $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$?"
	./$name "echo \$?" > $dossier/${n}a  2> $dossier/${n}ae
	echo "0" > $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$-"
	./$name "echo \$-" > $dossier/${n}a  2> $dossier/${n}ae
	echo "a" > $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		printf "$first\n"
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$$"
	./$name "echo \$$" > $dossier/${n}a  2> $dossier/${n}ae
	touch $dossier/${n}be
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -s $dossier/${n}a -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$0"
	./$name "echo \$0" > $dossier/${n}a  2> $dossier/${n}ae
	echo "21sh" > $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"

	test_name="Test var \$~"
	./$name "echo \$~" > $dossier/${n}a  2> $dossier/${n}ae
	echo $~ > $dossier/${n}b
	touch $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))
	# printf "\n"
}

ft_test_return_value()
{
	printf "\n\n========================\n"
	printf "Test valeur de retour\n"
	printf "========================\n"

	test_name="Test ls ; echo \$?"
	./$name "ls" "echo \$?"> $dossier/${n}a  2> $dossier/${n}ae
	ls > $dossier/${n}b 2> $dossier/${n}be
	echo $? >> $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test ls -z ; echo \$?"
	./$name "ls -z" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	ls -z > $dossier/${n}b 2> $dossier/${n}be
	echo $? >> $dossier/${n}b
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test command not found ; echo \$?"
	./$name "nimp" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "127" > $dossier/${n}b
	echo "21sh: command not found: nimp" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test echo \${} ; echo \$?"
	./$name "echo \${}" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "1" > $dossier/${n}b
	echo "21sh: \${}: bad substitution" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test /dev/null ; echo \$?"
	./$name "/dev/null" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "126" > $dossier/${n}b
	echo "21sh: /dev/null: Permission denied" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test /dev/null | wc ; echo \$?"
	./$name "/dev/null | wc" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "       0       0       0" > $dossier/${n}b
	echo "0" >> $dossier/${n}b
	echo "21sh: /dev/null: Permission denied" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test ls | /dev/null ; echo \$?"
	./$name "ls | /dev/null" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "126" > $dossier/${n}b
	echo "21sh: /dev/null: Permission denied" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test ls | nimp ; echo \$?"
	./$name "ls | nimp" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "127" > $dossier/${n}b
	echo "21sh: command not found: nimp" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

	test_name="Test nimp | wc ; echo \$?"
	./$name "nimp | wc" "echo \$?"> $dossier/${n}a 2> $dossier/${n}ae
	echo "       0       0       0" > $dossier/${n}b
	echo "0" >> $dossier/${n}b
	echo "21sh: command not found: nimp" > $dossier/${n}be
	if [ -f $dossier/${n}a ]; then
		first=`diff $dossier/${n}a $dossier/${n}b`
	else
		first=""
	fi
	sec=`diff $dossier/${n}ae $dossier/${n}be`
	if [ -f $dossier/${n}a -a -z "$first" -a -z "$sec" ]; then
		printf "$test_name: ${CGR}Ok$NO\n"
	else
		printf "$test_name: ${CRE}No$NO\n"
		cat $dossier/${n}a
		printf "$sec\n"
		error=$((error+1))
		printf "\nQuitter [y/N]? "
		read inputuser
		if [ "$inputuser" == "y" ]; then printf "Log file: $dossier/$n \n"; exit; fi
	fi
	n=$((n+1))

}

ft_test_parser()
{
	printf "\n\n========================\n"
	printf "Test Parser\n"
	printf "========================\n"


	# ls |||||||||||| wc
	# ls || wc
	# ls | | wc
	# >>
	# <<
	printf "$CRE A faire\n$NO"

}

ft_test_signaux()
{
	printf "\n\n========================\n"
	printf "Test Signaux\n"
	printf "========================\n"

	printf "$CRE A faire\n$NO"

}

if [ -z "$valgrind_launch" -o "$valgrind_launch" == "0" ]; then
	ft_gest_error
	ft_valgrind
fi
ft_make

printf "=============================\n"
printf "|           DEBUT           |\n"
printf "=============================\n"

ft_test_basic
ft_test_builtin
ft_test_redirection
ft_test_multiple_command
ft_test_pipe
ft_env
ft_path
ft_test_signaux
ft_test_parser
ft_test_return_value
ft_test_variable

printf "\n"

printf "\nTester la norme [y/N]? "
read inputuser
if [ "$inputuser" == "y" ]; then 
	norminette srcs includes libft/srcs libft/includes
	printf "\n\nAuthor:\n"
	cat -e author
fi
printf "\n"

if [ -f $dossier/valgrind.log ]; then
	echo "Log valgrind: $dossier/valgrind.log"
fi
printf "\n"

printf "${CRE}Error: $error$NO\n"
