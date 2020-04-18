#include"iostream"
int main()
{
    double x,a,b,c,d,f;
    scanf("%lf %lf %lf %lf %lf",&x,&a,&b,&c,&d);
    f=a*x*x*x+b*x*x+c*x+d;
    printf("%.7f",f);
}