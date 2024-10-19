# Riddle

## Getting Started
First running the executable ./riddle we got:
```bash
-bash: ./riddle: Permission denied
```
To examine the permissions of that file we used:
```bash
ls -la riddle
```
which resulted to:
```bash
-rw-r--r-- 1 pj pj 40360 Oct 18  2023 riddle
```

So we used chmod to add execute permissions (we added all permissions to everyone):
```bash
chmod 777 riddle
```

## Challenge 0
Running the ./riddle executable we got:
```bash
Challenge   0: 'Hello there'
Hint:          'Processes run system calls'.
FAIL
```

First tool we used is `strace`. According to its man page:

_In  the  simplest case strace runs the specified command until it exits.  It intercepts and records the system calls which are called by a process and the signals which are received by a process.  The name of each  system call,  its  arguments  and its return value are printed on standard error or to the file specified with the -o option._

By examining the result of this system call we found:
```bash
openat(AT_FDCWD, ".hello_there", O_RDONLY) = -1 ENOENT (No such file or directory)
```

**openat**: According to the man page:

The openat() system call operates in exactly the same way as open(2)... Given a pathname for a file, open() returns a file descriptor... open() and creat() return the new file descriptor, or -1 if an error occurred (in which case, errno is set appropriately).

_O_RDONLY_: The argument flag, which request opening the file read-only.

_ENOENT_: O_CREAT is not set and the named file does not exist. Or, a directory component in pathname does not exist or is a dangling symbolic link.


So we need to create the `.hello_there` file with read only permissions:
```bash
touch .hello_there
```

And running the ./riddle executable once more, we got:
```bash
Challenge   0: 'Hello there'
Hint:          'Processes run system calls'.
SUCCESS
```

## Challenge 1

Running the ./riddle executable we got:
```bash
Challenge   1: 'Gatekeeper'
Hint:          'Stand guard, let noone pass'.
... I found the doors unlocked. FAIL
```

At the output of `strace` we notice the following line:
```bash
openat(AT_FDCWD, ".hello_there", O_WRONLY) = 4
```
Using `ls -la .hello_there` we noticed that the file had writting user permissions, which we don't want, according to the hint. So gave read-only permissions.
```bash
chmod 444 .hello_there
```

And by running the ./riddle file again:
```bash
Challenge   1: 'Gatekeeper'
Hint:          'Stand guard, let noone pass'.
SUCCESS
```

## Challenge 2

Running the ./riddle executable we got:
```bash
Challenge   2: 'A time to kill'
Hint:          'Stuck in the dark, help me move on'.
You were eaten by a grue. FAIL
```
and we notice that the execution pauses for a while.

On the result from the `strace` call we noticed:
```bash
pause() = ? ERESTARTNOHAND (To be restarted if no handler)
```

From the `pause` man page:

_pause() causes the calling process (or thread) to sleep until a signal is delivered that either terminates the process or causes the invocation of a signal-catching function._

So we need to send a signal to continue the execution. For that we will create a bash file named `challenge_1.sh`:
```bash
#!/bin/bash

# Get the pid of the riddle process
processId=$(ps -ef | grep 'riddle' | grep -v 'grep' | awk '{ printf $2 }')

# Send signal to the riddle process
kill -SIGCONT $processId

```

Then add executing permissions:
```bash
chmod +x challenge.sh
```

And after we ran the ./riddle file and then our ./challenge.sh we got:
```bash
Challenge   2: 'A time to kill'
Hint:          'Stuck in the dark, help me move on'.
SUCCESS
```

## Challenge 3

Running the ./riddle executable we got:
```bash
Challenge   3: 'what is the answer to life the universe and everything?'
Hint:          'ltrace'.
FAILL
```

From the man page of `ltrace`:

_ltrace intercepts and records the dynamic library calls which are called by the executed process and the signals which are received by that process. It can also intercept and print the system calls executed by the program._

The hint indicates us to use it, so after using it we saw:
```bash
getenv("ANSWER") = nil
```
but we havn't created any enviroment variable name `ANSWER`.

Let's do that and of cource set it equal to 42, because that the answer to everything:
```bash
export ANSWER="42"
```

And after running the ./riddle file we got:
```bash
Challenge   3: 'what is the answer to life the universe and everything?'
Hint:          'ltrace'.
SUCCESS
```

## Challenge 4

Running the ./riddle executable we got:
```bash
Challenge   4: 'First-in, First-out'
Hint:          'Mirror, mirror on the wall, who in this land is fairest of all?'.
I cannot see my reflection. FAIL
```

