# CS-40-HW1
Darius-Stefan Iavorschi (diavor01)
Alijah Jackson (ajacks11)
->

filesofpix Assignment


Problem Statement: 
  Extracting original image data from the corrupted files by identifying and separating the legitimate rows.

Use Cases: 
  Restore a corrupted PGM file by identifying valid image rows and discarding injected ones.
  Validate input files for structural integrity.
  Efficiently read files containing arbitrary-length lines

Assumptions:
  Input - 
    Injected rows and original rows can be differentiated by their infusion sequence of non-digit bytes.
    Images are atleast a 2x2 grid
    The input can come from both stdin or from a file
  Corruption -
    The corrupted images originate from "plain" PGM files(P2)
    Each original row is terminated by a newline character (\n) that remains unchanged.
    Corruption does not alter the row order but adds extra rows
  
Constraints:
  The restoration program should process even large images efficiently under 20 seconds
  Hanson’s data structures are available and must be used when appropriate(except for arrays)

Hanson's data abstractions used and their purposes:
1) Sequences
   Purposes: -> they can store data sequentially
             -> they can resize dynamically
2) Atoms
   Purpose: -> they allow for atom equality, which is essential for comparing the injected sequences
   
File Descriptions:
-> main.c: checks the type of input received and passes it on to the restoration function
-> readaline.c and its header file: reads a corrupted line of text at a time. A line can contain any ASCII characters (including NULL) and is guaranteed to end with "\n"
-> functions.c and its header file: represent helper functions for the restoration function
-> restoration.c and its header file: the file responsible for the restoration process from a corrupted plain file to a p5 pgm file
-> conversion.c and its header file: helper file that includes functions to convert the sequence of numbers to binary and write both the contents and header to a p5 format pgm file

Architecture:
For readaline, we will be using a pointer to char called buffer to store the characters. Buffer stops receiving input when finding the endline character. 

For the restoration part, we need 2 data structures: a matrix in numbers matrix_nums (a pointer to pointer to integer), which will represent the final, restored p2 pgm file, and a pointer to char, which will store Hanson's atoms. We prefer this implementation over a pointer to a pointer to char because Atoms allow pointer equality. Since the lines from the original file have been injected with the same sequence of characters, all atoms we store must be equal.

We will use the readaline function to read the corrupted file line by line. The input will be separated into 2 parts: matrix[i] (a pointer to int, where we store the numbers from the input line) and atoms[i] (where we store anything else). After every line iteration, we check if we found any 2 equal atoms (which should be easy considering atoms allow pointer equality). We repeat the process until we find 2 equal atoms, atoms[x] and atoms[y]. We delete all the lines from both data structures besides x and y.

Now that we know the correct sequence of characters with which the original lines have been injected, we simply need to check every corrupted line individually and store the numbers in matrix_nums if the sequence matches. 




Data structures used:
Seq_T atom_sequence: Represents a sequence of atoms. The first step is establishing the correct sequence of characters the original lines have been injected with. We add a new atom to the sequence until we find identical atoms.
Seq_T newRow: representing a sequence of numbers...
Seq_T: representing a sequence of newRows



Implementations:
  readaline- The readaline function reads characters one by one from the input file and stores them in a buffer (the buffer is allocated dynamically so that its size can change). The loop stops if it encounters the endline character or the end of the file. Then *datapp gets updated and the size of the buffer is returned. If the size is 0 (meaning we no characters have been read, and thus we reached the end of the line), the buffer is freed and the function returns 0.

  

  Restoration-
  
  Conversion-
    opening the input and output file and checking if the input file is in the correct "P2" format. 
    skip any comments present in the file and read the image dimensions and maximum pixel value.
    write the appropriate "P5" header to the output file 
    read each pixel value from the P2 file and write it in binary format to the P5 file.
    closes the file

  Tests:
   - The readaline function will be tested using diff for each corrupted file provided. Additional files will be provided to test each expected error. After that, we will try using larger pgm files (width>1000).
   - 

  
  



