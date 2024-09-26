#ifndef student_info_H
#define student_info_H
#include "utils.h"
#include <stdlib.h>  
#include<string.h>
typedef struct StudentInfo {
    char studentId[20];
    char name[50];
    char collegeName[100]; // 新增学院名字段
    char majorName[100];   // 新增专业名字段
} StudentInfo;

typedef struct StudentNode {
    StudentInfo info;
    struct StudentNode* next;
} StudentNode;

typedef struct {
    char collegeName[100]; // 学院名
    char majorName[100];   // 专业名
    char courseName[100];  // 课程名
    char courseCode[20];   // 课程代码
    float credits;         // 学分
} CourseInfo;
typedef struct CourseNode {
    CourseInfo info;
    struct CourseNode* next;
} CourseNode;


__declspec(selectany) StudentNode* heads = NULL; // 全局头指针，初始时链表为空
__declspec(selectany) CourseNode* headc = NULL; // 全局变量，指向课程信息链表的头部

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
