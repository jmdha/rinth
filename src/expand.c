#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <assert.h>
#include <string.h>

#include "bounds.h"
#include "expand.h"
#include "db.h"
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

static void create_clear(const struct task* task) {
    for (uint i = 0; i < task->predicate_count; i++)
        db_prep(DB, sql_delete(&task->predicates[i]), &CLEARS[CLEAR_COUNT++]);
}

// HACK: Replace _objects_ with action arguments
static void create_tables(const struct task* task) {
    char* buffer = malloc(1000);
    sprintf(buffer, "CREATE TABLE _objects_ (arg0 INTEGER);");
    db_exec(DB, buffer);
    for (uint i = 0; i < task->object_count; i++) {
        buffer = malloc(1000);
        sprintf(buffer, "INSERT INTO _objects_ (arg0) VALUES (%d)", i);
        db_exec(DB, buffer);
    }
    for (uint i = 0; i < task->predicate_count; i++)
        db_exec(DB, sql_table(&task->predicates[i], task->predicate_vars[i]));
}

static void create_actions(const struct task* task) {
    for (uint i = 0; i < task->scheme_count; i++)
        db_prep(DB, sql_action(task->predicates, &task->schemes[i]), &ACTIONS[ACTION_COUNT++]);
}

static void create_inserts(const struct task* task) {
    for (uint i = 0; i < task->predicate_count; i++)
        db_prep(DB, sql_insert(&task->predicates[i], task->predicate_vars[i]), &INSERTS[INSERT_COUNT++]);
}

static void populate(const state* s) {
    u16 predicate, args[16];
    uint len;
    state_iter* si = state_iter_new(s);
    while (state_iter_step(si, &predicate, &len, args)) {
        for (uint i = 0; i < len; i++)
            sqlite3_bind_int(INSERTS[predicate], 1 + i, args[i]);
        db_exec_stmt(DB, INSERTS[predicate]);
    }
    state_iter_free(si);
}

void expand(const state* s) {
    for (uint i = 0; i < CLEAR_COUNT; i++) {
        sqlite3_step(CLEARS[i]);
        sqlite3_reset(CLEARS[i]);
    }
    populate(s);
    ACTIVE = 0;
    for (uint i = 0; i < ACTION_COUNT; i++)
        sqlite3_reset(ACTIONS[i]);
}

bool expand_step(const state* old, uint* action, u16* args, state** new) {
    if (ACTIVE >= ACTION_COUNT) return false;
    const uint index = ACTIVE;
    if (!db_step(DB, ACTIONS[ACTIVE])) {
        ACTIVE++;
        return expand_step(old, action, args, new);
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
    db_init(&DB);
    TRACE("Create SQL tables");
    create_tables(task);
    TRACE("Prepare clear statement");
    create_clear(task);
    TRACE("Prepare action statements");
    create_actions(task);
    TRACE("Prepare insert statements");
    create_inserts(task);
}
