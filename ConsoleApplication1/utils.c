#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> // ������ѧ�⣬���ڼ����׼���

void PrintMenu() {

    time_t now;
    struct tm* now_tm;
    char buf[64];
    now = time(NULL);
    now_tm = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", now_tm); // �޸����ֻ��ʾ������

    printf("|-----------------------------------------------|\n");
    printf("|              ������ɼ�����ϵͳ               |\n");
    printf("|-------------��ǰʱ��%s----------------|\n", buf);
    printf("|         1. ��¼                               |\n");
    printf("|         2. ע��                               |\n");
    printf("|         3. ��������                           |\n");
    printf("|         4. �˳�                               |\n");
    printf("|-----------------------------------------------|\n\n");
    printf("����������ѡ��: ");

}
void GetPassword(char* pwd, int maxLen) {
    int i = 0;
    char ch;

    while ((ch = _getch()) != '\r' && i < maxLen - 1) {
        if (ch == '\b' && i > 0) { // �����˸�
            printf("\b \b");
            i--;
        }
        else if (ch >= 32 && ch <= 126) { // �ɴ�ӡ�ַ�
            pwd[i++] = ch;
            printf("*");
        }
        }
    pwd[i] = '\0'; // ȷ���ַ�����null��β

    printf("\n");
    }
void LogOperation(const char* username, const char* operation) {
    FILE* file = fopen("memory.txt", "a"); // ��׷��ģʽ���ļ�
    if (!file) {
        printf("�޷���memory.txt�ļ�����д�롣\n");
        return;
    }

    // ��ȡ��ǰʱ����Ϊ����ʱ��
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0'; // �Ƴ�ctime���ص��ַ���ĩβ�Ļ��з�

    // д�������¼�������û�����
    fprintf(file, "[%s] %s: %s\n", timeStr, username, operation);
    fclose(file);
}
void ShowOperationLogs() {
    printf("��ʾ���в�����¼��\n");
    FILE* file = fopen("memory.txt", "r");
    if (!file) {
        printf("�޷���memory.txt�ļ����ж�ȡ��\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
}

