/* 
 * RISC-V Assembler
 * Created by: Muhammad Atif
 * Date: September 11, 2024
 *
 * This file serves as the main entry point for a RISC-V assembler written in C.
 * It processes an input assembly file and generates the corresponding machine code
 * in either hexadecimal or binary format. The assembler reads the input file in two passes:
 *   1. The first pass handles label parsing and symbol resolution.
 *   2. The second pass translates assembly instructions into machine code.
 * Usage: ./assembler_main <input_file> <output_file> <-h|-b>
 *   -h: Outputs the machine code in hexadecimal format.
 *   -b: Outputs the machine code in binary format.
 */

#include "assembler.h"  // Include the header file that contains function declarations and constants

int main(int argc, char *argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc < 4) {
        // Print usage instructions if incorrect arguments are provided
        fprintf(stderr, "Usage: %s <input_file> <output_file> <-h|-b>\n", argv[0]);
        return 1;
    }

    // Store the input file name, output file name, and the format option (hex or binary)
    const char *input_file_name = argv[1];
    const char *output_file_name = argv[2];

    // Open the input file for reading
    FILE *input_file = fopen(input_file_name, "r");
    if (!input_file) {
        // Display an error message if the input file cannot be opened
        perror("Error opening input file");
        return 1;
    }

    // Open the output file for writing
    FILE *output_file = fopen(output_file_name, "w");
    if (!output_file) {
        // Display an error message if the output file cannot be opened
        perror("Error opening output file");
        fclose(input_file);  // Close the input file before exiting
        return 1;
    }
    bool isHex = (strcmp(argv[3], "-h") == 0);
    bool isBin = (strcmp(argv[3], "-b") == 0);
    if (!isHex & !isBin ) {
        fprintf(stderr, "Invalid Output flag. Usage: %s <input_file> <output_file> <-h|-b>\n", argv[0]);
        return 1;
    }

    char line[MAX_LINE_LENGTH];  // Buffer to hold each line from the input file
    // First pass: read each line, replacing commas and handling label definitions
    while (fgets(line, sizeof(line), input_file)) {
        removeComment(line);
        replaceCommas(line);   // Replace commas with spaces for easier processing
        first_pass(line);      // Handle label resolution and symbol table population
    }

    // Close the input and output files after the first pass
    fclose(input_file);
    fclose(output_file);

    // Reopen the input file for the second pass
    input_file = fopen(input_file_name, "r");
    if (!input_file) {
        perror("Error opening input file");
        return 1;
    }

    // Reopen the output file for writing the machine code
    output_file = fopen(output_file_name, "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);  // Close the input file before exiting
        return 1;
    }

    // Second pass: read each line again, assemble instructions into machine code
    while (fgets(line, sizeof(line), input_file)) {
        removeComment(line);
        replaceCommas(line);   // Ensure commas are replaced again
        unsigned int machine_code = assemble_instruction(line);  // Assemble the instruction to machine code

        // Output the machine code in hex or binary format based on the user's choice
        if (isHex & machine_code) {
            output_hex(machine_code, output_file);  // Output the machine code in hexadecimal format
        } else if (isBin & machine_code) {
            output_binary(machine_code, output_file);  // Output the machine code in binary format
        }
    }

    // Close the input and output files after the second pass
    fclose(input_file);
    fclose(output_file);

    return 0;  // Return success if everything executed correctly
}
