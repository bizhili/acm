#include"iostream"
int main()
{
    float N,A;
    scanf("%f",&N);
    if(N>=0.0f && N<5.0f)
        A=-N+2.5;
    else if(N>=5.0f && N<10.f)
        A=2-1.5*(N-3)*(N-3);
    else
        A=N/2-1.5;
    printf("%.3f",A);
    return 0;
}