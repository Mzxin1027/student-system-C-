#include "score_management.h"
#include "student_info.h"
#include"achievement_management.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float ConvertScoreToGPA(float score) {
    if (score < 60) {
        return 0.0;
    }
    else if (score < 64) {
        return 1.0;
    }
    else if (score < 67) {
        return 1.3;
    }
    else if (score < 70) {
        return 1.7;
    }
    else if (score < 74) {
        return 2.0;
    }
    else if (score < 77) {
        return 2.3;
    }
    else if (score < 80) {
        return 2.7;
    }
    else if (score < 84) {
        return 3.0;
    }
    else if (score < 87) {
        return 3.3;
    }
    else if (score < 90) {
        return 3.7;
    }
    else {
        return 4.0;
    }
};
void LoadScoresFromFile() {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("δ�ҵ����еĳɼ����ݡ�\n");
        return;
    }

    head = NULL; // �����������

    ScoreInfo info;
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) { // ע���������һ����ʽ���ַ���Ϊ%f
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            printf("�ڴ����ʧ�ܡ�\n");
            fclose(file);
            return;
        }
        newNode->data = info;
        newNode->next = head;
        head = newNode;
    }
    fclose(file);
}
void SaveScoresToFile() {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("�򿪳ɼ��ļ�д��ʧ�ܡ�\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f\n", // ע���������һ����ʽ���ַ�����Ϊ%.2f
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.score,
            current->data.gpa, current->data.credit); // ѧ����С����ʽ����
        current = current->next;
    }
    fclose(file);
}
void AddNewScoreRecord() {
    int isValidScore = 0;
    ScoreInfo newRecord;
    LoadStudentInfoFromFile();
    LoadScoresFromFile();
    printf("������ѧ��: ");
    int p;
    do {
        p = 1;
        StudentNode* record = heads;
        scanf("%19s", newRecord.studentId);
        isValidScore = 1; // �����û�����Ч
        while (record != NULL) {
            if (strcmp(record->info.studentId, newRecord.studentId) == 0) {
                strcpy(newRecord.college, record->info.collegeName);
                strcpy(newRecord.major, record->info.majorName);
                strcpy(newRecord.name, record->info.name);
                p = 0;
                printf("�ɹ�����\n");
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            isValidScore = 0;
            printf("ѧ�Ų����ڣ�����������:");
        }
    } while (isValidScore == 0);
    StudentNode* record = heads;
    char coursename[100][50];
    float credits[50];
    int i;
    printf("�γ����ƣ�");
    course_option(&i, newRecord.college, newRecord.major, coursename, credits);
    for (int n = 0; n <= i; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n������ѡ�");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == i + 1) { return; }
        else if (option3 > i + 1 || option3 < 1) {
            printf("����������������룺");
            continue;
        }
        else {
            strcpy(newRecord.courseName, coursename[option3 - 1]);
            newRecord.credit = credits[option3 - 1];
            break;
        }
    } while (1);

// ����Ƿ��Ѵ�����ͬѧ�ŵļ�¼������֤ѧԺ��רҵ���Ƿ�һ��
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->data.studentId, newRecord.studentId) == 0&&strcmp(current->data.courseName,newRecord.courseName)==0) {
            // ѧ���Ѵ��ڣ����γ��Ƿ��Ѵ���

            printf("�γ̳ɼ���¼�룬�����ظ�����\n");
            return;
        }
        current = current->next;
    }


    while (isValidScore == 1) {
        printf("������ɼ� (��������0-100): ");
        if (scanf("%f", &newRecord.score) != 1 || newRecord.score < 0 || newRecord.score > 100) {
            printf("��Ч�ĳɼ������������롣\n");
            isValidScore = 0;
            while (getchar() != '\n'); // ������뻺����
        }
        else {
            isValidScore = 0;
        }
    }

    newRecord.gpa = ConvertScoreToGPA(newRecord.score);

    

    // ���ѧ�Ų����ڻ�������ѧԺ/רҵ��ƥ�䣬����¼�¼
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    newNode->data = newRecord;
    newNode->next = head;
    head = newNode;

    SaveScoresToFile(); // ����˺�����ʵ�֣����ڽ����ݱ��浽�ļ�
    printf("�µĳɼ���¼�ѳɹ���ӡ�\n");
}
void ModifyScoreRecord() {
    JudgeRole();
    LoadScoresFromFile();
    char studentId[20];
    char courseName[100];
    float newScore; // �޸ı�������Ϊfloat��֧�ָ�����
    printf("������Ҫ�޸ĳɼ���¼��ѧ��ѧ��: ");
    scanf("%s", studentId);
    printf("������Ҫ�޸ĳɼ���¼�Ŀγ�����: ");
    scanf(" %[^\n]", courseName);

    Node* current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.courseName, courseName) == 0) {
            printf("��ǰ%s��%s�ĳɼ���%.2f��\n", studentId, courseName, current->data.score);

            int isValidScore = 0;
            while (!isValidScore) {
                printf("�������³ɼ� (��������0-100): ");
                // ʹ��%f��ʽ˵�������ܸ���������
                if (scanf("%f", &newScore) != 1 || newScore < 0 || newScore > 100) {
                    printf("��Ч�ĳɼ������������롣\n");
                    // ������뻺�������Է�ֹ��Ч������ź�������
                    while (getchar() != '\n');
                }
                else {
                    isValidScore = 1;
                }
            }

            // ���³ɼ�
            current->data.score = newScore;
            // �����³ɼ����¼���GPA
            current->data.gpa = ConvertScoreToGPA(newScore);

            printf("�ɼ���¼���³ɹ���\n");
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("δ�ҵ�ƥ��ĳɼ���¼��\n");
    }
    else {
        // �ɼ����º����±��������ļ�
        SaveScoresToFile();
    }
}

