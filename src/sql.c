#include <stdio.h>
#include <stdlib.h>

#include "sql.h"

char* sql_delete(const string* name) {
    char* buf = malloc(1000);
    sprintf(buf, "DELETE FROM \"%.*s\";", name->len, name->ptr);
    return buf;
}

char* sql_table(const string* name, uint vars) {
    // Convert nullary table to unary
    vars = ((vars == 0) ? 1 : vars);
    char* buf = malloc(1000);
    char tmp[100];
    uint offset = 0;
    for (uint i = 0; i < vars; i++) {
        offset += sprintf(tmp + offset, "arg%d INTEGER", i);
        if (i != vars - 1)
            offset += sprintf(tmp + offset, ", ");
    }
    sprintf(buf, "CREATE TABLE \"%.*s\" (%s);", name->len, name->ptr, tmp);
    return buf;
}

char* sql_insert(const string* name, uint vars) {
    // Convert nullary table to unary
    vars = ((vars == 0) ? 1 : vars);
    char* buf = malloc(1000);
    char buf_params[100];
    char buf_values[100];
    uint offset_params = 0;
    uint offset_values = 0;
    for (uint i = 0; i < vars; i++) {
        offset_params += sprintf(buf_params + offset_params, "\"arg%d\"", i);
        offset_values += sprintf(buf_values + offset_values, "?%d", i + 1);
        if (i + 1 < vars) {
           offset_params += sprintf(buf_params + offset_params, ", ");
           offset_values += sprintf(buf_values + offset_values, ", ");
        }
    }
    sprintf(buf, "INSERT INTO \"%.*s\" (%s) VALUES (%s);",
            name->len, name->ptr, buf_params, buf_values);
    return buf;
}

char* sql_action(const string* preds, const Scheme* scheme) {
    char* buf = malloc(4000);
    char buf_names[100];
    char buf_tables[100];
    char buf_joins[1000];
    uint offset_names   = 0;
    uint offset_tables  = 0;
    uint offset_joins   = 0;
    for (uint i = 0; i < scheme->vars; i++) {
        offset_tables += sprintf(buf_tables + offset_tables, "_objects_ o%d", i);
        offset_names  += sprintf(buf_names  + offset_names, "o%d.arg0 AS arg%d", i, i);
        if (i != scheme->vars - 1) {
            offset_names  += sprintf(buf_names  + offset_names, ", ");
            offset_tables += sprintf(buf_tables + offset_tables, ", ");
        }
    }
    for (uint i = 0; i < scheme->pre_count; i++) {
        const Atom* atom = &scheme->pre[i];
        if (atom->arg_count == 0)
            continue;
        offset_joins += sprintf(buf_joins + offset_joins, "\nINNER JOIN \"%.*s\" AS t%d ON",
                                preds[atom->predicate].len, preds[atom->predicate].ptr, i);
        for (uint t = 0; t < atom->arg_count; t++) {
            offset_joins += sprintf(buf_joins + offset_joins, 
                                    " o%d.arg0 = t%d.arg%d", atom->args[t], i, t);
            if (t != atom->arg_count - 1)
                offset_joins += sprintf(buf_joins + offset_joins, " AND");
        }
    }
    for (uint i = 0; i < scheme->pre_count; i++) {
        const Atom* atom = &scheme->pre[i];
        if (atom->arg_count != 0)
            continue;
        offset_joins += sprintf(buf_joins + offset_joins, 
                                  "\nWHERE EXISTS (SELECT * FROM \"%.*s\")",
                                  preds[atom->predicate].len,
                                  preds[atom->predicate].ptr
                                  );
    }
    sprintf(buf, "SELECT %s FROM %s%s;", buf_names, buf_tables, buf_joins);
    return buf;
}
