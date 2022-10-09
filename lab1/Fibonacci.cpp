#include "Fibonacci.h"

fibonacci::fibonacci(int _order)
{
    current_pos = 0;
    order = _order;
    index_end = order - 1;
    for (int i = 0; i < order; i++)
    {
        fib_series.push_back(0);
    }
    fib_series[index_end] = 1;
}

void fibonacci::make_order_vector()
{
    int temp1 = 0;
    int temp2 = 0;
    for (int i = 0; i < fib_series.size(); i++)
    {
        temp1 = temp1 + fib_series[i];
        if (i > order)
        {
            temp2 = temp2 + fib_series[i - order];
        }
    }
    fib_series.push_back(temp1 - temp2);
}

void fibonacci::make_dist_mass()
{
    mass = new int[order];
    int finish = current_pos + order;
    for (int i = 0; i < order; i++)
    {
        int temp = 0;
        for (int j = current_pos; j < finish; j++)
        {
            temp = temp + fib_series[j];
        }
        current_pos++;
        mass[i] = temp;
    }
    current_pos = finish - order + 1;
}