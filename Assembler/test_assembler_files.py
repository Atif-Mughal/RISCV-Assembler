import os
import subprocess

# Define the paths
testing_application_path = 'TestingApplication'
output_directory = 'output_machine_code'

# Create the output directory if it doesn't exist
os.makedirs(output_directory, exist_ok=True)

# Get all assembly files in the TestingApplication directory
files = os.listdir(testing_application_path)

# Filter for assembly files
asm_files = [file for file in files if file.endswith('.s')]

# Print header
print("==========================================")
print("          Assembly File Tester            ")
print("==========================================")
print(f"Found {len(asm_files)} assembly files in '{testing_application_path}':")
print(", ".join(asm_files) or "None")
print("==========================================")

# Lists to track results
correct_outputs = []
incorrect_outputs = []

# Iterate through each assembly file
for asm_file in asm_files:
    # Construct the output file name
    output_file = os.path.join(output_directory, 'output_' + asm_file.replace('.s', '.txt'))
    
    # Construct the corresponding dump file name
    dump_file = asm_file.replace('test_', 'hex_').replace('.s', '.dump')
    dump_file_path = os.path.join(testing_application_path, dump_file)

    # Print new test start message
    print("\n" + "=" * 50)
    print(f" Starting Test for: '{asm_file}'")
    print("=" * 50)

    # Construct the assembler command
    assembler_command = f"./assembler {os.path.join(testing_application_path, asm_file)} {output_file} -h"
    
    # Execute the assembler command
    print(f"Running assembler for: {asm_file}...")
    try:
        subprocess.run(assembler_command, shell=True, check=True)
        print(f"Successfully generated output file: {output_file}")
    except subprocess.CalledProcessError as e:
        print(f"Error running assembler for {asm_file}: {e}")
        continue
    
    if os.path.exists(dump_file_path):
        # Construct the check.py command
        check_command = f"python3 check.py {output_file} {dump_file_path}"
        
        # Execute the check.py command
        print("\n" + "=" * 50)
        print(f" Checking Output: '{output_file}'")
        print(f"   vs. '{dump_file_path}'")
        print("=" * 50)
        
        result = subprocess.run(check_command, shell=True, capture_output=True, text=True)
        
        # Print the result
        print("Comparison Result:")
        print(result.stdout)
        
        # Check the result for correctness
        if "Files are identical" in result.stdout:  # Modify this based on actual success indicator
            correct_outputs.append(asm_file)
        else:
            incorrect_outputs.append(asm_file)
    else:
        print(f"\nWarning: Dump file '{dump_file}' not found for '{asm_file}'.")

# Print footer with results
print("==========================================")
print("              Testing Complete            ")
print("==========================================")
print(f"Total Assembly Files Processed: {len(asm_files)}")
print(f"Files with Correct Output: {len(correct_outputs)}")
print(f"Files with Incorrect Output: {len(incorrect_outputs)}")
print("\nFiles with Correct Output:")
print("\n".join(correct_outputs) if correct_outputs else "None")
print("\nFiles with Incorrect Output:")
print("\n".join(incorrect_outputs) if incorrect_outputs else "None")