void UpdateTransferInfo() {
    char studentId[20];
    char newCollege[100];
    char newMajor[100];
    int found = 0;

    printf("����ѧ��ѧ��: ");
    scanf("%19s", studentId);
    printf("������ѧԺ��: ");
    int i, j;
    char college[100][100];
    char major[100][100];
    College_option(&i, college);
    for (int n = 0; n <= i; n++) {
        printf("%d . %s ", n + 1, college[n]);
    }
    printf("\n������ѡ��:");
    int option1;
    do
    {
        scanf("%d", &option1);
        getchar();
        if (option1 - 1 > i || option1 < 1) {
            printf("����������:");
            continue;
        }
        else if (option1 - 1 == i) {
            system("cls");
            return;
        }
        else {
            strcpy(newCollege, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("������רҵ��: ");
    Major_option(&j, option1, college, major);
    for (int n = 0; n <= j; n++) {
        printf("%d . %s ", n + 1, major[n]);
    }
    printf("\n������ѡ��:");
    int option2;
    do
    {
        scanf("%d", &option2);
        getchar();
        if (option2 - 1 > j || option2 < 1) {
            printf("\n����������:");
            continue;
        }
        else if (option2 - 1 == j) {
            system("cls");
            return;
        }
        else {
            strcpy(newMajor, major[option2 - 1]);
            break;
        }

    } while (1);
    // �������гɼ����ݵ�����
    LoadScoresFromFile();

    // ������������ƥ���ѧ����¼
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            strcpy(current->data.college, newCollege);
            strcpy(current->data.major, newMajor);
            found = 1;
        }
        current = current->next;
    }

    if (found) {
        // ����ҵ���������ѧ����¼�������º������д���ļ�
        SaveScoresToFile();
        printf("�ɹ�Ϊѧ�� %s ����ѧԺΪ %s ��רҵΪ %s��\n", studentId, newCollege, newMajor);
    }
    else {
        printf("δ�ҵ�ѧ����Ϣ %s��\n", studentId);
    }
}

void SaveScoresByCollege() {
    if (head == NULL) {
        printf("û�гɼ����ݿɹ����档\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        // ����ѧԺ����רҵ�������ļ���
        char collegeAndMajorFilename[200]; // ���������С�����ɸ������ļ���
        sprintf(collegeAndMajorFilename, "%s-%s_scores.txt", current->data.college, current->data.major); // �����ļ�����ʽΪ "ѧԺ��-רҵ��_scores.txt"

        // ��׷��ģʽ�򿪻򴴽���ѧԺ����רҵ���������ļ�
        FILE* collegeAndMajorFile = fopen(collegeAndMajorFilename, "a");
        if (!collegeAndMajorFile) {
            printf("�޷��򿪻򴴽��ļ� %s\n", collegeAndMajorFilename);
            current = current->next; // ������һ���ڵ�
            continue;
        }

        // ����ǰ�ɼ���¼д���Ӧ��ѧԺ��רҵ�ļ�
        fprintf(collegeAndMajorFile, "%s,%s,%s,%s,%s,%.2f,%.2f,%.f\n",
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.score,
            current->data.gpa, current->data.credit);
        fclose(collegeAndMajorFile); // д����ɺ�ر��ļ�

        current = current->next; // ������һ���ڵ�
    }

    printf("��ѧԺ��רҵ����ĳɼ��ļ��ѳɹ����ɡ�\n");
}

float CalculateInnovationBonus(char* studentId) {
    InnovationProjectNode* current = innovationHead;
    float bonus = 0.0;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            bonus += 0.04; // ����ÿ������Ŀ����0.04
        }
        current = current->next;
    }
    return bonus;
}

void AddNewInnovationProject() {
    InnovationProjectInfo newProject;
    printf("��������ĿID: ");
    scanf("%19s", newProject.projectId);
    printf("���������ѧ����ѧ��: ");
    scanf("%19s", newProject.studentId);
    printf("��������Ŀ�ļӷֵ���: ");
    scanf("%f", &newProject.bonusPoints);

    // �����½ڵ㲢��ӵ�����ͷ��
    InnovationProjectNode* newNode = (InnovationProjectNode*)malloc(sizeof(InnovationProjectNode));
    if (newNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    newNode->data = newProject;
    newNode->next = innovationHead;
    innovationHead = newNode;

    // ׷���µĴ���Ŀ��¼��dc.txt�ļ�
    FILE* file = fopen("dc.txt", "a"); // ע������ʹ��׷��ģʽ"a"
    if (!file) {
        printf("��dc.txt�ļ�ʧ�ܡ�\n");
        free(newNode); // ������ļ�ʧ�ܣ��ͷ�֮ǰ������ڴ�
        return;
    }
    fprintf(file, "%s,%s,%.2f\n",
        newProject.projectId,
        newProject.studentId,
        newProject.bonusPoints);
    fclose(file);

    printf("�µĴ���Ŀ��¼�ѳɹ���Ӳ����浽dc.txt��\n");
}
// �޸�CalculateAverageGPA�������ڼ���ƽ��GPA֮ǰ����CalculateInnovationBonus����

void QueryStudentInfo() {
    char studentId[20];
    printf("������Ҫ��ѯ��ѧ��ѧ��: ");
    scanf("%19s", studentId);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("δ�ҵ����еĳɼ����ݡ�\n");
        return;
    }

    ScoreInfo info;
    int found = 0;
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        if (strcmp(info.studentId, studentId) == 0) {
            printf("ѧԺ: %s, רҵ: %s, ѧ������: %s, �γ�����: %s, �ɼ�: %.2f, ����: %.2f, ѧ��: %.2f\n",
                info.college, info.major, info.name, info.courseName, info.score, info.gpa, info.credit);
            found = 1;

        }
    }

    fclose(file);

    if (!found) {
        printf("δ�ҵ�ѧ�� %s �ĳɼ���¼��\n", studentId);
    }
}

void SaveScoresByCollegeToFile() {
    Node* current = head;
    CollegeGPAList* collegeGPAHead = NULL; // ѧԺGPA��Ϣ�����ͷָ��

    // �����ɼ���������ɼ���Ϣ������ÿ��ѧ����GPA
    while (current != NULL) {
        // ����ѧԺGPA��Ϣ����
        UpdateCollegeGPAList(&collegeGPAHead, current);
        current = current->next;
    }

    // ����ѧԺGPA��Ϣ��������ƽ��GPA�����浽�ļ�
    CollegeGPAList* collegeCurrent = collegeGPAHead;
    while (collegeCurrent != NULL) {
        // Ϊÿ��ѧԺ����һ��GPA�ļ�
        char collegeGPAFilename[150];
        sprintf(collegeGPAFilename, "%sGPA.txt", collegeCurrent->collegeName);
        FILE* gpaFile = fopen(collegeGPAFilename, "w");
        if (gpaFile) {
            CollegeStudentGPA* studentCurrent = collegeCurrent->studentHead;
            while (studentCurrent != NULL) {
                float averageGPA = studentCurrent->scoreCount > 0 ? studentCurrent->totalGPA / studentCurrent->scoreCount : 0;
                fprintf(gpaFile, "ѧ��: %s, ����: %s, רҵ��: %s, ƽ��GPA: %.2f\n",
                    studentCurrent->studentId, studentCurrent->name, studentCurrent->major, averageGPA);
                studentCurrent = studentCurrent->next;
            }
            fclose(gpaFile);
        }
        collegeCurrent = collegeCurrent->next;
    }

    // �ͷ�ѧԺGPA��Ϣ������ڴ�
    FreeCollegeGPAList(collegeGPAHead);
    printf("��ѧԺ����ƽ��GPA�ļ��ѳɹ����ɡ�\n");
}

