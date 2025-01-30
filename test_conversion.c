#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "readaline.h"
#include "seq.h"
#include "conversion.h"

// Define ssize_t if it's not already defined
#ifndef _SSIZE_T_DEFINED
typedef long ssize_t;
#define _SSIZE_T_DEFINED
#endif

// Function to print a sequence of characters (for testing purposes)
void print_sequence(Seq_T sequence) {
    for (int i = 0; i < Seq_length(sequence); i++) {
        int value = (int)(intptr_t)Seq_get(sequence, i);  // Correctly cast the value
        printf("%d ", value);
    }
    printf("\n");
}

void print_sequence_simple(Seq_T sequence) {
    for (int i = 0; i < Seq_length(sequence); i++) {
        int value = (int)(intptr_t)Seq_get(sequence, i);  // Correctly cast the value

        if(value == 255){
            printf("O");
        }else{
            printf("X");
        }
    }
    printf("\n");
}

// Custom implementation of getline
ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    size_t pos;
    int c;

    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }
    c = getc(stream);
    if (c == EOF) {
        return -1;
    }
    if (*lineptr == NULL) {
        *lineptr = malloc(128);
        if (*lineptr == NULL) {
            return -1;
        }
        *n = 128;
    }
    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *n) {
            size_t new_size = *n + (*n >> 1);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*lineptr, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *n = new_size;
            *lineptr = new_ptr;
        }
        (*lineptr)[pos++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }
    (*lineptr)[pos] = '\0';
    return pos;
}

// Function to read numbers from a file and output them into a string
size_t readNumbers(FILE *inputfd, char **datapp) {
    size_t size = 0;
    ssize_t len = getline(datapp, &size, inputfd);
    if (len == -1) return 0;  // End of file or error

    // Removing non-digit characters
    size_t j = 0;
    for (size_t i = 0; i < (size_t)len; i++) {  // Cast len to size_t
        if (isdigit((*datapp)[i]) || (*datapp)[i] == ' ') {
            (*datapp)[j++] = (*datapp)[i];
        }
    }
    (*datapp)[j] = '\0';  // Null-terminate the string
    return j;
}

// Function to test conversion and write to binary file
void conversion_test() {
    FILE *file = fopen("sunny-pixels.txt", "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    
    char *line = NULL;
    size_t len;
    FILE *out = fopen("sunny-test_new.pgm", "wb");
    if (!out) {
        perror("Failed to open output file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    int count = 0;
    while ((len = readaline(file, &line)) > 0) {
        Seq_T line_sequence = convert_line_to_sequence(line);
        if (line_sequence == NULL) {
            free(line);
            continue;
        }
        
        // Write the sequence to the binary file
        writeRowToBinary(line_sequence, out);
        
        if (count == 100) {
            printf("LINE\n");
            printf("%s\n", line);
            printf("SEQUENCE\n");
            print_sequence(line_sequence);
        }
    
        count++;
        Seq_free(&line_sequence);
        free(line);
        line = NULL;  // Reset line to NULL for getline
    }
    fclose(file);
    fclose(out);
    
    writeP5Header("sunny-test_new.pgm", 150, 150);
}

int main() {
    conversion_test();
    return 0;
}