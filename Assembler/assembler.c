/*
 * RISC-V Assembler Implementation
 * Created by: Muhammad Atif
 * Date: September 11, 2024
 *
 * This file contains the implementation of the RISC-V assembler written in C. 
 * The assembler translates assembly language instructions into machine code 
 * (either in hexadecimal or binary format). This file manages label handling, 
 * instruction parsing, and machine code generation. It uses a two-pass approach: 
 * first to handle labels and second to generate the final machine code.
 */

#include "assembler.h"

// Global label table to store labels and their corresponding memory addresses
Label labelTable[MAX_INSTRUCTIONS];
int labelCount = 0;  // Keeps track of the number of labels

/*
 * Adds a label to the label table with its corresponding address.
 * This function is called during the first pass when a label is encountered.
 * 
 * @param label: The label name to be added.
 * @param address: The memory address associated with the label.
 */
void add_label(const char *label, int address) {
    strcpy(labelTable[labelCount].label, label);  // Copy the label name to the label table
    labelTable[labelCount].address = address;     // Store the corresponding address
    labelCount++;  // Increment the label count after adding a new label
}

/*
 * Searches for a label in the label table and returns its address.
 * This function is used to resolve label references during the second pass.
 *
 * @param label: The label name to search for.
 * @return: The memory address of the label, or -1 if the label is not found.
 */
int find_label_address(const char *label) {
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labelTable[i].label, label) == 0) {
            return labelTable[i].address;  // Return the address if the label matches
            
        }
    }
    return -1;  // Label not found, return -1
}

/*
 * Removes the colon (':') from a label in assembly code.
 * Labels in assembly often have a trailing colon (e.g., "loop:").
 *
 * @param str: The label string to modify.
 */
void remove_colon(char *str) {
    int len = strlen(str);  // Get the length of the string

    // If the last character is a colon, replace it with a null terminator
    if (len > 0 && str[len - 1] == ':') {
        str[len - 1] = '\0';  // Remove the colon
    }
}

// Variables to track the number of instructions processed in two passes
int instruction_count =  0;   // Instruction count for the first pass
int instruction_count2 = 0;   // Instruction count for the second pass

/*
 * Converts a register name (e.g., "x1", "a0") into the corresponding register number.
 * Handles different register names such as "x0", "sp", "ra", etc.
 *
 * @param reg: The register name as a string.
 * @return: The corresponding register number (0-31), or -1 if the register is invalid.
 */
