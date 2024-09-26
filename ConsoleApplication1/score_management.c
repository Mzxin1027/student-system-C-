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
        printf("未找到现有的成绩数据。\n");
        return;
    }

    head = NULL; // 清空现有链表

    ScoreInfo info;
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) { // 注意这里最后一个格式化字符串为%f
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL) {
            printf("内存分配失败。\n");
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
        printf("打开成绩文件写入失败。\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s,%s,%.2f,%.2f,%.2f\n", // 注意这里最后一个格式化字符串改为%.2f
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.score,
            current->data.gpa, current->data.credit); // 学分以小数形式保存
        current = current->next;
    }
    fclose(file);
}
void AddNewScoreRecord() {
    int isValidScore = 0;
    ScoreInfo newRecord;
    LoadStudentInfoFromFile();
    LoadScoresFromFile();
    printf("请输入学号: ");
    int p;
    do {
        p = 1;
        StudentNode* record = heads;
        scanf("%19s", newRecord.studentId);
        isValidScore = 1; // 假设用户名有效
        while (record != NULL) {
            if (strcmp(record->info.studentId, newRecord.studentId) == 0) {
                strcpy(newRecord.college, record->info.collegeName);
                strcpy(newRecord.major, record->info.majorName);
                strcpy(newRecord.name, record->info.name);
                p = 0;
                printf("成功输入\n");
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            isValidScore = 0;
            printf("学号不存在，请重新输入:");
        }
    } while (isValidScore == 0);
    StudentNode* record = heads;
    char coursename[100][50];
    float credits[50];
    int i;
    printf("课程名称：");
    course_option(&i, newRecord.college, newRecord.major, coursename, credits);
    for (int n = 0; n <= i; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n请输入选项：");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == i + 1) { return; }
        else if (option3 > i + 1 || option3 < 1) {
            printf("输入错误，请重新输入：");
            continue;
        }
        else {
            strcpy(newRecord.courseName, coursename[option3 - 1]);
            newRecord.credit = credits[option3 - 1];
            break;
        }
    } while (1);

// 检查是否已存在相同学号的记录，并验证学院和专业名是否一致
    Node* current = head;
    while (current != NULL) {
        if (strcmp(current->data.studentId, newRecord.studentId) == 0&&strcmp(current->data.courseName,newRecord.courseName)==0) {
            // 学号已存在，检查课程是否已存在

            printf("课程成绩已录入，请勿重复输入\n");
            return;
        }
        current = current->next;
    }


    while (isValidScore == 1) {
        printf("请输入成绩 (浮点数，0-100): ");
        if (scanf("%f", &newRecord.score) != 1 || newRecord.score < 0 || newRecord.score > 100) {
            printf("无效的成绩，请重新输入。\n");
            isValidScore = 0;
            while (getchar() != '\n'); // 清空输入缓冲区
        }
        else {
            isValidScore = 0;
        }
    }

    newRecord.gpa = ConvertScoreToGPA(newRecord.score);

    

    // 如果学号不存在或姓名和学院/专业名匹配，添加新记录
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    newNode->data = newRecord;
    newNode->next = head;
    head = newNode;

    SaveScoresToFile(); // 假设此函数已实现，用于将数据保存到文件
    printf("新的成绩记录已成功添加。\n");
}
void ModifyScoreRecord() {
    JudgeRole();
    LoadScoresFromFile();
    char studentId[20];
    char courseName[100];
    float newScore; // 修改变量类型为float以支持浮点数
    printf("请输入要修改成绩记录的学生学号: ");
    scanf("%s", studentId);
    printf("请输入要修改成绩记录的课程名称: ");
    scanf(" %[^\n]", courseName);

    Node* current = head;
    int found = 0;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.courseName, courseName) == 0) {
            printf("当前%s在%s的成绩是%.2f。\n", studentId, courseName, current->data.score);

            int isValidScore = 0;
            while (!isValidScore) {
                printf("请输入新成绩 (浮点数，0-100): ");
                // 使用%f格式说明符接受浮点数输入
                if (scanf("%f", &newScore) != 1 || newScore < 0 || newScore > 100) {
                    printf("无效的成绩，请重新输入。\n");
                    // 清空输入缓冲区，以防止无效输入干扰后续输入
                    while (getchar() != '\n');
                }
                else {
                    isValidScore = 1;
                }
            }

            // 更新成绩
            current->data.score = newScore;
            // 根据新成绩重新计算GPA
            current->data.gpa = ConvertScoreToGPA(newScore);

            printf("成绩记录更新成功。\n");
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("未找到匹配的成绩记录。\n");
    }
    else {
        // 成绩更新后，重新保存链表到文件
        SaveScoresToFile();
    }
}

