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

/* find previous node function() */
CSortPtr get_prevnd(
				 CSortPtr head, 
				 CSortPtr a
				){
	if(head == a){
		// node[a] is first node 
		return NULL;
	}
	CSortPtr temp = head; // temp is current node
	CSortPtr pre_a = NULL; 
	
	while(temp && temp!=a){ //seach while not reach to end or 
		pre_a = temp;          // find previous node   
		temp = temp->next;
	}
	if(temp!= a){// node[a] absent on list
		fprintf(stderr, "\n error: node not found!\n");
		exit(EXIT_FAILURE); 
	}
	return pre_a;	
}

/* function to swap data of two nodes a and b*/
void swap(CSortPtr head, CSortPtr a, CSortPtr b)
{
          
  /* first check if a agrgument is null */            
	if( (head) == NULL ||               /* Empty list */ 
	    a == NULL || b == NULL){     /* one node is null */
       // Nothing to swap, just return 
		printf("\n Nothing to swap, just return \n");
		return;
  }

  /* printf("\nswap nodes: %d %d \n", (*a)->data, (*b)->data);
   * find previos nodes */
  CSortPtr pre_a = get_prevnd(head, a);
  CSortPtr pre_b = get_prevnd(head, b);
  
/* printf("\nPrev nodes: %d %d \n", pre_a->data, pre_b->data); */
		
  /* Now swap previous node's next */
  if(pre_a) pre_a->next = b; /* a's previous become b's previous, and */
  if(pre_b) pre_b->next = a; /* b's previous become a's previous */
 
  /* Now swap next fiels of candidate nodes */
  CSortPtr temp = NULL;  
	temp = a->next;
  a->next = b->next;
  b->next = temp;
  
  /* change head: if any node was a head */
  if(head == a) head = b;
  if(head == b) head = a;
  
}

CSortPtr
csort_new(void)
{
    /* TODO */
    CSortPtr new_obj = (CSortPtr)malloc(sizeof(CSort));
    new_obj->size = 0;
    new_obj->buffer = NULL;
    
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
        		swap(sort,curr, tmp);
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