int get_register_number(const char *reg) {
    if ((strcmp(reg, "x0") == 0) || (strcmp(reg, "zero") == 0)) 
        return 0;
    if ((strcmp(reg, "x1") == 0)  || (strcmp(reg, "ra") == 0)) 
        return 1;
    if ((strcmp(reg, "x2") == 0)  || (strcmp(reg, "sp") == 0)) 
        return 2;
    if ((strcmp(reg, "x3") == 0)  || (strcmp(reg, "gp") == 0)) 
        return 3;
    if ((strcmp(reg, "x4") == 0)  || (strcmp(reg, "tp") == 0)) 
        return 4;
    if ((strcmp(reg, "x5") == 0) || (strcmp(reg, "t0") == 0)) 
        return 5;
    if ((strcmp(reg, "x6") == 0) || (strcmp(reg, "t1") == 0)) 
        return 6;
    if ((strcmp(reg, "x7") == 0) || (strcmp(reg, "t2") == 0)) 
        return 7;
    if ((strcmp(reg, "x8") == 0) || (strcmp(reg, "s0") == 0)) 
        return 8;
    if ((strcmp(reg, "x9") == 0) || (strcmp(reg, "s1") == 0)) 
        return 9;
    if ((strcmp(reg, "x10") == 0) || (strcmp(reg, "a0") == 0)) 
        return 10;
    if ((strcmp(reg, "x11") == 0) || (strcmp(reg, "a1") == 0)) 
        return 11;
    if ((strcmp(reg, "x12") == 0) || (strcmp(reg, "a2") == 0)) 
        return 12;
    if ((strcmp(reg, "x13") == 0) || (strcmp(reg, "a3") == 0)) 
        return 13;
    if ((strcmp(reg, "x14") == 0) || (strcmp(reg, "a4") == 0)) 
        return 14;
    if ((strcmp(reg, "x15") == 0) || (strcmp(reg, "a5") == 0)) 
        return 15;
    if ((strcmp(reg, "x16") == 0) || (strcmp(reg, "a6") == 0)) 
        return 16;
    if ((strcmp(reg, "x17") == 0) || (strcmp(reg, "a7") == 0)) 
        return 17;
    if ((strcmp(reg, "x18") == 0) || (strcmp(reg, "s2") == 0)) 
        return 18;
    if ((strcmp(reg, "x19") == 0) || (strcmp(reg, "s3") == 0)) 
        return 19;
    if ((strcmp(reg, "x20") == 0) || (strcmp(reg, "s4") == 0)) 
        return 20;
    if ((strcmp(reg, "x21") == 0) || (strcmp(reg, "s5") == 0)) 
        return 21;
    if ((strcmp(reg, "x22") == 0) || (strcmp(reg, "s6") == 0)) 
        return 22;
    if ((strcmp(reg, "x23") == 0) || (strcmp(reg, "s7") == 0)) 
        return 23;
    if ((strcmp(reg, "x24") == 0) || (strcmp(reg, "s8") == 0)) 
        return 24;
    if ((strcmp(reg, "x25") == 0) || (strcmp(reg, "s9") == 0)) 
        return 25;
    if ((strcmp(reg, "x26") == 0) || (strcmp(reg, "s10") == 0)) 
        return 26;
    if ((strcmp(reg, "x27") == 0) || (strcmp(reg, "s11") == 0)) 
        return 27;
    if ((strcmp(reg, "x28") == 0) || (strcmp(reg, "t3") == 0)) 
        return 28;
    if ((strcmp(reg, "x29") == 0) || (strcmp(reg, "t4") == 0)) 
        return 29;
    if ((strcmp(reg, "x30") == 0) || (strcmp(reg, "t5") == 0)) 
        return 30;
    if ((strcmp(reg, "x31") == 0) || (strcmp(reg, "t6") == 0)) 
        return 31;
    
    // Additional check for "xN" format (e.g., "x1", "x2")
    if (strncmp(reg, "x", 1) == 0) {
        int reg_num = atoi(reg + 1);  // Convert string to integer
        if (reg_num >= 0 && reg_num <= 31) return reg_num;  // Return valid register number
    }
    
    return -1;  // Return -1 if the register is invalid
}

/*
 * Replaces commas in the given instruction string with spaces.
 * This function simplifies tokenizing assembly instructions by ensuring that 
 * registers and arguments are separated by spaces instead of commas.
 *
 * @param str: The instruction string to modify.
 */
void replaceCommas(char *str) {
    int i = 0;
    // Loop through the string and replace commas with spaces
    while (str[i] != '\0') {
        if (str[i] == ',') {
            str[i] = ' ';  // Replace comma with a space
        }
        i++;
    }
}
void removeBracket(char *str) {
    int i = 0;
    // Loop through the string and replace commas with spaces
    while (str[i] != '\0') {
        if (str[i] == ')') {
            str[i] = '\0';  // Replace comma with a space
        }
        if (str[i] == '('){
            str[i] = ' ';
        }
        i++;
    }
}
void separateImmediate(char *str) {
    int i = 0;
    int signal = 0;
    // Loop through the string and replace commas with spaces
    while (str[i] != '\0') {
        if (str[i] == '(') {
            str[i] = '\0';  // Replace comma with a space
            signal = 1;
        }
        if (signal == 1){
            str[i] = '\0';
        }
        
        i++;
    }
}
void separate_rs1(char *str) {
    int i = 0;
    int signal = 0;
    // Loop through the string and replace commas with spaces
    while (str[i] != '\0') {
        if (str[i] == '(' ){
            str[i] = ' ';  // Replace comma with a space
            signal = 1;
        }
        else if (signal == 0){
            str[i] = ' ';
        }
        else if (str[i] == ')'){
            str[i] == '\0';
        }
        
        i++;
    }
}
void removeComment(char* str) {
    // Find the position of '#' in the string
    char* comment = strchr(str, '#');
    
    if (comment != NULL) {
        // Replace the '#' and everything after it with a null terminator
        *comment = '\0';
    }
}

