#include "seqAlign.h"
#include <math.h>

void appWrapper(char* inputSequences, char* flag, char* parameters) {
    int i = 5, j = 5, k = 5, l = 5, o = 5, p = 5, size = 10000, score = 0;
    int* m = &i, *n = &j, *numMatch = &k, *numMis = &l, *numGap = &o, *numGapExt = &p;
    char *seq1 = (char*)malloc(sizeof(char) * size), *seq2 = (char*)malloc(sizeof(char) * size);
    char *optSeq1 = (char*)malloc(sizeof(char) * size), *optSeq2 = (char*)malloc(sizeof(char) * size);
    int* penalties = getParameters(parameters);
    char *seqName1 = (char*)malloc(sizeof(char) * 100), *seqName2 = (char*)malloc(sizeof(char) * 100);

    int match = *penalties, mismatch = *(penalties + 1), gapExt = *(penalties + 2), gap = *(penalties + 3);

    calculateStringLength(inputSequences, m, n, seq1, seq2, seqName1, seqName2);

    CELL (*table)[*m][*n] = malloc(sizeof *table);
    
    initializeTable(*m, *n, table, gapExt, gap); 
    
    if(strcmp(flag, "0") == 0) {
        score = fillTableGlobal(*m, *n, table, match, mismatch, gapExt, gap, seq1, seq2); 
        retraceGlobal(*m, *n, table, match, mismatch, gapExt, gap, numMatch, numMis, numGapExt, numGap, seq1, seq2, optSeq1, optSeq2);
    } else {
        score = fillTableLocal(*m, *n, table, match, mismatch, gapExt, gap, seq1, seq2); 
        retraceLocal(*m, *n, table, match, mismatch, gapExt, gap, numMatch, numMis, numGapExt, numGap, seq1, seq2, optSeq1, optSeq2);
    }

    displayOptAlignment(optSeq1, optSeq2, score, match, mismatch, gap, gapExt, seq1, seq2, *numMatch, *numMis, *numGapExt, *numGap, seqName1, seqName2);
}

void displayOptAlignment(char *optSeq1, char *optSeq2, int score, int match, int mismatch, int gap, int gapExt, char *seq1, char *seq2, int numMatch, int numMis, int numGapExt, int numGap, char *name1, char *name2) {
    FILE *output;
    output = fopen("report.txt", "w");
    if(output == NULL) printf("The file could not be opened.");

    fprintf(output, "INPUT:\n");
    fprintf(output, "********\n\n");

    fprintf(output, ">%s", name1);

    int i = 0, j = 0, k = 0;
    while(i < strlen(seq1)) {
        while(j < 60) {
            if(i < strlen(seq1)) {
                fprintf(output, "%c", seq1[i]);
            }
            i++;
            j++;
        }
        fprintf(output, "%c", '\n');
        j = 0;
    }

    fprintf(output, "\n>%s", name2);

    i = 0, j = 0;
    while(i < strlen(seq2)) {
        while(j < 60) {
            if(i < strlen(seq2)) {
                fprintf(output, "%c", seq2[i]);
            }
            i++;
            j++;
        }
        fprintf(output, "%c", '\n');
        j = 0;
    }

    fprintf(output, "\nOUTPUT:\n");
    fprintf(output, "********\n\n");
    fprintf(output, "Scores: match = %d mismatch = %d h = %d g = %d\n", match, mismatch, gapExt, gap);
    fprintf(output, "Sequence 1 = \"%s\", length = %lu\n", name1, strlen(seq1));
    fprintf(output, "Sequence 1 = \"%s\", length = %lu\n", name2, strlen(seq2));

    i = 0, j = 0, k = 0;
    printf("%d\n", strlen(optSeq1));
    while(i < strlen(optSeq1)) {
        fprintf(output, "S1  %d  ", i+1);
        k = i;
        while(j < 60) {
            if(i < strlen(optSeq1)) {
                fprintf(output, "%c", *(optSeq1 + i));
            }
            i++;
            j++;
        }
        fprintf(output, "  %d\n", i+1);
        fprintf(output, "S2  %d  ", i+1);
        j = 0;
        while(j < 60) {
            if(i < strlen(optSeq2)) {
                fprintf(output, "%c", optSeq2[k]);
            }
            i++;
            k++;
            j++;
        }
        fprintf(output, "  %d\n", k+1);
    }
}