After using `strace` we noticed that:
```bash
openat(AT_FDCWD, "magic_mirror", O_RDWR) = -1 ENOENT (No such file or directory)
```

So we created the `magic_mirror` file, with read and write permissions:
```bash
touch magic_mirror
```

Then after runing the strace call once again, we noticed:
```bash
write(4, "X", 1) = 1
read(4, "", 1) = 0
```
Our assumption is that the riddle executable attempts to write a random byte into the magic_mirror file and then reads back that exact same byte. However, in any normal file, performing a read operation after a write will always return the "next" byte rather than the one that was just written.

But, in a FIFO pipe (as the name of the challenge), a read after a write would always read the last byte that was just written.

So we removed the file and created a fifo pipe:
```bash
rm magic_mirror
mkfifo magic_mirror
```

And after running the ./riddle file we got:
```bash
Challenge   4: 'First-in, First-out'
Hint:          'Mirror, mirror on the wall, who in this land is fairest of all?'.
SUCCESS
```

## Challenge 5

Running the ./riddle executable we got:
```bash
Challenge   5: 'my favourite fd is 99'
Hint:          'when I bang my head against the wall it goes: dup! dup! dup!'.
FAIL
```

After using the `strace` call we noticed:
```bash
fcntl(99, F_GETFD) = -1 EBADF (Bad file descriptor)
```

From the man page of `fcntl`:

_The fcntl() function shall perform an operation on open files._ 

_F_GETFD_: Get the file descriptor flags defined in <fcntl.h> that are associated with the file descriptor fildes. File descriptor flags are associated with a single file descriptor and do not affect other file descriptors that refer to the same file.

_EBADF_: The fildes argument is not a valid open file descriptor.

We also saw the hint dup, so we search the dup system call:

`dup`: uses the lowest-numbered unused descriptor for the new descriptor. After a successful return from one of these system calls, the old and new file descriptors may be used interchangeably. They refer to the same open file description.

So we dup the ./riddle process the the file descriptor 99:
```bash
./riddle 99>&1
```

And we got the following output:
```bash
Challenge   5: 'my favourite fd is 99'
Hint:          'when I bang my head against the wall it goes: dup! dup! dup!'.
SUCCESS
```

## Challenge 6

Running the ./riddle executable we got:
```bash
Challenge   6: 'ping pong'
Hint:          'help us play!'.
[49694] PING!
FAIL
```

Using the `ltrace` system call we noticed:
```bash
fork() = 50147
fork() = 50148
```
And we also noticed that the message `Ping!` wasn't written by the executable, but from the first child.

Using the `strace` call with -f flag (this option tells strace to follow forks) we saw:
```bash
[pid 51501] read(33,  <unfinished ...>
[pid 51501] <... read resumed>0x7ffeebea87f4, 4) = -1 EBADF (Bad file descriptor)
[pid 51500] write(34, "\0\0\0\0", 4)    = -1 EBADF (Bad file descriptor)
```

We tried to redirect the file descriptor 33, and 34 to stdout and trace the results:
```bash
[pid 52392] read(53, 0x7fff97e0cc44, 4) = -1 EBADF (Bad file descriptor)
```

So, the one child process reads from fd 33 4 bytes. The other child process writes to fd 34 4 bytes. Also the other one reads from fd 53 4 bytes. And we can say that probably the first one writes to fd 54 4 bytes.

To achieve this functionality and help them play ping-pong we need to use pipes. We created this following .c file to achieve this:
```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>

int main() {

    int p1[2]; // Array to hold the file descriptors for the first pipe
    int p2[2]; // Array to hold the file descriptors for the second pipe
    
    // Create the first pipe. p1[0] is the read end, p1[1] is the write end.
    pipe(p1);

    // Create the second pipe. p2[0] is the read end, p2[1] is the write end.
    pipe(p2);

    // Duplicate the read end of p1 to file descriptor 33
    dup2(p1[0], 33);
    // Duplicate the write end of p1 to file descriptor 34
    dup2(p1[1], 34);
    // Duplicate the read end of p2 to file descriptor 53
    dup2(p2[0], 53);
    // Duplicate the write end of p2 to file descriptor 54
    dup2(p2[1], 54);

    // Path to the executable to be run
    char exec[] = "./riddle";
    // Arguments for the new program
    char *newargv[] = {exec, NULL, NULL, NULL };
    // Environment variables for the new program (none in this case)
    char *newenviron[] = { NULL };

    // Replace the current process with a new process image
    execve(exec, newargv, newenviron);
    
    return 0;
}
```

