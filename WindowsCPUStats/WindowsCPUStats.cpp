// WindowsCPUStats.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <pdh.h>

#pragma comment(lib, "pdh.lib")

int main() {
    PDH_STATUS status;
    PDH_HQUERY query;
    PDH_HCOUNTER counter;

    status = PdhOpenQuery(NULL, 0, &query);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to initialize PDH library." << std::endl;
        return 1;
    }

    // Add the counter for CPU usage
    status = PdhAddCounter(query, L"\\Processor(_Total)\\% Processor Time", 0, &counter);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to add counter." << std::endl;
        PdhCloseQuery(query);
        return 1;
    }

    // Collect and display CPU usage every second
    while (true) {
        status = PdhCollectQueryData(query);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Failed to collect query data." << std::endl;
            PdhCloseQuery(query);
            return 1;
        }

        PDH_FMT_COUNTERVALUE value;
        status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE, NULL, &value);
        if (status != ERROR_SUCCESS) {
            std::cerr << "Failed to get formatted counter value." << std::endl;
            PdhCloseQuery(query);
            return 1;
        }

        std::cout << "CPU Usage: " << value.doubleValue << "%" << std::endl;

        Sleep(1000); // Pause for 1 second
    }

    PdhCloseQuery(query);

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
