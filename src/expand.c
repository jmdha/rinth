#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>
#include <string.h>

#include "bounds.h"
#include "expand.h"
#include "log.h"
#include "scheme.h"
#include "sql.h"
#include "state.h"

sqlite3*      DB                        = NULL;
uint          CLEAR_COUNT               = 0;
sqlite3_stmt* CLEARS[MAX_PREDICATES]    = {NULL};
uint          ACTION_COUNT              = 0;
Scheme        SCHEMES[MAX_SCHEMES];
char*         ACTION_NAMES[MAX_ACTIONS] = {NULL};
sqlite3_stmt* ACTIONS[MAX_ACTIONS]      = {NULL};
uint          ACTIVE                    = 0;
uint          INSERT_COUNT              = 0;
sqlite3_stmt* INSERTS[MAX_PREDICATES]   = {NULL};

static void exec_sql(char* sql) {
    char* error;
    int rc = sqlite3_exec(DB, sql, NULL, NULL, &error);
    if (rc) {
        fprintf(stderr, "%s\n", error);
        fprintf(stderr, "%s\n", sql);
        exit(1);
    }
}

static void prepare_stmt(sqlite3_stmt** stmt, const char* sql) {
    if (sqlite3_prepare_v2(DB, sql, 32000, stmt, NULL)) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(DB));
        fprintf(stderr, "%s\n", sql);
        exit(1);
    }
}

static void create_clear(const struct task* task) {
    assert(DB);
    char buffer[10000];
    for (uint i = 0; i < task->predicate_count; i++) {
        sprintf(buffer, "DELETE FROM \"%.*s\";", 
                task->predicates[i].len, task->predicates[i].ptr);
        prepare_stmt(&CLEARS[i], buffer);
    }
}

static void create_tables(const struct task* task) {
    assert(DB);
    exec_sql("CREATE TABLE _objects_ (arg0 INTEGER);");
    for (uint i = 0; i < task->object_count; i++) {
        char buffer[1000];
        sprintf(buffer, "INSERT INTO _objects_ (arg0) VALUES (%d)", i);
        exec_sql(buffer);
    }
    for (uint i = 0; i < task->predicate_count; i++) {
        char sql[1000];
        sql_table(sql, &task->predicates[i], task->predicate_vars[i]);
        exec_sql(sql);
    }
}

static void create_actions(const struct task* task) {
    assert(DB);
    for (uint i = 0; i < task->scheme_count; i++) {
        char sql[1000];
        sql_action(sql, task->predicates, &task->schemes[i]);
        printf("%s\n", sql);
        prepare_stmt(&ACTIONS[ACTION_COUNT++], sql);
    }
}

static void create_inserts(const struct task* task) {
    assert(DB);
    for (uint i = 0; i < task->predicate_count; i++) {
        char sql[1000];
        sql_insert(sql, &task->predicates[i], task->predicate_vars[i]);
        printf("%s\n", sql);
        prepare_stmt(&INSERTS[INSERT_COUNT++], sql);
    }
}

static void populate(const state* s) {
    u16 predicate, args[16];
    uint len;
    state_iter* si = state_iter_new(s);
    while (state_iter_step(si, &predicate, &len, args)) {
        for (uint i = 0; i < len; i++)
            sqlite3_bind_int(INSERTS[predicate], 1 + i, args[i]);
        const int result = sqlite3_step(INSERTS[predicate]);
        if (result != SQLITE_DONE) {
            fprintf(stderr, "Internal error: populate failed with %d\n", result);
            exit(1);
        }
        sqlite3_reset(INSERTS[predicate]);
    }
    state_iter_free(si);
}

void expand(const state* s) {
    for (uint i = 0; i < CLEAR_COUNT; i++)
        sqlite3_step(CLEARS[i]);
    populate(s);
    ACTIVE = 0;
    for (uint i = 0; i < ACTION_COUNT; i++)
        sqlite3_reset(ACTIONS[i]);
}

bool expand_step(const state* old, uint* action, u16* args, state** new) {
    if (ACTIVE >= ACTION_COUNT) return false;
    const uint index = ACTIVE;
    int code;
    if ((code = sqlite3_step(ACTIONS[ACTIVE])) != SQLITE_ROW) {
        if (code == SQLITE_DONE) {
            ACTIVE++;
            return expand_step(old, action, args, new);
        } else {
            printf("%d\n", code);
            exit(1);
        }
    }
    *action = index;
    const uint len = sqlite3_column_count(ACTIONS[ACTIVE]);
    u16 vals[16];
    for (uint i = 0; i < len; i++)
        vals[i] = sqlite3_column_int(ACTIONS[index], i);
    *new = state_clone(old);
    for (uint i = 0; i < SCHEMES[index].eff_count; i++) {
        const Atom* atom = &SCHEMES[index].eff[i];
        for (uint t = 0; t < atom->arg_count; t++)
            args[t] = vals[atom->args[t]];
        if (atom->val)
            state_insert(*new, atom->predicate, atom->arg_count, args);
        else
            state_remove(*new, atom->predicate, atom->arg_count, args);
    }
    return true;
}

uint expand_count(const state* old) {
    uint action;
    u16 args[MAX_VARIABLES];
    state* s;
    uint count = 0;
    while (expand_step(old, &action, args, &s))
        count++;
    return count;
}

static void expand_fini() {
    if (DB)
        sqlite3_close(DB);
    DB = NULL;
    ACTIVE = 0;
    for (uint i = 0; i < ACTION_COUNT; i++) {
        free(ACTION_NAMES[i]);
        sqlite3_finalize(ACTIONS[i]);
        ACTION_NAMES[i] = NULL;
        ACTIONS[i]      = NULL;
    }
    memset(SCHEMES, 0, ACTION_COUNT * sizeof(Scheme));
    ACTION_COUNT = 0;
    for (uint i = 0; i < INSERT_COUNT; i++) {
        sqlite3_finalize(INSERTS[i]);
        INSERTS[i]      = NULL;
    }
    INSERT_COUNT = 0;

}

void expand_init(const struct task* task) {
    atexit(expand_fini);
    // In case of this being called multiple times
    // Prior instances are cleared first
    expand_fini();
    for (uint i = 0; i < task->scheme_count; i++)
        memcpy(&SCHEMES[i], &task->schemes[i], sizeof(Scheme));
    TRACE("Open SQL connection");
    sqlite3_open(":memory:", &DB);
    TRACE("Create SQL tables");
    create_tables(task);
    TRACE("Prepare clear statement");
    create_clear(task);
    TRACE("Prepare action statements");
    create_actions(task);
    TRACE("Prepare insert statements");
    create_inserts(task);
}
