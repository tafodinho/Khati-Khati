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
    int len = a->size;
    strncpy(buf, a->buffer, a->size);
    a->size = b->size;
    free(a->buffer);
    a->buffer = (char *)malloc(sizeof(char) * a->size);
    strncpy(a->buffer, b->buffer, b->size);
    b->size = len;
    free(b->buffer);
    b->buffer = (char *)malloc(sizeof(char) * b->size);
    strncpy(b->buffer, buf, b->size);
    free(buf);
}

CSortPtr
csort_new(void)
{
    /* TODO */
    CSortPtr new_obj = (CSortPtr)malloc(sizeof(CSort));
    new_obj->size = 0;
    
    return new_obj;
}

void
csort_add_buffer(CSortPtr sort, const char *buf, size_t len)
{
    /* TODO */

    CSortPtr head = sort;
    int i = 0, new_len = 0;
    char *buffer;
    
    /* checks for '\n' in buf */
    while ( i < len) {
    	if ( buf[i] == '\n')
    		break;
    	i++;
    }
    
     //printf("\nBuffer, i: %d", i);
    /* append string */
    if( i < len) {
    	if ( head->size < 1) {
    		new_len = len;
    		head->buffer = (char *)malloc(sizeof(char)* new_len);
    		strncpy(head->buffer, buf, len);
    	
    	} else {
    		new_len = head->size + len;
    		head->buffer = (char *)realloc(head->buffer, sizeof(char)* new_len);
    		strncat(head->buffer, buf, len);
    	}
    	
    	head->size = new_len;
    	sort = head;
    	head = csort_new();
    	head->next = sort;
    	
    	/* copy remaining buffer to new line */
    	new_len = len - i;
    	head->size = new_len;
    	head->buffer = (char *)malloc(sizeof(char)* new_len);
    	buffer = buf;
    	strncpy(head->buffer, buffer + i, head->size);
    	sort = head;
    	new_len = 0;
    	
    } else {
    
    	if ( head->size < 1) {
    		new_len = len;
    		head->buffer = (char *)malloc(sizeof(char)* new_len);
    		strncpy(head->buffer, buf, len);
    	
    	} else {
    		new_len = head->size + len;
    		head->buffer = (char *)realloc(head->buffer, sizeof(char)* new_len);
    		strncat(head->buffer, buf, len);
    	}
    	
    	head->size = new_len;
    	sort = head;
    	new_len = 0;
    }      
}

int
csort_output(CSortPtr sort, FILE *fp)
{
    /* TODO */
    
    CSortPtr top = sort;
    CSortPtr curr = sort;
    CSortPtr tmp;
    
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
