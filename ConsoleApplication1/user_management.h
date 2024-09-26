#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "utils.h"
#include <stdio.h>
#include<Windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <math.h>
typedef struct {
    char username[50];
    char password[50];
    char email[100];
    char role; // 's' for student, 'a' for admin
} UserInfo;
typedef struct Nodeu {
    UserInfo user;
    struct Nodeu* next;
} Nodeu;
Nodeu* headu = NULL; // 链表头指针
Nodeu* CreateUserNodeu(const char* username, const char* password, const char* email, char role);
const char* filenameu = "user.txt"; // 用户信息文件名

void Register();
void Login();
void ForgotPassword();
void SaveToFile();
void LoadFromFile();
void ChangePassword(const char* username);
void LoadCurrentUserRole();
void AddUserToList(Nodeu** head, Nodeu* newUser);
void ChangePassword(const char* username);
void GetPassword(char* pwd, int maxLen);
void StudentMenu(const char* username);
void AdminMenu(const char* username);
void PrintStudentMenu();
void PrintAdminMenu();
char currentUserRole;
void LoadCurrentUserRole();
void JudgeRole();
void AdminStudentinfoMenu(const char* username);
void AdminScoreManagementMenu(const char* username);
void AdminAchievementManagementMenu(const char* username);
void AdminStudentinfoMenu(const char* username);
void PrintSystemManagementMenu();
void AdminSystemManagementMenu(const char* username);
void UpdateStudentPassword();
void AdminCourseManagementMenu(const char* username);
void QueryAllStudentInfo();
void PrintStudentMenu() {
    printf("\n========================================\n");
    printf("                学生菜单\n");
    printf("========================================\n");
    printf("1. 查询我的成绩\n");
    printf("2. 查询我的成就\n");
    printf("3. 申诉成绩\n");
   
   printf("4. 修改我的密码\n");
    printf("5. 退出\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}

void PrintAdminMenu() {
    printf("\n========================================\n");
    printf("                管理员菜单\n");
    printf("========================================\n");
    printf("1. 学生信息管理系统\n");
    printf("2. 课程信息管理系统\n");
    printf("3. 学生成绩管理系统\n");
    printf("4. 学生成就管理系统\n");
    printf("5. 系统操作管理系统\n");
    printf("6. 返回\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
void PrintStudentinfoMenu() {
    printf("\n=======================================\n");
    printf("         管理员学生信息管理菜单\n");
    printf("========================================\n");
    printf("1. 添加学院名、专业名信息\n");
    printf("2. 添加学生信息\n");
    printf("3. 查询学生信息\n");
    printf("4. 查询所有学生信息\n");
    printf("5. 修改学生信息\n");
    printf("6. 更新成绩中学生信息\n");
    printf("7. 删除学生信息\n");
    printf("8. 删除成绩中学生信息\n");
    printf("9. 返回\n");
   // printf("6. 退出\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
void PrintCourseinfoMenu() {
    printf("\n=======================================\n");
    printf("         管理员课程信息管理菜单\n");
    printf("========================================\n");

    printf("1. 添加课程信息\n");
    printf("2. 查询具体学院课程信息\n");
    printf("3. 修改具体学院课程信息\n");
    printf("4. 查询所有课程信息\n");
    printf("5. 删除指定课程信息\n");

    printf("6. 返回\n");
    // printf("6. 退出\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
void PrintScoreManagementMenu() {
    printf("\n========================================\n");
    printf("            管理员成绩管理菜单\n");
    printf("========================================\n");
    printf("1. 添加新的成绩记录\n");
    printf("2. 修改现有的成绩记录\n");
    printf("3. 查询所有学生成绩\n");
    printf("4. 查询学生成绩\n");
    printf("5. 删除学生成绩\n");
    printf("6. 按学院和专业生成排名文件\n");
    printf("7. 按学院分类生成平均GPA文件\n");
    printf("8. 按学院生成成绩文件\n");
    printf("9. 查询特定学院的成绩排名\n");
    printf("10. 查询特定学院的学生成绩\n");
    printf("11.分析课程成绩统计信息\n"); // 新增选项
    printf("12. 预测学生的学习情况\n");
    printf("13. 学生申诉情况查询\n");
    printf("14. 返回\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
void PrintAchievementManagementMenu() {
    printf("\n========================================\n");
    printf("            管理员成就管理菜单\n");
    printf("========================================\n");
    printf("1. 添加新的成就记录\n");
    printf("2. 查询所有学生的成就记录\n");
    printf("3. 查询指定学生的成就记录\n");
    printf("4. 修改指定学生的成就记录\n");
    printf("5. 删除指定学生的某个成就记录\n");
    printf("6. 删除指定学生的所有成就记录\n");
           
    printf("7. 返回\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
void PrintSystemManagementMenu() {
    printf("\n========================================\n");
    printf("            管理员系统管理菜单\n");
    printf("========================================\n");
    printf("1. 查看所有操作记录\n");
    printf("2. 修改指定学生密码\n");
    printf("3. 修改密码\n");
    printf("4. 返回\n");
    printf("----------------------------------------\n");
    printf("请输入您的选择: ");
}
#endif
