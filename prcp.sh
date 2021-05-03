#!/bin/sh
inst="25 100 250 500 750 1000 13206 132068"
seed="7467634 489761 797464"

for i in $inst; 
do
	for s in $seed; 
	do
	#GRASP	EXEC	SEM			INST		TEMPO	SAIDA		LRC		ARQUIVO DE SOLUÇÃO
	if [ $i -lt 1000 ]
		then
		./pmm 	"${s}" 	'i'"${i}"'.txt'	60		saida.txt	45	'solucoes/sol_'"${i}"'_seed_'"${s}"'.txt'
	elif [ $i -eq 1000 ]
		then
		./pmm 	"${s}" 	'i'"${i}"'.txt'	120		saida.txt	45	'solucoes/sol_'"${i}"'_seed_'"${s}"'.txt'
	else
		./pmm 	"${s}" 	'i'"${i}"'.txt'	1800	saida.txt	45	'solucoes/sol_'"${i}"'_seed_'"${s}"'.txt'
	fi
	done
done
