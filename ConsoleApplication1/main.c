
#include "utils.h"

int main() {
    // ��ʼ���ͼ�������
    LoadScoresFromFile();
    LoadAchievementsFromFile();
    LoadFromFile();
    loadCollegeMajorPairs();
    LoadStudentInfoFromFile();
    LoadCourseInfoFromFile();
    int choice;
    do {
        PrintMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
        case 1: Login(); break;
        case 2: Register(); break;
        case 3: ForgotPassword(); break;
        case 4: printf("�˳���\n"); break;
        default: printf("����ѡ�������ԡ�\n");
        }
    } while (choice != 4);

    SaveToFile();
    SaveScoresToFile();
    SaveAchievementsToFile();
    SaveScoresByCollegeToFile();
    saveCollegeMajorPairs();
    SaveStudentInfoToFile();
    SaveCourseInfoToFile();
    return 0;
}
