#!/bin/sh
inst="25 250 500 750 1000 13206 132068"
seed="2	421	78643"

for i in $inst; 
do
	for s in $seed; 
	do
	#GRASP	EXEC	SEM			INST		TEMPO	SAIDA		LRC
	if [ $i -lt 1000 ]
	then
		./pmm 	"${s}" 	'i'"${i}"'.txt'	60		saida.txt	85
	elif [ $i -eq 1000 ]
		then
		./pmm 	"${s}" 	'i'"${i}"'.txt'	120		saida.txt	85
	else
		./pmm 	"${s}" 	'i'"${i}"'.txt'	1800	saida.txt	30
	fi
	done
done
