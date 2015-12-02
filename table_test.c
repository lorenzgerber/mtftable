/* Written by Lena Kallin Westin <kallin@cs.umu.se>.
May be used in the course Datastrukturer och Algoritmer (Python) at UmeÃ‚ University.
Usage exept those listed above requires permission by the author.
*/


/* Correctness testing algorithm:
 * 1. Tests if isempty returns true directly after a table is created
 * 2. Tests if isempty returns false directly after a table is created
 *    and one element (key-value-pair) is inserted to it.
 * 3. Tests a table by creating it and inserting one key-value-pair. After
 *    that it is checked that the returned values from a lookup are the ones
 *    expected. First by looking up a non-existent key, then by looking up
 *    an existent key.
 * 4. Tests a table by creating it and inserting three key-value-pairs with
 *    unique keys. After that, a lookup for all three keys are tested and
 *    it is checked that the returned values are the ones expected.
 * 5. Tests a table by creating it and inserting three key-value-pairs with
 *    the same keys. After that, a lookup for the key is tested and it is
 *    checked that the returned value is the last one inserted to the table.
 * 6. Tests a table by creating it and inserting one key-value-pair. After
 *    that the element is removed and it is checked that the table is empty.
 * 7. Tests a table by creating it and inserting three key-value-pairs. After
 *    that the elements is removed one at a time and it is checked that the
 *    table is empty after the third element is removed.
 * 8. Tests a table by creating it and inserting three key-value-pairs where
 *    all three pairs have identical keys. After that the element is removed
 *    and it is checked that the table is empty.
 *
 * There is also a module measuring time for insertions, lookups etc.
 * */
#include "table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Size of the table to generate
#define TABLESIZE 5000
#define SAMPLESIZE TABLESIZE*2

/* Helper function to allocate memory for and initialize a integer pointer
 * the value pointed to will be i
 *   i the value to point at
 * Returns
 *   a int pointer to i
 */
int *intPtrFromInt(int i){
  int *ip=malloc(sizeof(int));
  *ip=i;
  return ip;
}

/* Function to get time in ms
 * Returns
 *    current time in ms
 */
unsigned long get_milliseconds()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return (unsigned long)(tv.tv_sec*1000 + tv.tv_usec/1000);
}

/*Compare function used to compare two int values (pointed to by ip and ip2) are equal
 * ip, ip2 - pointers to two integers
 * Returns
 *    0 if values are equal, nonzero otherwize
 */
int compareInt(void *ip,void *ip2){
  return (*(int*)ip) - (*(int*)ip2);
}

/* Shuffles the numbers stored in seq
 *    seq - an array of randomnumbers to be shuffled
 *    n - the number of elements in seq to shuffle, i.e the indexes [0, n] 
 *        will be shuffled. However, seq might be larger than n...
 */
void randomshuffle(int seq[], int n) {
  for(int i=0;i<n;i++) {
    int switchPos=rand()%n;
    if(i!=switchPos) {
      int temp=seq[i];
      seq[i]=seq[switchPos];
      seq[switchPos]=temp;
    }
  }
}

/* Generate n unique random numbers. The numbers will be stored i seq
 *    seq - an array of randomnumbers created in the function
 *    n - the number of elements in seq
 */
void createRandomSample(int seq[], int n) {
  for(int i=0;i<n;i++) {
    seq[i]=i;
  }
  randomshuffle(seq,n);
}

/* Measures time taken to fill a table with values
 *    table - the table to fill
 *    keys - a list of keys to use
 *    values - a list of values to use
 */
void getInsertSpeed(Table *table, int *keys, int *values){
  unsigned long start;
  unsigned long end;
    
  // Insert all items
  printf("Insert %d items: \n", TABLESIZE);
  start = get_milliseconds();
  for(int i=0;i<TABLESIZE;i++) {
    table_insert(table, intPtrFromInt(keys[i]), intPtrFromInt(values[i]));
  }
  end =  get_milliseconds();
  printf("%lu ms.\n",end-start);
}

/* Measures time taken to do n lookups of existing keys in a table
 *    table - the table to fill
 *    keys - a list of keys to use
 *    values - a list of values to use
 *    n - the number of lookups to perform
 */
