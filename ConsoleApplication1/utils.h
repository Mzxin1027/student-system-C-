#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
void PrintMenu();
void GetPassword(char* pwd, int maxLen);
void LogOperation(const char* username, const char* operation);
void ShowOperationLogs();

#endif
