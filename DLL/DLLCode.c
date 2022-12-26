#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "HeaderDLL.h"

#define READPATH L"Users.csv"

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        break;
    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;
    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;
    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

void readData(struct users* people)
{
    HANDLE fileStart;
    fileStart = CreateFile(READPATH,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (fileStart == INVALID_HANDLE_VALUE)
    {
        MessageBox(NULL, L"При открытии файла возникла ошибка!", L"Окно программы", MB_OK);
        return 0;
    }
    DWORD countFileSymbols;
    LPWSTR argumentsFromFile = calloc(3099, sizeof(CHAR));
    if (!ReadFile(fileStart, argumentsFromFile, 3099, &countFileSymbols, NULL))
    {
        MessageBox(NULL, L"При чтении данных из файла возникла ошибка!", L"Окно программы", MB_OK);
        return 0;
    }

    char* dataConvertToStruct = strtok(argumentsFromFile, ";");
    int countRecords;
    for (int i = 0; i < 100; i++)
    {
        countRecords = i;
        people[i].surname = dataConvertToStruct;
        dataConvertToStruct = strtok(NULL, ";\r\n");
        people[i].name = dataConvertToStruct;
        dataConvertToStruct = strtok(NULL, ";\r\n");
        people[i].midname = dataConvertToStruct;
        dataConvertToStruct = strtok(NULL, ";\r\n");
        people[i].age = atoi(dataConvertToStruct);
        dataConvertToStruct = strtok(NULL, ";\r\n");
    }
    CloseHandle(fileStart);

    searchSurname(people, "Иванов");
    searchSurnameIvanova(people);
}

void searchSurname(struct users* people, char* surnameForSearch)
{
    struct users* currentPeople = malloc(sizeof(struct users));
    struct users* forCurrentPeople;
    int j = 0;
    char* forComprasion = surnameForSearch;
    for (int i = 0; i < 100; i++)
    {
        if (strcmp(people[i].surname, forComprasion) == 0)
        {
            currentPeople[j] = people[i];
            forCurrentPeople = realloc(currentPeople, (j + 2) * sizeof(struct users));
            j++;
            currentPeople = forCurrentPeople;
        }
    }
    writeToFile(currentPeople, j);
}

void searchSurnameIvanova(struct users* people)
{
    struct users* currentPeople = malloc(sizeof(struct users));
    struct users* forCurrentPeople;
    int j = 0;
    char* forComprasion = "Иванова";
    for (int i = 0; i < 100; i++)
    {
        if (strcmp(people[i].surname, forComprasion) == 0)
        {
            currentPeople[j] = people[i];
            forCurrentPeople = realloc(currentPeople, (j + 2) * sizeof(struct users));
            j++;
            currentPeople = forCurrentPeople;
        }
    }
    writeToFileIvanova(currentPeople, j);
}

void writeToFile(struct users* currentPeople, int countCurrentPeople)
{
    HANDLE fileResult = CreateFile(L"Result.csv",
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    DWORD countFileSymbols;
    float averageAge = 0;
    char* dataForWritting = calloc(1000, sizeof(char));

    for (int i = 0; i < countCurrentPeople; i++)
    {
        sprintf(dataForWritting, "%s;%s;%s;%d\n", currentPeople[i].surname, currentPeople[i].name, currentPeople[i].midname, currentPeople[i].age);
        WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
        averageAge += currentPeople[i].age;
    }
    averageAge /= countCurrentPeople;
    sprintf(dataForWritting, "Средний возраст: %f", averageAge);
    WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
    free(dataForWritting);
    CloseHandle(fileResult);
}

void writeToFileIvanova(struct users* currentPeople, int countCurrentPeople)
{
    HANDLE fileResult = CreateFile(L"ResultIvanova.csv",
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    DWORD countFileSymbols;
    float averageAge = 0;
    char* dataForWritting = calloc(1000, sizeof(char));
    for (int i = 0; i < countCurrentPeople; i++)
    {
        sprintf(dataForWritting, "%s;%s;%s;%d\n", currentPeople[i].surname, currentPeople[i].name, currentPeople[i].midname, currentPeople[i].age);
        WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
        averageAge += currentPeople[i].age;
    }
    averageAge /= countCurrentPeople;
    sprintf(dataForWritting, "Средний возраст: %f", averageAge);
    WriteFile(fileResult, dataForWritting, strlen(dataForWritting), &countFileSymbols, NULL);
    free(dataForWritting);
    CloseHandle(fileResult);
}