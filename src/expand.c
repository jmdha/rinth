#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>

#include "bounds.h"
#include "expand.h"
#include "log.h"

sqlite3*      DB                        = NULL;
uint          ACTION_COUNT              = 0;
char*         ACTION_NAMES[MAX_ACTIONS] = {NULL};
sqlite3_stmt* ACTIONS[MAX_ACTIONS]      = {NULL};


static char* table_sql(const string* name, uint columns) {
    char* buffer_out = malloc(32000);
    char* buffer_parameters = malloc(16000);
    uint offset = 0;
    for (uint i = 0; i < columns; i++) {
        char* buffer_parameter = malloc(100);
        sprintf(buffer_parameter, "arg%d", i);
        offset += sprintf(buffer_parameters + offset, "%s INTEGER", buffer_parameter);
        if (i != columns - 1)
            offset += sprintf(buffer_parameters + offset, ", ");
        free(buffer_parameter);
    }
    sprintf(buffer_out, "CREATE TABLE \"%.*s\" (%s);", name->len, name->ptr, buffer_parameters);
    free(buffer_parameters);
    return buffer_out;
}

static void exec_sql(char* sql) {
    char* error;
    int rc = sqlite3_exec(DB, sql, NULL, NULL, &error);
    if (rc) {
        fprintf(stderr, "%s\n", error);
        fprintf(stderr, "%s\n", sql);
        exit(1);
    }
}

static void create_tables(const struct task* task) {
    assert(DB);
    exec_sql("CREATE TABLE _objects_ (arg0 INTEGER);");
    for (uint i = 0; i < task->predicate_count; i++) {
        char* sql = table_sql(&task->predicates[i], task->predicate_vars[i]);
        exec_sql(sql);
        free(sql);
    }
}

static char* action_sql(const struct task* task, const Scheme* scheme) {
    char* buffer_out    = malloc(32000);
    char* buffer_names  = malloc(512);
    char* buffer_tables = malloc(512);
    char* buffer_joins  = malloc(1024);
    uint offset = 0;
    for (uint i = 0; i < scheme->vars; i++) {
        offset += sprintf(buffer_tables + offset, "_objects_ o%d", i);
        if (i != scheme->vars - 1)
            offset += sprintf(buffer_tables + offset, ", ");
    }
    offset = 0;
    for (uint i = 0; i < scheme->vars; i++) {
        offset += sprintf(buffer_names + offset, "o%d.arg0 as arg%d", i, i);
        if (i != scheme->vars - 1)
            offset += sprintf(buffer_names + offset, ", ");
    }
    offset = 0;
    for (uint i = 0; i < scheme->pre_count; i++) {
        const Atom* atom = &scheme->pre[i];
        offset += sprintf(
            buffer_joins + offset, "\nINNER JOIN \"%.*s\" AS t%d ON", 
            task->predicates[atom->predicate].len,
            task->predicates[atom->predicate].ptr,
            i
        );
        for (uint t = 0; t < atom->arg_count; t++) {
            offset += sprintf(
                buffer_joins + offset,
                " o%d.arg0 = t%d.arg%d", atom->args[t], i, t
            );
            if (t != atom->arg_count - 1)
                offset += sprintf(buffer_joins + offset, " AND");
        }
    }
    sprintf(buffer_out, "SELECT %s FROM %s%s;", buffer_names, buffer_tables, buffer_joins);
    free(buffer_names);
    free(buffer_tables);
    free(buffer_joins);
    return buffer_out;
}

static void create_actions(const struct task* task) {
    assert(DB);
    for (uint i = 0; i < task->scheme_count; i++) {
        char* sql = action_sql(task, &task->schemes[i]);
        exec_sql(sql);
        free(sql);
    }
}


static void populate(const state* s) {
}

void expand(const state* s, void (*callback)(uint, state*)) {
    populate(s);
}

// HACK:
uint COUNTER;
void counter(uint, state*) {
    COUNTER++;
}

uint expand_count(const state* s) {
    COUNTER = 0;
    expand(s, counter);
    return COUNTER;
}

static void expand_fini() {
    if (DB)
        sqlite3_close(DB);
    DB = NULL;
    for (uint i = 0; i < ACTION_COUNT; i++) {
        free(ACTION_NAMES[i]);
        sqlite3_finalize(ACTIONS[i]);
        ACTION_NAMES[i] = NULL;
        ACTIONS[i]      = NULL;
    }
    ACTION_COUNT = 0;
}

void expand_init(const struct task* task) {
    atexit(expand_fini);
    // In case of this being called multiple times
    // Prior instances are cleared first
    expand_fini();
    TRACE("Open SQL connection");
    sqlite3_open(":memory:", &DB);
    TRACE("Create SQL tables");
    create_tables(task);
    TRACE("Prepare action statements");
    create_actions(task);
}
