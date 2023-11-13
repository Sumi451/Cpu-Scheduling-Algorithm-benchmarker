#include <stdio.h>
#include <windows.h>

#define NUM_PROCESSES 5
#define NUM_OPERATIONS 100000000

struct Process {
    int process_id;
    long long burst_time;
};

void performTask(const struct Process process) {
    for (long long i = 0; i < process.burst_time * NUM_OPERATIONS; i++) {
        int result = i % 3 + i * 2 - i;
    }
}

int main() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    struct Process processes[NUM_PROCESSES] = {
        {1, 5},
        {2, 3},
        {3, 8},
        {4, 6},
        {5, 4}
    };

    // First-Come, First-Served (FCFS) scheduling
    printf("First-Come, First-Served (FCFS):\n");
    LARGE_INTEGER start, end, fcfsTime;
    QueryPerformanceCounter(&fcfsTime);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Executing process %d\n", processes[i].process_id);
        QueryPerformanceCounter(&start);
        performTask(processes[i]);
        QueryPerformanceCounter(&end);
        double duration = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        printf("Process %d completed in %f seconds\n", processes[i].process_id, duration);
    }
    QueryPerformanceCounter(&end);
    double totalFcfsTime = (double)(end.QuadPart - fcfsTime.QuadPart) / frequency.QuadPart;
    printf("Total execution time for FCFS: %f seconds\n\n", totalFcfsTime);

    // Shortest Job First (SJF) scheduling
    printf("Shortest Job First (SJF):\n");
    for (int i = 0; i < NUM_PROCESSES - 1; i++) {
        for (int j = 0; j < NUM_PROCESSES - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    QueryPerformanceCounter(&fcfsTime);
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("Executing process %d\n", processes[i].process_id);
        QueryPerformanceCounter(&start);
        performTask(processes[i]);
        QueryPerformanceCounter(&end);
        double duration = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        printf("Process %d completed in %f seconds\n", processes[i].process_id, duration);
    }
    QueryPerformanceCounter(&end);
    double totalSjfTime = (double)(end.QuadPart - fcfsTime.QuadPart) / frequency.QuadPart;
    printf("Total execution time for SJF: %f seconds\n\n", totalSjfTime);

    // Round Robin (RR) scheduling
    printf("Round Robin (RR):\n");
    int timeQuantum = 2; // Set time quantum for RR (change as needed)
    int remainingBurst[NUM_PROCESSES];
    for (int i = 0; i < NUM_PROCESSES; i++) {
        remainingBurst[i] = processes[i].burst_time;
    }
    int currentTime = 0;
    QueryPerformanceCounter(&start);
    while (1) {
        int done = 1;
        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (remainingBurst[i] > 0) {
                done = 0;
                int currentBurst = remainingBurst[i];
                if (remainingBurst[i] > timeQuantum) {
                    currentTime += timeQuantum;
                    remainingBurst[i] -= timeQuantum;
                } else {
                    currentTime += remainingBurst[i];
                    remainingBurst[i] = 0;
                }
                printf("Executing process %d till time %d\n", processes[i].process_id, currentTime);
                performTask(processes[i]);
                if (currentBurst > timeQuantum) {
                    printf("Process %d completed in %d seconds\n", processes[i].process_id, currentTime);
                }
            }
        }
        if (done == 1) {
            break;
        }
    }
    QueryPerformanceCounter(&end);
    double totalRrTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    printf("Total execution time for RR: %f seconds\n", totalRrTime);

    return 0;
}
