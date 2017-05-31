#!bin/bash	
files=$(ls | grep \.jpg$);
cnt=1;
echo "Random fileName begin ...\n";
for ele in ${files}
do
	git mv ${ele} $RANDOM"num"${cnt}".jpg";
	echo ${ele} "->" ${cnt}".jpg";	
	((cnt++));
done
echo "Random finished !\n";



echo "Index again ...\n";
files=$(ls | grep \.jpg$);
cnt=1;
for ele in ${files}
do
	git mv ${ele} ${cnt}".jpg";
	echo ${ele} "->" ${cnt}".jpg";	
	((cnt++));
done
echo "Finished !\n";
