Bryce Woods
brwo4424@colorado.edu

simple_char_driver.c : Device driver file that contains function definitions for all device functions, including registering device and allocating memory and unregistering device and deallocating memory.

test.c : Test program that tests the functions in simple_char_driver.c

Makefile : File used by make to compile simple_char_driver.c into simple_char_driver.ko

---Instructions---

1) From any terminal window run: sudo mknod -m 777 /dev/simple_character_device c 270 0
2) Cd into folder containing files and run: sudo make -C /lib/modules/$(uname -r)/build M=$PWD modules
3) Run: sudo insmod simple_char_driver.ko
4) Run: sudo gcc test.c
5) Run: ./a.out
