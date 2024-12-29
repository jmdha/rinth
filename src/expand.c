#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>

#include "bounds.h"
#include "expand.h"
#include "log.h"
#include "state.h"

sqlite3*      DB                        = NULL;
uint          ACTION_COUNT              = 0;
char*         ACTION_NAMES[MAX_ACTIONS] = {NULL};
sqlite3_stmt* ACTIONS[MAX_ACTIONS]      = {NULL};
uint          ACTIVE                    = 0;
uint          INSERT_COUNT              = 0;
sqlite3_stmt* INSERTS[MAX_PREDICATES]   = {NULL};

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
    for (uint i = 0; i < task->object_count; i++) {
        char buffer[1000];
        sprintf(buffer, "INSERT INTO _objects_ (arg0) VALUES (%d)", i);
        exec_sql(buffer);
    }
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
        sqlite3_prepare_v2(DB, sql, 32000, &ACTIONS[ACTION_COUNT++], NULL);
        free(sql);
    }
}

static char* insert_sql(const string* name, uint vars) {
    char* buffer_out        = malloc(10000);
    char* buffer_parameters = malloc(4000);
    char* buffer_values     = malloc(4000);
    uint offset_parameters  = 0;
    uint offset_values      = 0;
    for (uint i = 0; i < vars; i++) {
        offset_parameters += sprintf(buffer_parameters + offset_parameters, "\"arg%d\"", i);
        offset_values     += sprintf(buffer_values + offset_values, "?%d", i + 1);
        if (i + 1 < vars) {
           offset_parameters += sprintf(buffer_parameters + offset_parameters, ", ");
           offset_values     += sprintf(buffer_values + offset_values, ", ");
        }
    }
    sprintf(buffer_out, "INSERT INTO \"%.*s\" (%s) VALUES (%s);", 
            name->len, name->ptr, buffer_parameters, buffer_values);
    free(buffer_parameters);
    free(buffer_values);
    return buffer_out;
}

static void create_inserts(const struct task* task) {
    assert(DB);
    for (uint i = 0; i < task->predicate_count; i++) {
        char* sql = insert_sql(&task->predicates[i], task->predicate_vars[i]);
        const int rc = sqlite3_prepare_v2(DB, sql, 32000, &INSERTS[INSERT_COUNT++], NULL);
        if (rc) {
            fprintf(stderr, "%s\n", sqlite3_errmsg(DB));
            fprintf(stderr, "%s\n", sql);
            exit(1);
        }
        free(sql);
    }
}

static void populate(const state* s) {
    u16 predicate, args[16];
    uint len;
    state_iter* si = state_iter_new(s);
    while (state_iter_step(si, &predicate, &len, args)) {
        for (uint i = 0; i < len; i++)
            sqlite3_bind_int(INSERTS[predicate], i + 1, args[i]);
        const int result = sqlite3_step(INSERTS[predicate]);
        if (result != SQLITE_DONE) {
            fprintf(stderr, "Internal error: populate failed with %d\n", result);
            exit(1);
        }
    }
    state_iter_free(si);
}

void expand(const state* s) {
    populate(s);
    ACTIVE = 0;
    for (uint i = 0; i < ACTION_COUNT; i++)
        sqlite3_reset(ACTIONS[i]);
}

bool expand_step(uint* action, uint* len, uint* vals) {
    if (ACTIVE >= ACTION_COUNT) return false;
    int code;
    if ((code = sqlite3_step(ACTIONS[ACTIVE])) != SQLITE_ROW) {
        if (code == SQLITE_DONE) {
            ACTIVE++;
            return expand_step(action, len, vals);
        } else {
            printf("%d\n", code);
            exit(1);
        }
    }
    return true;
}

uint expand_count() {
    uint action, len, vals[100];
    uint count = 0;
    while (expand_step(&action, &len, vals))
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
    TRACE("Prepare insert statements");
    create_inserts(task);
}
