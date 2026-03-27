# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char szFilePath[256];
    FILE* pFile;
    char szLine[256];

    printf("Introduceti path-ul fisierului: ");
    fgets(szFilePath, sizeof(szFilePath), stdin);

    
    szFilePath[strcspn(szFilePath, "\n")] = 0;

  
    pFile = fopen(szFilePath, "r");

    if (pFile == NULL)
    {
        printf("Eroare: fisierul nu exista sau nu poate fi deschis.\n");
        return 1;
    }

    while (fgets(szLine, sizeof(szLine), pFile) != NULL)
    {
        printf("%s", szLine);
    }

    fclose(pFile);

    return 0;
}