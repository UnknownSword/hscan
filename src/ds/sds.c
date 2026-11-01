#include <ds/sds.h>

size_t sds_format_max = 256;

sds_t* sds_create(){
	sds_t* s = malloc(sizeof(sds_t));
	if(s == NULL) return NULL;
	s->capacity = 1;
	s->size = 0;
	s->value = malloc(s->capacity);
	s->value[0] = '\0';
	if(s->value == NULL){
		free(s);
		return NULL;
	}
	return s;
}
void sds_destroy(sds_t* s){
	if(s == NULL)
		return;
	free(s->value);
	free(s);
}
bool sds_resize(sds_t* s, size_t size){
	if(s->capacity <= size + 1){
		if(sds_reserve(s, size+1) == 0){
			return 0;
		}
	}
	s->size = size;
	s->value[size] = '\0';
	return 1;
}
bool sds_reserve(sds_t* s, size_t cap){
	if(s->capacity >= cap)
		return 1;
	char* new_mem = realloc(s->value, cap);
	if(new_mem == NULL)
		return 0;
	s->value = new_mem;
	s->capacity = cap;
	return 1;
}
sds_t* sds_clone(const sds_t* s){
	sds_t* clone = sds_create();
	if(sds_equal(clone, s->value) == 0){
		sds_destroy(clone);
		return 0;
	}
	return clone;
}
void sds_clear(sds_t* s){
	s->capacity = 1;
	s->size = 0;
	s->value = realloc(s->value, 1);
	s->value[0] = '\0';
}

size_t sds_size(const sds_t* s){ return s->size; }
size_t sds_capacity(const sds_t* s){ return s->capacity; }
char*  sds_value(sds_t* s){ return s->value; }
char*  sds_data(sds_t* s) { return s->value; }
char*  sds_at(sds_t* s, size_t index){ return s->value + index; }

sds_t* sds_fromstr(const char* str){
	sds_t* sds = sds_create();
	if(sds == NULL)
		return NULL;
	if(sds_equal(sds, str) == 0){
		sds_destroy(sds);
		return NULL;
	}
	return sds;
}

char* sds_tostr(sds_t* s){
	return s->value;
}

sds_t* sds_format(const char* fmt, ...){
	sds_t* s = sds_create();
	if(s == NULL)
		return NULL;
	va_list ap;
	va_start(ap, fmt);
	
	if(sds_resize(s, strlen(fmt) + sds_format_max) == 0){
		sds_destroy(s);
		return NULL;
	}
	size_t len = vsprintf(s->value, fmt, ap);
	
	sds_resize(s, len);
	
	va_end(ap);
	return s;
}

bool sds_equal(sds_t* s, const char* str){
	size_t len = strlen(str);
	if(sds_resize(s, len) == 0){
		return 0;
	}
	memcpy(s->value, str, len);
	s->value[s->size] = '\0';
	return 1;
}
bool sds_push (sds_t* s, const char* str){
	size_t len = strlen(str);
	if(s->capacity <= s->size + len){
		size_t newlen = ( s->capacity * 2 > s->size + len + 1 ? s->capacity * 2 : s->size + len + 1 );
		if(sds_reserve(s, newlen) == 0){
			return 0;
		}
	}
	memcpy(s->value + s->size, str, len);
	s->size += len;
	s->value[s->size] = '\0';
	return 1;
}
void sds_pop(sds_t* s, size_t shrink){
	shrink = ( s->size < shrink ? s->size : shrink );
	s->size -= shrink;
	s->value[s->size] = '\0';
	if(s->size * 4 < s->capacity){
		s->value = realloc(s->value, s->capacity / 2);
		s->capacity = s->capacity / 2;
	}
}
int8_t sds_cmp(const sds_t* s, const sds_t* t){
	if(s->size != t->size){
		return ( s->size > t->size ? 1 : -1 );
	}
	size_t len = sds_size(s);
	for(size_t i=0 ; i < len; i++){
		if(s->value[i] == t->value[i])
			continue;
		return ( s->value[i] > t->value[i] ? 1 : -1 );
	}
	return 0;
}
