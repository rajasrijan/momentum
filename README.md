# Momentum
Momentum is a fun experiment I had started while I was in School. Aim of this project is to write Operating System from scratch. 
Origianally I started writing in Assembly lenguage then quickly moved on to C and later C++. 

## Whats there in momentum?
* Boots using GRUB2
* Multi-threading (multiple threads, mutexes, semaphores)
* Memory Management (paging, malloc, etc...)
* Basic driver loading (IDE & SATA driver, partial USB 1.0 driver)
* Can read from FAT32 filesystem
* Basic C, C++ runtime
* Basic shell commands (e.g help, version, echo, lspci)

## Steps to build
1. Install MINGW (linux users can skip this)
2. Build cross-compiles toolchain(binutils, gcc, g++) for target "i686-elf". C, C++ and LD should have the name "x86_64-elf-gcc", "x86_64-elf-g++", "x86_64-elf-ld". If they are different then update CC, CXX,LD in makefile.
3. Install NASM assembler.
4. Run "make all"

If build is successful "kernel.elf" will be generated. "kernel.elf" is multiboot complient and can be loaded by any bootloader which supports multiboot.

## Some screenshots
#### Booting through grub2
![Startup](/images/startup.PNG)
#### OS Welcome Screen
![Welcome](/images/welcome.PNG)
#### Basic Shell
![Shell](/images/termanal.PNG)

## Author

* **Srijan Kumar Sharma** - [rajasrijan](https://github.com/rajasrijan)

## License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details
