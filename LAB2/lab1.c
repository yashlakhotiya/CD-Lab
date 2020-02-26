#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

const char types[][30]={
"Arithmetic_Operator",
"Assignment_Operator",
"Relational_Operator",
"Shorthand_Operator",
"Logical_Operator",
"Special_Symbol",
"Keyword",
"Number_Constant",
"String_Literal",
"Identifier",
"whitespace",
"comments",
"preprocessor_directive",
"end_of_file"
};

enum tokenType{
	arithmetic_operator=0,
	assignment_operator,
	relational_operator,
	shorthand_operator,
	logical_operator,
	special_symbol,
	keyword,
	numerical_constant,
	string_literal,
	identifier,
	whitespace,
	comments,
	preprocessor_directive,
	end_of_file
};

typedef struct token{
	int row_no;
	int col_no;
	int type;
	char lexeme[100];
}*TOKEN;

int isKeyword(char lexeme[]){
	int isK = 0;
	for(int i=0; i<32; i++){
		if(strcmp(keywords[i],lexeme) == 0){
			isK = 1;
			break;
		}
	}
	return isK;
}

void printToken(TOKEN temp){
	if(!(temp->type == end_of_file || temp->type == whitespace || temp->type == preprocessor_directive || temp->type == comments)){
		printf("<%d,%d>\t\t\t%s\t\t\t%s\n",temp->row_no,temp->col_no,types[temp->type],temp->lexeme);
	}
}

