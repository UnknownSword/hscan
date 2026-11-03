#include <core/display.h>

void show_output(hash_t h){
	size_t cnt = 0;
	for(ssize_t i=0 ; i < h.output_size ; i++){
		printf("%.8x", h.output[i]);
		if(hscan_show_space){
			printf(" ");
		}
		cnt++;
		if(hscan_max_width != 0 && cnt == hscan_max_width){
			cnt = 0;
			printf("\n");
		}
	}
	if(cnt != 0){
		printf("\n");
	}
}
