#ifndef CONVERSION_H
#define CONVERSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void writeP5Header(const char *filename, int width, int height);

void writeRowToBinary(Seq_T pixelRow, FILE *file);

void trim_whitespace(char *str);

Seq_T convert_line_to_sequence(const char *line);

#endif