int a,b;
void main() {
    int i,j;
    i=0;
    j=1;
    a = 0;
    if(i > j) {
        b = 5;
    }
    else
    {
        b=4;
    }

    if(i>j)
        b=5;
    else
        b=4;
    
    if(i>j)
        {b=5;}
    else
        b=4;
    
    if(i>j)
        b=5;
    else
        {b=4;}
    
    if(i>j)
        if(i>j)
            b=4;
        else
            b=5;
    else
        b=6;
}