Then we compile and run this file we got:
```bash
Challenge   6: 'ping pong'
Hint:          'help us play!'.
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
[53868] PING!
[53869] PONG!
[53869] PING!
[53868] PONG!
SUCCESS
```


## Challenge 7

Running the ./riddle executable we got:
```bash
Challenge   7: 'What's in a name?'
Hint:          'A rose, by any other name...'.
FAIL
```

After using `strace` we noticed:
```bash
lstat(".hey_there", 0x7ffe21cb0ba0) = -1 ENOENT (No such file or directory)
```

According to the man page of the `lstat` call:

_Returns information about a file._

_ENOENT_: A component of path does not exist, or path is an empty string.

So we create the file, and we got:
```
Oops. 54563 != 54570.
```

We used the `stat` system call, first on the .hello_there file, and after that on the .hey_there and noticed the following:
```bash
File: .hello_there
Size: 0               Blocks: 0          IO Block: 4096   regular empty file
Device: 8,32    Inode: 54563       Links: 1

File: .hey_there
Size: 0               Blocks: 0          IO Block: 4096   regular empty file
Device: 8,32    Inode: 54570       Links: 1
```

So the number that the challenge indicate us are inodes. An inode is a data structure on a filesystem that contains metadata about a file or directory. Each file or directory has an associated inode that stores important information about it, except for the actual data content and the filename.

Somehow we want to link those files together in order to have the same inode. That's what a hardlink does.

A hard link is a directory entry that associates a filename with an inode. Multiple filenames (hard links) can refer to the same inode, and thus the same file content.

To create hard links we deleted the .hey_there file and used:
```bash
ln .hello_there .hey_there
```

And after running the ./riddle file we got:
```bash
Challenge   7: 'What's in a name?'
Hint:          'A rose, by any other name...'.
...would smell as sweet. SUCCESS
```

## Challenge 8

Running the ./riddle executable we got:
```bash
Challenge   8: 'Big Data'
Hint:          'Checking footers'.
Data files must be present and whole. FAIL
```

After using `strace` we noticed:
```bash
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "bf00", O_RDONLY)      = -1 ENOENT (No such file or directory)
```

So we created the bf00 file and got:
```bash
Challenge   8: 'Big Data'
Hint:          'Checking footers'.
XXXXXXXXXXData files must be present and whole. FAIL
```

Then on the strace output we saw:
```bash
openat(AT_FDCWD, "bf00", O_RDONLY)      = 4
lseek(4, 1073741824, SEEK_SET)          = 1073741824
read(4, "", 16)                         = 0
```

and:
```bash
write(1, "X", 1X)                        = 1
close(4)                                = 0
openat(AT_FDCWD, "bf00", O_RDONLY)      = 4
lseek(4, 1073741824, SEEK_SET)          = 1073741824
read(4, "", 16)                         = 0
write(1, "X", 1X)                        = 1
close(4)                                = 0
...
```
which represent the X's we saw

From the man page of `lseek` we have:

_The lseek() function repositions the offset of the open file associated with the file descriptor fd to the argument offset according to the directive whence as follows._

The riddle process first opens the bf00 file and then uses lseek to change the file offset for the file descriptor fd=4. The SEEK_SET flag sets the offset directly to 1,073,741,824. It then attempts to read 16 bytes, but finds nothing—only EOF.

As a result, the riddle writes an 'X' character to stdout and starts the process over. Our next step is to increase the file size and write something at the 1,073,741,824 offset.

We did that using `truncate`:
```bash
truncate -s 1073741824 bf00
echo G >> bf00
```

Then after using `strace`once again we noticed:
```bash
openat(AT_FDCWD, "bf00", O_RDONLY)      = 4
lseek(4, 1073741824, SEEK_SET)          = 1073741824
read(4, "G\n", 16)                      = 2
```
which is what we expected, but also:
```bash
openat(AT_FDCWD, "bf01", O_RDONLY)      = -1 ENOENT (No such file or directory)
```

We then created the bf01 file, and we saw:
```bash
openat(AT_FDCWD, "bf01", O_RDONLY)      = 4
lseek(4, 1073741824, SEEK_SET)          = 1073741824
read(4, "", 16)                         = 0
```
the same output as of bf00.

So we created the following python script to iterate from bf00 to bf09:
```python
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
```

Then after running the script and then the ./riddle file we got:
```bash
Challenge   8: 'Big Data'
Hint:          'Checking footers'.
..........SUCCESS
```

## Challenge 9

