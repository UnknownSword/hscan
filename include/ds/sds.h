#ifndef DS_SDS_H_
#define DS_SDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

typedef struct{
	char* value;
	size_t capacity;
	size_t size;
} sds_t;

// always: size < capacity

extern size_t sds_format_max;

sds_t* sds_create();
void sds_destroy(sds_t*);
bool sds_resize(sds_t*, size_t);
bool sds_reserve(sds_t*, size_t);
sds_t* sds_clone(const sds_t*);
void sds_clear(sds_t*);

size_t sds_size(const sds_t*);
size_t sds_capacity(const sds_t*);
char*  sds_value(sds_t*);
char*  sds_data(sds_t*);
char*  sds_at(sds_t*, size_t);

sds_t* sds_fromstr(const char*);
char* sds_tostr(sds_t*);
sds_t* sds_format(const char* , ...);

bool sds_equal(sds_t*, const char*);
bool sds_push (sds_t*, const char*);
void sds_pop  (sds_t*, size_t);
int8_t sds_cmp(const sds_t*, const sds_t*);

#endif
