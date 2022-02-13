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
void calculateStringLength(char* inputSequences, int* m, int* n, char *seq1, char *seq2, char *seqName1, char *seqName2);
int fillTableGlobal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2);
int fillTableLocal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2);
void initializeTable(int m, int n, CELL (*table)[][n], int h, int g);
void retraceGlobal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2);
void retraceLocal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2);
int isBase(char c);
int max(int a, int b, int c);
int sub(int i, int j, int match, int mismatch, char *seq1, char *seq2);
int getNext(int max, int s, int d, int i);
void displayOptAlignment(char *optSeq1, char *optSeq2, int score, int match, int mismatch, int gap, int gapExt, char *seq1, char *seq2, int numMatch, int numMis, int numGapExt, int numGap, char *name1, char *name2);