#include <core/display.h>

void show_output(hash_t h, size_t show_max_width, bool show_space){
	size_t cnt = 0;
	for(ssize_t i=0 ; i < h.output_size ; i++){
		printf("%.8x", h.output[i]);
		if(show_space){
			printf(" ");
		}
		cnt++;
		if(show_max_width != 0 && cnt == show_max_width){
			cnt = 0;
			printf("\n");
		}
	}
	if(cnt != 0){
		printf("\n");
	}
}
