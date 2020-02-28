#include "utils.h"

int status = 1;

void Program(FILE *fa, int *row_no, int *col_no);
void declarations(FILE *fa, int *row_no, int *col_no);
void data_type(FILE *fa, int *row_no, int *col_no);
void identifier_list(FILE *fa, int *row_no, int *col_no);
void identifier_list_dash(FILE * fa, int *row_no, int *col_no);
void identifier_list_double_dash(FILE *fa, int *row_no, int *col_no);
void number(FILE *fa, int *row_no, int *col_no);
void statement_list(FILE *fa, int *row_no, int *col_no);
void statement(FILE *fa, int *row_no, int *col_no);
void switch_case(FILE *fa, int *row_no, int *col_no);
void inside_bracket(FILE *Fa, int *row_no, int *col_no);
void cases(FILE *fa, int *row_no, int *col_no);
void cases_dash(FILE *fa, int *row_no, int *col_no);
void decision_stat(FILE *fa, int *row_no, int *col_no);
void dprime(FILE *fa, int *row_no, int *col_no);
void looping_stat(FILE *fa, int *row_no, int *col_no);
void assign_stat(FILE *fa, int *row_no, int *col_no);
void expn(FILE *fa, int *row_no, int *col_no);
void eprime(FILE *fa, int *row_no, int *col_no);
void simple_expn(FILE *fa, int *row_no, int *col_no);
void seprime(FILE *fa, int *row_no, int *col_no);
void term(FILE *fa, int *row_no, int *col_no);
void tprime(FILE *fa, int *row_no, int *col_no);
void factor(FILE *fa, int *row_no, int *col_no);
void relop(FILE *fa, int *row_no, int *col_no);
void addop(FILE *fa, int *row_no, int *col_no);
void mulop(FILE *fa, int *row_no, int *col_no);

int main(){
	FILE *fa = fopen("test.c","r");
	if(fa == NULL){
		fprintf(stderr, "error opening file\n");
	}
	int row_no = 1, col_no = 0;
	Program(fa,&row_no,&col_no);

	if(status == 0){
		printf("error\n");
	}
	else{
		printf("success\n");
	}

}

void Program(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"main") == 0){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"(") == 0){
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,")") == 0){
				temp = getNextToken(fa,row_no,col_no);
				if(strcmp(temp->lexeme,"{") == 0){
					declarations(fa,row_no,col_no);
					printf("status after declarations %d\n",status);
					statement_list(fa,row_no,col_no);
					printf("status after statement_list %d\n",status);
					temp = getNextToken(fa,row_no,col_no);
					if(strcmp(temp->lexeme,"}") == 0){}
					else{
						printf("new status changing to 0 in Program\n");
						printf("missing '}' in line %d col %d\n",*row_no,*col_no);
						status = 0;
					}
				}
				else{
					printf("status changing to 0 in Program\n");
					printf("missing '{' in line %d col %d\n",*row_no,*col_no);
					status = 0;
				}
			}
			else{
				printf("status changing to 0 in Program\n");
				printf("missing ')' in line %d col %d\n",*row_no,*col_no);
				status = 0;
			}
		}
		else{
			printf("status changing to 0 in Program\n");
			printf("missing '(' in line %d col %d\n",*row_no,*col_no);
			status = 0;
		}
	}
	else{
		printf("status changing to 0 in Program\n");
		printf("missing 'main' in line %d col %d\n",*row_no,(*col_no) - (int)strlen(temp->lexeme));
		status = 0;
	}
}

void declarations(FILE *fa, int *row_no, int *col_no){
	printf("inside declarations\n");
	printf("status in declarations %d\n",status);
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("lexeme in declarations %s\n",temp->lexeme);
	if(strcmp(temp->lexeme,"int") == 0 || strcmp(temp->lexeme,"char") == 0){
		printf("inside if declarations\n");
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		data_type(fa,row_no,col_no);
		identifier_list(fa,row_no,col_no);
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,";") == 0){
			printf("end of declarations\n");
			declarations(fa,row_no,col_no);
		}
		else{
			printf("lexeme %s\n",temp->lexeme);
			printf("status in declarations changing to 0\n");
			status = 0;
		}
	}
	else{
		printf("inside declarations else %s\n",temp->lexeme);
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void data_type(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("inside datatype %s\n",temp->lexeme);
	if(strcmp(temp->lexeme,"int") == 0 || strcmp(temp->lexeme,"char") == 0){}
	else{
		status = 0;
	}
}

void identifier_list(FILE *fa, int *row_no, int *col_no){
	printf("inside identifier list\n");
	printf("status in identifier_list %d\n",status);
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == identifier){
		identifier_list_dash(fa,row_no,col_no);
	}
	else{
		status = 0;
	}
}