void UpdateTransferInfo() {
    char studentId[20];
    char newCollege[100];
    char newMajor[100];
    int found = 0;

    printf("输入学生学号: ");
    scanf("%19s", studentId);
    printf("请输入学院名: ");
    int i, j;
    char college[100][100];
    char major[100][100];
    College_option(&i, college);
    for (int n = 0; n <= i; n++) {
        printf("%d . %s ", n + 1, college[n]);
    }
    printf("\n请输入选项:");
    int option1;
    do
    {
        scanf("%d", &option1);
        getchar();
        if (option1 - 1 > i || option1 < 1) {
            printf("请重新输入:");
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

    printf("请输入专业名: ");
    Major_option(&j, option1, college, major);
    for (int n = 0; n <= j; n++) {
        printf("%d . %s ", n + 1, major[n]);
    }
    printf("\n请输入选项:");
    int option2;
    do
    {
        scanf("%d", &option2);
        getchar();
        if (option2 - 1 > j || option2 < 1) {
            printf("\n请重新输入:");
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
    // 加载现有成绩数据到链表
    LoadScoresFromFile();

    // 遍历链表，更新匹配的学生记录
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
        // 如果找到并更新了学生记录，将更新后的数据写回文件
        SaveScoresToFile();
        printf("成功为学生 %s 更新学院为 %s 和专业为 %s。\n", studentId, newCollege, newMajor);
    }
    else {
        printf("未找到学生信息 %s。\n", studentId);
    }
}

void SaveScoresByCollege() {
    if (head == NULL) {
        printf("没有成绩数据可供保存。\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        // 根据学院名和专业名生成文件名
        char collegeAndMajorFilename[200]; // 增加数组大小以容纳更长的文件名
        sprintf(collegeAndMajorFilename, "%s-%s_scores.txt", current->data.college, current->data.major); // 生成文件名格式为 "学院名-专业名_scores.txt"

        // 以追加模式打开或创建以学院名和专业名命名的文件
        FILE* collegeAndMajorFile = fopen(collegeAndMajorFilename, "a");
        if (!collegeAndMajorFile) {
            printf("无法打开或创建文件 %s\n", collegeAndMajorFilename);
            current = current->next; // 处理下一个节点
            continue;
        }

        // 将当前成绩记录写入对应的学院和专业文件
        fprintf(collegeAndMajorFile, "%s,%s,%s,%s,%s,%.2f,%.2f,%.f\n",
            current->data.college, current->data.major, current->data.studentId, current->data.name,
            current->data.courseName, current->data.score,
            current->data.gpa, current->data.credit);
        fclose(collegeAndMajorFile); // 写入完成后关闭文件

        current = current->next; // 处理下一个节点
    }

    printf("按学院和专业分类的成绩文件已成功生成。\n");
}

float CalculateInnovationBonus(char* studentId) {
    InnovationProjectNode* current = innovationHead;
    float bonus = 0.0;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            bonus += 0.04; // 对于每个大创项目，加0.04
        }
        current = current->next;
    }
    return bonus;
}

void AddNewInnovationProject() {
    InnovationProjectInfo newProject;
    printf("请输入项目ID: ");
    scanf("%19s", newProject.projectId);
    printf("请输入参与学生的学号: ");
    scanf("%19s", newProject.studentId);
    printf("请输入项目的加分点数: ");
    scanf("%f", &newProject.bonusPoints);

    // 创建新节点并添加到链表头部
    InnovationProjectNode* newNode = (InnovationProjectNode*)malloc(sizeof(InnovationProjectNode));
    if (newNode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    newNode->data = newProject;
    newNode->next = innovationHead;
    innovationHead = newNode;

    // 追加新的大创项目记录到dc.txt文件
    FILE* file = fopen("dc.txt", "a"); // 注意这里使用追加模式"a"
    if (!file) {
        printf("打开dc.txt文件失败。\n");
        free(newNode); // 如果打开文件失败，释放之前分配的内存
        return;
    }
    fprintf(file, "%s,%s,%.2f\n",
        newProject.projectId,
        newProject.studentId,
        newProject.bonusPoints);
    fclose(file);

    printf("新的大创项目记录已成功添加并保存到dc.txt。\n");
}
// 修改CalculateAverageGPA函数，在计算平均GPA之前调用CalculateInnovationBonus函数

void QueryStudentInfo() {
    char studentId[20];
    printf("请输入要查询的学生学号: ");
    scanf("%19s", studentId);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("未找到现有的成绩数据。\n");
        return;
    }

    ScoreInfo info;
    int found = 0;
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        if (strcmp(info.studentId, studentId) == 0) {
            printf("学院: %s, 专业: %s, 学生姓名: %s, 课程名称: %s, 成绩: %.2f, 绩点: %.2f, 学分: %.2f\n",
                info.college, info.major, info.name, info.courseName, info.score, info.gpa, info.credit);
            found = 1;

        }
    }

    fclose(file);

    if (!found) {
        printf("未找到学生 %s 的成绩记录。\n", studentId);
    }
}

void SaveScoresByCollegeToFile() {
    Node* current = head;
    CollegeGPAList* collegeGPAHead = NULL; // 学院GPA信息链表的头指针

    // 遍历成绩链表，保存成绩信息并计算每个学生的GPA
    while (current != NULL) {
        // 更新学院GPA信息链表
        UpdateCollegeGPAList(&collegeGPAHead, current);
        current = current->next;
    }

    // 遍历学院GPA信息链表，计算平均GPA并保存到文件
    CollegeGPAList* collegeCurrent = collegeGPAHead;
    while (collegeCurrent != NULL) {
        // 为每个学院创建一个GPA文件
        char collegeGPAFilename[150];
        sprintf(collegeGPAFilename, "%sGPA.txt", collegeCurrent->collegeName);
        FILE* gpaFile = fopen(collegeGPAFilename, "w");
        if (gpaFile) {
            CollegeStudentGPA* studentCurrent = collegeCurrent->studentHead;
            while (studentCurrent != NULL) {
                float averageGPA = studentCurrent->scoreCount > 0 ? studentCurrent->totalGPA / studentCurrent->scoreCount : 0;
                fprintf(gpaFile, "学号: %s, 姓名: %s, 专业名: %s, 平均GPA: %.2f\n",
                    studentCurrent->studentId, studentCurrent->name, studentCurrent->major, averageGPA);
                studentCurrent = studentCurrent->next;
            }
            fclose(gpaFile);
        }
        collegeCurrent = collegeCurrent->next;
    }

    // 释放学院GPA信息链表的内存
    FreeCollegeGPAList(collegeGPAHead);
    printf("按学院分类平均GPA文件已成功生成。\n");
}

// 定义用于判断正态分布的函数
int isNormalDistribution(float mean, float stddev, int totalCount, Node* head, const char* courseName) {
    // 简化的正态分布判断，实际应用中可能需要更复杂的统计测试
    // 这里只是一个基本的示例，实际情况下可能需要根据样本大小、偏度和峰度等进行判断
    float skewness = 0.0; // 偏度
    float kurtosis = 0.0; // 峰度
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

    // 简化的判断，实际情况下可能需要更复杂的逻辑
    return fabs(skewness) < 0.5 && fabs(kurtosis - 3) < 1;
}

// 新增的查看课程统计信息的函数
void AnalyzeCourseStatistics(const char* courseName) {
    FILE* file = fopen("score.txt", "r"); // 确保这是存储成绩信息的正确文件名
    if (!file) {
        printf("未找到现有的成绩数据。\n");
        return;
    }

    char outputFilename[128];
    sprintf(outputFilename, "%s_statistics.txt", courseName);

    FILE* outputFile = fopen(outputFilename, "w");
    if (!outputFile) {
        printf("无法创建或打开输出文件：%s\n", outputFilename);
        fclose(file);
        return;
    }

    int totalCount = 0, passCount = 0, excellentCount = 0;
    float maxScore = -1, minScore = 101; // 初始化最高分和最低分
    float scoreSum = 0;
    ScoreInfo info;

    // 第一次遍历：计算总分、最高分、最低分、及格人数和优秀人数
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

    // 重置文件指针到文件开头，准备第二次遍历
    fseek(file, 0, SEEK_SET);

    float varianceSum = 0;
    // 第二次遍历：计算方差
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        if (strcmp(info.courseName, courseName) == 0) {
            float deviation = info.score - mean;
            varianceSum += deviation * deviation;
        }
    }

    float variance = varianceSum / totalCount;


    // 输出到文件
    fprintf(outputFile, "课程 %s 的统计信息：\n", courseName);
    fprintf(outputFile, "总人数：%d\n", totalCount);
    fprintf(outputFile, "最高分：%f\n", maxScore);
    fprintf(outputFile, "最低分：%f\n", minScore);
    fprintf(outputFile, "及格率：%.2f%%\n", ((float)passCount / totalCount) * 100);
    fprintf(outputFile, "优秀率：%.2f%%\n", ((float)excellentCount / totalCount) * 100);
    fprintf(outputFile, "平均分：%.2f\n", mean);
    fprintf(outputFile, "方差：%.2f\n", variance); // 输出方差
    fclose(file);
    fclose(outputFile);
    printf("课程 %s 的统计信息已成功生成并保存到 %s。\n", courseName, outputFilename);
}


