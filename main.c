#include "seqAlign.h"

int main(int argc, char* argv[]) {
    //get names of files from command line
    char* inputSequences = argv[1];
    char* alignmentFlag = argv[2];
    char* parameters = argv[3];

    //open FASTA format file with s1 and s2


    //find the length of both strings
    int* m, *n;
    calculateS1Length(inputSequences, m, n);

    //create Dynamic Programming Table
    CELL table[100];

    //initialize first row and column of table




}