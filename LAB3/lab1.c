#include "utils.h"

int insert(TABLE symbol_table[], int *last_table_index,TABLE entry){
	int i = search(symbol_table,last_table_index,entry->name);
	if(i == -1){
		symbol_table[(*last_table_index)++] = entry;
		return ((*last_table_index)-1);
	}
	else{
		return i;
	}
}

void displaySymbolTable(TABLE symbol_table[], int *last_table_index){
	printf("ind\t|\tname\t|\ttype\t|\tsize\t|\tscope\t|\tn\t|\targument\t|\treturn type\n");
	for(int i=0; i<*last_table_index; i++){
		printf("%d\t|\t%s\t|\t%s\t|\t%d\t|\t%c\t|\t%d\t|\t%s\t|\t%s\n",symbol_table[i]->index,symbol_table[i]->name,symbol_table[i]->type,symbol_table[i]->size,symbol_table[i]->scope,symbol_table[i]->number_of_arguments,symbol_table[i]->converted_arguments,symbol_table[i]->return_type);
	}
}

void convertArgumentsOfSymbolTableToIDs(TABLE symbol_table[], int *last_table_index){
	char arr[200];
	for(int i=0; i<*last_table_index; i++){
		int k=0;
		if(strcmp(symbol_table[i]->type,"FUNC") == 0 && symbol_table[i]->number_of_arguments != 0){
			for(int j=0; j<symbol_table[i]->number_of_arguments; j++){
				int id = search(symbol_table,last_table_index,symbol_table[i]->arguments[j]);
				arr[k++] = 'i';
				arr[k++] = 'd';
				int p=0;
				char num[100];
				while(id != 0){
					num[p++] = (id%10)+'0';
					id /= 10;
				}
				for(int i=p-1; i>=0; i--){
					arr[k++] = num[i];
				}
				arr[k++] = ',';
				arr[k++] = ' ';
			}
			k--;//leave one extra space
			k--;//leave one extra comma
			arr[k] = '\0';
			strcpy(symbol_table[i]->converted_arguments,arr);
		}
		else{
			strcpy(arr,"NIL\t");
			strcpy(symbol_table[i]->converted_arguments,arr);
		}
	}
}

TOKEN temp = NULL;
int index_of_identifier = 1;

