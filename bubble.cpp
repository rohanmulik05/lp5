#include <iostream>
#include <chrono>
#include <cstdlib>
#include <omp.h>
#include <iomanip>
using namespace std;
using namespace chrono;

void print_array(int arr[], int size)
{
    cout << "Array: [ ";
    for (int i = 0; i < size; i++) 
    {
        cout << arr[i] << " ";
    }
    cout << "]\n";
}

void sequential_bubble_sort(int arr[], int size, double &seq_time) 
{
    int temp[size];
    copy(arr, arr + size, temp);

    auto start = high_resolution_clock::now();
    for (int i = 0; i < size - 1; i++) 
    {
        for (int j = 0; j < size - i - 1; j++) 
        {
            if (temp[j] > temp[j + 1]) 
            {
                swap(temp[j], temp[j + 1]);
            }
        }
    }
    auto end = high_resolution_clock::now();
    seq_time = duration<double>(end - start).count();
    cout << "\nSequential Bubble Sort Time: " << fixed << setprecision(10) << seq_time << " s\n";
    print_array(temp, size);
}

void parallel_odd_even_sort(int arr[], int size, double &seq_time) 
{
    int temp[size];
    copy(arr, arr + size, temp);

    auto start = high_resolution_clock::now();

    bool sorted = false;
    while (!sorted) 
    {
        sorted = true;

        #pragma omp parallel for shared(temp, size, sorted)
        for (int i = 1; i < size - 1; i += 2) 
        {
            if (temp[i] > temp[i + 1]) 
            {
                swap(temp[i], temp[i + 1]);
                sorted = false;
            }
        }

        #pragma omp parallel for shared(temp, size, sorted)
        for (int i = 0; i < size - 1; i += 2) 
        {
            if (temp[i] > temp[i + 1]) 
            {
                swap(temp[i], temp[i + 1]);
                sorted = false;
            }
        }
    }

    auto end = high_resolution_clock::now();
    double par_time = duration<double>(end - start).count();
    cout << endl;
    cout << "Parallel Bubble Sort Time: " 
         << fixed << setprecision(10) << par_time << " s\n";
    print_array(temp, size);
    cout << endl;
    cout << "Speedup: " << fixed << setprecision(10) << seq_time / par_time << "\n";
}


int main() 
{
    int size;
    int choice;

    cout << "Enter size of array: ";
    cin >> size;

    int arr[size];
    for (int &x : arr) x = rand() % size;

    double seq_bubble_time, seq_merge_time;

    print_array(arr, size);
    sequential_bubble_sort(arr, size, seq_bubble_time);
    parallel_odd_even_sort(arr, size, seq_bubble_time);
      
    
}

