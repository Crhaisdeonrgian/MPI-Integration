#include <math.h>
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
double Integration(double left, double right, long long count, double h, double tic);

double f(double x);

int main(int argc, char** argv)
{
    double tic = MPI_Wtime();
    long long int n, my_n;
    int my_rank, numtasks;
    n = atoll(argv[1]);
    double a = 0.0, b = 2.0, h, my_a, my_b;
    double my_int = 0.0, total_int = 0.0;
    int source;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    h = (b - a) / n;
    my_n = n / numtasks;
	if(n % numtasks)
                        my_n++;
        long long last_n = n - my_n * ((numtasks) - 1);
        if (rank == numtasks - 1) {
                my_n = last_n;
        }

    my_a = a + my_rank * my_n * h;
    my_b = my_a + my_n * h;
    my_int = Integration(my_a, my_b, my_n, h, tic);
    if (my_rank != 0) {
        MPI_Send(&my_int, 1, MPI_DOUBLE, 0, 0,
            MPI_COMM_WORLD);
    }
    else {
        total_int = my_int;
        for (source = 1; source < numtasks; source++) {
            MPI_Recv(&my_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += my_int;
        }
    }

    if (my_rank == 0) {
        printf("Интеграл sqrt(4-x*x) на отрезке от %f до %f I= %.6e\n", a, b, total_int);
        double toc = MPI_Wtime();
        printf("Прошло %f секунд, на %lld разбиений, на %d процессах\n", (toc - tic), n, numtasks);
        printf("%.6e\n", total_int);}

    MPI_Finalize();

    return 0;
}

double Integration(double left, double right, long long count, double h, double tic)
{

    double res, x;
    long long i;

    res = (f(left) + f(right)) / 2.0;
    for (i = 1; i < count; i++) {
        x = left + i * h;
        res += f(x);
    }
    res *= h;

    return res;
}

double f(double x)
{
    return sqrt(4 - x * x);
}
