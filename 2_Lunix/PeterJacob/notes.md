### Running a file (on kernel mode)
```bash
> make
    (it should produce a somename.ko file)
> insmod somename.ko
    (inserts the module into the kernel)
> lsmod | grep somename
    (If your module appears in the list, it has been loaded successfully)
> dmesg | tail -100
    (check for any messages related to your module)
```

### Running the Project
```bash
Terminal 1:
> make
> ./start.sh
    (change the port /dev/ttyS1 if needed)

Terminal 2:
> ./lunix-tcp.sh /dev/ttyS1
    (dorward data from a TCP endpoint to a specified tty device)

Terminal 3:
> dmesg
    (display system messages from the kernel ring buffer)
> cat /dev/lunix1-batt
    (to start the streaming)
```