void LoadCollegesFromScores(const char* filename, char colleges[MAX_COLLEGES][100], int* collegeCount) {
    FILE* file = fopen(filename, "r");
    char line[256];
    char college[100];
    *collegeCount = 0;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%99[^\n]", college);
        // 检查学院是否已在列表中
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
    printf("请输入要查询的学院名称: ");
    scanf("%99s", collegeName); // 注意使用%99s以避免缓冲区溢出

    Node* current = head;
    int found = 0;
    printf("学院 %s 的成绩如下：\n", collegeName);
    while (current != NULL) {
        if (strcmp(current->data.college, collegeName) == 0) {
            printf("学号: %s, 姓名: %s, 课程名称: %s, 成绩: %.2f, 绩点: %.2f\n",
                current->data.studentId, current->data.name,
                current->data.courseName, current->data.score, current->data.gpa);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        printf("未找到学院 %s 的成绩记录。\n", collegeName);
    }
}

void QueryScoreByStudentId(const char* studentId) {
    // 确保在查询之前，成绩数据已经加载到链表中
    // 如果成绩数据已经在程序其他部分加载，这一步可能不需要
    LoadScoresFromFile();

    Node* current = head;
    int found = 0; // 标记是否找到对应学生的成绩

    printf("学生ID %s 的成绩如下：\n", studentId);
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // 找到匹配的成绩记录，打印详细信息
            printf("课程名称: %s, 成绩: %.1f, 绩点: %.2f, 学分: %.2f\n",
                current->data.courseName, current->data.score, current->data.gpa, current->data.credit);
            found = 1; // 标记找到至少一条成绩记录
        }
        current = current->next;
    }

    if (!found) {
        // 如果未找到任何成绩记录
        printf("未找到学生ID %s 的任何成绩记录。\n", studentId);
    }
}



