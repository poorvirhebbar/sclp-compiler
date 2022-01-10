
float area;
float perimeter;

void main()
{
	float r;
	int x, y, k;
	float z;
	z = 3.0;
	x = 2;
	area = 4.0;


	while(x < 2){
		x = 3;
		x = 6;
	}

	x = !(2.0<3.4) || 2 > 3?4:6;

	if(x <= 5 || x < 4 && x > 5){
		x = 8;
		y = 7;
	}
	else
	{
		x = 9;
		y = 6;
	}
	

	do{
		if(x <= 5 || x < 4 || x > 5){
			 x = 8;
		}
		else{
			x = 9;
		}
	} while(x == 4);

	if(x >= 5 && z < 4.0){
		if(!((x != 9) || x >= 9)){
			x = 3;
			if(x>0){
				x = 8;
				x = ((2.0>3.4) && (2 < 3))?4:6;
				x = 9;
			}
			x = 5;
		}
		x = 8;
	}

	// while(y == 3) y = 9;
	// if(x == 3) y = 9;

	// do x = 1;
	// while(x == 2);

	// Circle
	area = 2.0 * 3.0;
	perimeter = 2.0 * 22.0 ;
}
