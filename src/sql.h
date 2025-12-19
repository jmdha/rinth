#pragma once

#include "misc.h"
#include "task.h"

char *sql_delete(const string *name);

char *sql_table(const string *name, uint vars);

char *sql_insert(const string *name, uint vars);

char *sql_action(const string *preds, const schema_t *scehema);
