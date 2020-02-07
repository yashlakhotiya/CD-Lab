#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include "utils.h"

int main(int a,int b){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	FILE *fa = fopen("arm.c","r");
	int row_no = 1, col_no = -1;
	while(temp ->type != end_of_file){
		temp = getNextToken(fa,&row_no,&col_no);
		printf("%s\n",types[temp->type]);
	}
}