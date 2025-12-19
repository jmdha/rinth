#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "db.h"
#include "log.h"

void db_init(sqlite3** db) { sqlite3_open(":memory:", db); }

void db_exec(sqlite3* db, char* sql) {
        TRACE("db_exec: %s", sql);
        if (sqlite3_exec(db, sql, NULL, NULL, NULL)) {
                fprintf(stderr, "Failed to execute sql command.\n");
                fprintf(stderr, "%s\n", sql);
                exit(1);
        }
        free(sql);
}

void db_prep(sqlite3* db, char* sql, sqlite3_stmt** stmt) {
        TRACE("db_exec: %s", sql);
        if (sqlite3_prepare_v2(db, sql, 1000, stmt, NULL)) {
                fprintf(stderr, "Failed to prepare sql statement.\n");
                fprintf(stderr, "%s\n", sqlite3_errmsg(db));
                fprintf(stderr, "%s\n", sql);
                exit(1);
        }
        free(sql);
}

void db_exec_stmt(sqlite3* db, sqlite3_stmt* stmt) {
        const int result = sqlite3_step(stmt);
        if (result != SQLITE_DONE) {
                fprintf(stderr, "Failed to execute sql statement.\n");
                fprintf(stderr, "%s\n", sqlite3_errmsg(db));
                exit(1);
        }
        sqlite3_reset(stmt);
}

bool db_step(sqlite3* db, sqlite3_stmt* stmt) {
        const int result = sqlite3_step(stmt);
        if (result != SQLITE_ROW && result != SQLITE_DONE) {
                fprintf(stderr, "Failed to iterate sql statement.\n");
                fprintf(stderr, "%s\n", sqlite3_errmsg(db));
                exit(1);
        }
        return result == SQLITE_ROW;
}