void getRandomExistingLookupSpeed(Table *table, int *keys, int n){
  unsigned long start;
  unsigned long end;
    
  printf("%d random lookups: \n",n );
  start = get_milliseconds();
  for(int i=0;i<n;i++) {
    // The existing keys in the table are stored in index [0, TABLESIZE-1] 
    // in the key-array
    int pos = rand()%TABLESIZE;
    table_lookup(table,&keys[pos]);
  }
  end = get_milliseconds();
  printf("%lu ms.\n", end-start);
}

/* Measures time taken to do n lookups of non-existing keys in a table
 *    table - the table to fill
 *    keys - a list of keys to use
 *    values - a list of values to use
 *    n - the number of lookups to perform
 */
void getRandomNonExistingLookupSpeed(Table *table, int *keys, int n){
  unsigned long start;
  unsigned long end;

  printf("%d lookups with non-existent keys: \n", n);
  // We know the exisiting keys have indexes in [0, TABLESIZE-1] so if we
  // try to lookup keys in the area [TABLESIZE, 2*TABLESIZE-1]
  // they will not exist
  start =  get_milliseconds();
  int startindex = TABLESIZE;
  for(int i=0;i<n;i++){
    table_lookup(table,&keys[startindex + (i%TABLESIZE)]);
  }
  end = get_milliseconds();
  printf("%lu ms.\n" ,end-start);
}

/* Measures time taken to do n lookups of existing keys in a table when the
 * keys chosen are from only a part of all available keys
 *    table - the table to fill
 *    keys - a list of keys to use
 *    values - a list of values to use
 *    n - the number of lookups to perform
 */
void getSkewedLookupSpeed(Table *table, int *keys, int n){
  unsigned long start;
  unsigned long end;

  // Lookup skewed to a certain range (in this case the middle third
  // of the keys used)
  int startindex = TABLESIZE/3;
  int stopindex = TABLESIZE*2/3;
  int partition = stopindex - startindex + 1;

  printf("%d skewed lookups: \n" ,n);
  start =  get_milliseconds();
  for(int i=0;i<n;i++) {
    int pos = rand()%partition + startindex;
    table_lookup(table,&keys[pos]);
  }
  end = get_milliseconds();
  printf("%lu ms.\n" ,end-start);
}

/* Measures time taken remove all keys from a table
 *    table - the table to fill
 *    keys - a list of keys to use
 *    values - a list of values to use
 */
void getRemoveSpeed(Table *table, int *keys){
  unsigned long start;
  unsigned long end;

  // Remove all items, not in the same order as they were inserted
  printf("Remove all items: \n");
  start = get_milliseconds();
  randomshuffle(keys, TABLESIZE);  // to get a new order of the keys
  for(int i=0;i<TABLESIZE;i++) {
    table_remove(table,&keys[i]);
  }
  end = get_milliseconds();
  printf("%lu ms.\n" ,end-start);
}

/* Tests if isempty returns true directly after a table is created.
 */
void testIsempty(){
  Table *table = table_create(compareInt);

  if (!table_isEmpty(table)){
    printf("An newly created empty table is said to be nonempty.\n");
    exit(EXIT_FAILURE);
  }
  printf("Isempty returns true directly after a table is created. - OK\n");
  table_free(table);
}

/* Tests if isempty returns false directly after a table is created
 * and one element (key-value-pair) is inserted to it.
 */
void testInsertSingleElement(){
  Table *table = table_create(compareInt);
  char key[5] = "key1";
  char value[7] = "value1";

  table_insert(table, key, value);
  if (table_isEmpty(table)){
    printf("A table with one inserted element is seen as empty.\n");
    exit(EXIT_FAILURE);
  }
  printf("Isempty false if one element is inserted to table. - OK\n");
  table_free(table);
}

/* Tests looking up the key key in a table table. Checks that lookup is
 *  returning the value value
 *     table - the table to do the lookup in
 *     key - the key to lookup
 *     value - the expected value
 */
