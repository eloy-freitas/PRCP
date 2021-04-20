#!/bin/sh
inst="25 100 250 500 750 1000 "
seed="2	421	78643"

for i in $inst; 
do
	for s in $seed; 
	do 
	#GRASP	EXEC	SEM		MET	INST				TEMPO	SAIDA		LRC
			./pmm 	"${s}" 	'i'"${i}"'.txt'	5		saida.txt	30
	done
done
