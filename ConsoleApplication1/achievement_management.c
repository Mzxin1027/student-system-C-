#include "achievement_management.h"
#include "student_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void LoadAchievementsFromFile() {
    achievementHead = NULL;
    FILE* file = fopen(achievementFilename, "r");
    if (!file) {
        printf("δ�ҵ����еĳɾ����ݡ�\n");
        return;
    }
    AchievementInfo info;
    while (fscanf(file, "%19[^,],%49[^,],%c,%49[^,],%99[^\n]\n",
        info.studentId, info.studentName, &info.achievementType,
        info.advisorName, info.achievementName) == 5) {
        AchievementNode* newNode = (AchievementNode*)malloc(sizeof(AchievementNode));
        if (newNode == NULL) {
            printf("�ڴ����ʧ�ܡ�\n");
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
        printf("���ļ�ʧ�ܡ�\n");
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
        p = 1; printf("������ѧ��: ");

        scanf("%19s", newAchievement.studentId);
        isValidScore = 1; // �����û�����Ч
        while (record != NULL) {
            if (strcmp(record->info.studentId, newAchievement.studentId) == 0) {
                strcpy(newAchievement.studentName, record->info.name);
                p = 0;
                printf("�ɹ�����\n");
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            isValidScore = 0;
            printf("ѧ�Ų����ڣ�����������:\n");
        }
    } while (isValidScore == 0);

    printf("������ָ����ʦ����: ");
    scanf(" %[^\n]", newAchievement.advisorName);
    printf("������ɾ���: ");
    scanf(" %[^\n]", newAchievement.achievementName);
    int isValidType = 0;
    while (!isValidType) {
        printf("������ɾ����� (A/B): ");
        scanf(" %c", &newAchievement.achievementType);
        if (newAchievement.achievementType == 'A' || newAchievement.achievementType == 'B') {
            isValidType = 1;
        }
        else {
            printf("��Ч�ĳɾ����ͣ������� A �� B��\n");
        }
    }
    AchievementNode* newNode = (AchievementNode*)malloc(sizeof(AchievementNode));
    if (newNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    newNode->data = newAchievement;
    newNode->next = achievementHead;
    achievementHead = newNode;

    SaveAchievementsToFile(); // ������º�������ļ�
    printf("�µĳɾͼ�¼��ӳɹ���\n");
}

void StatAchievements() {
    printf("�ɹ��ֲ�:\n");
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
    printf("���� A �ɹ�: %d\n", typeACount);
    printf("���� B �ɹ�: %d\n", typeBCount);
}

void QueryAchievementsByStudentId(const char* studentId) {
    // ����achievementHead����δ��ʼ��������δ���أ�������ȳ��Լ�������

    AchievementNode* current = achievementHead;
    int found = 0;
    printf("ѧ��ѧ�� %s �ĳɾ����£�\n", studentId);
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // ��ʾѧ�������ͳɾ���Ϣ
            printf("ѧ������: %s, �ɹ�����: %c, ָ����ʦ: %s, �ɾ���: %s\n",
                current->data.studentName,
                current->data.achievementType,
                current->data.advisorName,
                current->data.achievementName);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("δ�ҵ�ѧ�� %s �ĳɾͼ�¼��\n", studentId);
    }
}
float CalculateAchievementBonus(char* studentId) {
    AchievementNode* current = achievementHead;
    float bonus = 0.0;
    int hasTypeA = 0; // ����Ƿ���A��ɾ�
    int hasTypeB = 0; // ����Ƿ���B��ɾ�

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // ���ݳɾ��������ñ��
            if (current->data.achievementType == 'A' && !hasTypeA) {
                bonus += 0.05; // A��ӷ�0.05
                hasTypeA = 1; // ����Ѽӷ֣������ظ�
            }
            else if (current->data.achievementType == 'B' && !hasTypeB) {
                bonus += 0.03; // B��ӷ�0.03
                hasTypeB = 1; // ����Ѽӷ֣������ظ�
            }
        }
        current = current->next;
    }

    return bonus;
}
void QueryAllAchievements() {
    // ȷ���ڲ�ѯ֮ǰ���ɾ������Ѿ����ص�������
    // ����ɾ������Ѿ��ڳ����������ּ��أ���һ�����ܲ���Ҫ


    AchievementNode* current = achievementHead;
    if (current == NULL) {
        printf("��ǰû���κ�ѧ���ɾͼ�¼��\n");
        return;
    }

    printf("����ѧ���ɾͼ�¼���£�\n");
    while (current != NULL) {
        // ��ӡÿ��ѧ���ĳɾ���Ϣ
        printf("ѧ��ѧ��: %s, ѧ������: %s, �ɹ�����: %c, ָ����ʦ: %s, �ɾ���: %s\n",
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

    printf("������Ҫ�޸ĳɾͼ�¼��ѧ��ѧ��: ");
    scanf("%19s", studentId);
    printf("������Ҫ�޸ĳɾͼ�¼�ĳɾ���: ");
    scanf(" %[^\n]", achievementName);

    // ����ƥ��ĳɾͼ�¼
    AchievementNode* current = achievementHead;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.achievementName, achievementName) == 0) {
            // �ҵ���Ҫ�޸ĵĳɾͼ�¼
            printf("�ҵ��ɾͼ�¼��%s, %s, %c, %s\n", current->data.studentName, current->data.achievementName, current->data.achievementType, current->data.advisorName);

            // ��ȡ�µĳɾ���Ϣ
            printf("�������µ�ָ����ʦ����: ");
            scanf(" %[^\n]", newAdvisorName);
            printf("�������µĳɾ����� (A/B): ");
            scanf(" %c", &newAchievementType);
            printf("�������µĳɾ���: ");
            scanf(" %[^\n]", newAchievementName);

            // ���³ɾͼ�¼
            strcpy(current->data.advisorName, newAdvisorName);
            current->data.achievementType = newAchievementType;
            strcpy(current->data.achievementName, newAchievementName);

            printf("�ɾͼ�¼���³ɹ���\n");
            SaveAchievementsToFile(); // ���º󱣴���µ��ļ�
            return;
        }
        current = current->next;
    }

    printf("δ�ҵ�ѧ��Ϊ %s �ҳɾ���Ϊ %s �ĳɾͼ�¼���޷��޸ġ�\n", studentId, achievementName);
}
void DeleteAchievementRecord() {
    char studentId[20];
    char achievementName[100];

    printf("������Ҫɾ���ɾͼ�¼��ѧ��ѧ��: ");
    scanf("%19s", studentId);
    printf("������Ҫɾ���ɾͼ�¼�ĳɾ���: ");
    scanf(" %[^\n]", achievementName);

    AchievementNode* current = achievementHead;
    AchievementNode* previous = NULL;
    int found = 0; // ����Ƿ��ҵ���ɾ���˼�¼

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.achievementName, achievementName) == 0) {
            found = 1; // ����ҵ���¼
            // ����ҵ���Ҫɾ���ĳɾͼ�¼
            if (previous == NULL) {
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                achievementHead = current->next;
            }
            else {
                // ���Ҫɾ���Ľڵ㲻��ͷ�ڵ�
                previous->next = current->next;
            }
            free(current); // �ͷŽڵ��ڴ�
            printf("ѧ��ѧ��Ϊ %s �ĳɾͼ�¼ %s �ѱ�ɾ����\n", studentId, achievementName);
            SaveAchievementsToFile(); // ɾ���󱣴���µ��ļ�
            break; // �ɹ�ɾ�����˳�ѭ��
        }
        previous = current;
        current = current->next;
    }

    if (!found) {
        // �������������û���ҵ�����ʾû�и�ѧ����ָ���ɾͼ�¼
        printf("δ�ҵ�ѧ��Ϊ %s �ҳɾ���Ϊ %s �ĳɾͼ�¼���޷�ɾ����\n", studentId, achievementName);
    }
}

