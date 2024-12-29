🔭 Project Repository: RISC-V Assembler Repository

📫 Contact Me: atifmughal62843@gmail.com

🌐 LinkedIn: Muhammad Atif

Muhammad Atif (Developer)

LinkedIn | Email

Converts RISC-V assembly instructions to machine code.

Supports pseudo-instructions by translating them into valid instructions.

Outputs machine code in binary and hexadecimal formats.

Robust error handling and detailed error messages for debugging.

Automated testing with Python scripts for efficient validation.

Prerequisites

C Compiler: GCC

Python: For running testing scripts

Compiling the Assembler

Use the provided Makefile to compile the assembler:

make

Running the Assembler

To convert an assembly file into machine code, use the following command:

./assembler <input_file.s> <output_file> -h

<input_file.s>: The assembly file.

<output_file>: The output file for machine code.

Automating with Python Scripts

test_assembler_files.py

This script runs the assembler on multiple assembly files and compares the generated machine code with expected outputs.

Place the assembly files in the TestingApplication folder.

Run the script:

python3 test_assembler_files.py

assemble_files.py

This script automates machine code generation:

Place the assembly files in the input_files folder.

Run the script:

python3 assemble_files.py

The generated machine code files will be saved in the output_machine_code folder.




This project is a RISC-V Assembler developed under the supervision of Mr. Umer Farooq, Ms. Hira Sohail, and Mr. Muhammad Bilal. It converts assembly language instructions into machine code, supporting standard RISC-V instructions and pseudo-instructions, while ensuring robust error handling and output in multiple formats.

We welcome contributions to enhance this project! Here's how you can contribute:

Fork the repository.

Create a new branch for your feature or bugfix.

Commit your changes and push them to your fork.

Open a pull request.

This project is open-source and contributions are welcome.