// ���������ж���̬�ֲ��ĺ���
int isNormalDistribution(float mean, float stddev, int totalCount, Node* head, const char* courseName) {
    // �򻯵���̬�ֲ��жϣ�ʵ��Ӧ���п�����Ҫ�����ӵ�ͳ�Ʋ���
    // ����ֻ��һ��������ʾ����ʵ������¿�����Ҫ����������С��ƫ�Ⱥͷ�ȵȽ����ж�
    float skewness = 0.0; // ƫ��
    float kurtosis = 0.0; // ���
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->data.courseName, courseName) == 0) {
            float deviation = current->data.score - mean;
            skewness += pow(deviation, 3);
            kurtosis += pow(deviation, 4);
        }
        current = current->next;
    }
    skewness /= totalCount;
    kurtosis /= totalCount;
    skewness = skewness / pow(stddev, 3);
    kurtosis = kurtosis / pow(stddev, 4);

    // �򻯵��жϣ�ʵ������¿�����Ҫ�����ӵ��߼�
    return fabs(skewness) < 0.5 && fabs(kurtosis - 3) < 1;
}

// �����Ĳ鿴�γ�ͳ����Ϣ�ĺ���
void AnalyzeCourseStatistics(const char* courseName) {
    FILE* file = fopen("score.txt", "r"); // ȷ�����Ǵ洢�ɼ���Ϣ����ȷ�ļ���
    if (!file) {
        printf("δ�ҵ����еĳɼ����ݡ�\n");
        return;
    }

    char outputFilename[128];
    sprintf(outputFilename, "%s_statistics.txt", courseName);

    FILE* outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        printf("�޷������������ļ���%s\n", outputFilename);
        fclose(file);
        return;
    }

    int totalCount = 0, passCount = 0, excellentCount = 0;
    float maxScore = -1, minScore = 101; // ��ʼ����߷ֺ���ͷ�
    float scoreSum = 0;
    ScoreInfo info;

    // ��һ�α����������ܷ֡���߷֡���ͷ֡�������������������
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        if (strcmp(info.courseName, courseName) == 0) {
            totalCount++;
            scoreSum += info.score;
            if (info.score >= 85) excellentCount++;
            if (info.score >= 60) passCount++;
            if (info.score > maxScore) maxScore = info.score;
            if (info.score < minScore) minScore = info.score;
        }
    }

    float mean = scoreSum / totalCount;

    // �����ļ�ָ�뵽�ļ���ͷ��׼���ڶ��α���
    fseek(file, 0, SEEK_SET);

    float varianceSum = 0;
    // �ڶ��α��������㷽��
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        if (strcmp(info.courseName, courseName) == 0) {
            float deviation = info.score - mean;
            varianceSum += deviation * deviation;
        }
    }

    float variance = varianceSum / totalCount;


    // ������ļ�
    fprintf(outputFile, "�γ� %s ��ͳ����Ϣ��\n", courseName);
    fprintf(outputFile, "��������%d\n", totalCount);
    fprintf(outputFile, "��߷֣�%f\n", maxScore);
    fprintf(outputFile, "��ͷ֣�%f\n", minScore);
    fprintf(outputFile, "�����ʣ�%.2f%%\n", ((float)passCount / totalCount) * 100);
    fprintf(outputFile, "�����ʣ�%.2f%%\n", ((float)excellentCount / totalCount) * 100);
    fprintf(outputFile, "ƽ���֣�%.2f\n", mean);
    fprintf(outputFile, "���%.2f\n", variance); // �������
    fclose(file);
    fclose(outputFile);
    printf("�γ� %s ��ͳ����Ϣ�ѳɹ����ɲ����浽 %s��\n", courseName, outputFilename);
}


void LoadCollegesFromScores(const char* filename, char colleges[MAX_COLLEGES][100], int* collegeCount) {
    FILE* file = fopen(filename, "r");
    char line[256];
    char college[100];
    *collegeCount = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%99[^\n]", college);
        // ���ѧԺ�Ƿ������б���
        int found = 0;
        for (int i = 0; i < *collegeCount; ++i) {
            if (strcmp(colleges[i], college) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(colleges[*collegeCount], college);
            (*collegeCount)++;
        }
    }
    fclose(file);
}

void QueryScoresByCollege() {
    char collegeName[100];
    printf("������Ҫ��ѯ��ѧԺ����: ");
    scanf("%99s", collegeName); // ע��ʹ��%99s�Ա��⻺�������

    Node* current = head;
    int found = 0;
    printf("ѧԺ %s �ĳɼ����£�\n", collegeName);
    while (current != NULL) {
        if (strcmp(current->data.college, collegeName) == 0) {
            printf("ѧ��: %s, ����: %s, �γ�����: %s, �ɼ�: %.2f, ����: %.2f\n",
                current->data.studentId, current->data.name,
                current->data.courseName, current->data.score, current->data.gpa);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("δ�ҵ�ѧԺ %s �ĳɼ���¼��\n", collegeName);
    }
}

void QueryScoreByStudentId(const char* studentId) {
    // ȷ���ڲ�ѯ֮ǰ���ɼ������Ѿ����ص�������
    // ����ɼ������Ѿ��ڳ����������ּ��أ���һ�����ܲ���Ҫ
    LoadScoresFromFile();

    Node* current = head;
    int found = 0; // ����Ƿ��ҵ���Ӧѧ���ĳɼ�

    printf("ѧ��ID %s �ĳɼ����£�\n", studentId);
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // �ҵ�ƥ��ĳɼ���¼����ӡ��ϸ��Ϣ
            printf("�γ�����: %s, �ɼ�: %.1f, ����: %.2f, ѧ��: %.2f\n",
                current->data.courseName, current->data.score, current->data.gpa, current->data.credit);
            found = 1; // ����ҵ�����һ���ɼ���¼
        }
        current = current->next;
    }

    if (!found) {
        // ���δ�ҵ��κγɼ���¼
        printf("δ�ҵ�ѧ��ID %s ���κγɼ���¼��\n", studentId);
    }
}



void SortAndSaveGPA(const char* inputFilename) {
    FILE* inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        printf("Unable to open file %s for reading.\n", inputFilename);
        return;
    }

    StudentGPA students[MAX_COLLEGES]; // �������100��ѧ��
    int count = 0;
    while (fscanf(inputFile, "%[^,],%[^,],%f\n", students[count].studentId, students[count].name, &students[count].gpa) == 3) {
        count++;
    }
    fclose(inputFile);

    qsort(students, count, sizeof(StudentGPA), compareGPA);

    char outputFilename[256];
    snprintf(outputFilename, sizeof(outputFilename), "%s_ranked.txt", inputFilename); // ���������ļ���
    FILE* outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        printf("Unable to open file %s for writing.\n", outputFilename);
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(outputFile, "%s,%s,%.2f\n", students[i].studentId, students[i].name, students[i].gpa);
    }
    fclose(outputFile);
}
// �������ڴ�score.txt����ȡ����Ψһ��ѧԺ��רҵ���
void extractCollegeMajorPairs() {
    FILE* file = fopen("score.txt", "r");
    if (!file) {
        printf("Unable to open score.txt\n");
        return;
    }

    char line[256], college[100], major[100];
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%99[^,],%99[^,]", college, major) == 2) {
            addToList(college, major);
        }
    }
    fclose(file);
}

