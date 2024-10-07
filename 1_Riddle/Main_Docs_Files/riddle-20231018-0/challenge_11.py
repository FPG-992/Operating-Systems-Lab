from pwn import *
import os

file = open('secret_number','r')
sh = process('./riddle')
file_content = file.read().strip()
print(file_content)
sh.interactive()
