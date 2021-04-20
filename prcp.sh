#!/bin/sh
inst="25 100 250 500 1000 13206 132068"
seed="0 35254"

for i in $inst; 
do
	for s in $seed; 
	do 
	#GRASP	EXEC	SEM		MET	INST				TEMPO	SAIDA		LRC
			./pmm 	"${s}" 	'i'"${i}"'.txt'	5		saida.txt	15
	done
done