void AddStudentToCollegeMajorList(CollegeMajorScoreEntry** head, ScoreInfo* studentInfo) {
    CollegeMajorScoreEntry* current = *head;
    CollegeMajorScoreEntry* last = NULL;
    while (current != NULL) {
        if (strcmp(current->college, studentInfo->college) == 0 && strcmp(current->major, studentInfo->major) == 0) {
            // �ҵ��˶�Ӧ��ѧԺ��רҵ����ѧ����Ϣ��ӵ�scores�ַ�����
            // ע�⣺������Ҫ��ScoreInfoת��Ϊ�ַ�����ʽ��׷�ӵ�current->scores��
            // �����Թ�������ַ����������
            return;
        }
        last = current;
        current = current->next;
    }
    // ���û���ҵ���Ӧ��ѧԺ��רҵ�������½ڵ�
    CollegeMajorScoreEntry* newEntry = (CollegeMajorScoreEntry*)malloc(sizeof(CollegeMajorScoreEntry));
    strcpy(newEntry->college, studentInfo->college);
    strcpy(newEntry->major, studentInfo->major);
    // ��ʼ��scores�ַ�����Ȼ��ѧ����Ϣ��ӽ�ȥ
    // �����Թ�������ַ����������
    newEntry->next = NULL;
    if (last == NULL) {
        *head = newEntry;
    }
    else {
        last->next = newEntry;
    }
}
void PredictPostgraduateOpportunity(const char* studentId) {
    LoadScoresFromFile();
    Node* current = head;
    float totalGPA = 0.0;
    int courseCount = 0;
    int lowScoreCount = 0; // �ɼ�����60�ֵĿγ�����
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            totalGPA += current->data.gpa;
            courseCount++;
            if (current->data.score < 60) {
                printf("��Ҫ��ǿ�Ŀγ�: %s\n", current->data.courseName);
                lowScoreCount++;
            }
        }
        current = current->next;
    }

    if (courseCount == 0) {
        printf("δ�ҵ�ѧ�� %s �ĳɼ���¼��\n", studentId);
        return;
    }

    float averageGPA = totalGPA / courseCount;
    printf("ƽ��GPA: %.2f\n", averageGPA);

    // ������Ŀ
    float innovationBonus = CalculateInnovationBonus(studentId);
    if (innovationBonus > 0) {
        printf("����Ŀ�ӷ�: %.2f\n", innovationBonus);
    }

    // Ԥ�Ᵽ�л���
    if (averageGPA >= 3.5 && lowScoreCount == 0) {
        printf("ѧ�� %s �кܺõı��л��ᡣ\n", studentId);
    }
    else {
        printf("ѧ�� %s ��Ҫ�����·���Ŭ������߱��л��᣺\n", studentId);
        if (averageGPA < 3.5) {
            printf("- ���ƽ��GPA����ǰƽ��GPAΪ %.2f\n", averageGPA);
        }
        if (lowScoreCount > 0) {
            printf("- ��ǿ�����γ�ѧϰ���� %d �ſγ̳ɼ�����60�֡�\n", lowScoreCount);
        }
    }
}

// �������ڼ�������ѧԺ��רҵ����Ƿ��Ѿ�������������
int existsInList(const char* college, const char* major) {
    CollegeMajorPair* current = uniqueCollegeMajorPairs;
    while (current != NULL) {
        if (strcmp(current->college, college) == 0 && strcmp(current->major, major) == 0) {
            return 1; // �Ѵ���
        }
        current = current->next;
    }
    return 0; // ������
}

//���ѧԺ��רҵ// �������ڽ��µ�ѧԺ��רҵ�����ӵ�������
void addToList(const char* college, const char* major) {
    if (existsInList(college, major)) {
        // ���ѧԺ��רҵ������Ѵ��ڣ����ظ����
        printf("ѧԺ��רҵ������Ѵ��ڣ������ظ����");
        return;
    }

    CollegeMajorPair* newNode = (CollegeMajorPair*)malloc(sizeof(CollegeMajorPair));
    if (newNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    strcpy(newNode->college, college);
    strcpy(newNode->major, major);
    newNode->next = uniqueCollegeMajorPairs;
    uniqueCollegeMajorPairs = newNode;
    saveCollegeMajorPairs();
}

void loadCollegeMajorPairs() {
    FILE* file = fopen("info.txt", "r");
    if (!file) {
        printf("info.txt�ļ������ڣ��������µ��ļ���\n");
        return;
    }

    char college[100], major[100];
    while (fscanf(file, "%99[^,],%99[^\n]\n", college, major) == 2) {
        // ����addToList�����ڲ��Ѿ��������ظ������߼�
        // ����ֱ�ӵ���addToList��������
        addToList(college, major);
    }

    fclose(file);

}

void saveCollegeMajorPairs() {
    FILE* file = fopen("info.txt", "w"); // ��дģʽ���ļ�������ԭ������
    if (!file) {
        printf("�޷��򿪻򴴽�info.txt�ļ���\n");
        return;
    }

    CollegeMajorPair* current = uniqueCollegeMajorPairs;
    while (current != NULL) {
        fprintf(file, "%s,%s\n", current->college, current->major);
        current = current->next;
    }

    fclose(file);
}
void DeleteAllScoresForStudent(const char* studentId) {
    Node* current = head;
    Node* previous = NULL;
    int found = 0; // ����Ƿ��ҵ���ɾ��������һ����¼

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // �ҵ���Ҫɾ���ĳɼ���¼
            found = 1;
            if (previous == NULL) {
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                head = current->next;
                free(current); // �ͷŽڵ��ڴ�
                current = head; // ����currentΪ�µ�ͷ�ڵ�
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
        printf("ѧ��ѧ��Ϊ %s �����гɼ���¼�ѱ�ɾ����\n", studentId);
        SaveScoresToFile(); // ɾ�������󱣴���µ��ļ�
    }
    else {
        // �������������û���ҵ�����ʾû�и�ѧ���ĳɼ���¼
        printf("δ�ҵ�ѧ��Ϊ %s �ĳɼ���¼���޷�ɾ����\n", studentId);
    }
}
void DeleteScoreRecord(const char* studentId, const char* courseName) {
    Node* current = head;
    Node* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.courseName, courseName) == 0) {
            // ����ҵ���Ҫɾ���ĳɼ���¼
            if (previous == NULL) {
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                head = current->next;
            }
            else {
                // ���Ҫɾ���Ľڵ㲻��ͷ�ڵ�
                previous->next = current->next;
            }
            free(current); // �ͷŽڵ��ڴ�
            printf("ѧ��ѧ��Ϊ %s �Ŀγ� %s �ɼ���¼�ѱ�ɾ����\n", studentId, courseName);
            SaveScoresToFile(); // ɾ���󱣴���µ��ļ�
            return;
        }
        previous = current;
        current = current->next;
    }
    // �������������û���ҵ�����ʾû�и�ѧ���ĳɼ���¼
    printf("δ�ҵ�ѧ��Ϊ %s �ҿγ�����Ϊ %s �ĳɼ���¼���޷�ɾ����\n", studentId, courseName);
}



