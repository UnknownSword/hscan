#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <core/hscan.h>
#include <core/display.h>

const char* inp_name = "./";
ssize_t output_size = 16;
size_t show_max_width = 4;
bool show_space = 1;


void usage(char* prog_name);
void handle_args(int, char**);

int main(int argc, char* argv[]){
	handle_args(argc, argv);
	// ======== this is main code ==========
	hash_t h = hscan(inp_name, output_size);
	
	show_output(h, show_max_width, show_space);
	if(h.output != NULL){
		free(h.output);
	}
	return 0;
}

void usage(char* prog_name){
	printf("Usage : %s -i <input file> [options]\n", prog_name);
	puts("OPTIONS:");
	puts("\t-l <output size>");
	puts("\t-w <width>");
	puts("\t-s <space [y/n]>");
	exit(0);
}
void handle_args(int argc, char* argv[]){
	if(argc == 1){
		usage(argv[0]);
	}
	for(int i=1 ; i < argc ; i++){
		if(argv[i][0] == '-'){
			if( argv[i][1] == '\0'){
				usage(argv[0]);
			}
			if(strcmp(argv[i], "-i") == 0){
				if(i + 1 == argc) usage(argv[0]);
				inp_name = argv[i+1];
				i++;
			}
			else if(strcmp(argv[i], "-l") == 0){
				if(i + 1== argc) usage(argv[0]);
				output_size = atoi(argv[i+1]);
				i++;
			}
			else if(strcmp(argv[i], "-w") == 0){
				if(i + 1== argc) usage(argv[0]);
				show_max_width = atoi(argv[i+1]);
				i++;
			}
			else if(strcmp(argv[i], "-s") == 0){
				if(i + 1 == argc){
					show_space ^= 1;
					continue;
				}
				if(	strcmp(argv[i+1], "y") == 0 || 
					strcmp(argv[i+1], "Y") == 0 ||
					strcmp(argv[i+1], "1") == 0 ){
					show_space = 1;
				}
				else{
					show_space = 0;
				}
				i++;
			}
			else{
				usage(argv[0]);
			}
		}
		else{
			inp_name = argv[i];
		}
	}
}

