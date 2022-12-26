#pragma once
typedef int(_cdecl* forReadData)(struct users*);

struct users
{
	char* surname;
	char* name;
	char* midname;
	int age;
};