int a,b,c;
float x,y,z;

int f1(int,int);

int f2(int a){
	if(a == 1)
		return 1;
	else
		return a*f2(a-1);
}

void main() {
    int a,b;
    a = 1;
    b = 3;
    print f1(a, b);
    print f2(a);
}

int f1(int p, int q) {
    p = q;
    a = b;
    return p+q*p;
}
