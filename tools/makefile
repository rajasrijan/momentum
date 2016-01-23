all:ImCopy.exe AppendRamDrive.exe

ImCopy.exe:ImCopy.c
	gcc -O0 --std=c11 -mno-ms-bitfields -g -o $@ $^

AppendRamDrive.exe:AppendRamDrive.c
	gcc -O0 --std=c11 -mno-ms-bitfields -g -o $@ $^
	
clean:
	$(RM) ImCopy.exe