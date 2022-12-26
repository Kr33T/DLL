#pragma once
__declspec(dllimport) int myFunc(LPWSTR str);
__declspec(dllimport) void readData(struct users* people);
__declspec(dllimport) void searchSurname(struct users* people);
__declspec(dllimport) void writeToFile(struct users* currentPeople, int countCurrentPeople);
__declspec(dllimport) void writeToFileIvanova(struct users* currentPeople, int countCurrentPeople);
__declspec(dllimport) void searchSurnameIvanova(struct users* people);



struct users
{
    char* surname;
    char* name;
    char* midname;
    int age;
};