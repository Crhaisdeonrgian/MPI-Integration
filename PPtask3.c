#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
double Integrationwithfor(double left, double right,double (double), long long count, double h, int numthreads);
double Integrationwithoutfor(double left, double right,double (double), long long count, double h, int numthreads);

double f(double x);
double Integrationwithfor(double left, double right,double f(double), long long count, double h, int numthreads)
{

    double res, x;
    long long i;
    res = (f(left) + f(right)) / 2.0;
        #pragma omp parallel reduction(+ :res) num_threads(numthreads)
        {
                #pragma omp for
                for (i=0; i<count; i++){
                        x=left+i*h;
                        res+=f(x);
                }
        }
        res*=h;
    return res;
}
double Integrationwithoutfor(double left, double right,double f(double), long long count, double h, int numthreads)
{

    double res, x;
    long long i;
    res =(f(left) + f(right)) / 2.0;
        #pragma omp parallel reduction(+: res) num_threads(numthreads)
                {
                        int id=omp_get_thread_num();
                        for (i=id; i< count; i+=numthreads){
                                x=left+i*h;
                                res+=f(x);
                        }
                }
        res *= h;
        return res;
}
double f(double x)
{
    return sqrt(4 - x * x);
}

int main(int argc, char** argv)
{
    double tic = omp_get_wtime();
    long long int n;
    n=199000000;
    double a = 0.0, b = 2.0, h;
    double total_int = 0.0;
    int source;
    int numthreads=1;
    h = (b - a) / n;
    total_int = Integrationwithfor(a, b,f, n, h, numthreads);
    //total_int = Integrationwithoutfor(a, b,f, n, h, numthreads);
                printf("Интеграл sqrt(4-x*x) на отрезке от %f до %f I= %.6e\n", a, b, total_int);
        double toc = omp_get_wtime();
        printf("Прошло %f секунд, на %lld разбиений, на %d процессах\n", (toc - tic), n, numthreads);
        printf("%.6e\n", total_int);

    return 0;
}