Running the ./riddle executable we got:
```bash
Challenge   9: 'Connect'
Hint:          'Let me whisper in your ear'.
I am trying to contact you...\n...but you don't seem to listen.\nFAIL
```

After using the `strace` call we noticed:
```bash
socket(AF_INET, SOCK_STREAM, IPPROTO_IP) = 4
connect(4, {sa_family=AF_INET, sin_port=htons(49842), sin_addr=inet_addr("127.0.0.1")}, 16) = -1 ECONNREFUSED (Connection refused)
```

So the riddle process creates a socket which returns the file descriptor 4 and then it tried to to use this open socket to connect to localhost port 49842.

From the man page of the `netcat`:

_The nc (or netcat) utility is used for just about anything under the sun involving TCP or UDP. It can open TCP connections, send UDP packets, listen on arbitrary TCP and UDP ports, do port scanning, and deal with both IPv4 and IPv6._

So to listen the port 49842 we opened a new window and typed:
```bash
nc -l 49842
```

After we run the ./riddle file, on the listener window we saw:
```bash
How much is 60420 + 1?
```

and after we enter 60421, on the riddle window we noticed:
```bash
Challenge   9: 'Connect'
Hint:          'Let me whisper in your ear'.
I am trying to contact you...\nSUCCESS
```

## Challenge 10

Running the ./riddle executable we after we typed 42, we saw:
```bash
Challenge  10: 'ESP'
Hint:          'Can you read my mind?'.
What hex number am I thinking of right now? 42
You're only guessing, and I am wasting my time.\nFAIL
```

After using `strace` we noticed:
```bash
openat(AT_FDCWD, "secret_number", O_RDWR|O_CREAT|O_TRUNC, 0600) = 4
unlink("secret_number") 
```

From the man page of `unlink`:

_unlink() deletes a name from the file system. If that name was the last link to a file and no processes have the file open the file is deleted and the space it was using is made available for reuse. If the name was the last link to a file but any processes still have the file open the file will remain in existence until the last file descriptor referring to it is closed._

This file does not have any links pointing to it, meaning the unlink system call would typically delete it. However, because the riddle process has already opened the file, it will continue to exist until the last file descriptor referencing it is closed.

So it appears that riddle writes a random number to the secret number file and then it deletes the file. So we can hyperlink this file to another one to prevent deletion and accessing the content of it.

We used:
```bash
touch secret_number
ln secret_number sec_link
```

So after running the ./riddle file we used cat on the sec_link file and then passed the correct number to the prompt:
```bash
Challenge  10: 'ESP'
Hint:          'Can you read my mind?'.
What hex number am I thinking of right now? 9C665B03
SUCCESS
```

## Challenge 11

Running the ./riddle executable we after we typed 42, we saw:
```bash
Challenge  11: 'ESP-2'
Hint:          'Can you read my mind?'.
What hex number am I thinking of right now? 42
You're only guessing, and I am wasting my time.\nFAIL
```

Pretty simiral to the previous challenge. So let's try the same approach:
```
touch secret_number
ln secret_number sec_link2
```

And we got:
```bash
Challenge  11: 'ESP-2'
Hint:          'Can you read my mind?'.
You're employing treacherous tricks... FAIL
```

After using `strace` we noticed:
```bash
openat(AT_FDCWD, "secret_number", O_RDWR|O_CREAT|O_TRUNC, 0600) = 4
unlink("secret_number")                 = 0
fstat(4, {st_mode=S_IFREG|0600, st_size=0, ...}) = 0
```

That's why calls fstat. If we have a link pointing to the file, and prints that we're using treacherous tricks.

According to the man page:

_fstat() is identical to stat(). These functions return information about a file._

So to solve that issue we opened the file on another process and retrieved the content from there. Here is the script we used:
```python
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
```

`pwntools` is a powerful Python library used primarily in the field of binary exploitation and capture-the-flag (CTF) competitions. It is widely regarded as a go-to framework for tasks related to reverse engineering, exploit development, and interacting with processes, remote servers, and binaries.

So, after running the script we got:
```bash
The number I am thinking of right now is [uppercase matters]: D1AD7A0C
```
which we typed them into the prompt, and got:
```bash
Challenge  11: 'ESP-2'
Hint:          'Can you read my mind?'.
What hex number am I thinking of right now? $ D1AD7A0C
SUCCESS
```

## Challenge 12

Running the ./riddle executable, we saw:
```bash
Challenge  12: 'A delicate change'
Hint:          'Do only what is required, nothing more, nothing less'.
I want to find the char 'M' at 0x7f6e4847f06f
FAIL
```