void testLookupExistingKey(Table *table, char *key, char *value){
  char *returnValue = (char *)table_lookup(table, key);
  if (returnValue==NULL){
    printf("Looked up an existing key, table claims it does not exist.\n");
    exit(EXIT_FAILURE);
  }
  if (returnValue != value){
    printf("Looked up a key but the value returned was wrong.");
    printf(" Expected: %s but got %s", value, returnValue);
    exit(EXIT_FAILURE);
  }
}

/*  Tests a table by creating it and inserting one key-value-pair. After
 *  that it is checked that the returned values from a lookup are the ones
 *  expected. First by looking up a non-existent key, then by looking up
 *  an existent key.
 *
 *  It is assumed that testInsertSingleValue has been run before calling
 *  this test, i.e. that it has been tested that inserting one element
 *  results in an nonempty table.
 */
void testLookupSingleElement(){
  Table *table = table_create(compareInt);
  char *key = "key1";
  char *key2 = "key2";
  char *value = "value1";
  char *value2;

  table_insert(table, key, value);

  // Lookup a non-existent key
  value2 = (char *)table_lookup(table, key2);
  if (value2!=NULL){
    printf("Looked up non-existing key, table claims it does exist.\n");
    exit(EXIT_FAILURE);
  }
  printf("Test of looking up non-existing key in a table with one element - OK\n");

  // Lookup an existent key
  testLookupExistingKey(table, key, value);
  printf("Looking up existing key in a table with one element - OK\n");
  table_free(table);
}

/* Tests a table by creating it and inserting three key-value-pairs with
 *  unique keys. After that, a lookup for all three keys are tested and
 *  it is checked that the returned values are the ones expected.
 *
 *  It is assumed that testInsertSingleValue and testLookupSingleValue
 *  has been run before calling this test, i.e. that it has been tested
 *  that inserting one element results in an nonempty table and that
 *  looking up an key-value-pairs is working with tablesize=1.
 */
void testInsertLookupDifferentKeys(){
  Table *table = table_create(compareInt);
  char *key1 = "key1";
  char *key2 = "key2";
  char *key3 = "key3";
  char *value1 = "value1";
  char *value2 = "value2";
  char *value3 = "value3";

  table_insert(table, key1, value1);
  table_insert(table, key2, value2);
  table_insert(table, key3, value3);

  testLookupExistingKey(table, "key1", "value1");
  testLookupExistingKey(table, "key2", "value2");
  testLookupExistingKey(table, "key3", "value3");
  printf("Looking up three existing keys-value pairs in a table with three elements - OK\n");
  table_free(table);
}

/* Tests a table by creating it and inserting three key-value-pairs with
 *  the same keys. After that, a lookup for the key is tested and it is
 *  checked that the returned value is the last one inserted to the table.
 *
 *  It is assumed that testInsertSingleValue and testLookupSingleValue
 *  has been run before calling this test, i.e. that it has been tested
 *  that inserting one element results in an nonempty table and that
 *  looking up an key-value-pairs is working with tablesize=1.
 *
 */
void testInsertLookupSameKeys(){
  Table *table = table_create(compareInt);
  char *key1 = "key1";
  char *value1 = "value1";
  char *value2 = "value2";
  char *value3 = "value3";

  table_insert(table, key1, value1);
  table_insert(table, key1, value2);
  table_insert(table, key1, value3);

  testLookupExistingKey(table, key1, value3);
  printf("Looking up existing key and value after inserting the same key three times with different values - OK\n");
  table_free(table);
}

/* Tests a table by creating it and inserting one key-value-pair. After
 *  that the element is removed and it is checked that the table is empty.
 *
 *  It is assumed that testInsertSingleValue has been run before calling
 *  this test, i.e. that it has been tested that inserting one element
 *  results in an nonempty table.
 */
void testRemoveSingleElement(){
  Table *table = table_create(compareInt);
  char *key1 = "key1";
  char *value1 = "value1";

  table_insert(table, key1, value1);
  table_remove(table, key1);
  if (!table_isEmpty(table)){
    printf("Removing the last element from a table does not result in an empty table.\n");
    exit(EXIT_FAILURE);
  }
  printf("Inserting one element and removing it, checking that the table gets empty - OK\n");
  table_free(table);
}

