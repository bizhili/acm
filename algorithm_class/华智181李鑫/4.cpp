#include"iostream"
int main()
{
    int n;
    double A=0.0;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        i%2 ? A+=(double)1/i :A-=(double)1/i;
    printf("%.4lf",A);
}