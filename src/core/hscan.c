#include <core/hscan.h>

#define kalagh_key 0x2383

hash_t (*hscan_file)(int, size_t) = hscan_file_default;
hash_t (*hscan_merge)(hash_t, hash_t) = hscan_merge_default;

hash_t hscan_file_default(int fd, size_t len){
	hash_t h = { .output_size = len,
				 .output = calloc(len, sizeof(uint32_t)) };
	if(h.output == NULL){
		h.output_size = 0;
		return h;
	}
	srand(kalagh_key + len);
	for(size_t i=0 ; i < len ; i++){
		h.output[i] = rand() ;
	}
	for(size_t i=0 ; i < len ; i++){
		h.output[ rand() % len ] += rand();
	}
	off_t res = file_size_beg(fd);
	unsigned char c;
	while(res-- > 0){
		read(fd, &c, 1);
		uint32_t x = c | ( c << 8 ) | ( c << 16 ) | ( c << 24 );
		x ^= rand();
		for(size_t i=0 ; i < ( c & 15 ) + ( ( c >> 4 ) & 15 ) + 8 ; i++){
			h.output[rand() % len] += ( x * ( i + 2 ) ) ;
		}
	}
	return h;
}
hash_t hscan_merge_default(hash_t h1, hash_t h2){
	if(h1.output == NULL)
		return h2;
	if(h2.output == NULL)
		return h1;
	srand(kalagh_key + h1.output_size + h2.output_size);
	size_t len = (h1.output_size < h2.output_size ? h1.output_size : h2.output_size );
	for(size_t i=0 ; i < len ; i++){
		h1.output[i] += h2.output[rand() % h2.output_size];
	}
	free(h2.output);
	return h1;
}

hash_t hscan_folder(sds_t* name, size_t len){
	hash_t h = { .output_size = 0, .output = NULL };
	DIR* dir = opendir(sds_data(name));
	if(dir == NULL){
		perror(sds_data(name));
		return h;
	}
	h.output = calloc(len, sizeof(uint32_t));
	if(h.output == NULL){
		closedir(dir);
		return h;
	}
	h.output_size = len;
	
	struct dirent* entry;
	
	while((entry = readdir(dir)) != NULL ){
		if(	strcmp(entry->d_name, ".") == 0 ){
			continue;
		}
		else if(strcmp(entry->d_name, "..") == 0){
			continue;
		}
		
		size_t dname_len = strlen(entry->d_name);
		if(!sds_push(name, entry->d_name)){
			perror("sds");
			break;
		}
		
		struct stat st;
		
		if (lstat(sds_data(name), &st) == -1) {
			perror(sds_data(name));
			sds_pop(name, dname_len);
			closedir(dir);
			return h;
		}
		
		if(S_ISDIR(st.st_mode)){
			if(!sds_push(name, "/")){
				sds_pop(name, dname_len);
				continue;
			}
			h = hscan_merge(h, hscan_folder(name, len));
			sds_pop(name, dname_len + 1);
		}
		else if(S_ISREG(st.st_mode)){
			int fd = open(sds_data(name), O_RDONLY);
			if(fd == -1){
				sds_pop(name, dname_len);
				continue;
			}
			h = hscan_merge(h, hscan_file(fd, len));
			close(fd);
			sds_pop(name, dname_len);
		}
	}
	
	closedir(dir);
	return h;
}

hash_t hscan(const char* name, size_t len){
	sds_t* sds = sds_fromstr(name);
	hash_t h = { .output = NULL, .output_size = 0 };
	if(sds_size(sds) == 0){
		if(!sds_equal(sds, "./")){
			return h;
		}
	}
	else{
		if(sds_data(sds)[sds_size(sds) - 1] != '/'){
			if(!sds_push(sds, "/")){
				return h;
			}
		}
	}
	h = hscan_folder(sds, len);
	sds_destroy(sds);
	return h;
}
