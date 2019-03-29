#include <math.h>
#include <stdio.h>

#define PI 3.1415926f
#define l0 0.1035f
#define l1 0.088f
#define l2 0.17f

int a[7]={-1,-1,-1,-1,-1,-1,-1};

int ServoSetPluseAndTime(int id, int p, int time) {
	if(p<500){
	   p = 500;
	}
	
	if(p>2500)
	{
	   p = 2500;
	}
	a[id] = p;
	
    return 0;
}
void computeNum1(float x, float y){
   
   float h = atan(x/y);
   float d0 = h*180/PI;
   ServoSetPluseAndTime(1, (2000 *  d0 / 180.0 + 1500.0), 2000);
}



int model(float x, float y, float alpha) {
	float m, n, k, a, b, c, theta1, theta2, theta3, s1ps2;
	m = l2 * cos(alpha) - x;
	n = l2 * sin(alpha) - y;
	k = (l1 * l1 - l0 * l0 - m * m - n * n) / 2 / l0; 
	a = m * m + n * n;            
	b = -2 * n * k;
	c = k * k - m * m;

	if (b * b - 4 * a * c <= 0)   
	{

		return 1; 
	}
	theta1 = (-b + sqrt(b * b - 4 * a * c)) / 2 / a; 
	theta1 = asin(theta1) * 180 / PI;        

	if (theta1 > 90)
		theta1 = 90;          
	if (theta1 < -90)
		theta1 = -90;

	k = (l0 * l0 - l1 * l1 - m * m - n * n) / 2 / l1;
	a = m * m + n * n;                      
	b = -2 * n * k;
	c = k * k - m * m;

	if (b * b - 4 * a * c <= 0)  
	{

		return 2;        
	}
	s1ps2 = (-b - sqrt(b * b - 4 * a * c)) / 2 / a;
	s1ps2 = asin(s1ps2) * 180 / PI;         

	if (s1ps2 > 90)
		theta2 = 90;
	if (s1ps2 < -90)
		theta2 = -90;

	theta2 = s1ps2 - theta1;
	if (theta2 > 90)
		theta2 = 90;       
	if (theta2 < -90)
		theta2 = -90;    

	theta3 = alpha * 180 / PI - theta1 - theta2;  
	if (theta3 > 180)
		theta3 = 180;
	if (theta3 < 0)
		theta3 = 0;	    

ServoSetPluseAndTime(2, (2000 * (90-theta1) / 180.0 + 500.0), 2000); 
ServoSetPluseAndTime(3, (2000 * (90+theta2) / 180.0 + 500.0), 2000); 
ServoSetPluseAndTime(4, (2000 * (90+theta3) / 180.0 + 500.0), 2000); 
	
		printf("theta1 for 2=%f\r\n", theta1);
	printf("theta2 for 3=%f\r\n", theta2);
	printf("theta3 for 4=%f\r\n", theta3);
	float y1 = l0*sin(theta1*PI/180);
	float y2 = l1*sin((theta1+theta2)*PI/180);
	float y3 = l2*sin((theta1+theta2+theta3)*PI/180);
	float x1 = l0*cos(theta1*PI/180);
	float x2 = l1*cos((theta1+theta2)*PI/180);
	float x3 = l2*cos((theta1+theta2+theta3)*PI/180);
	    printf("l0=%f l1=%f l2=%f l=%f\n",l0,l1,l2, l1+l2+l0);
		printf("x=%f\n",x);

	printf("   %f       %f     %f   x = %f\n", x1,x2 , x3,x1+x2+x3);
		printf("y=%f\n",y);
	printf("   %f       %f     %f   y = %f\n", y1,y2 , y3,y1+y2+y3);
	return 0; 

}

int* getPosture(float x, float y){
	
	float h = -0.03;///
	
	float alpha = PI*0.7;
	float step = 0.000002;
	float delta=0;
    computeNum1(x,y);
	do{
      if(model(h, sqrt(x*x + y*y), PI-alpha) ==0 ){
	     return a;
	  }
	  if(model(h, sqrt(x*x + y*y), PI+alpha) ==0 ){
	     return a;
	  }	
	  delta += step;
	}while(PI-step<alpha);
	printf("error %f\n",step);
	return 0;
}

int test() {
   if((int*)0==getPosture(0.0, 0.20)) 
   {
	   printf("error");
	   return 0;
   
   }
   for (int i=1;i<=6;i++){
        printf("#%dP%dT%d!\n", i, a[i], 2000);
   }
	return 0;
}
