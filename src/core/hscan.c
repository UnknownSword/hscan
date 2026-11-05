#include <core/hscan.h>

const char* hscan_input_name = "./";
ssize_t		hscan_output_size = 16;
size_t		hscan_max_width = 4;
bool		hscan_show_space = 1;
bool		hscan_show_color = 1;
size_t		hscan_line_number = 0;

hash_t (*hscan_file)(int) = hscan_file_default;
hash_t (*hscan_merge)(hash_t, hash_t) = hscan_merge_default;

hash_t hscan_file_default(int fd){
	hash_t h = { .output_size = hscan_output_size,
				 .output = calloc(hscan_output_size, sizeof(uint32_t)) };
	if(h.output == NULL){
		h.output_size = 0;
		return h;
	}
	off_t res = file_size_beg(fd);
	srand(0x23ef83 + res);
	for(size_t i=0 ; i < hscan_output_size ; i++){
		h.output[i] = rand() ^ ( rand() << 1 ) ;
	}
	for(size_t i=0 ; i < hscan_output_size ; i++){
		h.output[ rand() % hscan_output_size ] += rand();
	}
	
	char buffer[1024];
	int bytes;
	while( (bytes = read(fd, buffer, sizeof(buffer))) > 0 ){
		uint32_t x = 0x23ef83 + res + bytes;
		uint32_t y = x ^ ( ( x << 16 ) | ( x >> 16 ) );
		for(int i=0 ; i < bytes ; i++){
			x *= 137;
			x += buffer[i];
			h.output[y % h.output_size] += x + y;
			y = ( ( x << 8 ) | ( x >> 24 ) ) + x;
		}
		
	}
	
	if(bytes == -1){
		free(h.output);
		h.output_size = 0;
		return h;
	}
	return h;
}
hash_t hscan_merge_default(hash_t h1, hash_t h2){
	if(h1.output == NULL)
		return h2;
	if(h2.output == NULL)
		return h1;
	srand(0x23ef83 + h1.output_size + h2.output_size);
	size_t len = (h1.output_size < h2.output_size ? h1.output_size : h2.output_size );
	for(size_t i=0 ; i < len ; i++){
		h1.output[i] += h2.output[rand() % h2.output_size];
	}
	free(h2.output);
	return h1;
}

hash_t hscan_folder(sds_t* name, int indent){
	hash_t h = { .output_size = 0, .output = NULL };
	DIR* dir = opendir(sds_data(name));
	if(dir == NULL){
		sds_t* sds = sds_format(COL_RED "%*s[*] %s\n", indent, "", sds_data(name));
		if(sds != NULL){
			perror(sds_data(sds));
			sds_destroy(sds);
		}
		else{
			perror(sds_data(name));
		}
		return h;
	}
	h.output = calloc(hscan_output_size, sizeof(uint32_t));
	if(h.output == NULL){
		if(hscan_show_color)
			fprintf(stderr, COL_RED, "%*s[*] %s: cant allocate memory\n", indent, "" , sds_data(name));
		closedir(dir);
		return h;
	}
	if(hscan_show_color)
		fprintf(stderr, COL_BLUE "%*s[+] %s\n", indent, "", sds_data(name));
	h.output_size = hscan_output_size;
	
	struct dirent* entry;
	
	while((entry = readdir(dir)) != NULL ){
		if(	strcmp(entry->d_name, ".") == 0 ){
			continue;
		}
		else if(strcmp(entry->d_name, "..") == 0){
			continue;
		}
		
		size_t dname_hscan_output_len = strlen(entry->d_name);
		if(!sds_push(name, entry->d_name)){
			perror("sds");
			break;
		}
		
		struct stat st;
		
		if (lstat(sds_data(name), &st) == -1) {
			perror(sds_data(name));
			sds_pop(name, dname_hscan_output_len);
			closedir(dir);
			return h;
		}
		
		if(S_ISDIR(st.st_mode)){
			if(!sds_push(name, "/")){
				sds_pop(name, dname_hscan_output_len);
				continue;
			}
			h = hscan_merge(h, hscan_folder(name, indent + 2));
			sds_pop(name, dname_hscan_output_len + 1);
		}
		else if(S_ISREG(st.st_mode)){
			int fd = open(sds_data(name), O_RDONLY);
			if(fd == -1){
				sds_pop(name, dname_hscan_output_len);
				continue;
			}
			if(hscan_show_color)
				fprintf(stderr, COL_GREEN "%*s[+] %s\n", indent + 2, "", entry->d_name);
			h = hscan_merge(h, hscan_file(fd));
			close(fd);
			sds_pop(name, dname_hscan_output_len);
		}
	}
	
	closedir(dir);
	return h;
}

hash_t hscan(){
	hash_t h = { .output = NULL, .output_size = 0 };
	if(hscan_input_name[0] == '\0'){
		fprintf(stderr, COL_RED COL_BOLD COL_UNDERLINE "-> SCAN FAIL !\nREASON: NO INPUT FILE !\n" COL_RESET);
		return h;
	}
	
	switch( file_type(hscan_input_name) ){
		case FILETYPE_FILE:
			int fd = open(hscan_input_name, O_RDONLY);
			if(fd == -1){
				fprintf(stderr, COL_RED COL_BOLD COL_UNDERLINE "ERROR !\n");
				perror(hscan_input_name);
				fprintf(stderr, COL_RESET);
			}
			else{
				fprintf(stderr, COL_GREEN "[+] %s\n" COL_RESET, hscan_input_name);
				h = hscan_file(fd);
				close(fd);
			}
			break;
		case FILETYPE_DIRECTORY:
			sds_t* sds = sds_fromstr(hscan_input_name);
			if(sds_data(sds)[sds_size(sds) - 1] != '/'){
				if(!sds_push(sds, "/")){
					return h;
				}
			}
			h = hscan_folder(sds, 0);
			sds_destroy(sds);
			break;
		default:
			//perror("");
			break;
	}
	if(h.output == NULL){
		if(hscan_show_color){
			fprintf(stderr, COL_RED COL_BOLD COL_UNDERLINE "-> SCAN FAIL !\n");
			perror("REASON");
			fprintf(stderr, COL_RESET);
		}
	}
	else{
		if(hscan_show_color){
			fprintf(stderr, COL_CYAN COL_BOLD COL_UNDERLINE "-> SCAN COMPELETED SUCCESSFULLY !" COL_RESET "\n");
		}
	}
	return h;
}
