#Makefile


all:
	gcc -g -o apriori apriori.c apriori_main.c
	gcc -g -mcmodel=medium -o fptree fptree.c fpmain.c
