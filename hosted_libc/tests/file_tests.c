#include <stdio.h>
#include <errno.h>
int main()
{
    FILE *fp = fopen("test.txt", "w");
    if (fp == NULL)
    {
        printf("Failed to open file! errno [%d]\n", errno);
        return errno;
    }
    else
    {
        printf("File opened successfuly!\n");
    }
    return 0;
}