//���ļ��ж�ȡѧԺ��
void College_option(int* i, char college[][100]) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("info.txt�ļ������ڣ��������µ��ļ���\n");
        return;
    }
    char major[100];
    *i = 0;
    fscanf(file, "%99[^,],%99[^\n]\n", college[*i], major);
    (*i)++;
    while (feof(file) == 0) {
        fscanf(file, "%99[^,],%99[^\n]\n", college[*i], major);
        for (int k = 0; k < *i; k++) {
            if (strcmp(college[*i], college[k]) == 0) {
                (*i)--;
                break;
            }
        }
        (*i)++;
    }
    strcpy(college[*i], "����");
    fclose(file);
}

//�Ӷ�ӦѧԺ�ж�ȡ��Ӧרҵ����
void Major_option(int* j, int option1, const char college[][100], char major[][100]) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("info.txt�ļ������ڣ��������µ��ļ���\n");
        return;
    }
    char college_[100];

    *j = 0;
    while (feof(file) == 0) {
        fscanf(file, "%99[^,],%99[^\n]\n", college_, major[*j]);
        if (strcmp(college_, college[option1 - 1]) == 0) {
            (*j)++;
        }
    }
    strcpy(major[*j], "����");
    fclose(file);
}

//�Ӷ�ӦѧԺ�ж�ȡ��Ӧרҵ����
void course_option(int* i, const char college[50], const char major[50], char course[100][50], float credits[50]) {
    FILE* pf = fopen("courses.txt", "r");
    if (pf == NULL) {
        printf("courses.txt�ļ������ڣ��������µ��ļ���\n");
        return;
    }
    *i = 0;
    CourseInfo current;
    while (fscanf(pf, "%99[^,],%99[^,],%99[^,],%f\n", current.collegeName, current.majorName, current.courseName, &current.credits) == 4)
    {
        if (strcmp(current.collegeName, college) == 0 && strcmp(current.majorName, major) == 0) {
            strcpy(course[*i], current.courseName);
            credits[*i] = current.credits;
            (*i)++;
        }
    }
    strcpy(course[*i], "����");
    fclose(pf);
}
void Stu_appeal_score(const char username[50]) {
    loadappealsInfoTofile();
    node* record = appealshead;
    printf("****��������Ϣ****\n");
    int Found = 0;
    while (record != NULL)
    {
        if (strcmp(username, record->data.studentId) == 0) {
            printf("ѧ�ţ�%s��������%s���γ̣�%s���Ƿ����߳ɹ���%c\n",
                record->data.studentId, record->data.name, record->data.courseName, record->data.appealinfo);
            Found = 1;
        }
        record = record->next;
    }
    if (Found == 0) {
        printf("����������Ϣ\n");
    }
    printf("****TΪ�ɹ���FΪʧ��****\n");
    appealshead = NULL;
    LoadScoresFromFile();
    printf("�������������ߵĿγ̣�");
    char cousename[50];
    scanf("%s", cousename);

    AppealsInfo info;

    Node* current = head;
    int found = 0;
    while (current != NULL) {
        strcpy(info.college, current->data.college);
        strcpy(info.major, current->data.major);
        strcpy(info.studentId, current->data.studentId);
        strcpy(info.name, current->data.name);
        strcpy(info.courseName, current->data.courseName);
        if (strcmp(current->data.studentId, username) == 0 && strcmp(current->data.courseName, cousename) == 0)
        {
            info.appealinfo = 'F';
            found = 1;
            break;
        }
        current = current->next;
    }
    if (found == 0) {
        printf("�����������ߵĿγ̳ɼ���Ϣ");
        return;
    }
    node* newnode = (node*)malloc(sizeof(node));
    if (newnode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    newnode->data = info;
    newnode->next = appealshead;
    appealshead = newnode;
    addappealsInfoTofile();
    printf("�ɼ������ϱ��ɹ�");
}
void addappealsInfoTofile() {
    FILE* file = fopen("appealsInfo.txt", "a");
    if (!file) {
        printf("�������ļ���д��ʧ�ܡ�\n");
        return;
    }
    node* current = appealshead;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s,%s,%c\n",
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.appealinfo);
        current = current->next;
    }
    fclose(file);
    appealshead = NULL;
}
void loadappealsInfoTofile() {
    FILE* pfile = fopen("appealsInfo.txt", "r");
    if (!pfile) {
        printf("δ��������Ϣ��\n");
        return;
    }
    appealshead = NULL;
    AppealsInfo current;
    while (fscanf(pfile, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%c\n", current.college, current.major,
        current.studentId, current.name, current.courseName, &current.appealinfo) == 6)
    {
        node* newnode = (node*)malloc(sizeof(node));
        if (newnode == NULL) {
            printf("�ڴ����ʧ�ܡ�\n");
            return;
        }
        newnode->data = current;
        newnode->next = appealshead;
        appealshead = newnode;

    }
    fclose(pfile);
}
void QueryappealsInfo() {
    loadappealsInfoTofile();

    node* record = appealshead;
    while (record != NULL)
    {
        printf("ѧԺ��%s��רҵ��%s��ѧ�ţ�%s��������%s���γ̣�%s���Ƿ����߳ɹ���%c\n",
            record->data.college, record->data.major, record->data.studentId, record->data.name,
            record->data.courseName, record->data.appealinfo);
        record = record->next;
    }
    printf("****TΪ�ɹ���FΪʧ��****\n");
    printf("���޸ĳɹ����������Ӧѧ�źͶ�Ӧ�γ̣�����������Ϣ\n");
    printf("�������Ӧѧ�ţ�");
    char stuID[20], coursename[50];
    scanf("%s", stuID);
    printf("�������Ӧ�γ̣�");
    scanf("%s", coursename);
    node* record1 = appealshead;
    int found = 0;
    while (record1 != NULL)
    {
        if (strcmp(stuID, record1->data.studentId) == 0 && strcmp(coursename, record1->data.courseName) == 0) {
            record1->data.appealinfo = 'T';
            found = 1;
        }
        record1 = record1->next;
    }
    if (found = 0) {
        printf("�޶�Ӧѧ�Ż�γ�");
        return;
    }
    saveappealsInfoTofile();
    printf("���ĳɹ�");
}
void saveappealsInfoTofile() {
    FILE* file = fopen("appealsInfo.txt", "w");
    if (!file) {
        printf("�������ļ���д��ʧ�ܡ�\n");
        return;
    }
    node* current = appealshead;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s,%s,%c\n",
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.appealinfo);
        current = current->next;
    }
    fclose(file);
    appealshead = NULL;
}

