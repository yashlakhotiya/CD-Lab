#include <stdio.h>
#include <stdlib.h>
int main(){
	FILE *fa,*fb;

	char c,d;
	
	fa = fopen("lab1.c","r");
	if(fa == NULL){
		printf("error opening file\n");
		exit(0);
	}
	fb = fopen("lab2_out.c","w");
	if(fb == NULL){
		printf("error opening file\n");
		exit(0);
	}

	c = getc(fa);
	while(c != EOF){
		if(c == '#'){
			while(c != '\n'){
				c = getc(fa);
			}
		}
		else{
			putc(c,fb);
		}
		c = getc(fa);
	}
	fclose(fa);
	fclose(fb);
	return 0;
}