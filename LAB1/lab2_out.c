//REPLACES BLANK SPACES AND TABS BY A SINGLE BLANK SPACE


int main(){
	FILE *fa,*fb;

	char c,d;
	
	fa = fopen("lab1.c","r");
	if(fa == NULL){
		printf("error opening file\n");
		exit(0);
	}
	fb = fopen("lab1_out.c","w");
	if(fb == NULL){
		printf("error opening file\n");
		exit(0);
	}

	c = getc(fa);
	while(c != EOF){
		if(!(c == ' ' || c == '\t' || c == '\n')){
			putc(c,fb);
		}
		else{
			d = getc(fa);
			while(d == ' ' || d == '\t' || d == '\n'){
				d = getc(fa);
			}
			if(d != EOF){
				putc(SPACE,fb);
				putc(d,fb);
			}
		}
		c = getc(fa);
	}
	fclose(fa);
	fclose(fb);
	return 0;
}