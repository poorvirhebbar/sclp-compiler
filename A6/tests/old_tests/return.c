int a,b,c;
float x,y,z;

int f1(int p, int q) {
    p = q;
    a = b;
    return p+q*p;
}

void main() {
    int a,b;
    a = 1;
    b = 3;
    f1(a, b);
}