void SortAndSaveGPA(const char* inputFilename) {
    FILE* inputFile = fopen(inputFilename, "r");
    if (!inputFile) {
        printf("Unable to open file %s for reading.\n", inputFilename);
        return;
    }

    StudentGPA students[MAX_COLLEGES]; // 假设最多100个学生
    int count = 0;
    while (fscanf(inputFile, "%[^,],%[^,],%f\n", students[count].studentId, students[count].name, &students[count].gpa) == 3) {
        count++;
    }
    fclose(inputFile);

    qsort(students, count, sizeof(StudentGPA), compareGPA);

    char outputFilename[256];
    snprintf(outputFilename, sizeof(outputFilename), "%s_ranked.txt", inputFilename); // 生成排名文件名
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
// 函数用于从score.txt中提取所有唯一的学院和专业组合
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
            // 找到了对应的学院和专业，将学生信息添加到scores字符串中
            // 注意：这里需要将ScoreInfo转换为字符串形式并追加到current->scores中
            // 这里略过具体的字符串处理过程
            return;
        }
        last = current;
        current = current->next;
    }
    // 如果没有找到对应的学院和专业，创建新节点
    CollegeMajorScoreEntry* newEntry = (CollegeMajorScoreEntry*)malloc(sizeof(CollegeMajorScoreEntry));
    strcpy(newEntry->college, studentInfo->college);
    strcpy(newEntry->major, studentInfo->major);
    // 初始化scores字符串，然后将学生信息添加进去
    // 这里略过具体的字符串处理过程
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
    int lowScoreCount = 0; // 成绩低于60分的课程数量
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            totalGPA += current->data.gpa;
            courseCount++;
            if (current->data.score < 60) {
                printf("需要加强的课程: %s\n", current->data.courseName);
                lowScoreCount++;
            }
        }
        current = current->next;
    }

    if (courseCount == 0) {
        printf("未找到学生 %s 的成绩记录。\n", studentId);
        return;
    }

    float averageGPA = totalGPA / courseCount;
    printf("平均GPA: %.2f\n", averageGPA);

    // 检查大创项目
    float innovationBonus = CalculateInnovationBonus(studentId);
    if (innovationBonus > 0) {
        printf("大创项目加分: %.2f\n", innovationBonus);
    }

    // 预测保研机会
    if (averageGPA >= 3.5 && lowScoreCount == 0) {
        printf("学生 %s 有很好的保研机会。\n", studentId);
    }
    else {
        printf("学生 %s 需要在以下方面努力以提高保研机会：\n", studentId);
        if (averageGPA < 3.5) {
            printf("- 提高平均GPA，当前平均GPA为 %.2f\n", averageGPA);
        }
        if (lowScoreCount > 0) {
            printf("- 加强基础课程学习，有 %d 门课程成绩低于60分。\n", lowScoreCount);
        }
    }
}

