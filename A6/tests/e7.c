// different return types in decl and definition of same function

int x;
int a,b;

void func1(int a,int b);

int func1(int a,int b){
	return b;
}

void main(){
	a=1;b=2;
	x=func1(a,b);
	print x;

}