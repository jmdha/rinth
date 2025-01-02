#pragma once

#include <sqlite3.h>
#include <stdbool.h>

void db_init(sqlite3**);
void db_exec(sqlite3*, char*);
void db_prep(sqlite3*, char*, sqlite3_stmt**);
void db_exec_stmt(sqlite3*, sqlite3_stmt*);
bool db_step(sqlite3*, sqlite3_stmt*);
