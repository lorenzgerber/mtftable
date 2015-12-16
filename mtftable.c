/* An implementation of the datatype table using a
 * Move-To-Front List. The datatype is constructed
 * on top of singly-linked list implementation 'dlist.c'.
 * Written by Lorenz Gerber <dv15lgr@cs.umu.se>.
 */
#include <stdio.h>
#include "table.h"
#include "dlist.h"

// defintion of the table struct
typedef struct MyTable {
  dlist *values;
  CompareFunction *cf;
  KeyFreeFunc *keyFree;
  ValueFreeFunc *valueFree;
} MyTable;

// definition of the table element struct
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
Table *table_create(CompareFunction *compare_function){
  // allocation of dynamic memory for the table struct
  MyTable *t = calloc(sizeof (MyTable),1);
  if (!t)
    return NULL;

  // placing an empty dlist in t->values and setting the memory handler
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

  // reserving dynamic memory for the new table element
  TableElement *e=malloc(sizeof(TableElement));
  e->key = key;
  e->value = value;

  // using dlist_insert to place the new table element in t->values
  dlist_insert(t->values,dlist_first(t->values),e);
  
}

/* Finds a value according to it's key, moves the asscociated table
 * element to the first place in the underlying dlist and returns the value.
 * Note that the underlying dlist is implemented with physical and logical
 * list positioning.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 * Returns: Pointer to the item's value if the lookup succeded. NULL if the
 *          lookup failed. The pointer is owned by the table type, and the
 *          user should not attempt to deallocate it. It will remain valid
 *          until the item is removed from the table, or the table is
 *          destroyed. */
VALUE table_lookup(Table *table, KEY key) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);


  // traversing through the list trying to
  // match the lookup key
  while (!dlist_isEnd(t->values,p)) {
    i=dlist_inspect(t->values,p);
    if (t->cf(i->key,key)==0) {

      // temp1 points to the cell after the matching one
      dlist_position temp1 = p->next->next;
      // temp2 points to the current first cell
      dlist_position temp2 = t->values->head->next;

      // the matching cell is connected to the head
      // hence set to the first position
      t->values->head->next=p->next;
      // the matching one is set to point to
      // the current first
      p->next->next=temp2;
      // the cell before the matching one is
      // connected to the one after the matching one
      p->next=temp1;
      return i->value;
    }

    p=dlist_next(t->values,p);
  }
  return NULL;
}

/* Removes an item from the table given its key.
 * If the key doesn't exist, nothing happens.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 */
void table_remove(Table *table, KEY key) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);

  // stepping through the list trying to
  // match the remove key
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

/* Destroys a table, deallocating all the memory it uses.
 *  table - Pointer to the table. After the function completes this pointer
 *          will be invalid for further use. */
void table_free(Table *table) {
  MyTable *t = (MyTable*)table;
  TableElement *i;
  dlist_position p=dlist_first(t->values);

  // traversing through the dlist and
  // removing all entries, if needed using
  // the valueFree function
  while (!dlist_isEnd(t->values,p)) {
    i=dlist_inspect(t->values,p);
    if(t->keyFree!=NULL)
      t->keyFree(i->key);
    if(t->valueFree!=NULL)
      t->valueFree(i->value);
    p=dlist_remove(t->values,p);
  }
  
  // freeing / removing the dlist and the table struct
  dlist_free(t->values);
  free(t);
}
