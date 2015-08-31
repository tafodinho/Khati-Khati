#Makefile


all:
	gcc -g -o apriori apriori.c apriori_main.c
	pcc -g -o -mcmodel=medium fptree fptree.c fpmain.c
