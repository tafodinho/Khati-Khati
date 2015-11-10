/*
 * sort.c: Lines sorting module
 *
 * See the description in the header file.
 *
 * You should put all the helper functions, structs, data types, etc. here too.
 */

#include "sort.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct _CSort {
    /* TODO */
    int size;
    char *buffer;
    CSortPtr next;
};

/****************************/
/* Write your solution here */
/****************************/

/* function to swap data of two nodes a and b*/
void swap(CSortPtr a, CSortPtr b)
{
    char *buf;
    buf = (char *)malloc(sizeof(char) * a->size);
    strncpy(buf, a->buffer, a->size);
    strncpy(a->buffer, b->buffer, b->size);
    strncpy(b->buffer, buf, a->size);
}

CSortPtr
csort_new(void)
{
    /* TODO */
    CSort new_obj;
    new_obj.size = 1;
    new_obj.buffer = (char *)malloc(sizeof(char));
    CSortPtr tmp = &new_obj;
    return tmp;
}

void
csort_add_buffer(CSortPtr sort, const char *buf, size_t len)
{
    /* TODO */
    CSortPtr head = csort_new();
    
    head->size = len;
    head->buffer = (char *)malloc(sizeof(char) * len);
    strncpy(head->buffer, buf, len);
    
    if (sort->next != NULL) {
    	head->next = sort->next;
    	sort->next = head;
    } else {
    	sort->next = head;
    	head->next = NULL;
    }
}

/* Bubble sort the linked list */
int b_sort(CSortPtr head)
{
    int swapped;
    CSortPtr p;
    CSortPtr l = NULL;
 
    do
    {
        swapped = 0;
        p = head;
 
        while (p != l) {
        	if (strncmp(p->buffer, p->next->buffer, p->size) > 0) { 
                swap(p, p->next);
                swapped = 1;
            }
            
            p = p->next;
       }
       
       l = p;
    } while (swapped);
    
    return 0;
}

int
csort_output(CSortPtr sort, FILE *fp)
{
    /* TODO */
    int n = b_sort(sort);
    
    CSortPtr tmp = sort;
    fprintf(fp, "\n");
    while (tmp != NULL) {
        fprintf(fp, "%s", tmp->buffer);
        tmp = tmp->next;
    }
    
    if (n < 0)
    	return n;
    
    return 0; 
}

void
csort_free(CSortPtr sort)
{
    /* TODO */
    CSortPtr t, tmp = sort;
    
    while ( tmp->next != NULL) {
    	t = tmp;
 		free(tmp->buffer);
 		tmp = tmp->next;
    	free(t);
    }
}
