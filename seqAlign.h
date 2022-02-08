#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DP_cell {
    int Sscore;
    int Dscore;
    int Iscore;
} CELL;

void appWrapper(char* inputSequences, char* flag, char* parameters);
int* getParameters(char* parameters);
void calculateStringLength(char* inputSequences, int* m, int* n);
void initializeTable(int m, int n, CELL (*table)[][n], int h, int g);
int isBase(char c);