void identifier_list_dash(FILE * fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("status in identifier_list_dash %d\n",status);
	if(strcmp(temp->lexeme,",") == 0){
		identifier_list(fa,row_no,col_no);
	}
	else if(strcmp(temp->lexeme,"[") == 0){
		number(fa,row_no,col_no);
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"]") == 0){
			identifier_list_double_dash(fa,row_no,col_no);
		}
		else{
			status = 0;
		}
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void identifier_list_double_dash(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("status in identifier_list_double_dash %d\n",status);
	if(strcmp(temp->lexeme,",") == 0){
		identifier_list(fa,row_no,col_no);
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void number(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == numerical_constant){}
	else{
		status = 0;
	}
}

void statement_list(FILE *fa, int *row_no, int *col_no){
	printf("inside statement list\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("lexeme in statement_list %s\n",temp->lexeme);
	if(strcmp(temp->lexeme,"break") == 0){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,";") == 0){}
		else{
			status = 0;
		}
	}
	else if(temp->type == identifier || strcmp(temp->lexeme,"switch") == 0 || strcmp(temp->lexeme,"if") == 0 || strcmp(temp->lexeme,"for") == 0 || strcmp(temp->lexeme,"while") == 0){
		printf("inside if statement_list\n");
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		statement(fa,row_no,col_no);
		statement_list(fa,row_no,col_no);
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void statement(FILE *fa, int *row_no, int *col_no){
	printf("inside statement\n");
	printf("status in statement %d\n",status );
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("lexeme in statement %s\n",temp->lexeme);
	if(temp->type == identifier){
		//assignment
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		assign_stat(fa,row_no,col_no);
		temp = getNextToken(fa,row_no,col_no);
		printf("lexeme in identifier %s\n",temp->lexeme);
		if(strcmp(temp->lexeme,";") == 0){}
		else{
			status = 0;
		}
	}
	else if(strcmp(temp->lexeme,"switch") == 0){
		//decision statement
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		switch_case(fa,row_no,col_no);
	}
	else if(strcmp(temp->lexeme,"if") == 0){
		//decision statement
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		decision_stat(fa,row_no,col_no);
	}
	else if(strcmp(temp->lexeme,"while") == 0 || strcmp(temp->lexeme,"for") == 0){
		//looping statement
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		looping_stat(fa,row_no,col_no);
	}
	else{
		status = 0;
	}
}

void switch_case(FILE *fa, int *row_no, int *col_no){
	printf("inside switch function\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"switch") == 0){
		printf("inside switch if\n");
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"(") == 0){
			inside_bracket(fa,row_no,col_no);
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,")") == 0){
				temp = getNextToken(fa,row_no,col_no);
				if(strcmp(temp->lexeme,"{") == 0){
					printf("inside starting { of switch\n");
					printf("status %d\n",status);
					cases(fa,row_no,col_no);
					printf("status aftr cases %d\n",status);
					temp = getNextToken(fa,row_no,col_no);
					if(strcmp(temp->lexeme,"}") == 0){
						printf("inside ending } of switch\n");
					}
					else{
						status = 0;
					}
				}
				else{
					status = 0;
				}
			}else{
				status = 0;
			}	
		}
		else{
			status = 0;
		}
	}
	else{
		status = 0;
	}
}

void inside_bracket(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == numerical_constant || temp->type == string_literal || temp->type == identifier){}
	else{
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		simple_expn(fa,row_no,col_no);
	}
	printf("status after inside_bracket end %d\n",status);
}

void cases(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"case") == 0){
		inside_bracket(fa,row_no,col_no);
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,":") == 0){
			statement_list(fa,row_no,col_no);
			cases_dash(fa,row_no,col_no);
		}
		else{
			status = 0;
		}
	}
	else{
		status = 0;
	}
}

void cases_dash(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"case") == 0){
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		cases(fa,row_no,col_no);
	}
	else{
		fseek(fa,-1*strlen(temp->lexeme),SEEK_CUR);
		return;
	}
}

void decision_stat(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"if") == 0){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"(") == 0){
			expn(fa,row_no,col_no);
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,")") == 0){
				temp = getNextToken(fa,row_no,col_no);
				if(strcmp(temp->lexeme,"{") == 0){
					statement_list(fa,row_no,col_no);
					temp = getNextToken(fa,row_no,col_no);
					if(strcmp(temp->lexeme,"}") == 0){
						printf("end of if\n");
						dprime(fa,row_no,col_no);
					}
					else status = 0;
				}
				else status = 0;
			}
			else status = 0;
		}
		else status = 0;
	}
	else status = 0;
}

