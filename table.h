/*
 * Copyright 2012 Johan Eliasson (johane@cs.umu.se). TillÃ¥telse ges fÃ¶r anvÃ¤ndning 
 * pÃ¥ kurserna i Datastrukturer och algoritmer vid UmeÃ¥ Universitet. All annan 
 * anvÃ¤ndning krÃ¤ver fÃ¶rfattarens tillstÃ¥nd.
 */

#ifndef TableTest_TableTest_h
#define TableTest_TableTest_h
#include <stdbool.h>

/* Type for keys in the table */
typedef void *KEY;
/* Type for values in the table */
typedef void *VALUE;

/* Type for function comparing two keys (see create for details)*/
typedef int CompareFunction(KEY,KEY);

/*Types for memory deallocation functions */
typedef void KeyFreeFunc(KEY);
typedef void ValueFreeFunc(VALUE);

typedef void /* void hÃ¤r kan ni om ni vill byta ut mot en egen struct i era tabellimplementationer */ Table;

/* Creates a table.
 *  compare_function - Pointer to a function that is called for comparing
 *                     two keys. The function should return <0 if the left
 *                     parameter is smaller than the right parameter, 0 if
 *                     the parameters are equal, and >0 if the left
 *                     parameter is larger than the right item.
 * Returns: A pointer to the table. NULL if creation of the table failed. */
Table *table_create(CompareFunction *compare_function);

/* Install a memory handling function responsible for removing a key when removed from the table
 *  table - Pointer to the table.
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by keys inserted into the table*/
void table_setKeyMemHandler(Table *table,KeyFreeFunc *freeFunc);

/* Install a memory handling function responsible for removing a value when removed from the table
 *  table - Pointer to the table.
 *  freeFunc- Pointer to a function that is called for  freeing all
 *                     the memory used by values inserted into the table*/
void table_setValueMemHandler(Table *table,ValueFreeFunc *freeFunc);

/* Determines if the table is empty.
 *  table - Pointer to the table.
 * Returns: false if the table is not empty, true if it is. */
bool table_isEmpty(Table *table);

/* Inserts a key and value pair into the table. If memhandlers are set the table takes
 * ownership of the key and value pointers and is responsible for 
 * deallocating them when they are removed.
 *  table - Pointer to the table.
 *  key   - Pointer to the key.
 *  value - Pointer to the value.
 */
void table_insert(Table *table, KEY key, VALUE value);

/* Finds a value given its key.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 * Returns: Pointer to the item's value if the lookup succeded. NULL if the
 *          lookup failed. The pointer is owned by the table type, and the
 *          user should not attempt to deallocate it. It will remain valid
 *          until the item is removed from the table, or the table is
 *          destroyed. */
VALUE table_lookup(Table *table, KEY key);

/* Removes an item from the table given its key.
 *  table - Pointer to the table.
 *  key   - Pointer to the item's key.
 */
void table_remove(Table *table, KEY key);

/* Destroys a table, deallocating all the memory it uses.
 *  table - Pointer to the table. After the function completes this pointer
 *          will be invalid for further use. */
void table_free(Table *table);

#endif
