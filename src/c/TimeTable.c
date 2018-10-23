#include <pebble.h>
#include <stdint.h>
#include <@smallstoneapps/linked-list/linked-list.h>
#include "TaskTable.h"
#include "TimeTable.h"
#include "AddTask.h"
#include "AppMessages.h"
#include "TaskList.h"

//int table_size = 8;

void setMenuExpand(int index);
void setMenuCollapse(int index);

Row menuRows[table_size];

void setMenuRows(char *str, int i){
  //for(int i = 0; i<= table_size; i++){
    /*menuRows[i].name = malloc(sizeof(char) * (1+1));
    //menuRows[i].name = i;
    dec_to_str(menuRows[i].name, i, 1);*/

  //}
  menuRows[i].name = malloc(sizeof(char) * (strlen(str)+1));
  strcpy(menuRows[i].name, str);





}


void setMenuCount(int count, int i){

  menuRows[i].tasks = count;
  setMenuExpand(i);
  setMenuCollapse(i);

  DEBUG_MSG("Collapse: %s", menuRows[i].menuCollapse);
  DEBUG_MSG("Expand: %s", menuRows[i].menuExpand);

}

void setMenuExpand(int index) {

  DEBUG_MSG("text declared");


  int tasksLength = get_int_length(menuRows[index].tasks);

  DEBUG_MSG("Int length: %d", tasksLength);

  char *number_buffer = malloc(sizeof(char) *  (tasksLength + 1));
  dec_to_str(number_buffer, menuRows[index].tasks, tasksLength);

  DEBUG_MSG("number_buffer: %s", number_buffer);

  menuRows[index].menuExpand = malloc(sizeof(char) * (strlen(" Tasks") + tasksLength + 1));
  //menuRows[index].menuExpand = "Tasks: ";

  DEBUG_MSG("mid text");

  strcpy(menuRows[index].menuExpand, number_buffer);
  strcat(menuRows[index].menuExpand, " Tasks");


  free(number_buffer);
  number_buffer = NULL;

  DEBUG_MSG("Final text: %s", menuRows[index].menuExpand);


}

void setMenuCollapse(int index) {
  int tasksLength = get_int_length(menuRows[index].tasks);

  DEBUG_MSG("Int length: %d", tasksLength);

  char *number_buffer = malloc(sizeof(char) *  (tasksLength + 1));
  dec_to_str(number_buffer, menuRows[index].tasks, tasksLength);

  DEBUG_MSG("number_buffer: %s", number_buffer);

  menuRows[index].menuCollapse = malloc(sizeof(char) * (strlen(menuRows[index].name) + strlen(": ") + tasksLength + 1));
  //menuRows[index].menuCollapse = "Tasks: ";

  strcpy(menuRows[index].menuCollapse, menuRows[index].name);

  strcat(menuRows[index].menuCollapse, ": ");

  DEBUG_MSG("mid text: %s", menuRows[index].menuCollapse);

  strcat(menuRows[index].menuCollapse, number_buffer);

  free(number_buffer);
  number_buffer = NULL;

  DEBUG_MSG("Final text: %s", menuRows[index].menuCollapse);
}

void setTask(int row, /*int index,*/ char *str, int s_time){

  DEBUG_MSG("Setting Task %d", row);
  DEBUG_MSG("Task Description: %s", str);
  DEBUG_MSG("Task Time %d", s_time);



  Task insertTask;
  insertTask.description = malloc(sizeof(char) * (strlen(str) + 1));
  strcpy(insertTask.description, str);

  DEBUG_MSG("Task Description: %s", insertTask.description);


  insertTask.time = s_time;

  if(linked_list_count(menuRows[row].taskList)==0){
    linked_list_clear(menuRows[row].taskList);
    menuRows[row].taskList = linked_list_create_root();

  }

  //linked_list_insert(menuRows[row].taskList, insertTask, index);
  linked_list_append(menuRows[row].taskList, &insertTask);

  DEBUG_MSG("Count: %d", linked_list_count(menuRows[row].taskList));


  //DEBUG_MSG("Task name: %s", menuRows[row].taskList)
}


void dec_to_str (char* str, uint32_t val, size_t digits)
{
  size_t i=1u;

  for(; i<=digits; i++)
  {
    str[digits-i] = (char)((val % 10u) + '0');
    val/=10u;
  }

  str[i-1u] = '\0'; // assuming you want null terminated strings?
}
