#pragma once
#include "FileIO.h"
#include "Fibonacci.h"
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;



class polyphase
{
public:
    vector <int> position_series;
    polyphase() = default;;
    void generate(string filename, int amount, int random_border);
    int show_binary_file_eof(string filename);
    int show_binary_file_length(string filename);
    int amount_of_series(string filename);
    int check_sort(string filename);
    void first_distribution(string filename, filemanager &manager);
    void merge(filemanager &manager);
    bool eof(string filename);
};

inline bool polyphase::eof(string filename)
{
    fstream f;
    f.open(filename, ios::in | ios::binary | ios::ate);
    bool eof = !f.tellg();
    f.close();
    return eof;
}

void polyphase::merge(filemanager &manager)
{
    multimap <int, int> buffer;
    int count = manager.get_in();
    int *length_of_series = new int[count];
    int sum_of_series = 2;
    int current_length;

    while (sum_of_series > 1)
    {
        current_length = 0;
        for (int i = 0; i < count; i++)
        {
            if (manager.input[i].empty_series)
            {
                length_of_series[i] = 0;
                manager.input[i].empty_series--;
            }
            else
            {
                if (!manager.input[i].real_series)
                {
                    manager.index_swap(i, 0);
                    if (manager.input[i].empty_series)
                    {
                        i--;
                        continue;
                    }
                }
                length_of_series[i] = manager.read(i);
                manager.input[i].real_series--;
                buffer.insert(pair <int, int>(manager.read(i), i));
            }
            current_length += length_of_series[i]--;
        }

        manager.write(0, current_length);
        manager.output[0].real_series++;

        auto buf = buffer.begin();
        while (!buffer.empty())
        {
            buf = buffer.begin();
            manager.write(0, buf->first);
            if (length_of_series[buf->second]--)
            {
                buffer.insert(pair <int, int>(manager.read(buf->second), buf->second));
            }
            buffer.erase(buf);
        }

        sum_of_series = 0;
        for (int i = 0; i < count; i++)
        {
            sum_of_series += manager.input[i].real_series + manager.input[i].empty_series;
        }
        sum_of_series += manager.output[0].real_series + manager.output[0].empty_series;
    }
}

void polyphase::first_distribution(string filename, filemanager &manager)
{
    fstream f;
    f.open(filename, ios::in | ios::binary);
    int counter = manager.get_out();
    fibonacci fib_vector(counter);
    fib_vector.make_order_vector();
    fib_vector.make_dist_mass();
    int current = 0;
    int next = 0;
    int current_file = 0;
    int length = 0;
    streampos start_pos = manager.output[current_file].file_object.tellg();
    streampos end_pos;
    manager.output[current_file].real_series++;
    manager.write(current_file, 0);
    f.read((char *)&next, sizeof(int));

    int *diff = new int[counter];
    diff[0] = 0;
    for (int i = 1; i < counter; i++)
    {
        diff[i] = 1;
    }
    bool flag = false;
    while (true)
    {

        while (current <= next)
        {
            manager.write(current_file, next);
            length++;
            current = next;
            if (!f.eof())
            {

                f.read((char *)&next, sizeof(int));
            }
            else
            {
                flag = !flag;
                break;
            }
        }
        if (flag == 1) break;
        end_pos = manager.output[current_file].file_object.tellg();
        manager.output[current_file].file_object.seekg(start_pos);
        manager.write(current_file, length);
        manager.output[current_file].file_object.seekg(end_pos);
        if ((current_file < counter - 1) && ((diff[current_file] + 1) == diff[current_file + 1]))
        {
            current_file++;
            diff[current_file]--;
            manager.output[current_file].real_series++;
        }
        else
        {
            if (!diff[counter - 1])
            {
                fib_vector.make_order_vector();
                fib_vector.make_dist_mass();
                for (int i = 0; i < counter; i++)
                {
                    diff[i] = fib_vector.mass[i] - manager.output[i].real_series;
                }
            }
            current_file = 0;
            diff[current_file]--;
            manager.output[current_file].real_series++;
        }
        start_pos = manager.output[current_file].file_object.tellg();
        manager.write(current_file, 0);
        length = 0;

        manager.write(current_file, next);
        length++;
        current = next;
        if (!f.eof())
        {
            f.read((char *)&next, sizeof(int));
        }
        else
        {
            break;
        }
    }
    end_pos = manager.output[current_file].file_object.tellg();
    manager.output[current_file].file_object.seekg(start_pos);
    manager.write(current_file, length);
    manager.output[current_file].file_object.seekg(end_pos);
    for (int i = 0; i < counter; i++)
    {
        manager.output[i].empty_series = diff[i];
    }
    f.close();
}