// 函数用于检查给定的学院和专业组合是否已经存在于链表中
int existsInList(const char* college, const char* major) {
    CollegeMajorPair* current = uniqueCollegeMajorPairs;
    while (current != NULL) {
        if (strcmp(current->college, college) == 0 && strcmp(current->major, major) == 0) {
            return 1; // 已存在
        }
        current = current->next;
    }
    return 0; // 不存在
}

//添加学院和专业// 函数用于将新的学院和专业组合添加到链表中
void addToList(const char* college, const char* major) {
    if (existsInList(college, major)) {
        // 如果学院和专业的组合已存在，则不重复添加
        printf("学院和专业的组合已存在，请勿重复添加");
        return;
    }

    CollegeMajorPair* newNode = (CollegeMajorPair*)malloc(sizeof(CollegeMajorPair));
    if (newNode == NULL) {
        printf("内存分配失败。\n");
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
        printf("info.txt文件不存在，将创建新的文件。\n");
        return;
    }

    char college[100], major[100];
    while (fscanf(file, "%99[^,],%99[^\n]\n", college, major) == 2) {
        // 由于addToList函数内部已经包含了重复检查的逻辑
        // 这里直接调用addToList函数即可
        addToList(college, major);
    }

    fclose(file);

}

void saveCollegeMajorPairs() {
    FILE* file = fopen("info.txt", "w"); // 以写模式打开文件，覆盖原有内容
    if (!file) {
        printf("无法打开或创建info.txt文件。\n");
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
    int found = 0; // 标记是否找到并删除了至少一条记录

    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0) {
            // 找到了要删除的成绩记录
            found = 1;
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                head = current->next;
                free(current); // 释放节点内存
                current = head; // 更新current为新的头节点
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
        printf("学生学号为 %s 的所有成绩记录已被删除。\n", studentId);
        SaveScoresToFile(); // 删除操作后保存更新到文件
    }
    else {
        // 如果遍历完链表还没有找到，表示没有该学生的成绩记录
        printf("未找到学号为 %s 的成绩记录，无法删除。\n", studentId);
    }
}
void DeleteScoreRecord(const char* studentId, const char* courseName) {
    Node* current = head;
    Node* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->data.studentId, studentId) == 0 && strcmp(current->data.courseName, courseName) == 0) {
            // 如果找到了要删除的成绩记录
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                head = current->next;
            }
            else {
                // 如果要删除的节点不是头节点
                previous->next = current->next;
            }
            free(current); // 释放节点内存
            printf("学生学号为 %s 的课程 %s 成绩记录已被删除。\n", studentId, courseName);
            SaveScoresToFile(); // 删除后保存更新到文件
            return;
        }
        previous = current;
        current = current->next;
    }
    // 如果遍历完链表还没有找到，表示没有该学生的成绩记录
    printf("未找到学号为 %s 且课程名称为 %s 的成绩记录，无法删除。\n", studentId, courseName);
}



//从文件中读取学院名
void College_option(int* i, char college[][100]) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("info.txt文件不存在，将创建新的文件。\n");
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
    strcpy(college[*i], "返回");
    fclose(file);
}

//从对应学院中读取对应专业名称
void Major_option(int* j, int option1, const char college[][100], char major[][100]) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("info.txt文件不存在，将创建新的文件。\n");
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
    strcpy(major[*j], "返回");
    fclose(file);
}

