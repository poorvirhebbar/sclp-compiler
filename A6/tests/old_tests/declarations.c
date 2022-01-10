int a,b,c;
float x,y,z;

int f1(int, float);

int main() {
    a = 1;
    b = 2;
    print a;
    print b;


    a = f1(a,b);

}


int f1(int p, int q) {
    if(p == 0 || p ==1) {
        a = 3;
    }
    while(p>q) {
        p = p-1;
    }

    return p+q;
}

