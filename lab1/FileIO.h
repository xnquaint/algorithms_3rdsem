#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;



class filemanager
{
private:
    struct file_definition
    {
        fstream file_object;
        string filename;
        int real_series;
        int empty_series;
        file_definition(string _filename);
        void swap(file_definition &rhs);
    };
public:
    vector <file_definition> input, output;
    int in_count, out_count;
    filemanager(int in_count, int out_count);
    void fileswap();
    void index_swap(int index_in, int index_out);
    int read(int index);
    void write(int index, int value);
    ~filemanager();
    int get_in();
    int get_out();
};