#include <stdio.h>
#include <ctime>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <chrono>

using namespace std::chrono;

double y(double x) {
	return 5 / sqrt(8 - 4 * x * x);
}

int main()
{
	double a = -1;
	double b = 1;

	int N = 200000000;
	double h = (b - a) / N;

	double res = 0;
	int i;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for(i = 0; i < N; ++i)
	{
		res += h * y(a + h * i);
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> duration = (t2 - t1);
	printf("Answer = %f, Time = %f ticks\n\n", res, duration.count());

	cilk::reducer_opadd <double> cilk_res(0);

	t1 = high_resolution_clock::now();
	cilk_for (i = 0; i < N; ++i)
	{
		cilk_res += h * y(a + h * i);
	}
	t2 = high_resolution_clock::now();

	duration = (t2 - t1);
	printf("Answer = %f, Parallel time = %f ticks\n\n", cilk_res.get_value(), duration.count());

	return 0;
}