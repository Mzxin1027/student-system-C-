
#include "utils.h"

int main() {
    // 初始化和加载数据
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
        case 4: printf("退出。\n"); break;
        default: printf("错误选择，请重试。\n");
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
