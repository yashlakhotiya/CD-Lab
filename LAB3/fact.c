int fact5;
int fact (int n){
	int val;
	if(n>1)
	{
		val=n*factorial(n-1);
		return (val);
	}
	else
	{
		return (1);
	}
}
//double primeno(double prime){
//printf("hey");
//}
int main(){
	printf("factorial program\n");
	fact5=fact(5);
	printf("fact5=%d\n",fact5);
}