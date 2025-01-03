#pragma once

#include "misc.h"
#include "task.h"

char* sql_delete(const string*);
char* sql_table(const string*, uint);
char* sql_insert(const string*, uint);
char* sql_action(const string*, const Scheme*);
