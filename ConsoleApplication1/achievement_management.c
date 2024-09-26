#include "achievement_management.h"
#include "student_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void LoadAchievementsFromFile() {
    achievementHead = NULL;
    FILE* file = fopen(achievementFilename, "r");
    if (!file) {
        printf("未找到现有的成就数据。\n");
        return;
    }
    AchievementInfo info;
    while (fscanf(file, "%19[^,],%49[^,],%c,%49[^,],%99[^\n]\n",
        info.studentId, info.studentName, &info.achievementType,
        info.advisorName, info.achievementName) == 5) {
        AchievementNode* newNode = (AchievementNode*)malloc(sizeof(AchievementNode));
        if (newNode == NULL) {
            printf("内存分配失败。\n");
            fclose(file);
            return;
        }
        newNode->data = info;
        newNode->next = achievementHead;
        achievementHead = newNode;
    }
    fclose(file);
}
void SaveAchievementsToFile() {
    FILE* file = fopen(achievementFilename, "w");
    if (!file) {
        printf("打开文件失败。\n");
        return;
    }
    AchievementNode* current = achievementHead;
    while (current != NULL) {
        fprintf(file, "%s,%s,%c,%s,%s\n",
            current->data.studentId, current->data.studentName, current->data.achievementType,
            current->data.advisorName, current->data.achievementName);
        current = current->next;
    }
    fclose(file);
}
void AddNewAchievementRecord() {
    LoadAchievementsFromFile();
    AchievementInfo newAchievement;
    LoadStudentInfoFromFile();
    int isValidScore;
    int p;
    do {
        StudentNode* record = heads;
        p = 1; printf("请输入学号: ");

        scanf("%19s", newAchievement.studentId);
        isValidScore = 1; // 假设用户名有效
        while (record != NULL) {
            if (strcmp(record->info.studentId, newAchievement.studentId) == 0) {
                strcpy(newAchievement.studentName, record->info.name);
                p = 0;
                printf("成功输入\n");
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            isValidScore = 0;
            printf("学号不存在，请重新输入:\n");
        }
    } while (isValidScore == 0);

    printf("请输入指导老师姓名: ");
    scanf(" %[^\n]", newAchievement.advisorName);
    printf("请输入成就名: ");
    scanf(" %[^\n]", newAchievement.achievementName);
    int isValidType = 0;
    while (!isValidType) {
        printf("请输入成就类型 (A/B): ");
        scanf(" %c", &newAchievement.achievementType);
        if (newAchievement.achievementType == 'A' || newAchievement.achievementType == 'B') {
            isValidType = 1;
        }
        else {
            printf("无效的成就类型，请输入 A 或 B。\n");
        }
    }
    AchievementNode* newNode = (AchievementNode*)malloc(sizeof(AchievementNode));
    if (newNode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    newNode->data = newAchievement;
    newNode->next = achievementHead;
    achievementHead = newNode;

    SaveAchievementsToFile(); // 保存更新后的链表到文件
    printf("新的成就记录添加成功。\n");
}

void StatAchievements() {
    printf("成果分布:\n");
    AchievementNode* current = achievementHead;
    int typeACount = 0, typeBCount = 0;
    while (current != NULL) {
        if (current->data.achievementType == 'A') {
            typeACount++;
        }
        else if (current->data.achievementType == 'B') {
            typeBCount++;
        }
        current = current->next;
    }
    printf("类型 A 成果: %d\n", typeACount);
    printf("类型 B 成果: %d\n", typeBCount);
}

void QueryAchievementsByStudentId(const char* studentId) {
    // 假设achievementHead可能未初始化或数据未加载，因此首先尝试加载数据

    AchievementNode* current = achievementHead;
    int found = 0;
    printf("学生学号 %s 的成就如下：\n", studentId);
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // 显示学生姓名和成就信息
            printf("学生姓名: %s, 成果类型: %c, 指导教师: %s, 成就名: %s\n",
                current->data.studentName,
                current->data.achievementType,
                current->data.advisorName,
                current->data.achievementName);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("未找到学生 %s 的成就记录。\n", studentId);
    }
}
float CalculateAchievementBonus(char* studentId) {
    AchievementNode* current = achievementHead;
    float bonus = 0.0;
    int hasTypeA = 0; // 标记是否有A类成就
    int hasTypeB = 0; // 标记是否有B类成就

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // 根据成就类型设置标记
            if (current->data.achievementType == 'A' && !hasTypeA) {
                bonus += 0.05; // A类加分0.05
                hasTypeA = 1; // 标记已加分，避免重复
            }
            else if (current->data.achievementType == 'B' && !hasTypeB) {
                bonus += 0.03; // B类加分0.03
                hasTypeB = 1; // 标记已加分，避免重复
            }
        }
        current = current->next;
    }

    return bonus;
}
void QueryAllAchievements() {
    // 确保在查询之前，成就数据已经加载到链表中
    // 如果成就数据已经在程序其他部分加载，这一步可能不需要


    AchievementNode* current = achievementHead;
    if (current == NULL) {
        printf("当前没有任何学生成就记录。\n");
        return;
    }

    printf("所有学生成就记录如下：\n");
    while (current != NULL) {
        // 打印每个学生的成就信息
        printf("学生学号: %s, 学生姓名: %s, 成果类型: %c, 指导教师: %s, 成就名: %s\n",
            current->data.studentId, current->data.studentName,
            current->data.achievementType, current->data.advisorName,
            current->data.achievementName);
        current = current->next;
    }
}
void ModifyAchievementRecord() {
    char studentId[20];
    char achievementName[100];
    char newAdvisorName[50];
    char newAchievementType;
    char newAchievementName[100];

    printf("请输入要修改成就记录的学生学号: ");
    scanf("%19s", studentId);
    printf("请输入要修改成就记录的成就名: ");
    scanf(" %[^\n]", achievementName);

    // 查找匹配的成就记录
    AchievementNode* current = achievementHead;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.achievementName, achievementName) == 0) {
            // 找到了要修改的成就记录
            printf("找到成就记录：%s, %s, %c, %s\n", current->data.studentName, current->data.achievementName, current->data.achievementType, current->data.advisorName);

            // 获取新的成就信息
            printf("请输入新的指导老师姓名: ");
            scanf(" %[^\n]", newAdvisorName);
            printf("请输入新的成就类型 (A/B): ");
            scanf(" %c", &newAchievementType);
            printf("请输入新的成就名: ");
            scanf(" %[^\n]", newAchievementName);

            // 更新成就记录
            strcpy(current->data.advisorName, newAdvisorName);
            current->data.achievementType = newAchievementType;
            strcpy(current->data.achievementName, newAchievementName);

            printf("成就记录更新成功。\n");
            SaveAchievementsToFile(); // 更新后保存更新到文件
            return;
        }
        current = current->next;
    }

    printf("未找到学号为 %s 且成就名为 %s 的成就记录，无法修改。\n", studentId, achievementName);
}
void DeleteAchievementRecord() {
    char studentId[20];
    char achievementName[100];

    printf("请输入要删除成就记录的学生学号: ");
    scanf("%19s", studentId);
    printf("请输入要删除成就记录的成就名: ");
    scanf(" %[^\n]", achievementName);

    AchievementNode* current = achievementHead;
    AchievementNode* previous = NULL;
    int found = 0; // 标记是否找到并删除了记录

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.achievementName, achievementName) == 0) {
            found = 1; // 标记找到记录
            // 如果找到了要删除的成就记录
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                achievementHead = current->next;
            }
            else {
                // 如果要删除的节点不是头节点
                previous->next = current->next;
            }
            free(current); // 释放节点内存
            printf("学生学号为 %s 的成就记录 %s 已被删除。\n", studentId, achievementName);
            SaveAchievementsToFile(); // 删除后保存更新到文件
            break; // 成功删除后退出循环
        }
        previous = current;
        current = current->next;
    }

    if (!found) {
        // 如果遍历完链表还没有找到，表示没有该学生的指定成就记录
        printf("未找到学号为 %s 且成就名为 %s 的成就记录，无法删除。\n", studentId, achievementName);
    }
}

void DeleteAllAchievementsForStudent(const char* studentId) {
    AchievementNode* current = achievementHead;
    AchievementNode* previous = NULL;
    int found = 0; // 标记是否找到并删除了至少一条记录

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // 如果找到了要删除的成就记录
            found = 1;
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                achievementHead = current->next;
                free(current); // 释放节点内存
                current = achievementHead; // 更新current为新的头节点
            }
            else {
                // 如果要删除的节点不是头节点
                previous->next = current->next;
                free(current); // 释放节点内存
                current = previous->next; // 继续遍历
            }
            continue; // 继续检查下一个节点，不更新previous
        }
        previous = current;
        current = current->next;
    }

    if (found) {
        printf("学生学号为 %s 的所有成就记录已被删除。\n", studentId);
        SaveAchievementsToFile(); // 删除操作后保存更新到文件
    }
    else {
        // 如果遍历完链表还没有找到，表示没有该学生的成就记录
        printf("未找到学号为 %s 的任何成就记录，无法删除。\n", studentId);
    }
}