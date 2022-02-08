#include "seqAlign.h"
#include <math.h>

void appWrapper(char* inputSequences, char* flag, char* parameters) {
    int i = 5;
    int* m = &i, *n = &i;

    int* penalties = getParameters(parameters);

    int match = *penalties, mismatch = *(penalties + 1), gapExt = *(penalties + 2), gap = *(penalties + 3);
    int pen[4] = {match, mismatch, gapExt, gap};

    calculateStringLength(inputSequences, m, n);

    CELL (*table)[*m][*n] = malloc(sizeof *table);
    //struct of that size cannot be declared

    initializeTable(*m, *n, table, pen[2], pen[3]); 
    // CELL cell;
    // cell.Dscore = 18;
    // (*table)[0][0] = cell;
    printf("%d", (*table)[0][0].Dscore);

    //Pass by reference simply by adding &. It doesn't need to be a pointer type ahead of time.
    //If you pass a variable that was declared as a pointer by reference, then you can change the actual
    //value of the pointer, which is the address that it points to
    //you can totally change the value of a pointer by just passing the pointer to a function by copy
    //CELL* cell = (CELL*)malloc(sizeof(CELL));
    //table[0][0] = cell;
    //must dynamically allocate if using variable size

    //table[0][0]->Sscore = 0, table[0][0]->Dscore = 0, table[0][0]->Iscore = 0;
    //printf("%d\n", m);
    // for(int i = 1; i < *m; i++) {
    //     table[i][0].Sscore = -10000, table[i][0].Dscore = h + i*g, table[i][0].Iscore = -10000;
    //         printf("%d\n", *m);
    // }
    // for(int j = 1; j < n; j++) {
    //     table[0][j].Sscore = -10000, table[j][0].Dscore = -10000, table[j][0].Iscore = h + j*g;
    // }
    // printf("%d", table[0][0].Dscore);
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

    //printf("match = %d\nmismatch = %d\ngap extension = %d\ngap = %d\n", penalties[0], penalties[1], penalties[2], penalties[3]);

    return penalties;
}

void initializeTable(int m, int n, CELL (*table)[][n], int h, int g) {
    CELL cell;
    cell.Dscore = 18;
    (*table)[0][0] = cell;
    //table[0][0]->Sscore = 0, table[0][0]->Dscore = 0, table[0][0]->Iscore = 0;
    //printf("%d\n", m);
    // for(int i = 1; i < *m; i++) {
    //     table[i][0].Sscore = -10000, table[i][0].Dscore = h + i*g, table[i][0].Iscore = -10000;
    //         printf("%d\n", *m);
    // }
    // for(int j = 1; j < n; j++) {
    //     table[0][j].Sscore = -10000, table[j][0].Dscore = -10000, table[j][0].Iscore = h + j*g;
    // }
}

void calculateStringLength(char* inputSequences, int* m, int* n) {
    char c;
    int markerCount = 0;
    int tempM = 0, tempN = 0;

    FILE *inSequences = fopen(inputSequences, "r");
    
    while((c = fgetc(inSequences)) != EOF) {
        if(isBase(c) == 1) tempM++;
        if(c == '>' && markerCount == 0) markerCount++;
        if(isBase(c) == 1 && markerCount == 1) tempN++;
    }

    *m = tempM;
    *n = tempN;

    printf("m = %d\n", *m);
    printf("n = %d\n", *n);
}

int isBase(char c) {
    if(c == 'A' || c =='G' || c == 'T' || c == 'A') return 1;
    else return 0;
}