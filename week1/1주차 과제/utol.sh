echo "Working directory:"  
read directory 

if [ $directory ]  
then
	cd $directory
	if [ $? -ne 0 ]
	then 
		echo "Error Message : cannot access"
		exit 0
	fi
fi
for dir in *
do
	newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` 
	mv $dir $newname
done 
