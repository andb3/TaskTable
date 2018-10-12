#ifndef TIME_TABLE_H
#define TIME_TABLE_H

#include <pebble.h>

#define table_size 8

typedef struct {
  char *name;
  int tasks;
} Row;

void dec_to_str (char* str, uint32_t val, size_t digits);


//extern int table_size;
extern Row menuRows[table_size];

void setMenuRows(char *str, int i);
void setMenuCount(int count, int i);



//Row menuRows[table_size];

#endif
