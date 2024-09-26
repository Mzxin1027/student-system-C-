#ifndef ACHIEVEMENT_MANAGEMENT_H
#define ACHIEVEMENT_MANAGEMENT_H
#include "utils.h"
#include "score_management.h"
__declspec(selectany) char* achievementFilename = "achieve.txt";
typedef struct AchievementInfo {
    char studentId[20]; // ѧ��ID
    char studentName[50]; // ѧ������
    char achievementType; // �ɾ����ͣ����� 'A' �� 'B'
    char advisorName[50]; // ָ����ʦ����
    char achievementName[100]; // �ɾ���
} AchievementInfo;

// �ɹ���Ϣ����ڵ�

typedef struct AchievementNode {
    AchievementInfo data;
    struct AchievementNode* next;
    char achievementType[50]; // �ɹ����ͣ���SCI��EI��
    char achievementLevel[50]; // �ɹ��ȼ�������Ҽ���ʡ����
    char teacherName[50]; // ָ����ʦ����
    int achievementCount; // �ɹ�����
} AchievementNode;

typedef struct {
    ScoreInfo scoreInfo; // ԭ�еĳɼ���Ϣ
    AchievementInfo achievementInfo; // ���������ʳɼ���Ϣ
    TransferInfo transferInfo; // ������תרҵ��Ϣ
} ExtendedInfo;

__declspec(selectany) AchievementNode* achievementHead = NULL; // �ɹ���Ϣ����ͷָ��


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
