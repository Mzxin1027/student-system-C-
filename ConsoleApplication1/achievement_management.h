#ifndef ACHIEVEMENT_MANAGEMENT_H
#define ACHIEVEMENT_MANAGEMENT_H
#include "utils.h"
#include "score_management.h"
__declspec(selectany) char* achievementFilename = "achieve.txt";
typedef struct AchievementInfo {
    char studentId[20]; // 学生ID
    char studentName[50]; // 学生姓名
    char achievementType; // 成就类型，例如 'A' 或 'B'
    char advisorName[50]; // 指导老师姓名
    char achievementName[100]; // 成就名
} AchievementInfo;

// 成果信息链表节点

typedef struct AchievementNode {
    AchievementInfo data;
    struct AchievementNode* next;
    char achievementType[50]; // 成果类型，如SCI、EI等
    char achievementLevel[50]; // 成果等级，如国家级、省级等
    char teacherName[50]; // 指导教师姓名
    int achievementCount; // 成果数量
} AchievementNode;

typedef struct {
    ScoreInfo scoreInfo; // 原有的成绩信息
    AchievementInfo achievementInfo; // 新增的素质成绩信息
    TransferInfo transferInfo; // 新增的转专业信息
} ExtendedInfo;

__declspec(selectany) AchievementNode* achievementHead = NULL; // 成果信息链表头指针


void AddNewAchievementRecord();
void QueryAchievementsByStudentId();
void SaveAchievementsToFile();
void LoadAchievementsFromFile();
void StatAchievements(); 
float  CalculateAchievementBonus(char* studentId);
void QueryAllAchievements();
void ModifyAchievementRecord();
void DeleteAchievementRecord();
void DeleteAllAchievementsForStudent(const char* studentId);

#endif
