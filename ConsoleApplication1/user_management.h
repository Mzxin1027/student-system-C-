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
Nodeu* headu = NULL; // ����ͷָ��
Nodeu* CreateUserNodeu(const char* username, const char* password, const char* email, char role);
const char* filenameu = "user.txt"; // �û���Ϣ�ļ���

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
    printf("                ѧ���˵�\n");
    printf("========================================\n");
    printf("1. ��ѯ�ҵĳɼ�\n");
    printf("2. ��ѯ�ҵĳɾ�\n");
    printf("3. ���߳ɼ�\n");
   
   printf("4. �޸��ҵ�����\n");
    printf("5. �˳�\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}

void PrintAdminMenu() {
    printf("\n========================================\n");
    printf("                ����Ա�˵�\n");
    printf("========================================\n");
    printf("1. ѧ����Ϣ����ϵͳ\n");
    printf("2. �γ���Ϣ����ϵͳ\n");
    printf("3. ѧ���ɼ�����ϵͳ\n");
    printf("4. ѧ���ɾ͹���ϵͳ\n");
    printf("5. ϵͳ��������ϵͳ\n");
    printf("6. ����\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
void PrintStudentinfoMenu() {
    printf("\n=======================================\n");
    printf("         ����Աѧ����Ϣ����˵�\n");
    printf("========================================\n");
    printf("1. ���ѧԺ����רҵ����Ϣ\n");
    printf("2. ���ѧ����Ϣ\n");
    printf("3. ��ѯѧ����Ϣ\n");
    printf("4. ��ѯ����ѧ����Ϣ\n");
    printf("5. �޸�ѧ����Ϣ\n");
    printf("6. ���³ɼ���ѧ����Ϣ\n");
    printf("7. ɾ��ѧ����Ϣ\n");
    printf("8. ɾ���ɼ���ѧ����Ϣ\n");
    printf("9. ����\n");
   // printf("6. �˳�\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
void PrintCourseinfoMenu() {
    printf("\n=======================================\n");
    printf("         ����Ա�γ���Ϣ����˵�\n");
    printf("========================================\n");

    printf("1. ��ӿγ���Ϣ\n");
    printf("2. ��ѯ����ѧԺ�γ���Ϣ\n");
    printf("3. �޸ľ���ѧԺ�γ���Ϣ\n");
    printf("4. ��ѯ���пγ���Ϣ\n");
    printf("5. ɾ��ָ���γ���Ϣ\n");

    printf("6. ����\n");
    // printf("6. �˳�\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
void PrintScoreManagementMenu() {
    printf("\n========================================\n");
    printf("            ����Ա�ɼ�����˵�\n");
    printf("========================================\n");
    printf("1. ����µĳɼ���¼\n");
    printf("2. �޸����еĳɼ���¼\n");
    printf("3. ��ѯ����ѧ���ɼ�\n");
    printf("4. ��ѯѧ���ɼ�\n");
    printf("5. ɾ��ѧ���ɼ�\n");
    printf("6. ��ѧԺ��רҵ���������ļ�\n");
    printf("7. ��ѧԺ��������ƽ��GPA�ļ�\n");
    printf("8. ��ѧԺ���ɳɼ��ļ�\n");
    printf("9. ��ѯ�ض�ѧԺ�ĳɼ�����\n");
    printf("10. ��ѯ�ض�ѧԺ��ѧ���ɼ�\n");
    printf("11.�����γ̳ɼ�ͳ����Ϣ\n"); // ����ѡ��
    printf("12. Ԥ��ѧ����ѧϰ���\n");
    printf("13. ѧ�����������ѯ\n");
    printf("14. ����\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
void PrintAchievementManagementMenu() {
    printf("\n========================================\n");
    printf("            ����Ա�ɾ͹���˵�\n");
    printf("========================================\n");
    printf("1. ����µĳɾͼ�¼\n");
    printf("2. ��ѯ����ѧ���ĳɾͼ�¼\n");
    printf("3. ��ѯָ��ѧ���ĳɾͼ�¼\n");
    printf("4. �޸�ָ��ѧ���ĳɾͼ�¼\n");
    printf("5. ɾ��ָ��ѧ����ĳ���ɾͼ�¼\n");
    printf("6. ɾ��ָ��ѧ�������гɾͼ�¼\n");
           
    printf("7. ����\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
void PrintSystemManagementMenu() {
    printf("\n========================================\n");
    printf("            ����Աϵͳ����˵�\n");
    printf("========================================\n");
    printf("1. �鿴���в�����¼\n");
    printf("2. �޸�ָ��ѧ������\n");
    printf("3. �޸�����\n");
    printf("4. ����\n");
    printf("----------------------------------------\n");
    printf("����������ѡ��: ");
}
#endif
