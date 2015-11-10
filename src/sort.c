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
    free(buf);
}

CSortPtr
csort_new(void)
{
    /* TODO */
    CSortPtr new_obj = (CSortPtr)malloc(sizeof(CSort));
    
    return new_obj;
}

void
csort_add_buffer(CSortPtr sort, const char *buf, size_t len)
{
    /* TODO */
    CSortPtr head = csort_new();
    head->size = len;
    head->buffer = (char *)malloc(sizeof(char) * 10);
    head->next = NULL;
    strncpy(head->buffer, buf, len);
    
    if (sort->next != NULL) {
    	head->next = sort->next;
    	sort->next = head;
    	
    } else {
    	sort->next = head;
    	head->next = NULL;
    }
}

int
csort_output(CSortPtr sort, FILE *fp)
{
    /* TODO */
    
    CSortPtr top = sort;
    CSortPtr curr = sort;
    CSortPtr tmp;
    CSortPtr prev;
    
    if ( curr == NULL)
    	return -1;
	
    while(curr != NULL) {
        tmp = curr->next;
        while (tmp != NULL) {
        	if(strncmp(curr->buffer, tmp->buffer, curr->size) > 0) {
        		swap(curr, tmp);
        	}
        	tmp = tmp->next;
        }
        curr = curr->next;
    }
    
    fprintf(fp, "\n");
    while (top != NULL) {
        fprintf(fp, "\n%s", top->buffer);
        top = top->next;
    }
    
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
