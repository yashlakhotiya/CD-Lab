#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char *keywords[] = {
"auto", 
"break", 
"case", 
"char", 
"continue", 
"do", 
"default", 
"const", 
"double", 
"else", 
"enum", 
"extern", 
"for", 
"if", 
"goto", 
"float", 
"int", 
"long", 
"register", 
"return", 
"signed", 
"static", 
"sizeof", 
"short", 
"struct", 
"switch", 
"typedef", 
"union", 
"void", 
"while", 
"volatile", 
"unsigned"
};//size=32

// void findWord(char line[512], char temp[100]){
// 	for(int i=0; i<strlen(line); i++){
		
// 	}
// }

void FindKeyword(char line[512], int line_no,FILE *fb){
	int col_no = 0;
	char *result = line;
	char *init = &line[0];
	for(int i=0; i<32; i++){
		if(result){
			while(result = strstr(result, keywords[i])){
				col_no = result - init;
				fprintf(fb,"(row: %d, col: %d) %s\n",line_no,col_no,keywords[i]);
				printf("(row: %d, col: %d) %s\n",line_no,col_no,keywords[i]);
				result += strlen(keywords[i]);
			}
		}
		result = line;
	}
}

int main(){
	char line[512];
	FILE *fa,*fb;

	char c,d;
	
	fa = fopen("lab3.c","r");
	if(fa == NULL){
		printf("error opening file\n");
		exit(0);
	}
	fb = fopen("lab3_out.c","w");
	if(fb == NULL){
		printf("error opening file\n");
		exit(0);
	}
	int line_no=1;

	while(fgets(line,512,fa) != NULL){
		FindKeyword(line,line_no,fb);
		line_no++;
	}

	fclose(fa);
	return 0;
}