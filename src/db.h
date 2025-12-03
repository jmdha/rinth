#pragma once

#include <sqlite3.h>
#include <stdbool.h>

// Create an in memory sqlite database
void db_init(
	sqlite3** db
);

// Execute sql on db
void db_exec(
	sqlite3* db,
	char* sql
);

// Prepare given sql as stmt
void db_prep(
	sqlite3* db,
	char* sql,
	sqlite3_stmt** stmt
);

// Execute stmt on db
void db_exec_stmt(
	sqlite3* db,
	sqlite3_stmt* stmt
);

// Perform step of stmt
// Returns false if no more steps
bool db_step(
	sqlite3* db,
	sqlite3_stmt* stmt
);
