#ifndef __CORE_STRUCTS_H
#define __CORE_STRUCTS_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <utils/file.h>
#include <ds/sds.h>

#define debug fprintf(stderr, "\nCODE PASS LINE %d IN FUNCTION %s\n", __LINE__, __FUNCTION__)

typedef struct{
	size_t output_size;
	uint32_t* output;
} hash_t;

extern hash_t (*hscan_file)(int, size_t);		 // fd, output size
extern hash_t (*hscan_merge)(hash_t, hash_t);

hash_t  hscan_file_default(int, size_t);
hash_t hscan_merge_default(hash_t, hash_t);
hash_t hscanf_folder(sds_t*, size_t); // path, output size
hash_t hscan(const char*, size_t);			 // path name ( file or folder )

#endif