void splitString(char* str, char* before, char* after) {
    // Find the position of ':' in the string
    char* colonPos = strchr(str, ':');

    if (colonPos != NULL) {
        // Copy the part before the ':'
        strncpy(before, str, colonPos - str);
        before[colonPos - str] = '\0';  // Add null terminator

        // Copy the part after the ':'
        strcpy(after, colonPos + 1);
    } else {
        // If ':' is not found, copy the entire string to 'before' and set 'after' as empty
        strcpy(before, str);
        after[0] = '\0';
    }
}

/**
 * Perform the first pass of instruction parsing and label handling.
 * 
 * This function parses the input instruction to check for supported opcodes, 
 * counts instructions, and adds labels for future reference. It assumes the input
 * instruction follows a specific format, typically used in assembly language.
 * During the first pass, labels are recorded, and instruction counts are incremented 
 * based on the opcode type (R-type, I-type, etc.).
 *
 * param: instruction A string representing the current assembly instruction.
 */


void first_pass(char *instruction) {
    // Variables to store different parts of the instruction
    char opcode[20], rd[20], rs1[20], rs2[20],  label[30], label2[30], temp_inst[50];
    int count;

    // Parse the instruction, assuming a fixed format like "opcode rd, rs1, rs2"
    count = sscanf(instruction, "%s %s %s %s", opcode, rd, rs1, rs2);
    splitString(instruction, label, temp_inst);
    if (temp_inst[0] != '\0'){
        strcpy(instruction, temp_inst);
        count = sscanf(instruction, "%s %s %s %s", opcode, rd, rs1, rs2);
        remove_colon(label);  // Remove the colon from the label
        sscanf(label, "%s", label2);
        //printf("%s\n", label2);

        add_label(label2, instruction_count + 1);  // Add label to the table
    }

    
    // Check if it's an R-type instruction (with 4 fields parsed)
    if (count == 4) {
        // Handle specific R-type opcodes and increment the instruction count
        if (strcmp(opcode, "add") == 0 || strcmp(opcode, "sub") == 0 || strcmp(opcode, "or") == 0 ||
            strcmp(opcode, "and") == 0 || strcmp(opcode, "xor") == 0 || strcmp(opcode, "sll") == 0 ||
            strcmp(opcode, "srl") == 0 || strcmp(opcode, "sra") == 0 || strcmp(opcode, "slt") == 0 ||
            strcmp(opcode, "sltu") == 0 ) {
            instruction_count++;  // Increment count for R-type instructions
        }
        // Handle I-type instructions like "addi" or shifts with immediate values
        else if (strcmp(opcode, "addi") == 0 || strcmp(opcode, "slli") == 0 || strcmp(opcode, "slti") == 0 ||
                 strcmp(opcode, "sltiu") == 0 || strcmp(opcode, "xori") == 0 || strcmp(opcode, "srli") == 0 ||
                 strcmp(opcode, "srai") == 0 || strcmp(opcode, "ori") == 0 || strcmp(opcode, "andi") == 0) {
            instruction_count++;
        }
        // Handle special instructions like JALR, branch instructions, etc.
        else if (strcmp(opcode, "jalr") == 0 || strcmp(opcode, "beq") == 0 || strcmp(opcode, "bne") == 0 ||
                 strcmp(opcode, "blt") == 0 || strcmp(opcode, "bge") == 0 || strcmp(opcode, "bltu") == 0 ||
                 strcmp(opcode, "bgeu") == 0 || strcmp(opcode, "bgt") == 0 || strcmp(opcode, "ble") == 0) {
            instruction_count++;
        }
    }
    // Check if it's a memory instruction with 3 fields parsed (e.g., "lw x1, 0(x2)")
    else if (count == 3) {
        if (strcmp(opcode, "lb") == 0 || strcmp(opcode, "lh") == 0 || strcmp(opcode, "lw") == 0 ||
            strcmp(opcode, "lbu") == 0 || strcmp(opcode, "lhu") == 0 || strcmp(opcode, "sb") == 0 ||
            strcmp(opcode, "sh") == 0 || strcmp(opcode, "sw") == 0) {
            instruction_count++;  // Increment count for memory operations
        }
        // Handle special instructions like "auipc" or "lui"
        else if (strcmp(opcode, "auipc") == 0 || strcmp(opcode, "lui") == 0 || strcmp(opcode, "jal") == 0) {
            instruction_count++;
        }
        else if (strcmp(opcode, "mv") == 0 || strcmp(opcode, "li") == 0) {
            instruction_count++;
        }
    }
    //Pseudo Instructions like j and jr
    else if (count == 2){
        if (strcmp(opcode, "j") == 0 || strcmp(opcode, "jr") == 0){
            instruction_count++;
        }
    }
    //Pseudo Instructions like ret
    else if (count == 1){
        if (strcmp(opcode, "ret") ==  0){
            instruction_count++;
        }
    }
    
}

