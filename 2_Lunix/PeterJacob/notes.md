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
