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
    char major[100]; // רҵ
    char class[50]; // �༶
} TransferInfo;

// ��ѧ����Ϣ�ṹ����չ�԰������ʳɼ���תרҵ��Ϣ

typedef struct ScoreInfo {
    char college[100];
    char major[100];
    char studentId[20];
    char name[50];
    char courseName[100];
    float score;
    float gpa;
    float credit;
    int rank; // ע�⣺ԭʼ�����в�û����ScoreInfo�ж���rank��������QueryStudentInfo�г��Դ�ӡ�������������������ֶΡ�
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
    CollegeStudentGPA* studentHead; // ָ���ѧԺѧ��GPA��Ϣ�����ͷָ��
    struct CollegeGPAList* next;
} CollegeGPAList;

// ȫ�ֱ���

// ����ڵ�
typedef struct Node {
    ScoreInfo data;
    struct Node* next;
} Node;

__declspec(selectany) Node* head = NULL; // ����ͷָ��
// ����Ŀ��Ϣ�ṹ��
typedef struct {
    char projectId[20]; // ��ĿID
    char studentId[20]; // ����ѧ����ѧ��
    float bonusPoints; // ����Ŀ�ļӷֵ���
} InnovationProjectInfo;

// ����Ŀ��Ϣ����ڵ�
typedef struct InnovationProjectNode {
    InnovationProjectInfo data;
    struct InnovationProjectNode* next;
} InnovationProjectNode;
typedef struct {
    char studentId[20];
    char name[50];
    float totalScore; // ѧ���ܷ�
    int scoreCount; // ѧ���ɼ����������ڼ���ƽ����
    float gpa; // �洢�������ƽ��GPA
} StudentScore;

typedef struct {
    char collegeName[100];
    StudentScore students[MAX_STUDENTS]; // ����һ��ѧԺ�����MAX_STUDENTS��ѧ��
    int studentCount; // ʵ��ѧ������
} CollegeScore;


__declspec(selectany) InnovationProjectNode* innovationHead = NULL; // ����Ŀ��Ϣ����ͷָ��
typedef struct StudentGPAInfo {
    char studentId[20];
    float totalWeightedGPA; // �ܼ�ȨGPA�������Ƴɼ�����ѧ�ֵ��ܺ�
    float totalCredits; // ��ѧ��
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

// ����CollegeMajorPair�ṹ���uniqueCollegeMajorPairsȫ�ֱ����Ķ������£�
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
    char appealinfo;//�����ж������Ƿ�ɹ�
}AppealsInfo;

typedef struct node {
    AppealsInfo data;
    struct node* next;
}node;
typedef struct {
    char college[100];
    char major[100];
    char scores[10000]; // �������гɼ����ݵ��ַ�����ʾ�������������
} CollegeMajorScores;

typedef struct CollegeMajorScoreEntry {
    char college[100];
    char major[100];
    char scores[10000]; // �������гɼ����ݵ��ַ�����ʾ�������������
    struct CollegeMajorScoreEntry* next;
} CollegeMajorScoreEntry;
__declspec(selectany)node* appealshead = NULL;//���߳ɼ���Ϣ����ڵ�
__declspec(selectany) const char* filename = "score.txt";
__declspec(selectany) CollegeMajorScoreEntry* cmsHead = NULL; // ȫ�ֱ�����ָ�������ͷ��



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


// �������������ڱȽ�����ѧ����GPA�������������㷨
int CompareStudentGPA(const void* a, const void* b);

// ���������������ͷ�ѧԺרҵѧ��GPA��Ϣ����
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