//从对应学院中读取对应专业名称
void course_option(int* i, const char college[50], const char major[50], char course[100][50], float credits[50]) {
    FILE* pf = fopen("courses.txt", "r");
    if (pf == NULL) {
        printf("courses.txt文件不存在，将创建新的文件。\n");
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
    strcpy(course[*i], "返回");
    fclose(pf);
}
void Stu_appeal_score(const char username[50]) {
    loadappealsInfoTofile();
    node* record = appealshead;
    printf("****已申诉信息****\n");
    int Found = 0;
    while (record != NULL)
    {
        if (strcmp(username, record->data.studentId) == 0) {
            printf("学号：%s，姓名：%s，课程：%s，是否申诉成功：%c\n",
                record->data.studentId, record->data.name, record->data.courseName, record->data.appealinfo);
            Found = 1;
        }
        record = record->next;
    }
    if (Found == 0) {
        printf("暂无申诉信息\n");
    }
    printf("****T为成功，F为失败****\n");
    appealshead = NULL;
    LoadScoresFromFile();
    printf("请输入你想申诉的课程：");
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
        printf("暂无你想申诉的课程成绩信息");
        return;
    }
    node* newnode = (node*)malloc(sizeof(node));
    if (newnode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    newnode->data = info;
    newnode->next = appealshead;
    appealshead = newnode;
    addappealsInfoTofile();
    printf("成绩申诉上报成功");
}
void addappealsInfoTofile() {
    FILE* file = fopen("appealsInfo.txt", "a");
    if (!file) {
        printf("打开申诉文件或写入失败。\n");
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
        printf("未有申诉信息。\n");
        return;
    }
    appealshead = NULL;
    AppealsInfo current;
    while (fscanf(pfile, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%c\n", current.college, current.major,
        current.studentId, current.name, current.courseName, &current.appealinfo) == 6)
    {
        node* newnode = (node*)malloc(sizeof(node));
        if (newnode == NULL) {
            printf("内存分配失败。\n");
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
        printf("学院：%s，专业：%s，学号：%s，姓名：%s，课程：%s，是否申诉成功：%c\n",
            record->data.college, record->data.major, record->data.studentId, record->data.name,
            record->data.courseName, record->data.appealinfo);
        record = record->next;
    }
    printf("****T为成功，F为失败****\n");
    printf("若修改成功，请输入对应学号和对应课程，更改申诉信息\n");
    printf("请输入对应学号：");
    char stuID[20], coursename[50];
    scanf("%s", stuID);
    printf("请输入对应课程：");
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
        printf("无对应学号或课程");
        return;
    }
    saveappealsInfoTofile();
    printf("更改成功");
}
void saveappealsInfoTofile() {
    FILE* file = fopen("appealsInfo.txt", "w");
    if (!file) {
        printf("打开申诉文件或写入失败。\n");
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

// 从info.txt中读取所有学院和专业组合，供用户选择
void LoadCollegesAndMajors(char colleges[MAX_COLLEGES][100], char majors[MAX_COLLEGES][100], int* count) {
    FILE* file = fopen("info.txt", "r");
    if (!file) {
        printf("无法打开info.txt文件进行读取。\n");
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
// 查询并打印指定专业的排名信息
void QueryRankingByCollegeAndMajor(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("无法打开文件 %s 进行读取或文件不存在。\n", filename);
        return;
    }

    printf("排名信息如下：\n");
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

    printf("请选择学院和专业：\n");
    for (int i = 0; i < count; i++) {
        printf("%d. 学院：%s - 专业：%s\n", i + 1, colleges[i], majors[i]);
    }

    int choice;
    printf("输入你的选择（数字）：");
    scanf("%d", &choice);
    getchar();
    // 验证用户输入
    if (choice < 1 || choice > count) {
        printf("无效选择。\n");
        return;
    }

    // 根据用户选择生成文件名
    char filename[256];
    sprintf(filename, "%s-%s-GPA-Ranked.txt", colleges[choice - 1], majors[choice - 1]);

    // 查询并显示排名信息
    QueryRankingByCollegeAndMajor(filename);
}
// 辅助函数，检查学生ID是否已经存在于数组中
int isStudentIdProcessed(const char studentIds[][20], int size, const char* studentId) {
    for (int i = 0; i < size; i++) {
        if (strcmp(studentIds[i], studentId) == 0) {
            return 1; // 找到了，已处理
        }
    }
    return 0; // 未处理
}
void QueryAllScores() {
    FILE* file = fopen("score.txt", "r");
    if (!file) {
        printf("无法打开score.txt文件进行读取。\n");
        return;
    }

    ScoreInfo info;
    printf("所有学生成绩记录如下：\n");
    while (fscanf(file, "%99[^,],%99[^,],%19[^,],%49[^,],%99[^,],%f,%f,%f\n",
        info.college, info.major, info.studentId, info.name,
        info.courseName, &info.score, &info.gpa, &info.credit) == 8) {
        // 打印每条记录的详细信息
        printf("学院: %s, 专业: %s, 学号: %s, 姓名: %s, 课程名称: %s, 成绩: %.2f, 绩点: %.2f, 学分: %.2f\n",
            info.college, info.major, info.studentId, info.name,
            info.courseName, info.score, info.gpa, info.credit);
    }

    fclose(file);
}

void FreeCollegeMajorPairList(CollegeMajorPair* head) {
    while (head != NULL) {
        CollegeMajorPair* temp = head;
        head = head->next;

        // 释放与当前CollegeMajorPair关联的学生GPA信息链表
        StudentGPAInfo* gpaInfo = temp->studentGPAHead;
        while (gpaInfo != NULL) {
            StudentGPAInfo* gpaTemp = gpaInfo;
            gpaInfo = gpaInfo->next;
            free(gpaTemp);
        }

        // 释放当前的CollegeMajorPair节点
        free(temp);
    }
}





int compareGPA(const void* a, const void* b) {
    const StudentGPA* ga = (const StudentGPA*)a;
    const StudentGPA* gb = (const StudentGPA*)b;
    // 降序排序
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

    // GPA更高的学生应排在前面，因此如果a的GPA大于b的，返回-1
    if (studentA->gpa > studentB->gpa) return -1;
    if (studentA->gpa < studentB->gpa) return 1;
    return 0;
}

void UpdateCollegeGPAList(CollegeGPAList** head, Node* scoreNode) {
    // 查找或创建学院节点
    CollegeGPAList* college = *head;
    while (college != NULL && strcmp(college->collegeName, scoreNode->data.college) != 0) {
        college = college->next;
    }
    if (college == NULL) { // 如果学院不存在，则创建新学院节点
        college = (CollegeGPAList*)malloc(sizeof(CollegeGPAList));
        strcpy(college->collegeName, scoreNode->data.college);
        college->studentHead = NULL;
        college->next = *head;
        *head = college;
    }

    // 在学院节点中查找或创建学生节点
    CollegeStudentGPA* student = college->studentHead;
    while (student != NULL && strcmp(student->studentId, scoreNode->data.studentId) != 0) {
        student = student->next;
    }
    if (student == NULL) { // 如果学生不存在，则创建新学生节点
        student = (CollegeStudentGPA*)malloc(sizeof(CollegeStudentGPA));
        strcpy(student->studentId, scoreNode->data.studentId);
        strcpy(student->name, scoreNode->data.name);
        strcpy(student->major, scoreNode->data.major);
        student->totalGPA = 0;
        student->scoreCount = 0;
        student->next = college->studentHead;
        college->studentHead = student;
    }

    // 更新学生的GPA信息
    student->totalGPA += scoreNode->data.gpa;
    student->scoreCount += 1;
}

void FreeCollegeGPAList(CollegeGPAList* head) {
    CollegeGPAList* current = head;
    while (current != NULL) {
        CollegeGPAList* temp = current;
        current = current->next;

        // 释放学生 GPA 链表
        CollegeStudentGPA* studentCurrent = temp->studentHead;
        while (studentCurrent != NULL) {
            CollegeStudentGPA* tempStudent = studentCurrent;
            studentCurrent = studentCurrent->next;
            free(tempStudent);
        }

        free(temp); // 释放学院节点
    }
}
 
// 比较函数，用于qsort，比较两个StudentGPA结构体的GPA

void RankAllGPAFiles() {
    CollegeMajorPair* currentPair = uniqueCollegeMajorPairs;

    while (currentPair != NULL) {
        // 为当前学院和专业的所有学生创建一个动态数组
        int studentCount = 0;
        StudentGPAInfo** studentArray = (StudentGPAInfo**)malloc(sizeof(StudentGPAInfo*) * MAX_STUDENTS);
        StudentGPAInfo* currentStudent = currentPair->studentGPAHead;

        while (currentStudent != NULL) {
            studentArray[studentCount++] = currentStudent;
            currentStudent = currentStudent->next;
        }

        // 对学生数组进行GPA排序
        qsort(studentArray, studentCount, sizeof(StudentGPAInfo*), compareStudentGPA);

        // 创建文件名并打开文件
        char filename[256];
        sprintf(filename, "%s-%s_GPA_Ranked.txt", currentPair->college, currentPair->major);
        FILE* file = fopen(filename, "w");
        if (!file) {
            printf("无法创建文件 %s\n", filename);
            free(studentArray);
            currentPair = currentPair->next;
            continue;
        }

        // 写入排名信息
        for (int i = 0; i < studentCount; i++) {
            fprintf(file, "Rank: %d, Student ID: %s, Name: %s, GPA: %.2f\n",
                i + 1, studentArray[i]->studentId, studentArray[i]->name, studentArray[i]->gpa);
        }

        // 关闭文件并释放资源
        fclose(file);
        free(studentArray);

        // 移动到下一个学院和专业组
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

    // 读取文件并填充college.students数组
    while (fscanf(file, "%[^,],%[^,],%f\n",
        college.students[college.studentCount].studentId,
        college.students[college.studentCount].name,
        &college.students[college.studentCount].gpa) == 3) {
        college.studentCount++;
    }

    // 对学生进行GPA排名
    qsort(college.students, college.studentCount, sizeof(StudentScore), compareGPA);

    fclose(file);

    // 写入排名结果到学院名gpa.txt
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
        printf("无法打开agpa.txt文件进行读取。\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), agpaFile)) {
        char studentId[20], name[50], major[100], college[100];
        float finalGPA; // 包括大创等成就加分后的最终GPA
        // 假设agpa.txt的格式为：学号, 姓名, 专业名, 学院, 最终GPA
        if (sscanf(line, "%19[^,],%49[^,],%99[^,],%99[^,],%f\n", studentId, name, major, college, &finalGPA) == 5) {
            char collegeFilename[150];
            sprintf(collegeFilename, "%sgpa.txt", college); // 生成以学院名命名的文件名

            // 以追加模式打开或创建学院对应的gpa文件
            FILE* collegeFile = fopen(collegeFilename, "a");
            if (!collegeFile) {
                printf("无法打开或创建文件 %s\n", collegeFilename);
                continue;
            }

            // 将学生信息和最终GPA写入对应的学院gpa文件
            fprintf(collegeFile, "学号: %s, 姓名: %s, 专业名: %s, 最终GPA: %.2f\n", studentId, name, major, finalGPA);
            fclose(collegeFile); // 写入完成后关闭文件
        }
    }
    fclose(agpaFile); // 关闭agpa.txt文件
    printf("按学院分类的平均GPA文件已成功生成。\n");
}
void CalculateAverageGPA() {
    FILE* file = fopen("agpa.txt", "w");
    if (!file) {
        printf("无法打开agpa.txt文件进行写入。\n");
        return;
    }

    char processedStudentIds[100][20]; // 假设最多处理100个学生
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

            // 处理成就加分
            float achievementBonus = CalculateAchievementBonus(current->data.studentId);
            float averageGPA = totalCredits > 0 ? (totalWeightedGPA / totalCredits) + achievementBonus : 0.0;
            fprintf(file, "学号: %s, 姓名: %s, 专业名: %s, 平均GPA: %.2f\n",
                current->data.studentId, current->data.name, current->data.major, averageGPA);
        }

        current = current->next;
    }

    fclose(file);
    printf("平均GPA文件agpa.txt已成功生成。\n");
}
void GenerateRankingFile() {
    FILE* file = fopen("paiming.txt", "w");
    if (!file) {
        printf("无法打开排名文件进行写入。\n");
        return;
    }

    CalculateAndAssignRanks();  // 确保已经计算和排序了GPA

    Node* current = head;
    float previousGPA = -1.0;  // 用于跟踪前一个学生的GPA
    int actualRank = 0;  // 实际排名
    int displayedRank = 0;  // 显示的排名（考虑并列情况）

    while (current != NULL) {
        actualRank++;
        if (current->data.gpa != previousGPA) {  // 如果GPA与前一个学生不同
            displayedRank = actualRank;
        }
        // 在每行记录中添加学生的专业信息
        fprintf(file, "Rank: %d, Name: %s, Student ID: %s, Major: %s, GPA: %.2f\n",
            displayedRank, current->data.name, current->data.studentId, current->data.major, current->data.gpa);
        previousGPA = current->data.gpa;
        current = current->next;
    }

    fclose(file);
    printf("排名文件成功生成。\n");
}
int Partition(Node** nodesArray, int low, int high) {
    float pivot = nodesArray[high]->data.gpa; // 选择最后一个元素的GPA作为基准
    int i = (low - 1); // 小于pivot的元素的索引

    for (int j = low; j <= high - 1; j++) {
        // 如果当前元素的GPA大于或等于pivot
        if (nodesArray[j]->data.gpa >= pivot) {
            i++; // 递增索引
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

        QuickSort(nodesArray, low, pi - 1); // 对基准左侧的元素进行排序
        QuickSort(nodesArray, pi + 1, high); // 对基准右侧的元素进行排序
    }
}
void UpdateRanks(Node** nodesArray, int count) {
    int rank = 1;
    for (int i = 0; i < count; i++) {
        if (i > 0 && nodesArray[i]->data.gpa == nodesArray[i - 1]->data.gpa) {
            // 如果当前学生的GPA与前一个学生相同，则并列排名
            nodesArray[i]->data.rank = nodesArray[i - 1]->data.rank;
        }
        else {
            // 否则，排名为当前位置加1
            nodesArray[i]->data.rank = rank;
        }
        rank++;
    }
}
void CalculateAndAssignRanks() {
    // 创建一个动态数组来存储链表中的所有节点指针
    int count = 0;
    Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    Node** nodesArray = (Node**)malloc(count * sizeof(Node*));
    if (nodesArray == NULL) {
        printf("内存分配失败。\n");
        return;
    }

    current = head;
    for (int i = 0; i < count; i++) {
        nodesArray[i] = current;
        current = current->next;
    }

    // 使用快速排序对节点进行排序
    QuickSort(nodesArray, 0, count - 1);

    // 更新排名，并考虑并列排名的情况
    UpdateRanks(nodesArray, count);

    free(nodesArray);
}



//计算GPA并生成排名文件
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
            printf("内存分配失败。\n");
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
                fprintf(file, "学号：%s,GPA：%.2f\n", gpa->data.studentID, gpa->data.gpa);
                gpa = gpa->next;
            }          
           }
            fclose(file);
            printf("成功生成文件：%s-%s-GPA-Ranked.txt\n", studentGpa1.college, studentGpa1.major);
            gparecord1 = gparecord1->next;
            
    }
}