// ��info.txt�ж�ȡ����ѧԺ��רҵ��ϣ����û�ѡ��
void LoadCollegesAndMajors(char colleges[MAX_COLLEGES][100], char majors[MAX_COLLEGES][100], int* count) {
    FILE* file = fopen("info.txt", "r");
    if (!file) {
        printf("�޷���info.txt�ļ����ж�ȡ��\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    *count = 0;
    while (fgets(line, sizeof(line), file) && *count < MAX_COLLEGES) {
        sscanf(line, "%99[^,],%99[^\n]", colleges[*count], majors[*count]);
        (*count)++;
    }
    fclose(file);
}
// ��ѯ����ӡָ��רҵ��������Ϣ
void QueryRankingByCollegeAndMajor(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("�޷����ļ� %s ���ж�ȡ���ļ������ڡ�\n", filename);
        return;
    }

    printf("������Ϣ���£�\n");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}
void QueryMajorRankingProcess() {
    char colleges[MAX_COLLEGES][100], majors[MAX_COLLEGES][100];
    int count;
    LoadCollegesAndMajors(colleges, majors, &count);

    printf("��ѡ��ѧԺ��רҵ��\n");
    for (int i = 0; i < count; i++) {
        printf("%d. ѧԺ��%s - רҵ��%s\n", i + 1, colleges[i], majors[i]);
    }

    int choice;
    printf("�������ѡ�����֣���");
    scanf("%d", &choice);
    getchar();
    // ��֤�û�����
    if (choice < 1 || choice > count) {
        printf("��Чѡ��\n");
        return;
    }

    // �����û�ѡ�������ļ���
    char filename[256];
    sprintf(filename, "%s-%s-GPA-Ranked.txt", colleges[choice - 1], majors[choice - 1]);

    // ��ѯ����ʾ������Ϣ
    QueryRankingByCollegeAndMajor(filename);
}
// �������������ѧ��ID�Ƿ��Ѿ�������������
int isStudentIdProcessed(const char studentIds[][20], int size, const char* studentId) {
    for (int i = 0; i < size; i++) {
        if (strcmp(studentIds[i], studentId) == 0) {
            return 1; // �ҵ��ˣ��Ѵ���
        }
    }
    return 0; // δ����
}
void QueryAllScores() {
    FILE* file = fopen("score.txt", "r");
    if (!file) {
        printf("�޷���score.txt�ļ����ж�ȡ��\n");
        return;
    }

    ScoreInfo info;
    printf("����ѧ���ɼ���¼���£�\n");
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        // ��ӡÿ����¼����ϸ��Ϣ
        printf("ѧԺ: %s, רҵ: %s, ѧ��: %s, ����: %s, �γ�����: %s, �ɼ�: %.2f, ����: %.2f, ѧ��: %.2f\n",
            info.college, info.major, info.studentId, info.name,
            info.courseName, info.score, info.gpa, info.credit);
    }

    fclose(file);
}

void FreeCollegeMajorPairList(CollegeMajorPair* head) {
    while (head != NULL) {
        CollegeMajorPair* temp = head;
        head = head->next;

        // �ͷ��뵱ǰCollegeMajorPair������ѧ��GPA��Ϣ����
        StudentGPAInfo* gpaInfo = temp->studentGPAHead;
        while (gpaInfo != NULL) {
            StudentGPAInfo* gpaTemp = gpaInfo;
            gpaInfo = gpaInfo->next;
            free(gpaTemp);
        }

        // �ͷŵ�ǰ��CollegeMajorPair�ڵ�
        free(temp);
    }
}





int compareGPA(const void* a, const void* b) {
    const StudentGPA* ga = (const StudentGPA*)a;
    const StudentGPA* gb = (const StudentGPA*)b;
    // ��������
    return (gb->gpa > ga->gpa) - (ga->gpa > gb->gpa);
}

static int compareStudentGPA(const void* a, const void* b) {
    const StudentGPAInfo* studentA = *(const StudentGPAInfo**)a;
    const StudentGPAInfo* studentB = *(const StudentGPAInfo**)b;
    if (studentA->gpa > studentB->gpa) return -1;
    else if (studentA->gpa < studentB->gpa) return 1;
    else return 0;
}

int CompareStudentGPA(const void* a, const void* b) {
    const StudentGPAInfo* studentA = (const StudentGPAInfo*)a;
    const StudentGPAInfo* studentB = (const StudentGPAInfo*)b;

    // GPA���ߵ�ѧ��Ӧ����ǰ�棬������a��GPA����b�ģ�����-1
    if (studentA->gpa > studentB->gpa) return -1;
    if (studentA->gpa < studentB->gpa) return 1;
    return 0;
}

void UpdateCollegeGPAList(CollegeGPAList** head, Node* scoreNode) {
    // ���һ򴴽�ѧԺ�ڵ�
    CollegeGPAList* college = *head;
    while (college != NULL && strcmp(college->collegeName, scoreNode->data.college) != 0) {
        college = college->next;
    }
    if (college == NULL) { // ���ѧԺ�����ڣ��򴴽���ѧԺ�ڵ�
        college = (CollegeGPAList*)malloc(sizeof(CollegeGPAList));
        strcpy(college->collegeName, scoreNode->data.college);
        college->studentHead = NULL;
        college->next = *head;
        *head = college;
    }

    // ��ѧԺ�ڵ��в��һ򴴽�ѧ���ڵ�
    CollegeStudentGPA* student = college->studentHead;
    while (student != NULL && strcmp(student->studentId, scoreNode->data.studentId) != 0) {
        student = student->next;
    }
    if (student == NULL) { // ���ѧ�������ڣ��򴴽���ѧ���ڵ�
        student = (CollegeStudentGPA*)malloc(sizeof(CollegeStudentGPA));
        strcpy(student->studentId, scoreNode->data.studentId);
        strcpy(student->name, scoreNode->data.name);
        strcpy(student->major, scoreNode->data.major);
        student->totalGPA = 0;
        student->scoreCount = 0;
        student->next = college->studentHead;
        college->studentHead = student;
    }

    // ����ѧ����GPA��Ϣ
    student->totalGPA += scoreNode->data.gpa;
    student->scoreCount += 1;
}

void FreeCollegeGPAList(CollegeGPAList* head) {
    CollegeGPAList* current = head;
    while (current != NULL) {
        CollegeGPAList* temp = current;
        current = current->next;

        // �ͷ�ѧ�� GPA ����
        CollegeStudentGPA* studentCurrent = temp->studentHead;
        while (studentCurrent != NULL) {
            CollegeStudentGPA* tempStudent = studentCurrent;
            studentCurrent = studentCurrent->next;
            free(tempStudent);
        }

        free(temp); // �ͷ�ѧԺ�ڵ�
    }
}
 
// �ȽϺ���������qsort���Ƚ�����StudentGPA�ṹ���GPA

