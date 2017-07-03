
if [ -z "$1" ];
then
	echo "Must Name Project"
	#check if var 1 is filled
else
	if [ -d "$1" ];
	#check if folder already exists with that name
	then
		echo "Project Already Exists. Choose another project name"
	else
	#if all is well. Make the project folder.
		cp -r TEMPLATE/ $1
		cd $1
		chmod 777 Target.sh
	fi
fi


