
	if [ -n "$1" ]
	#check if variable is set to something other than null
	then
	#if so, check if it is a valid target.
		if grep -qw "$1" ../../targets.txt
		#use grep to find exact string match in Targets.txt
		then
		#if found. Make the board file and change the target in makefile
			mkdir -p boards/$1
			sed -i "/TARGET\ =/c\TARGET\ =\ $1" Makefile
		else
		#if not found. List possible Targets
			echo
			echo "Target Not Found!"
			echo
			cat ../../targets.txt
			echo
		fi
	else
	#if not set. Tell user.
		echo
		echo Must Choose Target
		echo
		cat ../../targets.txt
		echo
	fi