void dprime(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"else") == 0){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"{") == 0){
			statement_list(fa,row_no,col_no);
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,"}") == 0){}
			else{
				status = 0;
			}
		}
		else{
			status = 0;
		}
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void looping_stat(FILE *fa, int *row_no, int *col_no){
	printf("inside looping_stat\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"while") == 0){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"(") == 0){
			expn(fa,row_no,col_no);
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,")") == 0){
				temp = getNextToken(fa,row_no,col_no);
				if(strcmp(temp->lexeme,"{") == 0){
					statement_list(fa,row_no,col_no);
					temp = getNextToken(fa,row_no,col_no);
					if(strcmp(temp->lexeme,"}") == 0){}
					else status = 0;
				}
				else status = 0;
			}
			else status = 0;
		}
		else status = 0;
	}
	else if(strcmp(temp->lexeme,"for") == 0){
		printf("inside for\n");
		printf("status in for %d\n",status);
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"(") == 0){
			assign_stat(fa,row_no,col_no);
			temp = getNextToken(fa,row_no,col_no);
			if(strcmp(temp->lexeme,";") == 0){
				expn(fa,row_no,col_no);
				temp = getNextToken(fa,row_no,col_no);
				if(strcmp(temp->lexeme,";") == 0){
					assign_stat(fa,row_no,col_no);
					temp = getNextToken(fa,row_no,col_no);
					if(strcmp(temp->lexeme,")") == 0){
						temp = getNextToken(fa,row_no,col_no);
						if(strcmp(temp->lexeme,"{") == 0){
							printf("status before statement_list() call in 'for' %d\n",status);
							statement_list(fa,row_no,col_no);
							temp = getNextToken(fa,row_no,col_no);
							if(strcmp(temp->lexeme,"}") == 0){
								printf("inside }\n");
							}
							else status = 0;
						}
						else status = 0;
					}
					else status = 0;
				}
				else status = 0;
			}
			else status = 0;
		}
		else status = 0;
	}
	printf("status after for %d\n",status);
}

void assign_stat(FILE *fa, int *row_no, int *col_no){
	printf("in assign_stat\n");

	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	printf("lexeme in assign_stat %s\n",temp->lexeme);
	if(temp->type == identifier){
		temp = getNextToken(fa,row_no,col_no);
		if(strcmp(temp->lexeme,"=") == 0){
			expn(fa,row_no,col_no);
		}
		else{
			status = 0;
		}
	}
	else{
		status = 0;
	}
	printf("status after assign_stat %d\n",status);
}

void expn(FILE *fa, int *row_no, int *col_no){
	printf("in expn\n");
	simple_expn(fa,row_no,col_no);
	eprime(fa,row_no,col_no);
}

void eprime(FILE *fa, int *row_no, int *col_no){
	printf("in eprime\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == relational_operator){
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		relop(fa,row_no,col_no);
		simple_expn(fa,row_no,col_no);
		printf("status after eprime %d\n",status);
	}
	else{
		//epsilon
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		printf("status after eprime %d\n",status);
		return;
	}
}

void simple_expn(FILE *fa, int *row_no, int *col_no){
	printf("inside simple_expn\n");
	term(fa,row_no,col_no);
	seprime(fa,row_no,col_no);
}

void seprime(FILE *fa, int *row_no, int *col_no){
	printf("inside seprime\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"+") == 0 || strcmp(temp->lexeme,"-") == 0){
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		addop(fa,row_no,col_no);
		term(fa,row_no,col_no);
		seprime(fa,row_no,col_no);
		printf("status after seprime %d\n",status);
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		printf("status after seprime %d\n",status);
		return;
	}

}

void term(FILE *fa, int *row_no, int *col_no){
	printf("inside tern\n");
	factor(fa,row_no,col_no);
	tprime(fa,row_no,col_no);
}

void tprime(FILE *fa, int *row_no, int *col_no){
	printf("inside tprime\n");
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"%") == 0 || strcmp(temp->lexeme,"*") == 0 || strcmp(temp->lexeme,"/") == 0){
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		mulop(fa,row_no,col_no);
		factor(fa,row_no,col_no);
		tprime(fa,row_no,col_no);
	}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		return;
	}
}

void factor(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == identifier){}
	else if(temp->type == numerical_constant){}
	else{
		fseek(fa,-1 * strlen(temp->lexeme), SEEK_CUR);
		status = 0;
	}
}

void relop(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(temp->type == relational_operator){}
	else{
		status = 0;
	}
}

void addop(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"+") == 0 || strcmp(temp->lexeme,"-") == 0){}
	else{
		status = 0;
	}
}

void mulop(FILE *fa, int *row_no, int *col_no){
	TOKEN temp = (TOKEN)malloc(sizeof(struct token));
	temp = getNextToken(fa,row_no,col_no);
	if(strcmp(temp->lexeme,"%") == 0 || strcmp(temp->lexeme,"*") == 0 || strcmp(temp->lexeme,"/") == 0){
		printf("mulop %s\n",temp->lexeme);
	}
	else{
		status = 0;
	}
}