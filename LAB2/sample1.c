//Identification of relational operators from the given input file

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char relational_operators[6][10] = {
	"==",
	">=",
	"<=",
	"!=",
	">",
	"<"
};

void findRelop(char line[], int row_no, FILE *fb){
	int col_no = 0;
	char *result = line;
	char *init = &line[0];

	for(int i=0; i<6; i++){
		result = line;
		if(result){
			while(result = strstr(result,relational_operators[i])){
				col_no = result-init;
				fprintf(fb,"(row: %d, col: %d) %s\n",row_no,col_no,relational_operators[i]);
				result += strlen(relational_operators[i]);
			}
		}
	}

}

int main(){
	FILE *fa, *fb;
	char line_buffer[512];
	fa = fopen("sample1.c","r");
	if(fa == NULL){
		printf("Error opening file\n");
		exit(0);
	}

	fb = fopen("sample1_out.c","w");
	if(fb == NULL){
		printf("Error creating file\n");
		exit(0);
	}

	int row_no = 1;
	while(fgets(line_buffer,512,fa) != NULL){
		findRelop(line_buffer,row_no,fb);
		row_no++;
	}
}