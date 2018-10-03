#ifndef TIME_TABLE_H
#define TIME_TABLE_H

#include <pebble.h>

#define table_size 8

typedef struct {
  char *name;
  int tasks;
} Row;

//extern int table_size;
extern Row menuRows[table_size];

void setMenuRows();



//Row menuRows[table_size];

#endif
