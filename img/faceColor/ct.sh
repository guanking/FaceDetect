#!bin/bash	
files=$(ls | grep \.jpg$);
cnt=1;
for ele in ${files}
do
	mv ${ele} ${cnt}".jpg";
	((cnt++));
done
