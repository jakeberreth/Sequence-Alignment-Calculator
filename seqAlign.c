#include "seqAlign.h"

void calculateS1Length(char* inputSequences, int* m, int* n) {
    char c;

    FILE *pInputSequencesFile = fopen(inputSequences, "r");
    
    while((c = fgetc(pInputSequencesFile)) != EOF) {
        printf("%c", c);
    }
}