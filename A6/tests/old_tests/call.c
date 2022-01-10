int a,b,c;
float x,y,z;

void f1(int p, int q) {
    p = q;
    a = b;
}

void main() {
    int a,b;
    a = 1;
    b = 3;
    f1(a, b);
}