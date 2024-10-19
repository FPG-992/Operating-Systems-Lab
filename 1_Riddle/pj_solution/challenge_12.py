from pwn import *
import os


# Launch the './riddle' binary in a new process
sh = process('./riddle')

# Sleep for 0.2 seconds to give the './riddle' process time to run
sleep(0.1)

# Get the name of the file that riddle wants read at offset 0x6f and store it to name.txt
os.system("ls -t /tmp | grep 'riddle' | head -1  > name.txt")

with open('name.txt', 'r') as file:
    filename = file.read()[:-1]
    print(filename)

# Read the './riddle' process output until it finds "char '", then receive the next character
sh.recvuntil(b"char '")
char = sh.recvuntil(b"'")[:-1]
print(char)

# Open the discovered file (from the /tmp directory) in read/write mode using its full path
fd = os.open(f"/tmp/{filename}",  os.O_RDWR)

# Seek the specific position (0x6f, or 111 in decimal) in the file
os.lseek(fd, 0x6f, 0)

# Write the previously received character into the file at the specified position
os.write(fd, char)

# Enter interactive mode with the './riddle' process, allowing for manual interaction
sh.interactive()
