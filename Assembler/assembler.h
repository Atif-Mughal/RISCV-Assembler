/*
 * RISC-V Assembler Header
 * Created by: Muhammad Atif
 * Date: September 11, 2024
 *
 * This header file defines the constants, structures, and function prototypes used 
 * in the RISC-V assembler. The assembler processes input assembly code and converts 
 * it into machine code in hexadecimal or binary format. The file also includes definitions 
 * for label management and helper functions used during the two-pass assembly process.
 */

#ifndef ASSEMBLER_H  // Include guard to prevent multiple inclusion of this header file
#define ASSEMBLER_H

#include <stdio.h>   // Standard input/output library for file handling
#include <string.h>  // String manipulation functions
#include <stdlib.h>  // Standard library for memory management and exit codes
#include <ctype.h>   // Character type functions 
#include <stdbool.h>

#define MAX_INSTRUCTIONS 100  // Maximum number of instructions the assembler can process
#define MAX_LINE_LENGTH 256   // Maximum length of a single line in the assembly file

// External variables to keep track of the number of labels and instructions during the assembly
extern int labelCount;        // Counts the number of labels in the assembly file
extern int instruction_count; // Tracks the number of instructions processed in the first pass
extern int instruction_count2; // Tracks the number of instructions processed in the second pass

// Structure to hold label names and their corresponding memory addresses
typedef struct {
    char label[MAX_LINE_LENGTH]; // The label name (symbol)
    int address;                 // The address associated with the label
} Label;

// Function declarations used in the assembler

// Adds a new label to the symbol table with its corresponding address
void add_label(const char *label, int address);

// Finds the memory address of a label by searching the symbol table
int find_label_address(const char *label);

// Removes the colon at the end of labels in assembly code (e.g., "loop:" becomes "loop")
void remove_colon(char *str);

// Converts a register name (e.g., "x1") into its corresponding register number
int get_register_number(const char *reg);

// Replaces commas in assembly code with spaces for easier tokenization
void replaceCommas(char *str);

// First pass through the assembly code to identify labels and symbols
void first_pass(char *instruction);

// Assembles an individual instruction into its corresponding machine code
unsigned int assemble_instruction(char *instruction);

// Outputs the machine code in hexadecimal format to the output file
void output_hex(unsigned int code, FILE *output_file);

// Outputs the machine code in binary format to the output file
void output_binary(unsigned int code, FILE *output_file);

void removeComment(char* str);

void splitString(char* str, char* before, char* after);

#endif // End of the include guard for ASSEMBLER_H
