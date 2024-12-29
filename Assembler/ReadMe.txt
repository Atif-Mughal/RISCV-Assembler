# Assembly File Processor

## Overview
This project automates the generation of machine code from assembly files. Users can simply place their assembly files in the designated input folder, and upon running the script, the corresponding machine code will be generated and saved in the output folder.

## Project Structure
/AssemblyFileProcessor
│
├── input_files/ # Directory containing the assembly source files (*.s)
│ ├── [your assembly files] # Add your assembly files here
│
├── output_machine_code/ # Directory where the generated machine code will be stored
│
├── TestingApplication/ # Directory for additional tests or related files
│
├── assemble_files.py # Python script to manage assembly file processing
│
├── assembler # Assembler executable
│
├── assembler.c # C source file for assembler
│
├── assembler.h # Header file for assembler
│
├── assembler_main.c # Main C source file for assembler
│
├── check.py # Python script for any additional checks (if applicable)
│
├── Makefile # Makefile for building the C components
│
├── ReadMe.txt # This README file
│
└── test_assembler_files.py # Python script for testing the assembler

## Requirements
- Python 3.x
- An assembler executable (ensure it's available and properly configured).
- A C compiler (if you need to build the C components).

## How to Run the Project
1. **Prepare Input Directory**: Place all your assembly files (*.s) in the `input_files` directory.
2. **Ensure Assembler**: Make sure the assembler executable is accessible in the path where the script runs.
3. **Run the Main Script**:
   Open a terminal, navigate to the project directory, and run the following command:
   ```bash
   make all
   python3 assemble_files.py