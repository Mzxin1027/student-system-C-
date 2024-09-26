#ifndef SCORE_MANAGEMENT_H
#define SCORE_MANAGEMENT_H
#include "utils.h"
#define MAX_STUDENTS 1000
#define MAX_COLLEGES 100
#define MAX_LINE_LENGTH 256
#include <stdbool.h>

typedef struct stugpa{
    char college[50];
    char major[50];
    char studentID[20];
    float gpa;
}stugpa;

typedef struct Node1 {
    stugpa data;
    struct Node1* next;
} Node1;
__declspec(selectany) Node1* gpahead = NULL;


typedef struct {
    char major[100]; // 专业
    char class[50]; // 班级
} TransferInfo;

// 将学生信息结构体扩展以包含素质成绩和转专业信息

typedef struct ScoreInfo {
    char college[100];
    char major[100];
    char studentId[20];
    char name[50];
    char courseName[100];
    float score;
    float gpa;
    float credit;
    int rank; // 注意：原始代码中并没有在ScoreInfo中定义rank，但是在QueryStudentInfo中尝试打印它。这里我们添加这个字段。
} ScoreInfo;

typedef struct CollegeStudentGPA {
    char studentId[20];
    char name[50];
    char major[100];
    float totalGPA;
    int scoreCount;
    struct CollegeStudentGPA* next;
} CollegeStudentGPA;

typedef struct CollegeGPAList {
    char collegeName[100];
    CollegeStudentGPA* studentHead; // 指向该学院学生GPA信息链表的头指针
    struct CollegeGPAList* next;
} CollegeGPAList;

// 全局变量

// 链表节点
typedef struct Node {
    ScoreInfo data;
    struct Node* next;
} Node;

__declspec(selectany) Node* head = NULL; // 链表头指针
// 大创项目信息结构体
typedef struct {
    char projectId[20]; // 项目ID
    char studentId[20]; // 参与学生的学号
    float bonusPoints; // 大创项目的加分点数
} InnovationProjectInfo;

// 大创项目信息链表节点
typedef struct InnovationProjectNode {
    InnovationProjectInfo data;
    struct InnovationProjectNode* next;
} InnovationProjectNode;
typedef struct {
    char studentId[20];
    char name[50];
    float totalScore; // 学生总分
    int scoreCount; // 学生成绩数量，用于计算平均分
    float gpa; // 存储计算出的平均GPA
} StudentScore;

typedef struct {
    char collegeName[100];
    StudentScore students[MAX_STUDENTS]; // 假设一个学院最多有MAX_STUDENTS个学生
    int studentCount; // 实际学生数量
} CollegeScore;


__declspec(selectany) InnovationProjectNode* innovationHead = NULL; // 大创项目信息链表头指针
typedef struct StudentGPAInfo {
    char studentId[20];
    float totalWeightedGPA; // 总加权GPA，即各科成绩乘以学分的总和
    float totalCredits; // 总学分
    char name[50];
    float gpa;
    struct StudentGPAInfo* next;
} StudentGPAInfo;
typedef struct {
    char studentId[20];
    char name[50];
    char major[100];
    float gpa;
} StudentGPA;

// 假设CollegeMajorPair结构体和uniqueCollegeMajorPairs全局变量的定义如下：
typedef struct CollegeMajorPair {
    char college[100];
    char major[100];
    StudentGPAInfo* studentGPAHead;
    struct CollegeMajorPair* next;
} CollegeMajorPair;

__declspec(selectany) CollegeMajorPair* uniqueCollegeMajorPairs = NULL;

typedef struct StuScoreAppealsInfo {
    char college[100];
    char major[100];
    char studentId[20];
    char name[50];
    char courseName[100];
    char appealinfo;//用于判断申诉是否成功
}AppealsInfo;

typedef struct node {
    AppealsInfo data;
    struct node* next;
}node;
typedef struct {
    char college[100];
    char major[100];
    char scores[10000]; // 假设所有成绩数据的字符串表示不超过这个长度
} CollegeMajorScores;

