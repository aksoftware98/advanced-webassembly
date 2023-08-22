#include <iostream> 
#include <chrono>
#include <thread>

using namespace std;

void long_operation() {
    auto now = chrono::system_clock::now(); 
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    cout << "Starting long operation at" << now_c << endl;
    // Start the timer 
    auto start = chrono::high_resolution_clock::now(); 
    // simulate a long operation
    int sum = 0;
    for (int index = 0; index < 1000000000; index++) {
        // do nothing
     
        sum++;
    }
    cout << "Sum is " << sum << endl;
    // Stop the timer and calculate the elapsed time
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    now = chrono::system_clock::now(); 
    now_c = std::chrono::system_clock::to_time_t(now);
    cout << "Starting long operation at" << now_c << endl;
}

int main() {
    long_operation();
    return 0;
}