void RankAllGPAFiles() {
    CollegeMajorPair* currentPair = uniqueCollegeMajorPairs;

    while (currentPair != NULL) {
        // Ϊ��ǰѧԺ��רҵ������ѧ������һ����̬����
        int studentCount = 0;
        StudentGPAInfo** studentArray = (StudentGPAInfo**)malloc(sizeof(StudentGPAInfo*) * MAX_STUDENTS);
        StudentGPAInfo* currentStudent = currentPair->studentGPAHead;

        while (currentStudent != NULL) {
            studentArray[studentCount++] = currentStudent;
            currentStudent = currentStudent->next;
        }

        // ��ѧ���������GPA����
        qsort(studentArray, studentCount, sizeof(StudentGPAInfo*), compareStudentGPA);

        // �����ļ��������ļ�
        char filename[256];
        sprintf(filename, "%s-%s_GPA_Ranked.txt", currentPair->college, currentPair->major);
        FILE* file = fopen(filename, "w");
        if (!file) {
            printf("�޷������ļ� %s\n", filename);
            free(studentArray);
            currentPair = currentPair->next;
            continue;
        }

        // д��������Ϣ
        for (int i = 0; i < studentCount; i++) {
            fprintf(file, "Rank: %d, Student ID: %s, Name: %s, GPA: %.2f\n",
                i + 1, studentArray[i]->studentId, studentArray[i]->name, studentArray[i]->gpa);
        }

        // �ر��ļ����ͷ���Դ
        fclose(file);
        free(studentArray);

        // �ƶ�����һ��ѧԺ��רҵ��
        currentPair = currentPair->next;
    }
}
void CalculateAndRankCollegeGPA(const char* collegeName) {
    char filename[128];
    sprintf(filename, "%s.txt", collegeName);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s\n", filename);
        return;
    }

    CollegeScore college;
    strcpy(college.collegeName, collegeName);
    college.studentCount = 0;

    // ��ȡ�ļ������college.students����
    while (fscanf(file, "%[^,],%[^,],%f\n",
        college.students[college.studentCount].studentId,
        college.students[college.studentCount].name,
        &college.students[college.studentCount].gpa) == 3) {
        college.studentCount++;
    }

    // ��ѧ������GPA����
    qsort(college.students, college.studentCount, sizeof(StudentScore), compareGPA);

    fclose(file);

    // д�����������ѧԺ��gpa.txt
    sprintf(filename, "%sgpa.txt", collegeName);
    file = fopen(filename, "w");
    for (int i = 0; i < college.studentCount; i++) {
        fprintf(file, "Rank: %d, Student ID: %s, Name: %s, GPA: %.2f\n",
            i + 1,
            college.students[i].studentId,
            college.students[i].name,
            college.students[i].gpa);
    }
    fclose(file);
}
void SaveGPAByCollege() {
    FILE* agpaFile = fopen("agpa.txt", "r");
    if (!agpaFile) {
        printf("�޷���agpa.txt�ļ����ж�ȡ��\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), agpaFile)) {
        char studentId[20], name[50], major[100], college[100];
        float finalGPA; // �����󴴵ȳɾͼӷֺ������GPA
        // ����agpa.txt�ĸ�ʽΪ��ѧ��, ����, רҵ��, ѧԺ, ����GPA
        if (sscanf(line, "%19[^,],%49[^,],%99[^,],%99[^,],%f\n", studentId, name, major, college, &finalGPA) == 5) {
            char collegeFilename[150];
            sprintf(collegeFilename, "%sgpa.txt", college); // ������ѧԺ���������ļ���

            // ��׷��ģʽ�򿪻򴴽�ѧԺ��Ӧ��gpa�ļ�
            FILE* collegeFile = fopen(collegeFilename, "a");
            if (!collegeFile) {
                printf("�޷��򿪻򴴽��ļ� %s\n", collegeFilename);
                continue;
            }

            // ��ѧ����Ϣ������GPAд���Ӧ��ѧԺgpa�ļ�
            fprintf(collegeFile, "ѧ��: %s, ����: %s, רҵ��: %s, ����GPA: %.2f\n", studentId, name, major, finalGPA);
            fclose(collegeFile); // д����ɺ�ر��ļ�
        }
    }
    fclose(agpaFile); // �ر�agpa.txt�ļ�
    printf("��ѧԺ�����ƽ��GPA�ļ��ѳɹ����ɡ�\n");
}
void CalculateAverageGPA() {
    FILE* file = fopen("agpa.txt", "w");
    if (!file) {
        printf("�޷���agpa.txt�ļ�����д�롣\n");
        return;
    }

    char processedStudentIds[100][20]; // ������ദ��100��ѧ��
    int processedCount = 0;

    Node* current = head;
    while (current != NULL) {
        int alreadyProcessed = 0;
        for (int i = 0; i < processedCount; i++) {
            if (strcmp(processedStudentIds[i], current->data.studentId) == 0) {
                alreadyProcessed = 1;
                break;
            }
        }

        if (!alreadyProcessed) {
            strcpy(processedStudentIds[processedCount++], current->data.studentId);

            float totalWeightedGPA = 0.0;
            float totalCredits = 0.0;
            Node* course = current;
            while (course != NULL) {
                if (strcmp(course->data.studentId, current->data.studentId) == 0) {
                    float courseGPA = ConvertScoreToGPA(course->data.score);
                    totalWeightedGPA += courseGPA * course->data.credit;
                    totalCredits += course->data.credit;
                }
                course = course->next;
            }

            // ����ɾͼӷ�
            float achievementBonus = CalculateAchievementBonus(current->data.studentId);
            float averageGPA = totalCredits > 0 ? (totalWeightedGPA / totalCredits) + achievementBonus : 0.0;
            fprintf(file, "ѧ��: %s, ����: %s, רҵ��: %s, ƽ��GPA: %.2f\n",
                current->data.studentId, current->data.name, current->data.major, averageGPA);
        }

        current = current->next;
    }

    fclose(file);
    printf("ƽ��GPA�ļ�agpa.txt�ѳɹ����ɡ�\n");
}
void GenerateRankingFile() {
    FILE* file = fopen("paiming.txt", "w");
    if (!file) {
        printf("�޷��������ļ�����д�롣\n");
        return;
    }

    CalculateAndAssignRanks();  // ȷ���Ѿ������������GPA

    Node* current = head;
    float previousGPA = -1.0;  // ���ڸ���ǰһ��ѧ����GPA
    int actualRank = 0;  // ʵ������
    int displayedRank = 0;  // ��ʾ�����������ǲ��������

    while (current != NULL) {
        actualRank++;
        if (current->data.gpa != previousGPA) {  // ���GPA��ǰһ��ѧ����ͬ
            displayedRank = actualRank;
        }
        // ��ÿ�м�¼�����ѧ����רҵ��Ϣ
        fprintf(file, "Rank: %d, Name: %s, Student ID: %s, Major: %s, GPA: %.2f\n",
            displayedRank, current->data.name, current->data.studentId, current->data.major, current->data.gpa);
        previousGPA = current->data.gpa;
        current = current->next;
    }

    fclose(file);
    printf("�����ļ��ɹ����ɡ�\n");
}
int Partition(Node** nodesArray, int low, int high) {
    float pivot = nodesArray[high]->data.gpa; // ѡ�����һ��Ԫ�ص�GPA��Ϊ��׼
    int i = (low - 1); // С��pivot��Ԫ�ص�����

    for (int j = low; j <= high - 1; j++) {
        // �����ǰԪ�ص�GPA���ڻ����pivot
        if (nodesArray[j]->data.gpa >= pivot) {
            i++; // ��������
            Node* temp = nodesArray[i];
            nodesArray[i] = nodesArray[j];
            nodesArray[j] = temp;
        }
    }
    Node* temp = nodesArray[i + 1];
    nodesArray[i + 1] = nodesArray[high];
    nodesArray[high] = temp;
    return (i + 1);
}
void QuickSort(Node** nodesArray, int low, int high) {
    if (low < high) {
        // Partitioning index
        int pi = Partition(nodesArray, low, high);

        QuickSort(nodesArray, low, pi - 1); // �Ի�׼����Ԫ�ؽ�������
        QuickSort(nodesArray, pi + 1, high); // �Ի�׼�Ҳ��Ԫ�ؽ�������
    }
}
void UpdateRanks(Node** nodesArray, int count) {
    int rank = 1;
    for (int i = 0; i < count; i++) {
        if (i > 0 && nodesArray[i]->data.gpa == nodesArray[i - 1]->data.gpa) {
            // �����ǰѧ����GPA��ǰһ��ѧ����ͬ����������
            nodesArray[i]->data.rank = nodesArray[i - 1]->data.rank;
        }
        else {
            // ��������Ϊ��ǰλ�ü�1
            nodesArray[i]->data.rank = rank;
        }
        rank++;
    }
}
void CalculateAndAssignRanks() {
    // ����һ����̬�������洢�����е����нڵ�ָ��
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    Node** nodesArray = (Node**)malloc(count * sizeof(Node*));
    if (nodesArray == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }

    current = head;
    for (int i = 0; i < count; i++) {
        nodesArray[i] = current;
        current = current->next;
    }

    // ʹ�ÿ�������Խڵ��������
    QuickSort(nodesArray, 0, count - 1);

    // ���������������ǲ������������
    UpdateRanks(nodesArray, count);

    free(nodesArray);
}



//����GPA�����������ļ�
void sortGPAbycollegeAndmajor() {
    LoadScoresFromFile();
    ScoreInfo info;
    Node* record1 = head;
    Node* record2 = head;
    gpahead = NULL;
    stugpa studentGpa;
    Node1* gpanode;
    int i = 0;
    int x = 0;
    char studentID[100][20];
    float GPA=0, Gredit=0;
    while (record1 != NULL) {
        record2 = head;
        strcpy(studentID[i], record1->data.studentId);
        x = 0;
            for (int m = 0; m < i; m++) {
                if (strcmp(studentID[i], studentID[m]) == 0) {
                    x = 1;
                    break;
                }
            }
            if (x == 1) {
                record1 = record1->next;
                continue;
            }

        while (record2 != NULL) {
            if (strcmp(studentID[i], record2->data.studentId) == 0) {
                strcpy(studentGpa.college, record2->data.college);
                strcpy(studentGpa.major, record2->data.major);
                strcpy(studentGpa.studentID, record2->data.studentId);
                GPA += record2->data.gpa * record2->data.credit;
                Gredit += record2->data.credit;
            }
            record2 = record2->next;
        }

        float achGPA = 0;

        LoadAchievementsFromFile();
        AchievementNode* achieverecord = achievementHead;
        while (achieverecord != NULL) {
            if (strcmp(achieverecord->data.studentId, studentID[i]) == 0) {
                if (achieverecord->data.achievementType == 'A') {
                    achGPA = 0.05;
                    break;
                }
                else {
                    achGPA = 0.03;
                }
            }
            achieverecord = achieverecord->next;
        }
        Node1* gpanode = (Node1*)malloc(sizeof(Node1));
        if (gpanode == NULL) {
            printf("�ڴ����ʧ�ܡ�\n");
            return;
        }
        studentGpa.gpa = GPA / Gredit+achGPA;
        gpanode->data = studentGpa;
        gpanode->next = gpahead;
        gpahead = gpanode;
        GPA = 0; Gredit = 0;
        record1 = record1->next;
        i++;
    }
    Node1* gparecord1 = gpahead;
    Node1* gparecord2 = gpahead;

    char filename[100];

    stugpa studentGpa1;
    Node1* gpa = NULL;
    int n = 0;
    while (gparecord1 != NULL) {
        gparecord2 = gpahead;
        studentGpa1 = gparecord1->data;
        gpa = NULL;
        n = 0;
        while (gparecord2 != NULL) {
            if (strcmp(studentGpa1.college, gparecord2->data.college) == 0 && strcmp(studentGpa1.major, gparecord2->data.major) == 0) {
                if (n == 0) {
                    Node1* gpanode = (Node1*)malloc(sizeof(Node1));
                    gpanode->data = gparecord2->data;
                    gpanode->next = gpa;
                    gpa = gpanode;
                    gparecord2 = gparecord2->next;
                    n++;
                }
                else
                {
                    Node1* gpa1 = gpa;
                    while (gpa1!=NULL)
                    {
                        if (gpa1->data.gpa > gparecord2->data.gpa) {
                            if (gpa1->next == NULL) {
                                Node1* gpanode = (Node1*)malloc(sizeof(Node1));
                                gpanode->data = gparecord2->data;
                                gpanode->next = NULL;
                                gpa1->next = gpanode;
                                gparecord2 = gparecord2->next;
                                break;
                            }
                            else {
                                gpa1 = gpa1->next;
                                continue;
                            }
                                 
                        }
                        else {
                            Node1* gpa2 = gpa;
                           
                            while (gpa2->next!=NULL) {
                                if (gpa2 == gpa1)break;
                                if (gpa2->next == gpa1) {
                                    break;
                                }
                                gpa2 = gpa2->next;
                            }
                            if (gpa2 == gpa1) {
                                Node1* gpanode = (Node1*)malloc(sizeof(Node1));
                                gpanode->data = gparecord2->data;
                                gpanode->next = gpa;
                                gpa = gpanode;
                                gparecord2 = gparecord2->next;
                                break;
                            }
                            Node1* gpanode = (Node1*)malloc(sizeof(Node1));
                            gpanode->data = gparecord2->data;
                            gpanode->next = gpa1;
                            gpa2->next= gpanode;
                            gparecord2 = gparecord2->next;
                            break;
                        }
                    }

                }
            }
            else {
                gparecord2 = gparecord2->next;
            }
        }
        /*Node1* gpa3 = gpa; 
        while (gpa3 != NULL) {
            printf("%f\n", gpa3->data.gpa);
            gpa3 = gpa3->next;
        }*/
        sprintf(filename, "%s-%s-GPA-Ranked.txt", studentGpa1.college, studentGpa1.major);
        FILE* file = fopen(filename, "w");
        if (file != NULL) {
            while (gpa != NULL) {
                fprintf(file, "ѧ�ţ�%s,GPA��%.2f\n", gpa->data.studentID, gpa->data.gpa);
                gpa = gpa->next;
            }          
           }
            fclose(file);
            printf("�ɹ������ļ���%s-%s-GPA-Ranked.txt\n", studentGpa1.college, studentGpa1.major);
            gparecord1 = gparecord1->next;
            
    }
}