/* Tests a table by creating it and inserting three key-value-pairs. After
 *  that the elements is removed one at a time and it is checked that the
 *  table is empty after the third element is removed.
 *
 *  It is assumed that testInsertSingleValue and testRemoveSingleValue
 *  have been run before calling this test, i.e. that it has been tested
 *  that inserting one element results in an nonempty table and removing
 *  one element from a table with one element results in an empty table.
 */
void testRemoveElementsDifferentKeys(){
  Table *table = table_create(compareInt);
  char *key1 = "key1";
  char *key2 = "key2";
  char *key3 = "key3";
  char *value1 = "value1";
  char *value2 = "value2";
  char *value3 = "value3";

  table_insert(table, key1, value1);
  table_insert(table, key2, value2);
  table_insert(table, key3, value3);

  table_remove(table, key1);
  if (table_isEmpty(table)){
    printf("Should be two elements left in the table but it says it is empty\n");
    exit(EXIT_FAILURE);
  }
  table_remove(table, key2);
  if (table_isEmpty(table)){
    printf("Should be one element left in the table but it says it is empty\n");
    exit(EXIT_FAILURE);
  }
  table_remove(table, key3);
  if (!table_isEmpty(table)){
    printf("Removing the last element from a table does not result in an empty table.\n");
    exit(EXIT_FAILURE);
  }
  printf("Inserting three elements and removing them, should end with empty table - OK\n");
  table_free(table);
}

/* Tests a table by creating it and inserting three key-value-pairs where
 *  all three pairs have identical keys. After that the element is removed
 *  and it is checked that the table is empty.
 *
 *  It is assumed that testInsertSingleValue and testRemoveSingleValue
 *  have been run before calling this test, i.e. that it has been tested
 *  that inserting one element results in an nonempty table and removing
 *  one element from a table with one element results in an empty table.
 */
void testRemoveElementsSameKeys(){
  Table *table = table_create(compareInt);
  char *key1 = "key1";
  char *value1 = "value1";
  char *value2 = "value2";
  char *value3 = "value3";

  table_insert(table, key1, value1);
  table_insert(table, key1, value2);
  table_insert(table, key1, value3);

  table_remove(table, key1);

  if (!table_isEmpty(table)){
    printf("Removing the last element from a table does not result in an empty table.\n");
    exit(EXIT_FAILURE);
  }
  printf("Inserting three elements with the same key and removing the key, should end with empty table - OK\n");
  table_free(table);
}

/*  Tests a table by performing a set of tests. Program exits if any
 *  error is found.
 */
void correctnessTest() {
  testIsempty();
  testInsertSingleElement();
  testLookupSingleElement();
  testInsertLookupDifferentKeys();
  testInsertLookupSameKeys();
  testRemoveSingleElement();
  testRemoveElementsDifferentKeys();
  testRemoveElementsSameKeys();
}

/* Tests the speed of a table using random numbers. First a number of
 * elements are inserted. Second a random lookup among the elements are
 * done followed by a skewed lookup (where a subset of the keys are
 * looked up more frequently). Finally all elements are removed.
 */
void speedTest() {
  Table *table = table_create(compareInt);
  table_setKeyMemHandler(table, free);
  table_setValueMemHandler(table, free);

  int randomsize = 2*TABLESIZE; // To make it easier testing non-existing keys later
  int *keys = malloc(randomsize*sizeof(int));
  int *values = malloc(randomsize*sizeof(int));
  createRandomSample(keys, randomsize);
  createRandomSample(values, TABLESIZE);

  getInsertSpeed(table, keys, values);
  getRandomExistingLookupSpeed(table, keys, SAMPLESIZE);
  getRandomNonExistingLookupSpeed(table, keys, SAMPLESIZE);
  getSkewedLookupSpeed(table, keys, SAMPLESIZE);
  getRemoveSpeed(table, keys);

  free(keys);
  free(values);
  table_free(table);
}

int main() {
  correctnessTest();
  printf("All correctness tests succeeded! Press enter to continue!\n");
  getchar();
  speedTest();
  printf("Test completed.\n");
  return 0;
}
