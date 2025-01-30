#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "seq.h"
#include "assert.h"

// Function to write P5 header
void writeP5Header(const char *filename, int width, int height) {
    assert(width > 0);
    assert(height > 0);
    assert(filename != NULL);

    FILE *file = fopen(filename, "rb+");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Move the existing contents to make room for the header
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    char *buffer = (char *)malloc(fileSize);
    if (buffer == NULL) {
        perror("Memory allocation error");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);
    fread(buffer, 1, fileSize, file);

    fseek(file, 0, SEEK_SET);

    fprintf(file, "P5\n%d %d\n255\n", width, height);
    fwrite(buffer, 1, fileSize, file);

    free(buffer);
    fclose(file);
}

// Function to write a row of pixels to binary file
void writeRowToBinary(Seq_T seq, FILE *out) {
    if (seq == NULL || out == NULL) {
        fprintf(stderr, "Error: Invalid input to writeRowToBinary\n");
        return;
    }

    int length = Seq_length(seq);
    for (int i = 0; i < length; i++) {
        int value = (int)(intptr_t)Seq_get(seq, i);

        unsigned char byte_value = (unsigned char)value;
        fputc(byte_value, out);
    }
}

// Function to trim leading and trailing whitespace from a string
void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) { // All spaces?
        return;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    end[1] = '\0';
}

// Function to convert a line into a sequence of numbers
Seq_T convert_line_to_sequence(const char *line) {
    if (line == NULL) {
        return NULL;
    }

    Seq_T sequence = Seq_new(0);
    if (sequence == NULL) {
        return NULL;
    }

    // Calculate length and allocate memory for copy
    size_t len = strlen(line) + 1;
    char *copy = malloc(len);
    if (copy == NULL) {
        Seq_free(&sequence);
        return NULL;
    }

    // Copy the input line to avoid modifying the original string
    strcpy(copy, line);

    // Use sscanf to read numbers directly from the string
    char *ptr = copy;
    int number;
    while (sscanf(ptr, "%d", &number) == 1) {
        Seq_addhi(sequence, (void *)(intptr_t)number);

        // Move ptr to the next non-whitespace character after the last number
        while (*ptr && !isspace((unsigned char)*ptr)) ptr++;
        while (*ptr && isspace((unsigned char)*ptr)) ptr++;
    }

    // Free the copied string memory
    free(copy);

    return sequence;
}






