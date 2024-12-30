#include <stdio.h>

#include "sql.h"

void sql_table(char* buf, const string* name, uint vars) {
    char tmp[100];
    uint offset = 0;
    for (uint i = 0; i < vars; i++) {
        offset += sprintf(tmp + offset, "arg%d INTEGER", i);
        if (i != vars - 1)
            offset += sprintf(tmp + offset, ", ");
    }
    sprintf(buf, "CREATE TABLE \"%.*s\" (%s);", name->len, name->ptr, tmp);
}

void sql_insert(char* buf, const string* name, uint vars) {
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
}

void sql_action(char* buf, const string* preds, const Scheme* scheme) {
    char buf_names[100];
    char buf_tables[100];
    char buf_joins[1000];
    uint offset_names  = 0;
    uint offset_tables = 0;
    uint offset_joins  = 0;
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
        offset_joins += sprintf(buf_joins + offset_joins, "\nINNER JOIN \"%.*s\" AS t%d ON",
                                preds[atom->predicate].len, preds[atom->predicate].ptr, i);
        for (uint t = 0; t < atom->arg_count; t++) {
            offset_joins += sprintf(buf_joins + offset_joins, 
                                    " o%d.arg0 = t%d.arg%d", atom->args[t], i, t);
            if (t != atom->arg_count - 1)
                offset_joins += sprintf(buf_joins + offset_joins, " AND");
        }
    }
    sprintf(buf, "SELECT %s FROM %s%s;", buf_names, buf_tables, buf_joins);
}
