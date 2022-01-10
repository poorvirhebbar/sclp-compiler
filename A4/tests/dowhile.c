int a,b;
void main() {
	float i,j;
	i=0.0;
	j=1.0;

	do {
		if(i>2.0)
			i = i + 1.0;
		else
			i = i + 2.0;
	}
	while(i<5.0 && i > 3.0 && i+j*5.0 != 3.0);
}