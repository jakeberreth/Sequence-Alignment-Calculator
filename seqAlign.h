#include <stdio.h>

void calculateS1Length(char* inputSequences, int* m, int* n);

typedef struct DP_cell {
    int Sscore;
    int Dscore;
    int Iscore;
    int optimalScore;
} CELL;