long int convertToDecimal(const char *str) {
    // Check if the string starts with "0x" for hexadecimal numbers
    if (strncmp(str, "0x", 2) == 0) {
        // Convert hexadecimal string to decimal
        return strtol(str, NULL, 16);
    } else {
        // Convert decimal string to decimal
        return strtol(str, NULL, 10);
    }
}


// Function to assemble an RISC-V instruction and convert it into machine code
unsigned int assemble_instruction(char *instruction) {
    char opcode[20], rd[20], rs1[20], rs2[20],  label[30], temp_inst[50]; // Buffers to hold parts of the instruction
    unsigned int machine_code = 0; // Store the final machine code (32 bits)
    int count, address;
    unsigned char rd_num, rs1_num, rs2_num; // Register numbers for rd, rs1, rs2
    signed int imm; // Immediate value for I-type instructions

    // Parse the instruction into opcode, rd, rs1, and rs2 (or imm for I-type)
    count = sscanf(instruction, " %s %s %s %s", opcode, rd, rs1, rs2);

    splitString(instruction, label, temp_inst);
    if (temp_inst[0] != '\0'){
        strcpy(instruction, temp_inst);
        count = sscanf(instruction, " %s %s %s %s", opcode, rd, rs1, rs2);
    }
    // If four components (opcode, rd, rs1, rs2/imm) are found
    if (count == 4) {
        // Handle R-type instruction: ADD
        if (strcmp(opcode, "add") == 0) {
            instruction_count2++; // Update instruction counter
            rd_num = get_register_number(rd); // Get register number for rd
            rs1_num = get_register_number(rs1); // Get register number for rs1
            rs2_num = get_register_number(rs2); // Get register number for rs2
            machine_code |= 0b0110011; // Set opcode for R-type
            machine_code |= ((rd_num  & 0x1F) << 7); // Set rd field
            machine_code |= (0b000 << 12); // Set funct3 (0 for add)
            machine_code |= ((rs1_num & 0x1F) << 15); // Set rs1 field
            machine_code |= ((rs2_num & 0x1F) << 20); // Set rs2 field
            machine_code |= (0b0000000 << 25); // Set funct7 (0 for add)
        }
        else if (strcmp(opcode, "sub") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0100000  << 25); //funct7

        }
        else if (strcmp(opcode, "or")== 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b110  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); //funct7
        }
        else if (strcmp(opcode, "and") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b111  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); //funct7
        }
        else if (strcmp(opcode, "xor") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b100  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); 
        }
        else if (strcmp(opcode, "sll") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b001  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); 
        }
        else if (strcmp(opcode, "srl") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); //funct7
        }
        else if (strcmp(opcode, "sra") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0100000  << 25); //funct7
        }
        else if (strcmp(opcode, "slt") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b010  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); //funct7
        }
        else if (strcmp(opcode, "sltu") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            rs2_num = get_register_number(rs2);
            machine_code |= 0b0110011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b011  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= (0b0000000  << 25); //funct7
        }
        else if (strcmp(opcode, "addi") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        } 
        else if (strcmp(opcode, "slli") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b001  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
            machine_code |= (0b0000000 << 25);
        }
        else if (strcmp(opcode, "slti") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b010  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "sltiu") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b011  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "xori") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b100  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "srli") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
            machine_code |= (0b0000000 << 25);
        }
        else if (strcmp(opcode, "srai") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
            machine_code |= (0b0100000 << 25);
        }
        else if (strcmp(opcode, "ori") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b110  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "andi") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b111  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "jalr") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            imm = convertToDecimal(rs2);
            machine_code |= 0b1100111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "beq") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            //printf("%s\n", rs2);
            imm = address - instruction_count2;
            //printf("%d %d %d\n", rs1_num, rs2_num, imm);
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "bne") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b001  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "blt") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b100  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 19);
        }
        else if (strcmp(opcode, "bgt") == 0){
            instruction_count2++;
            rs2_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b100  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "bge") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "ble") == 0){
            instruction_count2++;
            rs2_num = get_register_number(rd);
            rs1_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "bltu") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b110  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        else if (strcmp(opcode, "bgeu") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rs2_num = get_register_number(rs1);
            address = find_label_address(rs2);
            imm = address - instruction_count2;
            imm = imm << 2;
            machine_code |= 0b1100011;
            machine_code |= ((imm  & 0x800) >> 4);
            machine_code |= ((imm  & 0x1E) << 7);
            machine_code |= (0b111  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rs2_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0x7E0) << 20);
            machine_code |= ((imm  & 0x1000) << 20);
        }
        
    }
    else if (count == 3){
        if (strcmp(opcode, "lb") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0000011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "lh") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0000011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b001  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "lw") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0000011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b010  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "lbu") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0000011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b100  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "lhu") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0000011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b101  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
        else if (strcmp(opcode, "sb") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0100011;
            machine_code |= ((imm  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rd_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0xFE0) << 20);
        }
         else if (strcmp(opcode, "sh") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0100011;
            machine_code |= ((imm  & 0x1F) << 7);
            machine_code |= (0b001  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rd_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0xFE0) << 20);
        }
         else if (strcmp(opcode, "sw") == 0){
            instruction_count2++;
            char temp[20];
            sscanf(rs1, "%s(%s)", temp, rs1);
            separateImmediate(temp);
            separate_rs1(rs1);
            imm = convertToDecimal(temp);
            sscanf(rs1, "%s", rs1);
            removeBracket(rs1);
            rs1_num = get_register_number(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0100011;
            machine_code |= ((imm  & 0x1F) << 7);
            machine_code |= (0b010  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((rd_num  & 0x1F) << 20);
            machine_code |= ((imm  & 0xFE0) << 20);
        }
        else if (strcmp(opcode, "auipc") == 0){
            instruction_count2++;
            imm = convertToDecimal(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0010111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= ((imm & 0xFFFFF) << 12);
        }
        else if (strcmp(opcode, "lui") == 0){
            instruction_count2++;
            imm = convertToDecimal(rs1);
            rd_num = get_register_number(rd);
            machine_code |= 0b0110111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= ((imm & 0xFFFFF) << 12);
        }
        else if (strcmp(opcode, "jal") == 0){
            instruction_count2++;
            address = find_label_address(rs1);
            imm = address - instruction_count2;
            imm = imm << 2;
            rd_num = get_register_number(rd);
            machine_code |= 0b1101111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (imm & 0xFF000);
            machine_code |= ((imm & 0x800) << 9);
            machine_code |= ((imm & 0x7FE) << 20);
            machine_code |= ((imm & 0x100000) << 11);
        }
        else if (strcmp(opcode, "li") == 0){
            instruction_count2++;
            rd_num = get_register_number(rd);
            rs1_num = get_register_number("x0");
            imm = convertToDecimal(rs1);
            machine_code |= 0b0010011;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        } 
        else if (strcmp(opcode, "mv") == 0) {
            instruction_count2++; // Update instruction counter
            rd_num = get_register_number(rd); // Get register number for rd
            rs1_num = get_register_number(rs1); // Get register number for rs1
            rs2_num = get_register_number("x0"); // Get register number for rs2
            machine_code |= 0b0010011; // Set opcode for R-type
            machine_code |= ((rd_num  & 0x1F) << 7); // Set rd field
            machine_code |= (0b000 << 12); // Set funct3 (0 for add)
            machine_code |= ((rs1_num & 0x1F) << 15); // Set rs1 field
            machine_code |= ((rs2_num & 0x1F) << 20); // Set rs2 field
            machine_code |= (0b0000000 << 25); // Set funct7 (0 for add)
        }

        
    }
    else if (count == 2){
        if (strcmp(opcode, "j") == 0){
            instruction_count2++;
            address = find_label_address(rd);
            imm = address - instruction_count2;
            imm = imm << 2;
            rd_num = get_register_number("x0");
            machine_code |= 0b1101111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (imm & 0xFF000);
            machine_code |= ((imm & 0x800) << 9);
            machine_code |= ((imm & 0x7FE) << 20);
            machine_code |= ((imm & 0x100000) << 11);
        }
        else if (strcmp(opcode, "jr") == 0){
            instruction_count2++;
            rs1_num = get_register_number(rd);
            rd_num = get_register_number("x0");
            imm = 0;
            machine_code |= 0b1100111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
        }
    }
    else if (count == 1){
        if (strcmp(opcode, "ret") == 0){
            instruction_count2++;
            rs1_num = get_register_number("ra");
            rd_num = get_register_number("x0");
            imm = 0;
            machine_code |= 0b1100111;
            machine_code |= ((rd_num  & 0x1F) << 7);
            machine_code |= (0b000  << 12); //funct3
            machine_code |= ((rs1_num  & 0x1F) << 15);
            machine_code |= ((imm  & 0xFFF) << 20);
            
        }
    }

    return machine_code;
}

// Outputs the 32-bit machine code as an 8-character hexadecimal string to the specified file
void output_hex(unsigned int code, FILE *output_file) {
    // Use fprintf to print the 32-bit machine code in hexadecimal format to the file
    // "0x" is added as a prefix, and the code is printed as an 8-character hexadecimal value
    fprintf(output_file, "0x%08X\n", code);  // it ensures output is always 8 hex digits, with leading zeros if necessary
}


// Function to output machine code in binary
void output_binary(unsigned int code, FILE *output_file) {
    // Loop through each bit of the 32-bit machine code, starting from the most significant bit (bit 31)
    for (int i = 31; i >= 0; --i) {
        // Check if the ith bit of 'code' is set (1) or not (0), and output '1' or '0' accordingly
        fputc((code & (1 << i)) ? '1' : '0', output_file);
    }
    // After outputting all 32 bits, add a newline character to the file
    fputc('\n', output_file);
}