void createSymbolTable(FILE *fa, TABLE symbol_table[], char scope, int *row_no, int *col_no, int *last_table_index, int *number_of_arguments, char arguments[100][100]){
	char prev_prev_lexeme[100],prev_lexeme[100], curr_lexeme[100];
	temp = getNextToken(fa,row_no,col_no);

	while(temp->type != end_of_file){
		if(temp->type == numerical_constant || temp->type == string_literal){
			temp = getNextToken(fa,row_no,col_no);
			continue;
		}
		if(strcmp(temp->lexeme,"{") == 0){
			//printf("in {\n");
			createSymbolTable(fa,symbol_table,'L',row_no,col_no,last_table_index,0,arguments);
		}
		if(strcmp(temp->lexeme,"}") == 0 || temp->type == end_of_file){
			//printf("end of function %s %s\n",prev_prev_lexeme,prev_lexeme);
			//temp = getNextToken(fa,row_no,col_no);
			return;
		}
		if(scope == '\0'){ //if it is inside function parameter
			printf("in function parameters\n");

			strcpy(prev_lexeme,temp->lexeme);
			//printf("prev: %s\n",prev_lexeme);

			if(strcmp(temp->lexeme,")") != 0){//means function has parameters
				temp = getNextToken(fa,row_no,col_no);
				strcpy(curr_lexeme,temp->lexeme);
			}

			while(strcmp(temp->lexeme,")") != 0){
				
				TABLE temp_entry = (TABLE)malloc(sizeof(struct table_entry));

				strcpy(temp_entry->type,prev_lexeme);
				strcpy(temp_entry->name,curr_lexeme);
				temp_entry->size = findSizeOfDataType(prev_lexeme);
				//printf("type: %s\n",prev_lexeme);
				//printf("name: %s\n",curr_lexeme);


				temp_entry->scope = scope;

				strcpy(arguments[(*number_of_arguments)],curr_lexeme);
				temp_entry->index = index_of_identifier++;

				insert(symbol_table,last_table_index,temp_entry);
				(*number_of_arguments)++;

				temp = getNextToken(fa,row_no,col_no);
				if((strcmp(temp->lexeme,",") == 0)){
					temp = getNextToken(fa,row_no,col_no);//extract datatype
					strcpy(prev_lexeme,temp->lexeme);
					temp = getNextToken(fa,row_no,col_no);//extract variable name
					strcpy(curr_lexeme,temp->lexeme);
				}
				else if((strcmp(temp->lexeme,")") == 0)){
					continue;
				}
			}
			printf("end of function parameters\n");
			return;
		}
		else if(temp->type == keyword){
				//printf("in kw\n");
				strcpy(prev_prev_lexeme,temp->lexeme);//storing the type in prev_prev_lexeme
				
				temp = getNextToken(fa,row_no,col_no);
				strcpy(prev_lexeme,temp->lexeme);//storing the identifier in prev_lexeme

				if(temp->type == special_symbol){//if loop or while loop or do loop i.e. keyword followed by a special symbol
					if(strcmp(temp->lexeme,"(") == 0){
						temp = getNextToken(fa,row_no,col_no);
						while(strcmp(temp->lexeme,")") != 0){
							temp = getNextToken(fa,row_no,col_no);
						}
						
						temp = getNextToken(fa,row_no,col_no);

						if(temp->type == special_symbol){
							if(strcmp(temp->lexeme,"{") == 0){
								createSymbolTable(fa,symbol_table,'L',row_no,col_no,last_table_index,0,arguments);
							}
						}
					}
					else if(strcmp(temp->lexeme,"{")){//do loop
						createSymbolTable(fa,symbol_table,'L',row_no,col_no,last_table_index,0,arguments);
					}
				}
				else if(temp->type == identifier){ //declaration ; function or variable
					//printf("in identifier\n");
					temp = getNextToken(fa,row_no,col_no);
					strcpy(curr_lexeme,temp->lexeme);

					if(temp->type == special_symbol){ // variable declaration

						if(strcmp(temp->lexeme,",") == 0 || strcmp(temp->lexeme,"=") == 0){ //multi variable declaration
							printf("in comma or =\n");
							
							while(strcmp(temp->lexeme,";") != 0){
								printf("in !;\n");

								TABLE temp_entry = (TABLE)malloc(sizeof(struct table_entry));
								strcpy(temp_entry->type,prev_prev_lexeme);
								temp_entry->size = findSizeOfDataType(prev_prev_lexeme);
								strcpy(temp_entry->name,prev_lexeme);
								temp_entry->index = index_of_identifier++;
								temp_entry->scope = scope;

								printf("before insert\n");

								insert(symbol_table,last_table_index,temp_entry);

								printf("after insert\n");

								temp = getNextToken(fa,row_no,col_no);
								strcpy(prev_lexeme,temp->lexeme);

								while(strcmp(temp->lexeme,",") == 0 || strcmp(temp->lexeme,"=") == 0 || temp->type == string_literal || temp->type == numerical_constant){
									printf("in comma 1\n");
									temp = getNextToken(fa,row_no,col_no);
									strcpy(prev_lexeme,temp->lexeme);
									printf("in comma 2\n");
									continue;
								}
							}
						}
						else if(strcmp(temp->lexeme,"[") == 0){ //array declaration but with assumption that we cannot declare mixture of array and primitives
							while(strcmp(temp->lexeme,";") != 0){
								TABLE temp_entry = (TABLE)malloc(sizeof(struct table_entry));
								strcpy(temp_entry->type,prev_prev_lexeme);
								strcpy(temp_entry->name,prev_lexeme);
								temp_entry->index = index_of_identifier++;
								temp_entry->scope = scope;
								temp = getNextToken(fa,row_no,col_no); //extracting 'n' size of array in arr[n] and convert using atoi
								if(strcmp(temp->lexeme,"]") != 0){ //check if size is specified in array declaration i.e. arr[n] or arr[]
									temp_entry->size = findSizeOfDataType(prev_prev_lexeme) * atoi(temp->lexeme);
									temp = getNextToken(fa,row_no,col_no);//extracting ]
								}
								else{
									temp_entry->size = 0;
								}
								insert(symbol_table,last_table_index,temp_entry);
								//printf("extracting %s\n",temp->lexeme);
								temp = getNextToken(fa,row_no,col_no);//extracting , or ; or =
								//printf("extracting %s\n",temp->lexeme);

								if(strcmp(temp->lexeme,",") == 0){
									temp = getNextToken(fa,row_no,col_no);//extract identifier
									strcpy(prev_lexeme,temp->lexeme);
									temp = getNextToken(fa,row_no,col_no); //extract [
								}
								else if(strcmp(temp->lexeme,";") == 0){
									temp = getNextToken(fa,row_no,col_no);
									break;
								}
								else if(strcmp(temp->lexeme,"=") == 0){
									printf("in = %s\n",prev_lexeme);

									temp = getNextToken(fa,row_no,col_no);//extract { or a string_literal token
									if(strcmp(temp->lexeme,"{") == 0){
										while(strcmp(temp->lexeme,"}") != 0){
											temp = getNextToken(fa,row_no,col_no);
										}
									}
									else if(temp->type == string_literal){
										printf("string literal\n");
									}
									temp = getNextToken(fa,row_no,col_no);//extract , or ;
									if(strcmp(temp->lexeme,",") == 0){
										temp = getNextToken(fa,row_no,col_no);//extract identifier
										strcpy(prev_lexeme,temp->lexeme);
										temp = getNextToken(fa,row_no,col_no); //extract [
									}
									else if(strcmp(temp->lexeme,";") == 0){
										temp = getNextToken(fa,row_no,col_no);
										break;
									}
								}
							}
						}
						else if(strcmp(temp->lexeme,";") == 0){ //single variable declaration of type " int a; "
							//printf("one var dec\n");
							TABLE temp_entry = (TABLE)malloc(sizeof(struct table_entry));
							strcpy(temp_entry->type,prev_prev_lexeme);
							temp_entry->size = findSizeOfDataType(prev_prev_lexeme);
							strcpy(temp_entry->name,prev_lexeme);
							temp_entry->index = index_of_identifier++;
							temp_entry->scope = scope;

							insert(symbol_table,last_table_index,temp_entry);
						}
						else if(strcmp(temp->lexeme,"(") == 0){//inside function params, scope = '\0'
							//function declaration
							printf("in function () %s %s\n",prev_prev_lexeme,prev_lexeme);
							TABLE temp_entry = (TABLE)malloc(sizeof(struct table_entry));
							int func_index_in_table = *last_table_index;
							int number_of_arguments = 0;
							strcpy(temp_entry->return_type,prev_prev_lexeme);
							strcpy(temp_entry->name,prev_lexeme);
							strcpy(temp_entry->type,"FUNC");
							temp_entry->scope = scope;
							temp_entry->index = index_of_identifier++;
							insert(symbol_table,last_table_index,temp_entry);
							createSymbolTable(fa,symbol_table,'\0',row_no,col_no,last_table_index,&number_of_arguments,arguments);
							
							temp = getNextToken(fa,row_no,col_no);
							if(strcmp(temp->lexeme,"{") == 0){
								printf("in function body %s %s\n",prev_prev_lexeme,prev_lexeme);
								createSymbolTable(fa,symbol_table,'L',row_no,col_no,last_table_index,0,arguments);
								printf("end of function %s %s\n",prev_prev_lexeme,prev_lexeme);
							}
							symbol_table[func_index_in_table]->number_of_arguments = number_of_arguments;
							for(int i=0; i<number_of_arguments ; i++){
								strcpy(symbol_table[func_index_in_table]->arguments[i],arguments[i]);
							}
							temp = getNextToken(fa,row_no,col_no);
							printf("next token extracted is %s\n",temp->lexeme);
							continue;
						}
					}
				}
		}
		else{
			temp = getNextToken(fa,row_no,col_no);
			continue;
		}
	}
}

int main(){

	FILE *fa = fopen("arm.c","r");

	TABLE symbol_table[TABLELENGTH];

	if(fa == NULL){
		printf("error opening file\n");
		exit(0);
	}

	int row_no = 1;
	int col_no = -1;

	int last_table_index = 0;

	char arguments[100][100];

	createSymbolTable(fa,symbol_table,'G',&row_no,&col_no,&last_table_index,0,arguments);

	convertArgumentsOfSymbolTableToIDs(symbol_table,&last_table_index);

	displaySymbolTable(symbol_table,&last_table_index);
	fclose(fa);
	fa = fopen("arm.c","r");
	FILE *fb = fopen("lab1_output.txt","w");

	createOutputFile(fa,fb,symbol_table,&last_table_index);

	fclose(fa);
	fclose(fb);
}