#pragma once
#include <vector>
#include <iostream>
using namespace std;


class fibonacci
{
private:
    vector <int> fib_series;
    int current_pos;
    int index_end;
public:
    int *mass;
    int order;
    fibonacci(int _order);
    void make_order_vector();
    void make_dist_mass();
};