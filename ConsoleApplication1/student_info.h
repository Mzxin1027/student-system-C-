#ifndef student_info_H
#define student_info_H
#include "utils.h"
#include <stdlib.h>  
#include<string.h>
typedef struct StudentInfo {
    char studentId[20];
    char name[50];
    char collegeName[100]; // ����ѧԺ���ֶ�
    char majorName[100];   // ����רҵ���ֶ�
} StudentInfo;

typedef struct StudentNode {
    StudentInfo info;
    struct StudentNode* next;
} StudentNode;

typedef struct {
    char collegeName[100]; // ѧԺ��
    char majorName[100];   // רҵ��
    char courseName[100];  // �γ���
    char courseCode[20];   // �γ̴���
    float credits;         // ѧ��
} CourseInfo;
typedef struct CourseNode {
    CourseInfo info;
    struct CourseNode* next;
} CourseNode;


__declspec(selectany) StudentNode* heads = NULL; // ȫ��ͷָ�룬��ʼʱ����Ϊ��
__declspec(selectany) CourseNode* headc = NULL; // ȫ�ֱ�����ָ��γ���Ϣ�����ͷ��

void AddStudentInfoToList();
void SaveStudentInfoToFile();
void LoadStudentInfoFromFile();
void QueryStudentInfoById(const char* studentId);
void UpdateStudentInfoById(const char* studentId);
int CheckCollegeAndMajorExists(const char* collegeName, const char* majorName);
void DeleteStudentInfoById(const char* studentId);
void AddCourseInfoToList();
void SaveCourseInfoToFile();
void LoadCourseInfoFromFile();
void UpdateCourseInfo();
void QueryAllCourseInfo();
void QueryCourseInfoByCollege();


void DeleteCourseInfo();
#endif
