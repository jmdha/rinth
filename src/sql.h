#pragma once

#include "misc.h"
#include "scheme.h"

void sql_table(char*,  const string*, uint);
void sql_insert(char*, const string*, uint);
void sql_action(char*, const string*, const Scheme*);
