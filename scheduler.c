#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int waiting;
    int turnaround;
};

void fcfs(struct Process p[], int n);
void sjf(struct Process p[], int n);
void roundRobin(struct Process p[], int n, int quantum);

void printTable(struct Process p[], int n, float avg_wait, float avg_turn);

int main() {
    int n, choice, quantum;
    struct Process p[10];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nProcess %d:\n", p[i].pid);
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        p[i].remaining = p[i].burst;
    }

    while (1) {
        printf("\n===== CPU SCHEDULING MENU =====\n");
        printf("1. First Come First Serve (FCFS)\n");
        printf("2. Shortest Job First (SJF)\n");
        printf("3. Round Robin\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: fcfs(p, n); break;
            case 2: sjf(p, n); break;
            case 3:
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                roundRobin(p, n, quantum);
                break;
            case 4: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

// =====================
// FCFS Scheduling
// =====================
void fcfs(struct Process p[], int n) {
    struct Process temp;
    int total_wait = 0, total_turn = 0;
    float avg_wait, avg_turn;

    // Sort by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival > p[j + 1].arrival) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;
        p[i].waiting = time - p[i].arrival;
        time += p[i].burst;
        p[i].turnaround = p[i].waiting + p[i].burst;
        total_wait += p[i].waiting;
        total_turn += p[i].turnaround;
    }

    avg_wait = (float)total_wait / n;
    avg_turn = (float)total_turn / n;

    printf("\nFCFS Scheduling Results:\n");
    printTable(p, n, avg_wait, avg_turn);
}

// =====================
// SJF Scheduling
// =====================
void sjf(struct Process p[], int n) {
    int completed = 0, time = 0;
    int total_wait = 0, total_turn = 0;
    float avg_wait, avg_turn;
    int done[10] = {0};

    while (completed != n) {
        int idx = -1, min_burst = 9999;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && done[i] == 0 && p[i].burst < min_burst) {
                min_burst = p[i].burst;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        time += p[idx].burst;
        p[idx].turnaround = time - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done[idx] = 1;
        completed++;

        total_wait += p[idx].waiting;
        total_turn += p[idx].turnaround;
    }

    avg_wait = (float)total_wait / n;
    avg_turn = (float)total_turn / n;

    printf("\nSJF Scheduling Results:\n");
    printTable(p, n, avg_wait, avg_turn);
}

// =====================
// Round Robin Scheduling
// =====================
void roundRobin(struct Process p[], int n, int quantum) {
    int total_wait = 0, total_turn = 0, completed = 0;
    int time = 0;

    int remaining[n];
    for (int i = 0; i < n; i++)
        remaining[i] = p[i].burst;

    while (completed < n) {
        int flag = 0;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && p[i].arrival <= time) {
                flag = 1;
                if (remaining[i] > quantum) {
                    time += quantum;
                    remaining[i] -= quantum;
                } else {
                    time += remaining[i];
                    p[i].waiting = time - p[i].arrival - p[i].burst;
                    p[i].turnaround = time - p[i].arrival;
                    remaining[i] = 0;
                    completed++;
                    total_wait += p[i].waiting;
                    total_turn += p[i].turnaround;
                }
            }
        }
        if (flag == 0) time++;
    }

    float avg_wait = (float)total_wait / n;
    float avg_turn = (float)total_turn / n;

    printf("\nRound Robin Scheduling Results:\n");
    printTable(p, n, avg_wait, avg_turn);
}

// =====================
// Utility Function
// =====================
void printTable(struct Process p[], int n, float avg_wait, float avg_turn) {
    printf("\nPID\tArrival\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].waiting, p[i].turnaround);
    }
    printf("\nAverage Waiting Time: %.2f", avg_wait);
    printf("\nAverage Turnaround Time: %.2f\n", avg_turn);
}
