#ifndef _ALPM_HANDLE_H
#define _ALPM_HANDLE_H

#include <ruby.h>
#include <string.h>
#include <alpm.h>

#include "util.h"

struct _AlpmDB {
    alpm_db_t *db;
};
typedef struct _AlpmDB AlpmDB;

void Init_alpm_db(void);

#endif
