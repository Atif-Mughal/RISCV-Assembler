import os
import subprocess

# Define the paths
input_directory = 'input_files'
output_directory = 'output_machine_code'

# Create the output directory if it doesn't exist
os.makedirs(output_directory, exist_ok=True)

# Get all assembly files in the Input_files directory
files = os.listdir(input_directory)

# Filter for assembly files
asm_files = [file for file in files if file.endswith('.s')]

# Print header
print("=======================================================")
print("          Assembly File to Machine Code Generator      ")
print("=======================================================")
print(f"Found {len(asm_files)} assembly files in '{input_directory}':")
print(", ".join(asm_files) or "None")
print("=======================================================")

# Iterate through each assembly file
for asm_file in asm_files:
    # Construct the output file name
    output_file = os.path.join(output_directory, 'output_' + asm_file.replace('.s', '.txt'))

    # Print new test start message
    print("\n" + "=" * 50)
    print(f" Starting Generation for: '{asm_file}'")
    print("=" * 50)

    # Construct the assembler command
    assembler_command = f"./assembler {os.path.join(input_directory, asm_file)} {output_file} -h"
    
    # Execute the assembler command
    print(f"Running assembler for: {asm_file}...")
    try:
        subprocess.run(assembler_command, shell=True, check=True)
        print(f"Successfully generated output file: {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Error running assembler for {asm_file}: {e}")
        continue

# Print footer
print("==========================================")
print("              Generation Complete          ")
print("==========================================")
print(f"Total Assembly Files Processed: {len(asm_files)}")