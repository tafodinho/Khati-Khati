#Makefile


all:
	gcc -g -o apriori apriori.c apriori_main.c
	gcc -g -o -mcmodel=medium fptree fptree.c fpmain.c
