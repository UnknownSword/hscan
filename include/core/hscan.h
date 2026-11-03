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
#include <utils/col.h>
#include <ds/sds.h>

#define debug fprintf(stderr, "\nCODE PASS LINE %d IN FUNCTION %s\n", __LINE__, __FUNCTION__)

typedef struct{
	size_t output_size;
	uint32_t* output;
}hash_t;

extern const char*	hscan_input_name;
extern ssize_t		hscan_output_size;
extern size_t		hscan_max_width;
extern bool			hscan_show_space;
extern bool			hscan_show_color;

extern hash_t (*hscan_file)(int);		 // fd, output size
extern hash_t (*hscan_merge)(hash_t, hash_t);

hash_t  hscan_file_default(int);
hash_t hscan_merge_default(hash_t, hash_t);
hash_t hscanf_folder(sds_t*, int); // path, output size
hash_t hscan();			 // path name ( file or folder )

#endif
