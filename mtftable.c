//Written by Johan Eliasson <johane@cs.umu.se>.
//May be used in the course Datastrukturer och Algoritmer (C) at Umeå University.
//Usage exept those listed above requires permission by the author.

#include <stdio.h>
#include "table.h"
#include "dlist.h"

typedef struct MyTable {
  dlist *values;
  CompareFunction *cf;
  KeyFreeFunc *keyFree;
  ValueFreeFunc *valueFree;
} MyTable;

typedef struct TableElement{
  KEY key;
  VALUE value;
} TableElement;

/* Creates a table.
 *  compare_function - Pointer to a function that is called for comparing
 *                     two keys. The function should return <0 if the left
 *                     parameter is smaller than the right parameter, 0 if
 *                     the parameters are equal, and >0 if the left
 *                     parameter is larger than the right item.
 * Returns: A pointer to the table. NULL if creation of the table failed. */
Table *table_create(CompareFunction *compare_function)
{
  MyTable *t = calloc(sizeof (MyTable),1);
  if (!t)
    return NULL;
  t->values=dlist_empty();
  dlist_setMemHandler(t->values, free);
    
  t->cf = compare_function;
  return t;
}

/*
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by keys inserted into the table*/
void table_setKeyMemHandler(Table *table,KeyFreeFunc *freeFunc) {
  MyTable *t = (MyTable*)table;
  t->keyFree=freeFunc;
    
}
/*
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by values inserted into the table*/
void table_setValueMemHandler(Table *table,ValueFreeFunc *freeFunc) {
  MyTable *t = (MyTable*)table;
  t->valueFree=freeFunc;
}

/* Determines if the table is empty.
 *  table - Pointer to the table.
 * Returns: false if the table is not empty, true if it is. */
bool table_isEmpty(Table *table) {
  MyTable *t = (MyTable*)table;
  return dlist_isEmpty(t->values);
}


/* Inserts a key and value pair into the table. If memhandlers are set the table takes
 * ownership of the key and value pointers and is responsible for calling
 * the destroy function to free them when they are removed.
 *  table - Pointer to the table.
 *  key   - Pointer to the key.
 *  value - Pointer to the value.
 */
void table_insert(Table *table, KEY key,VALUE value) {
  MyTable *t = (MyTable*)table;
  TableElement *e=malloc(sizeof(TableElement));
  e->key = key;
  e->value = value;
  dlist_insert(t->values,dlist_first(t->values),e);
  
}

VALUE table_lookup(Table *table, KEY key) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);
  while (!dlist_isEnd(t->values,p)) {
    i=dlist_inspect(t->values,p);
    if (t->cf(i->key,key)==0) {

      dlist_position temp1 = p->next->next;
      dlist_position temp2 = t->values->head->next;

      t->values->head->next=p->next;
      p->next->next=temp2;
      p->next=temp1;
      return i->value;
    }

    p=dlist_next(t->values,p);
  }
  return NULL;
}

/* This function removes the element Corresponding to the given key*/
void table_remove(Table *table, KEY key) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);
  
  while (!dlist_isEnd(t->values,p)) {
    i=dlist_inspect(t->values,p);
    if (t->cf(i->key,key)==0) {
      if(t->keyFree!=NULL)
	t->keyFree(i->key);
      if(t->valueFree!=NULL)
	t->valueFree(i->value);
      p=dlist_remove(t->values,p);
    }
    else
      p=dlist_next(t->values,p);
  }
  
}

/*This function removes the table */
void table_free(Table *table) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);
  
  while (!dlist_isEnd(t->values,p)) {
    i=dlist_inspect(t->values,p);
    if(t->keyFree!=NULL)
      t->keyFree(i->key);
    if(t->valueFree!=NULL)
      t->valueFree(i->value);
    p=dlist_remove(t->values,p);
  }
  dlist_free(t->values);
  free(t);
}
