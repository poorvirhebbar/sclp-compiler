int mm;
int fibonacci(int a,int b)
{
    int z,zz,zzz;
    if(a == 0 || a == 1)
    {
    	zzz = 1;
       return zzz;
    }
    
         z=a-1;
        z = fibonacci(z,2);
        
         zz=a-2;
        
        zz = fibonacci(zz,2);

        zzz = z + zz;
   
        return 4;
   
}


void main()
{   
    int b;
    b=4;
    b = fibonacci(b,2);
    print b;
    return;
}


