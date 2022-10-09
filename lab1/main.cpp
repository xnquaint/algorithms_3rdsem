#include "PolyphaseSort.h"
#include <ctime>


#define input_name "input.txt"
#define output_name "output.txt"
#define debug_file "debug_info.txt"
using namespace std;

const bool debug_mode = false;
const long int bytes_in_mb = 1048576;


void show_output_files(polyphase &sorting, int amount_of_files)
{
    for (int i = 0; i < amount_of_files - 1; i++)
    {
        string file_name = "file_output_" + to_string(i) + ".txt";
        cout << file_name << ": ";
        sorting.show_binary_file_length(file_name);
    }
}


void sort_function(filemanager &manager, polyphase &sorting, bool show_output, clock_t &start_dist, clock_t &end_dist, clock_t &start_merge, clock_t &end_merge, int amount_of_files)
{
    start_dist = clock();
    sorting.first_distribution(input_name, manager);
    end_dist = clock();
    if (show_output)
    {
        cout << "Files contains (first distribution): " << endl;
        show_output_files(sorting, amount_of_files);
    }
    int min = manager.output[0].empty_series;
    for (int i = 1; i < amount_of_files - 1; i++)
    {
        if (min > manager.output[i].empty_series)
        {
            min = manager.output[i].empty_series;
        }
    }
    if (min != 0)
    {
        for (int i = 0; i < amount_of_files - 1; i++)
        {
            manager.output[i].empty_series -= min;
        }
        manager.input[0].empty_series += min;
    }
    manager.fileswap();
    cout << endl << "Sorting... ";
    start_merge = clock();
    sorting.merge(manager);
    end_merge = clock();
    manager.fileswap();
}


void view_debug_info(polyphase &sorting)
{
    cout << "File " << input_name << " include: " << endl;
    int quantity = sorting.show_binary_file_eof(input_name);
    cout << "Amount of generated: " << quantity << endl << endl;
    cout << "Checking series: " << endl;
    quantity = sorting.amount_of_series(input_name);
    cout << "Amount of series: " << quantity << endl << endl;
}

int main() {
    int file_size;
    int amount_of_files;
    cout << "Enter amount of MBs to be sorted: ";
    cin >> file_size;
    if (file_size < 0)
        file_size = abs(file_size);
    long int amount_of_numbers = (file_size  * bytes_in_mb) / 4;
    cout << "Enter amount of files: ";
    cin >> amount_of_files;
    if (amount_of_files <= 0)
        amount_of_files = 3;
    filemanager manager(1, amount_of_files - 1);
    polyphase sorting;
    clock_t start_generate, end_generate, start_dist, end_dist, start_merge, end_merge;
    start_generate = clock();
    sorting.generate(input_name, amount_of_numbers,
                     1000);
    end_generate = clock();
    if (debug_mode) {
        view_debug_info(
                sorting);
    }
    cout << "\n" << "File generated." << endl;
    sort_function(manager, sorting, debug_mode, start_dist, end_dist, start_merge,
                  end_merge, amount_of_files);

    fstream f;
    f.open(output_name, ios::out);
    int length = manager.read(0);
    for (int i = 0; i < length; i++) {
        f << " " << manager.read(
                0);
    }
    f.close();
    cout << endl;


    sorting.check_sort(output_name);
    float gen_t = ((double) end_generate - start_generate) / ((double) CLOCKS_PER_SEC);
    float dist_t = ((double) end_dist - start_dist) / ((double) CLOCKS_PER_SEC);
    float merge_t = ((double) end_merge - start_merge) / ((double) CLOCKS_PER_SEC);
    cout << "Number of elements: " << amount_of_numbers << endl;
    cout << "Number of files: " << amount_of_files << endl;
    cout << "Generation time: " << gen_t << " sec." << endl;
    cout << "First distribution time: " << dist_t << " sec." << endl;
    cout << "Merge time: " << merge_t << " sec." << endl << endl;


    fstream debug;
    debug.open(debug_file, ios::out |
                           ios::app);
    debug << "Number of elements: " << amount_of_numbers << endl;
    debug << "Number of files: " << amount_of_files << endl;
    debug << "Generation time: " << gen_t << " sec." << endl;
    debug << "First distribution time: " << dist_t << " sec." << endl;
    debug << "Merge time: " << merge_t << " sec." << endl << endl;
    debug.close();
    return 0;
}