typedef struct CollegeMajorScoreEntry {
    char college[100];
    char major[100];
    char scores[10000]; // 假设所有成绩数据的字符串表示不超过这个长度
    struct CollegeMajorScoreEntry* next;
} CollegeMajorScoreEntry;
__declspec(selectany)node* appealshead = NULL;//申诉成绩信息链表节点
__declspec(selectany) const char* filename = "score.txt";
__declspec(selectany) CollegeMajorScoreEntry* cmsHead = NULL; // 全局变量，指向链表的头部



void AddStudentToCollegeMajorList(CollegeMajorScoreEntry** head, ScoreInfo* studentInfo);
void SaveScoresToFile();
void LoadScoresFromFile();
void AddNewScoreRecord();
void ModifyScoreRecord();

void UpdateTransferInfo();

void AddNewAchievementRecord();

void StatAchievements();
void CalculateAndAssignRanks();
int Partition(Node** nodesArray, int low, int high);
void QuickSort(Node** nodesArray, int low, int high);
void UpdateRanks(Node** nodesArray, int count);
void CalculateAndAssignRanks();
void GenerateRankingFile();
int Partition(Node** nodesArray, int low, int high);
float ConvertScoreToGPA(float score);
void SaveAchievementsToFile();
void CalculateAverageGPA();
void SaveScoresByCollege();
void AddNewInnovationProject();
float CalculateInnovationBonus(char* studentId);

void SaveInnovationProjectsToFile();
void LoadInnovationProjectsFromFile();
void QueryStudentInfo();
void PrintStudentMenu();
void QueryScoreByStudentId(const char* studentId);
void StudentMenu(const char* username);
void SaveGPAByCollege();
void LogOperation(const char* username, const char* operation);

void ShowOperationLogs();
void QueryScoresByCollege();
void SaveScoresByCollegeToFile();
void UpdateCollegeGPAList();
void FreeCollegeGPAList(CollegeGPAList* head);
int isNormalDistribution();
void AnalyzeCourseStatistics(const char* courseName);
void CenterConsoleWindow();
void LoadCollegesFromScores();
void CalculateAndRankCollegeGPA(const char* collegeName);
int compareGPA();
void QueryScoreByStudentId(const char* studentId);
void CalculateAndSaveGPAByCollegeAndMajor();
//StudentGPAInfo* FindOrCreateStudentGPAInfo(StudentGPAInfo** head, const char* studentId);
void FreeStudentGPAInfoList(StudentGPAInfo* head);
void SortAndSaveGPA(const char* inputFilename);
int existsInList(const char* college, const char* major);

void extractCollegeMajorPairs();
void RankAllGPAFiles();
void PredictPostgraduateOpportunity(const char* studentId);
void CalculateGPA(const char* studentId, float* averageGPA, int* courseCount);
void AssessAchievements(const char* studentId, int* hasHighLevelAchievements, int* totalAchievementCount);
void addToList(const char* college, const char* major);
void loadCollegeMajorPairs();
void saveCollegeMajorPairs(); 
void DeleteAllScoresForStudent(const char* studentId);
void DeleteScoreRecord(const char* studentId, const char* courseName);
void QueryAllAchievements();

void QueryRankingByCollegeAndMajor(const char* filename);
void QueryMajorRankingProcess();
int isStudentIdProcessed(const char studentIds[][20], int size, const char* studentId);
void QueryAllScores();
void QueryAllCourseInfo();


// 辅助函数，用于比较两个学生的GPA，适用于排序算法
int CompareStudentGPA(const void* a, const void* b);

// 辅助函数，用于释放学院专业学生GPA信息链表
void FreeCollegeMajorPairList(CollegeMajorPair* head);
float ConvertScoreToGPA(float score);
void loadCollegeMajorPairs();
void saveCollegeMajorPairs();
void College_option(int* i, char college[][100]);
void Major_option(int* j, int option1, const char college[][100], char major[][100]);
void addappealsInfoTofile();
void loadappealsInfoTofile();
void saveappealsInfoTofile();
void course_option(int* i, const char college[50], const char major[50], char course[100][50], float credits[50]);


void sortGPAbycollegeAndmajor();

#endif