void DeleteAllAchievementsForStudent(const char* studentId) {
    AchievementNode* current = achievementHead;
    AchievementNode* previous = NULL;
    int found = 0; // ����Ƿ��ҵ���ɾ��������һ����¼

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // ����ҵ���Ҫɾ���ĳɾͼ�¼
            found = 1;
            if (previous == NULL) {
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                achievementHead = current->next;
                free(current); // �ͷŽڵ��ڴ�
                current = achievementHead; // ����currentΪ�µ�ͷ�ڵ�
            }
            else {
                // ���Ҫɾ���Ľڵ㲻��ͷ�ڵ�
                previous->next = current->next;
                free(current); // �ͷŽڵ��ڴ�
                current = previous->next; // ��������
            }
            continue; // ���������һ���ڵ㣬������previous
        }
        previous = current;
        current = current->next;
    }

    if (found) {
        printf("ѧ��ѧ��Ϊ %s �����гɾͼ�¼�ѱ�ɾ����\n", studentId);
        SaveAchievementsToFile(); // ɾ�������󱣴���µ��ļ�
    }
    else {
        // �������������û���ҵ�����ʾû�и�ѧ���ĳɾͼ�¼
        printf("δ�ҵ�ѧ��Ϊ %s ���κγɾͼ�¼���޷�ɾ����\n", studentId);
    }
}