#!bin/bash	
files=$(ls | grep \.jpg$);
cnt=1;
for ele in ${files}
do
	git mv ${ele} ${cnt}".jpg";
	echo ${ele} "->" ${cnt}".jpg";	
	((cnt++));
done