TOKEN getNextToken(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	//printf("fn row col %d %d\n",*row_no,*col_no);
	char lexeme[100];

	char c;

	c = getc(fa);
	(*col_no)++;

	if(c == EOF){
		//printf("in EOF\n");
		temp->type = end_of_file;
		return temp;
	}

	/* if(c == '\r'){
		printf("\\r row col %d %d\n",*row_no,*col_no);
		*col_no = -1;
		temp->type = whitespace;
		return temp;
	} */

	if(c == '\n'){
		//printf("newline row col %d %d\n",*row_no,*col_no);
		(*row_no)++;
		*col_no = -1;
		temp->type = whitespace;
		return temp;
	}

	if(c == ' ' || c == '\t'){
		//printf("in whitespace\n");
		temp->type = whitespace;
		return temp;
	}

	if(c == '#'){
		//printf("in #\n");
		c = getc(fa);
		(*col_no)++;
		while(c != '\n'){
			c = getc(fa);
			(*col_no)++;
		}
		*col_no = -1;
		(*row_no)++;
		temp->type = preprocessor_directive;
		return temp;
	}

	if(c == '/'){
		//printf("in comment\n");
		//printf("%c",c);
		int i=0;
		temp->lexeme[i++] = c;

		c = getc(fa);
		(*col_no)++;
		if(c == '/'){
			while(c != '\n'){
				c = getc(fa);
				(*col_no)++;
			}
			*col_no = -1;
			(*row_no)++;
			temp->type = comments;
			return temp;
		}
		else if(c == '*'){
			//printf("inside block comment\n");
			c = getc(fa);
			(*col_no)++;
			while(1){
				while(c != '*'){
					//printf("inside block comment\n");
					if(c == '\n'){
						*col_no = -1;
						(*row_no)++;
					}
					c = getc(fa);
					(*col_no)++;
				}
				c = getc(fa);
				(*col_no)++;
				if(c == '/'){
					temp->type = comments;
					return temp;
				}
			}
		}
		else{
			fseek(fa,-1,SEEK_CUR);
			temp->lexeme[i] = '\0';
			temp->type = arithmetic_operator;
			return temp;
		}
	}

	if(c == '"'){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		//printf("row col %d %d\n",*row_no,*col_no);
		c = getc(fa);
		(*col_no)++;
		while(c != '"'){
			lexeme[i++] = c;
			c = getc(fa);
			(*col_no)++;
		}
		lexeme[i++] = c;
		lexeme[i] = '\0';
		temp->type = string_literal;
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

	if(c == '\''){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		char prev = c;
		//printf("row col %d %d\n",*row_no,*col_no);
		c = getc(fa);
		(*col_no)++;
		while(c != '\''){
			lexeme[i++] = c;
			prev = c;
			c = getc(fa);
			(*col_no)++;
		}
		if(prev == '\\'){
			c = getc(fa);
			(*col_no)++;
		}

		lexeme[i++] = c;
		lexeme[i] = '\0';
		temp->type = string_literal;
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%'){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		if(c == '='){
			lexeme[i++] = c;
			lexeme[i] = '\0';
			temp->type = shorthand_operator;
		}
		else{
			(*col_no)--;
			fseek(fa,-1,SEEK_CUR);
			lexeme[i] = '\0';
			temp->type = arithmetic_operator;
		}
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

	if(c == '>' || c == '<' || c == '=' || c == '!'){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		if(c == '='){
			lexeme[i++] = c;
			lexeme[i] = '\0';
			temp->type = relational_operator;
		}
		else{
			if(c == '!'){
				temp->type = logical_operator;
			}
			else if(c == '='){
				temp->type = assignment_operator;
			}
			else{
				temp->type = relational_operator;
			}
			lexeme[i] = '\0';
			fseek(fa,-1,SEEK_CUR);
			(*col_no)--;
		}
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

	if(isalpha(c) || c == '_'){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		while(isalpha(c) || isdigit(c) || c == '_'){
			lexeme[i++] = c;
			c = getc(fa);
			(*col_no)++;
		}
		lexeme[i] = '\0';
		fseek(fa,-1,SEEK_CUR);
		(*col_no)--;
		//printf("col new %d\n",*col_no);
		strcpy(temp->lexeme,lexeme);

		if(isKeyword(lexeme)){
			temp->type = keyword;
		}
		else{
			temp->type = identifier;
		}
		return temp;
	}

	if(isdigit(c)){
		int i=0;
		lexeme[i++] = c;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		while(isdigit(c) || c == '.'){
			lexeme[i++] = c;
			c = getc(fa);
			(*col_no)++;
		}
		lexeme[i] = '\0';
		fseek(fa,-1,SEEK_CUR);
		(*col_no)--;
		strcpy(temp->lexeme,lexeme);
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		temp->type = numerical_constant;
		return temp;
	}

	if(c == '|'){
		temp->type = logical_operator;
		int i=0;
		lexeme[i++] = c;
		lexeme[i] = '\0';
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		if(c == '|'){
			lexeme[i++] = c;
			lexeme[i] = '\0';
			
		}
		else{
			(*col_no)--;
			fseek(fa,-1,SEEK_CUR);
		}
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

	if(c == '&'){
		temp->type = logical_operator;
		int i=0;
		lexeme[i++] = c;
		lexeme[i] = '\0';
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		c = getc(fa);
		(*col_no)++;
		if(c == '&'){
			lexeme[i++] = c;
			lexeme[i] = '\0';
		}
		else{
			(*col_no)--;
			fseek(fa,-1,SEEK_CUR);
		}
		strcpy(temp->lexeme,lexeme);
		return temp;
	}

		//printf("row col %d %d\n",*row_no,*col_no);
		int i=0;
		lexeme[i++] = c;
		lexeme[i] = '\0';
		temp->type = special_symbol;
		temp->row_no = *row_no;
		temp->col_no = *col_no;
		strcpy(temp->lexeme,lexeme);
		return temp;
}

int main(){
	// for(int i=0; i<14; i++){
	// 	printf("%d %s\n",i,types[i]);
	// }

	FILE *fa = fopen("lab1.c","r");

	int row_no = 1;
	int col_no = -1;

	if(fa == NULL){
		printf("error opening file\n");
		exit(0);
	}

	TOKEN temp = NULL;
	printf("<row_no,col_no>\t\t\ttype\t\t\tlexeme\n");
	int k = 0;
	do{
		//printf("k: %d\n",++k);
		temp = getNextToken(fa,&row_no,&col_no);
		//printf("type: %d\n",temp->type);
		printToken(temp);
	}while(temp->type != end_of_file);
}