The process sleeps for some time, so after examine the output of the `strace` call we noticed:
```bash
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
clock_nanosleep(CLOCK_REALTIME, 0, {tv_sec=2, tv_nsec=0}, 0x7ffc7f92a990) = 0
```
and
```bash
openat(AT_FDCWD, "/tmp/riddle-VmLY35", O_RDWR|O_CREAT|O_EXCL, 0600) = 4
ftruncate(4, 4096)                      = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, 4, 0) = 0x7fdb4686f000
```

According to the man page of `mmap` we have:

_mmap() creates a new mapping in the virtual address space of the calling process. If addr is NULL, then the kernel chooses the address at which to create the mapping; this is the most portable method of creating a new mapping._

_PROT_READ_: Pages may be read.

_PROT_WRITE_: Pages may be written.

_MAP_SHARED_: Share this mapping. Updates to the mapping are visible to other processes that map this file, and are carried through to the underlying file

It appears that the process is creating a file (different name each execution), then calls ftruncate to resize the file to 4KB. By running the script more times we noticed that the requested address is always 0x6f bytes after the address returned by mmap and that's where we have to pay close attention (on this execution 0x7fdb4686f06f).

So, we want to find the name of the file that riddle wants read at offset 0x6f. We have created the following script:
```python
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
```

After running the script we got:
```bash
[+] Starting local process './riddle': pid 67392
riddle-suR6Fg
b'K'
[*] Switching to interactive mode
 at 0x7f38d4ef406f
SUCCESS
```

## Challenge 13

Running the ./riddle executable we saw:
```bash
Challenge  13: 'Bus error'
Hint:          'Memquake! Don't lose the pages beneath your something something'.
FAIL
```

After using `strace` we noticed:
```bash
openat(AT_FDCWD, ".hello_there", O_RDWR|O_CREAT, 0600) = -1 EACCES (Permission denied)
```

Where _O_RDWR_ indicates read/write permissions and _EACCES_ the requested access to the file is not allowed. So we changed the permissions of the .hello_there file:
```bash
chmod 666 .hello_there
```

And then the process wait to read as displayed bellow:
```bash
ftruncate(4, 32768)                     = 0
mmap(NULL, 32768, PROT_READ|PROT_WRITE, MAP_SHARED, 4, 0) = 0x7fa2ae768000
ftruncate(4, 16384)
read(0, 
```

The riddle process begins by memory-mapping a file into its virtual memory, then truncates the file, reducing its size by half. Following this, the process is terminated by receiving a SIGBUS signal. This suggests that the process attempts to access memory that was originally part of the memory-mapped file but is now outside the file's new, smaller boundary after truncation.

Specifically, the process tried to access the memory address 0x7fa2ae76c000, which is 16384 bytes beyond the address returned by mmap(2) (0x7fa2ae768000). This caused the process to receive a SIGBUS signal, leading to its termination.

A potential solution would be to preserve the memory pages. By waiting and then restoring the file to its original size, the memory pages would be reallocated, making the memory access valid again.

So we run the ./riddle file and then we opened a new window and typed:
```bash
truncate --size=30K .hello_there
```

After that we type anything on the prompt and the result was:
```bash
Challenge  13: 'Bus error'
Hint:          'Memquake! Don't lose the pages beneath your something something'.
p
SUCCESS
```

## Challenge 14

Running the ./riddle executable we saw:
```bash
Challenge  14: 'Are you the One?'
Hint:          'Are you 32767? If not, reincarnate!'.
I don't like my PID, get me another one. FAIL
```

On the results of the `strace` call we noticed:
```bash
getpid() = 70965
```
That means that the pid of the riddle process is different from the once that the hint indicates. Unfortunatelly we cannot change the pid of an existing process, but we can set it before the process runs.

The file `/proc/sys/kernel/ns_last_pid` in Linux stores the value of the most recently assigned process ID (PID). When a new process is created, the kernel assigns it a unique PID. The value in ns_last_pid reflects the last PID that was issued. This file can be read to find out the PID of the most recently created process, and in some cases, it can be manually set (with root privileges) to influence the assignment of the next PID.

For example: If you write a value to this file (as root), the kernel will attempt to assign the next available PID after the one specified. However, this doesn't guarantee that the next process will get that exact PID due to PID allocation rules.

So we have created the following script to do that:
```bash
#!/bin/bash

sudo echo 32766 > /proc/sys/kernel/ns_last_pid

./riddle
```

And after running the script (add execute permissions) we got:
```bash
Challenge  14: 'Are you the One?'
Hint:          'Are you 32767? If not, reincarnate!'.
SUCCESS
```