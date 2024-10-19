import os


for i in range(0, 10):
    name = f'bf0{i}'

    # Open the file with read/write permissions; create it if it doesn't exist
    # The file descriptor 'f' is obtained, with permissions set to 400 (octal),
    # which means only the owner can read the file.
    f = os.open(name, os.O_RDWR|os.O_CREAT,  400)

    # Move the file pointer to the 1 GiB (1073741824 bytes) position in the file
    # '0' indicates the reference point is the beginning of the file
    os.lseek(f, 1073741824, 0)
    
    # Write a single byte G
    os.write(f, b'G')
    