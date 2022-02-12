#include "seqAlign.h"
#include <math.h>

void appWrapper(char* inputSequences, char* flag, char* parameters) {
    int i = 5, j = 5, k = 5, l = 5, o = 5, p = 5, size = 10000;
    int* m = &i, *n = &j, *numMatch = &k, *numMis = &l, *numGap = &o, *numGapExt = &p;
    char *seq1 = (char*)malloc(sizeof(char) * size), *seq2 = (char*)malloc(sizeof(char) * size);
    char* optSeq1 = (char*)malloc(sizeof(char) * size), *optSeq2 = (char*)malloc(sizeof(char) * size);
    int* penalties = getParameters(parameters);

    int match = *penalties, mismatch = *(penalties + 1), gapExt = *(penalties + 2), gap = *(penalties + 3);

    calculateStringLength(inputSequences, m, n, seq1, seq2);

    CELL (*table)[*m][*n] = malloc(sizeof *table);

    initializeTable(*m, *n, table, gapExt, gap); 

    int score = fillTable(*m, *n, table, match, mismatch, gapExt, gap, seq1, seq2); 

    retrace(*m, *n, table, gapExt, gap, numMatch, numMis, numGapExt, numGap, seq1, seq2, optSeq1, optSeq2);

}

void retrace(int m, int n, CELL (*table)[][n], int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2) {
    int i = 0, j = 0
    int case = 0;

    int maxLastCell = max((*table)[m][n].Sscore, (*table)[m][n].Dscore, (*table)[m][n].Iscore);
    
    while(i != 0 || j != 0) {
        if(maxLastCell == (*table)[m][n].Sscore || case == 1) {
            *(optSeq1 + i) = seq1[i], *(optSeq1 + i) = seq2[j];
            i -= 1, j -= 1;
        } else if(maxLastCell == (*table)[m][n].Dscore) {

        } else {

        }
    }
}

int fillTable(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2) {
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            (*table)[i][j].Sscore = max((*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore) + sub(i, j, match, mismatch, seq1, seq2);
            (*table)[i][j].Dscore = max((*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap); 
            (*table)[i][j].Iscore = max((*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
        }
    }

    int score = max((*table)[m][n].Sscore, (*table)[m][n].Dscore, (*table)[m][n].Iscore);

    printf("score = %d\n", score);

    return score;
}

int sub(int i, int j, int match, int mismatch, char *seq1, char *seq2) {
    if(seq1[i] == seq2[j]) {
        return match;
    }
    else return mismatch;
    
    return 0;
}

int max(int a, int b, int c) {
    if(a >= b && a >= c) return a;
    else if(b >= a && b >= c) return b;
    else if(c >= a && c >= b) return c;

    return 0;
}

int* getParameters(char* parameters) {
    FILE *inParams = fopen(parameters, "r");
    
    char match[20], mismatch[20], gapExt[20], gap[20];

    char line[20];

    fgets(line, 20, inParams);
    strcpy(match, &line[5]);
    fgets(line, 20, inParams);
    strcpy(mismatch, &line[8]);
    fgets(line, 20, inParams);
    strcpy(gapExt, &line[1]);
    fgets(line, 20, inParams);
    strcpy(gap, &line[1]);

    int matchInt = atoi(match);
    int mismatchInt = atoi(mismatch);
    int gapExtInt = atoi(gapExt);
    int gapInt = atoi(gap);

    static int penalties[4];
    
    penalties[0] = matchInt;
    penalties[1] = mismatchInt;
    penalties[2] = gapExtInt;
    penalties[3] = gapInt;

    printf("match = %d\nmismatch = %d\ngap extension = %d\ngap = %d\n", penalties[0], penalties[1], penalties[2], penalties[3]);

    return penalties;
}

void initializeTable(int m, int n, CELL (*table)[][n], int h, int g) {
    CELL cell;
    (*table)[0][0] = cell;
    (*table)[0][0].Sscore = 0, (*table)[0][0].Dscore = 0, (*table)[0][0].Iscore = 0;
    for(int i = 1; i < m; i++) (*table)[i][0].Sscore = -10000, (*table)[i][0].Dscore = h + i*g, (*table)[i][0].Iscore = -10000;
    for(int j = 1; j < n; j++) (*table)[0][j].Sscore = -10000, (*table)[0][j].Dscore = -10000, (*table)[0][j].Iscore = h + j*g;
}

void calculateStringLength(char* inputSequences, int* m, int* n, char *seq1, char *seq2) {
    char c;
    int markerCount = 0, tempM = 0, tempN = 0, size = 10000, i = 0;
    char tempStr[1];

    FILE *inSequences = fopen(inputSequences, "r");

    while((c = fgetc(inSequences)) != EOF) {
        if(c == '>' && markerCount == 0) markerCount++;
        if(c == '\n' && markerCount == 1) markerCount++;
        if(isBase(c) == 1 && markerCount == 2) {
            tempM++;
            *(seq1 + i) = c;
            i++;
        } 
        if(c == '>' && markerCount == 2) markerCount++;
        if(c == '\n' && markerCount == 3) {
            *(seq1 + i) = '\0';
            markerCount++;
            i = 0;
        }
        if(isBase(c) == 1 && markerCount == 4) {
            tempN++;
            *(seq2 + i) = c;
            i++;
        }
    }
    *(seq2 + i) = '\0';

    *m = tempM;
    *n = tempN;

    printf("m = %lu\n", strlen(seq1));
    printf("n = %lu\n", strlen(seq2));
}

int isBase(char c) {
    if(c == 'A' || c =='G' || c == 'T' || c == 'A') return 1;
    else return 0;
}