inline int polyphase::show_binary_file_length(string filename)
{
    if (eof(filename))
    {
        cout << "[empty]" << endl;
    }
    else
    {
        fstream f;
        f.open(filename, ios::in | ios::binary);
        int length;
        while (!f.eof())
        {
            f.read((char *)&length, sizeof(int));
            if (length)
            {
                cout << "[len: " << length << "] ";
                while (length)
                {
                    int temp;
                    f.read((char *)&temp, sizeof(int));
                    cout << temp << " ";
                    length--;
                }
            }
        }
        cout << endl;
        f.close();
    }
    return 0;
}

inline void polyphase::generate(string filename, int amount, int random_border)
{
    fstream f;
    srand(time(NULL));
    f.open(filename, ios::out | ios::binary);
    for (int i = 0; i < amount - 1; i++)
    {
        int value = rand() % random_border;
        f.write((char*)&value, sizeof(int));
    }
    f.close();
}

inline int polyphase::check_sort(string filename)
{
    bool flag = true;
    int counter = 1;
    int temp1, temp2;
    fstream f;
    f.open(filename, ios::in);
    f >> temp1;
    while (!f.eof())
    {
        f >> temp2;
        counter++;
        if (temp1 > temp2)
        {
            cout << "Sort error: " << counter << " counter";
            flag = false;
        }
        temp1 = temp2;
    }
    if (flag)
    {
        cout << "Sort is ok: " << counter << " elements sorted";
    }
    cout << endl;
    return flag;
}

inline int polyphase::show_binary_file_eof(string filename)
{
    fstream f;
    f.open(filename, ios::in | ios::binary);
    int temp;
    int quantity = 0;
    while (!f.eof())
    {
        int temp;
        f.read((char *)&temp, sizeof(int));
        cout << temp << " ";
        quantity++;
    }
    cout << endl;
    f.close();
    return quantity;
}



int polyphase::amount_of_series(string filename)
{
    fstream f;
    f.open(filename, ios::in | ios::binary);
    int quantity = 1;
    int current = 0;
    int next = 0;
    position_series.push_back(f.tellg());
    cout << "Current position: " << 1 + f.tellg() << " :: ";
    f.read((char *)&current, sizeof(int));
    cout << "Series number" << quantity << " starts from: " << current << endl;
    f.read((char *)&next, sizeof(int));
    while (!f.eof())
    {

        if (next > current)
        {
            f.read((char *)&current, sizeof(int));
        }
        else
        {
            quantity++;
            cout << "Current position: " << f.tellg() / sizeof(int) << " :: ";
            position_series.push_back(f.tellg());
            cout << "Series number " << quantity << " starts from: " << next << endl;
            f.read((char *)&current, sizeof(int));
        }
        if (current > next)
        {
            f.read((char *)&next, sizeof(int));
        }
        else
        {
            quantity++;
            cout << "Current position: " << f.tellg() / sizeof(int) << " :: ";
            position_series.push_back(f.tellg());
            cout << "Series number" << quantity << " starts from: " << current << endl;
            f.read((char *)&next, sizeof(int));
        }
    }
    f.close();
    return quantity;
}
