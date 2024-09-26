#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> // 引入数学库，用于计算标准差等

void PrintMenu() {

    time_t now;
    struct tm* now_tm;
    char buf[64];
    now = time(NULL);
    now_tm = localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", now_tm); // 修改这里，只显示年月日

    printf("|-----------------------------------------------|\n");
    printf("|              素质类成绩管理系统               |\n");
    printf("|-------------当前时间%s----------------|\n", buf);
    printf("|         1. 登录                               |\n");
    printf("|         2. 注册                               |\n");
    printf("|         3. 忘记密码                           |\n");
    printf("|         4. 退出                               |\n");
    printf("|-----------------------------------------------|\n\n");
    printf("请输入您的选择: ");

}
void GetPassword(char* pwd, int maxLen) {
    int i = 0;
    char ch;

    while ((ch = _getch()) != '\r' && i < maxLen - 1) {
        if (ch == '\b' && i > 0) { // 处理退格
            printf("\b \b");
            i--;
        }
        else if (ch >= 32 && ch <= 126) { // 可打印字符
            pwd[i++] = ch;
            printf("*");
        }
        }
    pwd[i] = '\0'; // 确保字符串以null结尾

    printf("\n");
    }
void LogOperation(const char* username, const char* operation) {
    FILE* file = fopen("memory.txt", "a"); // 以追加模式打开文件
    if (!file) {
        printf("无法打开memory.txt文件进行写入。\n");
        return;
    }

    // 获取当前时间作为操作时间
    time_t now = time(NULL);
    char* timeStr = ctime(&now);
    timeStr[strlen(timeStr) - 1] = '\0'; // 移除ctime返回的字符串末尾的换行符

    // 写入操作记录，包括用户名称
    fprintf(file, "[%s] %s: %s\n", timeStr, username, operation);
    fclose(file);
}
void ShowOperationLogs() {
    printf("显示所有操作记录：\n");
    FILE* file = fopen("memory.txt", "r");
    if (!file) {
        printf("无法打开memory.txt文件进行读取。\n");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }

    fclose(file);
}