int getNext(int max, int s, int d, int i) {
    if(max == s) return 1;
    if(max == d) return 2;
    else return 3;
}

void retraceGlobal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2) {
    int i = m - 1, j = n - 1, substitution = 0, next = 0;

    int maximum = max((*table)[i][j].Sscore, (*table)[i][j].Dscore, (*table)[i][j].Iscore);
    next = getNext(maximum, (*table)[i][j].Sscore, (*table)[i][j].Dscore, (*table)[i][j].Iscore);

    if(next == 1) {
        substitution = sub(i, j, match, mismatch, seq1, seq2);
        if(substitution == match) (*numMatch)++;
        else (*numMis)++;
    }
    if(next == 2 || next == 3) {
        (*numGap)++, (*numGapExt)++;
    }
    int k = 0;
    while(i != 0 || j != 0) {
        if(next == 1) {
            char c = seq1[i];
            //printf("%c", seq1[i]);
            //printf("%d", k);
            *(optSeq1 + k) = seq1[i-1];
            *(optSeq2 + k) = seq2[j];
            i -= 1, j -= 1;
            if(i == 0 || j == 0) break;
            maximum = max((*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore);
            next = getNext(maximum, (*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore);
            if(next == 2 || next == 3) (*numGap)++, (*numGapExt)++;
            //printf("%s", optSeq1);
        } else if(next == 2) {
            *(optSeq1 + k) = seq1[i];
            *(optSeq2 + k) = '_';
            i -= 1;
            maximum = max((*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap);
            next = getNext(maximum, (*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap);
            if(next == 2) (*numGap)++;
            if(next == 3) (*numGap)++, (*numGapExt)++;
        } else {
            *(optSeq1 + k) = '_';
            *(optSeq2 + k) = seq2[j];
            j -= 1;
            maximum = max((*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
            next = getNext(maximum, (*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
            if(next == 3) (*numGap)++;
            if(next == 2) (*numGap)++, (*numGapExt)++;
        }
        k++;
    }
    // printf("%d %d %d %d\n", *numMatch, *numMis, *numGap, *numGapExt);
    // printf("%d\n", (*numMatch)*match + (*numMis)*mismatch + (*numGap)*gap + (*numGapExt)*gapExt);
    //printf("%s\n\n\n%s", optSeq1, optSeq2);
    printf("%s", optSeq1);
    //for(int i = 0; i<k-1; i++) printf("%c", *(optSeq2 + i));
}

void retraceLocal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, int *numMatch, int *numMis, int *numGapExt, int *numGap, char *seq1, char *seq2, char *optSeq1, char *optSeq2) {
    int i = m - 1, j = n - 1, k = 0, substitution = 0, next = 0;

    int maximum = max((*table)[i][j].Sscore, (*table)[i][j].Dscore, (*table)[i][j].Iscore);
    next = getNext(maximum, (*table)[i][j].Sscore, (*table)[i][j].Dscore, (*table)[i][j].Iscore);

    if(next == 1) {
        substitution = sub(i, j, match, mismatch, seq1, seq2);
        if(substitution == match) (*numMatch)++;
        else (*numMis)++;
    }
    if(next == 2 || next == 3) {
        (*numGap)++, (*numGapExt)++;
    }
    
    while(i != 0 || j != 0) {
        if(next == 1) {
            *(optSeq1 + k) = *(seq1 + i), *(optSeq2 + k) = seq2[j];
            i -= 1, j -= 1;
            if(i == 0 || j == 0) break;
            maximum = max((*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore);
            next = getNext(maximum, (*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore);
            if(next == 2 || next == 3) (*numGap)++, (*numGapExt)++;
        } else if(next == 2) {
            *(optSeq1 + k) = seq1[i], *(optSeq2 + k) = '_';
            i -= 1;
            maximum = max((*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap);
            next = getNext(maximum, (*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap);
            if(next == 2) (*numGap)++;
            if(next == 3) (*numGap)++, (*numGapExt)++;
        } else {
            *(optSeq1 + k) = '_', *(optSeq2 + k) = seq2[j];
            j -= 1;
            maximum = max((*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
            next = getNext(maximum, (*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
            if(next == 3) (*numGap)++;
            if(next == 2) (*numGap)++, (*numGapExt)++;
        }
        k++;
    }
    // printf("%d %d %d %d\n", *numMatch, *numMis, *numGap, *numGapExt);
    // printf("%d\n", (*numMatch)*match + (*numMis)*mismatch + (*numGap)*gap + (*numGapExt)*gapExt);
    //printf("%s", optSeq1);
    //for(int i = 0; i<k-1; i++) printf("%c", *(optSeq2 + i));
}

int fillTableLocal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2) {
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            (*table)[i][j].Sscore = max((*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore) + sub(i, j, match, mismatch, seq1, seq2);
            (*table)[i][j].Dscore = max((*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap); 
            (*table)[i][j].Iscore = max((*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
        }
    }

    int score = max((*table)[m-1][n-1].Sscore, (*table)[m-1][n-1].Dscore, (*table)[m-1][n-1].Iscore);

    return score;
}

int fillTableGlobal(int m, int n, CELL (*table)[][n], int match, int mismatch, int gapExt, int gap, char *seq1, char *seq2) {
    for(int i = 1; i < m; i++) {
        for(int j = 1; j < n; j++) {
            (*table)[i][j].Sscore = max((*table)[i-1][j-1].Sscore, (*table)[i-1][j-1].Dscore, (*table)[i-1][j-1].Iscore) + sub(i, j, match, mismatch, seq1, seq2);
            (*table)[i][j].Dscore = max((*table)[i-1][j].Sscore + gapExt + gap, (*table)[i-1][j].Dscore + gap, (*table)[i-1][j].Iscore + gapExt + gap); 
            (*table)[i][j].Iscore = max((*table)[i][j-1].Sscore + gapExt + gap, (*table)[i][j-1].Dscore + gapExt + gap, (*table)[i][j-1].Iscore + gap);
        }
    }

    int score = max((*table)[m-1][n-1].Sscore, (*table)[m-1][n-1].Dscore, (*table)[m-1][n-1].Iscore);

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

int maxLocal(int a, int b, int c) {
    if(a >= b && a >= c && a >= 0) return a;
    else if(b >= a && b >= c && b >= 0) return b;
    else if(c >= a && c >= b && c >= 0) return c;

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

    return penalties;
}

void initializeTable(int m, int n, CELL (*table)[][n], int h, int g) {
    CELL cell;
    (*table)[0][0] = cell;
    (*table)[0][0].Sscore = 0, (*table)[0][0].Dscore = 0, (*table)[0][0].Iscore = 0;
    for(int i = 1; i < m; i++) (*table)[i][0].Sscore = -10000, (*table)[i][0].Dscore = h + i*g, (*table)[i][0].Iscore = -10000;
    for(int j = 1; j < n; j++) (*table)[0][j].Sscore = -10000, (*table)[0][j].Dscore = -10000, (*table)[0][j].Iscore = h + j*g;
}

void calculateStringLength(char* inputSequences, int* m, int* n, char *seq1, char *seq2, char *seqName1, char *seqName2) {
    char c;
    int markerCount = 0, tempM = 0, tempN = 0, size = 10000, i = 0, j = 0, k = 0;
    char tempStr[1];

    FILE *inSequences = fopen(inputSequences, "r");

    while((c = fgetc(inSequences)) != EOF) {
        if(c == '>' && markerCount == 0) {
            markerCount++;
        }
        if(markerCount == 1) {
            if(j != 0) {
                *(seqName1 + (j-1)) = c;
            }
            j++;
        }
        if(c == '\n' && markerCount == 1) {
            markerCount++;
        }
        if(isBase(c) == 1 && markerCount == 2) {
            tempM++;
            *(seq1 + i) = c;
            i++;
        } 
        if(c == '>' && markerCount == 2) { 
            markerCount++;
            j = 0; 
        }
        if(markerCount == 3) {
            if(j != 0)
                *(seqName2 + (j - 1)) = c;
            j++;
        }
        if(c == '\n' && markerCount == 3) {
            markerCount++;
        }
        if(isBase(c) == 1 && markerCount == 4) {
            tempN++;
            *(seq2 + k) = c;
            k++;
        }
    }
    //*(seq2 + i) = '\0';

    *m = tempM + 1;
    *n = tempN + 1;
}

int isBase(char c) {
    if(c == 'A' || c =='G' || c == 'T' || c == 'C') return 1;
    else return 0;
}