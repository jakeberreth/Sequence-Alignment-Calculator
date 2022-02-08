#include "seqAlign.h"

int main(int argc, char* argv[]) {
    printf("******************************\n");
    printf("Sequence Alignment Calculator\n");
    printf("******************************\n");

    //get names of files from command line
    char* inputSequences = argv[1];
    char* alignmentFlag = argv[2];
    char* parameters = argv[3];

    //open FASTA format file with s1 and s2
    appWrapper(inputSequences, alignmentFlag, parameters);




}