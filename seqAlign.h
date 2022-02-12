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
void calculateStringLength(char* inputSequences, int* m, int* n, char *seq1, char *seq2);
int fillTable(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2);
void initializeTable(int m, int n, CELL (*table)[][n], int h, int g);
void retrace(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2);
int isBase(char c);
int max(int a, int b, int c);
int sub(int i, int j, int match, int mismatch, char *seq1, char *seq2);