#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
	if (argc != 3)
		return 1;
	FILE *ramDrive = fopen(argv[1], "a+");
	fclose(ramDrive);
	ramDrive = fopen(argv[1], "rb+");
	if (ramDrive == 0)
		return 1;
	fseek(ramDrive,0,SEEK_SET);

	FILE *fp = fopen(argv[2], "rb+");
	if (fp == 0)
		return 1;

	struct fileInfoStruct
	{
		char name[32];
		uint32_t size;
		uint32_t type;	//	0=file,1=directory
		uint32_t fileInfoStruct_next;
		uint32_t fileInfoStruct_child;
	};

	//	Initilize and fill the fileinfostructure.
	struct fileInfoStruct myFile;
	memset(&myFile, 0, sizeof(myFile));
	memcpy(myFile.name, argv[2], (strlen(argv[2]) < 31) ? strlen(argv[2]) + 1 : 31);
	if (fseek(fp, 0, SEEK_END))
		return 1;
	myFile.size = ftell(fp);
	if (fseek(fp, 0, SEEK_SET))
		return 1;
	myFile.type = 0;
	myFile.fileInfoStruct_child = 0;
	myFile.fileInfoStruct_next = 0;

	struct fileInfoStruct currentFile;
	int fileInfoStructOffset = -1;

	//	Loop through files and append at end.
	int bytes_read = 0;
	do
	{
		bytes_read = fread(&currentFile, sizeof(struct fileInfoStruct), 1, ramDrive);

		if (bytes_read)
		{
			fileInfoStructOffset = ftell(ramDrive) - sizeof(struct fileInfoStruct);
			printf("Current File:%s,File size:%d", currentFile.name, currentFile.size);
			fseek(ramDrive, ((ftell(ramDrive) + currentFile.size + 511) / 512) * 512, SEEK_SET);
			printf("(Next file at %d)\n", ftell(ramDrive));
			if (strcmp(currentFile.name, myFile.name) == 0)
			{
				printf("Overwrite not supported. file[%s]", myFile.name);
				return 1;
			}
		}
	}
	while (bytes_read);

	printf("Reached end of file.Writing stuff\n");
	//	Write out the fileInfoStruct
	uint32_t fileInfoStruct_next = ftell(ramDrive);
	if (fwrite(&myFile, sizeof(struct fileInfoStruct), 1, ramDrive) != 1)
		return 1;
	for (int i = 0; i < myFile.size; i += 512)
	{
		char data[512];
		memset(data,0,512);
		int dataRead = fread(data, 1, 512, fp);
		fwrite(data, 1, dataRead, ramDrive);
	}
	int count = (((ftell(ramDrive) + 511) / 512) * 512) - ftell(ramDrive);

	//	Make it 512 aligned.
	for (int i = 0; i < count; ++i)
		fwrite((void*) &i, 1, 1, ramDrive);

	if (fileInfoStructOffset >= 0)
	{
		fflush(ramDrive);
		if (fseek(ramDrive, fileInfoStructOffset, SEEK_SET))
			return 1;

		printf("location %d\n", ftell(ramDrive));
		if (fwrite(&currentFile, sizeof(struct fileInfoStruct), 1, ramDrive) != 1)
			return 1;
		printf("Updated previous file ptr %d.New pos %d\n", fileInfoStructOffset, ftell(ramDrive));
	}
	else
		printf("New file,no need to updated previous file ptr.\n");
	printf("Done\n");
	fclose(ramDrive);
	fclose(fp);
	return 0;
}
