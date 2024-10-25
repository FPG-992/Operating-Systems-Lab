from pwn import *


# Create the secret file
open('secret_number', 'w')

with open('secret_number', 'r') as file:
    # Launch the new process './riddle' using pwntools' process function
    sh = process('./riddle')

    # Read the content of the 'secret_number' file and strip any extra whitespace or newline characters
    file_content = file.read().strip()
    
    print(file_content)

# Interact with the process to allow manual user input/output via the console
sh.interactive()
