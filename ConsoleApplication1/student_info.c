#include "student_info.h"

void AddStudentInfoToList() {
    StudentInfo student;
    //int collegeAndMajorExist = 0;
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
            strcpy(student.collegeName, college[option1 - 1]);
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
            strcpy(student.majorName, major[option2 - 1]);
            break;
        }

    } while (1);
    LoadStudentInfoFromFile();
    printf("请输入学号: ");
    int isValidstudentId;
    int p;
    do {
        p = 0;
        StudentNode* record = heads;
        scanf("%19s", student.studentId);
        isValidstudentId = 1; // 假设用户名有效
        while (record != NULL) {
            if (strcmp(record->info.studentId, student.studentId) == 0) {
                isValidstudentId = 0;
                p = 1;
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            printf("学号已存在，请重新输入:");
            continue;
        }

        if (strlen(student.studentId) != 8) {//检查是否为8位
            printf("无效的学号，只能输入8位数字,请重新输入\n");
            isValidstudentId = 0;
        }
        for (int i = 0; student.studentId[i] != '\0'; i++) {
            if (!isdigit(student.studentId[i])) { // 检查每个字符是否为数字
                printf("无效的学号，只能输入8位数字,请重新输入\n");
                isValidstudentId = 0; // 如果发现非数字字符，标记用户名为无效
                break; // 退出循环
            }
        }
    } while (isValidstudentId == 0);

    printf("请输入学生姓名: ");
    scanf(" %[^\n]", student.name);

    StudentNode* snewNode = (StudentNode*)malloc(sizeof(StudentNode));
    if (snewNode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    snewNode->info = student;
    snewNode->next = heads;
    heads = snewNode;
    SaveStudentInfoToFile();
    printf("学生信息已添加到链表。\n");
}
void SaveStudentInfoToFile() {
    FILE* file = fopen("studentinfo.txt", "w");
    if (file == NULL) {
        printf("无法打开文件 studentinfo.txt\n");
        return;
    }

    StudentNode* current = heads;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s\n", current->info.studentId, current->info.name, current->info.collegeName, current->info.majorName);
        current = current->next;
    }

    fclose(file);

    printf("学生信息已保存到文件。\n");
}
void LoadStudentInfoFromFile() {
    FILE* file = fopen("studentinfo.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 studentinfo.txt 或文件不存在。\n");
        return;
    }

    while (heads != NULL) {
        StudentNode* temp = heads;
        heads = heads->next;
        free(temp);
    }

    StudentInfo student;
    while (fscanf(file, "%19[^,],%49[^,],%99[^,],%99[^\n]\n", student.studentId, student.name, student.collegeName, student.majorName) == 4) {
        StudentNode* snewNode = (StudentNode*)malloc(sizeof(StudentNode));
        if (snewNode == NULL) {
            printf("内存分配失败。\n");
            fclose(file);
            return;
        }
        snewNode->info = student;
        snewNode->next = heads;
        heads = snewNode;
    }

    fclose(file);

    //   printf("学生信息已从文件加载。\n");
}
void QueryStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            // 找到匹配的学生学号，打印学生信息
            printf("找到学生信息：\n");
            printf("学号：%s\n", current->info.studentId);
            printf("姓名：%s\n", current->info.name);
            printf("学院名：%s\n", current->info.collegeName);
            printf("专业名：%s\n", current->info.majorName);
            return;
        }
        current = current->next;
    }
    // 如果遍历完链表还没有找到，表示没有该学生的信息
    printf("未找到学号为 %s 的学生信息。\n", studentId);
}
void UpdateStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    int found = 0;
    int collegeAndMajorExist = 0;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            found = 1;
            // 找到匹配的学生学号，提示用户输入新的学院名和专业名
            printf("找到学生信息：\n");
            printf("学号：%s\n", current->info.studentId);
            printf("姓名：%s\n", current->info.name);
            printf("当前学院名：%s\n", current->info.collegeName);
            printf("当前专业名：%s\n", current->info.majorName);


            while (!collegeAndMajorExist) {
                printf("请输入学院名: ");
                scanf(" %[^\n]", current->info.collegeName);
                printf("请输入专业名: ");
                scanf(" %[^\n]", current->info.majorName);

                if (CheckCollegeAndMajorExists(current->info.collegeName, current->info.majorName)) {
                    collegeAndMajorExist = 1; // 学院名和专业名存在
                }
                else {
                    printf("输入的学院名或专业名不存在，请重新输入。\n");
                }
            }
            printf("学生信息已更新。\n");
            return;
        }
        current = current->next;
    }

    if (!found) {
        // 如果遍历完链表还没有找到，表示没有该学生的信息
        printf("未找到学号为 %s 的学生信息。\n", studentId);
    }
}
int CheckCollegeAndMajorExists(const char* collegeName, const char* majorName) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 info.txt 或文件不存在。\n");
        return 0; // 表示学院名和专业名不存在
    }

    char line[256];
    char currentCollege[100], currentMajor[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        // 假设info.txt中的格式是学院名,专业名
        if (sscanf(line, "%99[^,],%99[^\n]", currentCollege, currentMajor) == 2) {
            if (strcmp(currentCollege, collegeName) == 0 && strcmp(currentMajor, majorName) == 0) {
                fclose(file);
                return 1; // 找到匹配的学院名和专业名，返回1表示存在
            }
        }
    }

    fclose(file);
    return 0; // 遍历完文件未找到匹配的学院名和专业名，返回0表示不存在
}
void DeleteStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    StudentNode* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            // 如果找到了要删除的学生信息
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                heads = current->next;
            }
            else {
                // 如果要删除的节点不是头节点
                previous->next = current->next;
            }
            free(current); // 释放节点内存
            printf("学生学号为 %s 的信息已被删除。\n", studentId);
            SaveStudentInfoToFile(); // 删除后保存更新到文件
            return;
        }
        previous = current;
        current = current->next;
    }
    // 如果遍历完链表还没有找到，表示没有该学生的信息
    printf("未找到学号为 %s 的学生信息，无法删除。\n", studentId);
}



void SaveCourseInfoToFile() {
    FILE* file = fopen("courses.txt", "w");
    if (file == NULL) {
        printf("无法打开文件 courses.txt\n");
        return;
    }

    CourseNode* current = headc;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%.2f\n", current->info.collegeName, current->info.majorName,
            current->info.courseName, current->info.credits);
        current = current->next;
    }

    fclose(file);
    printf("课程信息已保存到文件。\n");
}
void LoadCourseInfoFromFile() {
    FILE* file = fopen("courses.txt", "r");
    if (file == NULL) {
        printf("无法打开文件 courses.txt 或文件不存在。\n");
        return;
    }
    headc = NULL;
    CourseInfo course;
    while (fscanf(file, "%99[^,],%99[^,],%99[^,],%f\n", course.collegeName, course.majorName, course.courseName, &course.credits) == 4) {
        CourseNode* newNode = (CourseNode*)malloc(sizeof(CourseNode));
        if (newNode == NULL) {
            printf("内存分配失败。\n");
            fclose(file);
            return;
        }
        newNode->info = course;
        newNode->next = headc;
        headc = newNode;
    }

    fclose(file);
    //    printf("课程信息已从文件加载。\n");
}
void QueryAllCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;
    if (current == NULL) {
        printf("当前没有课程信息。\n");
        return;
    }

    printf("所有课程信息如下：\n");
    while (current != NULL) {
        printf("学院名称：%s\n", current->info.collegeName);
        printf("专业名称：%s\n", current->info.majorName);
        printf("课程名称：%s\n", current->info.courseName);
        printf("课程学分：%.2f\n\n", current->info.credits);
        current = current->next;
    }
}
void QueryCourseInfoByCollege() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;
    int found = 0; // 用于标记是否找到了至少一个匹配的课程
    CourseInfo course;
    printf("请输入学院名称: ");
    int i;
    char college[100][100];
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("查询学院为 \"%s\" 的课程信息如下：\n", course.collegeName);
    while (current != NULL) {
        if (strcmp(current->info.collegeName, course.collegeName) == 0) {
            // 如果当前节点的学院名与给定的学院名相匹配，打印课程信息
            printf("课程名称：%s\n", current->info.courseName);
            printf("专业名称：%s\n", current->info.majorName);
            printf("课程学分：%.2f\n\n", current->info.credits);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        // 如果没有找到任何匹配的课程，打印提示信息
        printf("未找到属于 \"%s\" 学院的课程信息。\n", course.collegeName);
    }
}
void AddCourseInfoToList() {
    LoadCourseInfoFromFile();
    CourseInfo course;

    printf("请输入学院名称: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("请输入专业名称: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    printf("请输入课程名称: ");
    scanf(" %[^\n]", course.courseName);

    printf("请输入课程学分: ");
    scanf("%f", &course.credits);

    CourseNode* record = headc;
    int p = 1;
    while (record != NULL) {
        p = 1;
        if (strcmp(record->info.collegeName, course.collegeName) == 0 &&
            strcmp(record->info.courseName, course.courseName) == 0 &&
            strcmp(record->info.majorName, course.majorName) == 0)
        {
            printf("课程已存在，请重新输入\n");
            p = 0;
            break;
        }
        record = record->next;
    }
    if (p == 0)return;

    CourseNode* newNode = (CourseNode*)malloc(sizeof(CourseNode));
    if (newNode == NULL) {
        printf("内存分配失败。\n");
        return;
    }
    newNode->info = course;
    newNode->next = headc;
    headc = newNode;
    printf("课程信息已添加到链表。\n");
    SaveCourseInfoToFile(); // 保存课程信息到文件
}
void UpdateCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;

    CourseInfo course;
    printf("请输入学院名称: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("请输入专业名称: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    char coursename[100][50];
    float credits[50];
    int k;
    printf("课程名称：");
    course_option(&k, course.collegeName, course.majorName, coursename, credits);
    for (int n = 0; n <= k; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n请输入选项：");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == k + 1) { return; }
        else if (option3 > k + 1 || option3 < 1) {
            printf("输入错误，请重新输入：");
            continue;
        }
        else {
            strcpy(course.courseName, coursename[option3 - 1]);
            course.credits = credits[option3 - 1];
            break;
        }
    } while (1);
    while (current != NULL) {
        if (strcmp(current->info.collegeName, course.collegeName) == 0 && strcmp(current->info.majorName, course.majorName) == 0 && 
            strcmp(current->info.courseName, course.courseName) == 0) {
            printf("找到课程：%s, 学院：%s, 专业：%s\n", course.courseName, course.collegeName, course.majorName);
            printf("当前课程学分：%.2f\n", current->info.credits);

            printf("请输入新的课程学分: ");
            scanf("%f", &current->info.credits);

            printf("课程信息已更新。\n");
            SaveCourseInfoToFile(); // 更新后保存课程信息到文件
            break;
        }
        current = current->next;
    }
}
void DeleteCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc, * previous = NULL;

    CourseInfo course;
    printf("请输入学院名称: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("请输入专业名称: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    char coursename[100][50];
    float credits[50];
    int k;
    printf("课程名称：");
    course_option(&k, course.collegeName, course.majorName, coursename, credits);
    for (int n = 0; n <= k; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n请输入选项：");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == k + 1) { return; }
        else if (option3 > k + 1 || option3 < 1) {
            printf("输入错误，请重新输入：");
            continue;
        }
        else {
            strcpy(course.courseName, coursename[option3 - 1]);
            course.credits = credits[option3 - 1];
            break;
        }
    } while (1);





    while (current != NULL) {
        if (strcmp(current->info.collegeName, course.collegeName) == 0 &&
            strcmp(current->info.majorName, course.majorName) == 0 &&
            strcmp(current->info.courseName, course.courseName) == 0) {
            if (previous == NULL) {
                // 如果要删除的节点是头节点
                headc = current->next;
            }
            else {
                // 如果要删除的节点不是头节点
                previous->next = current->next;
            }
            free(current); // 释放节点内存
            printf("课程 \"%s\" 信息已被删除。\n", course.courseName);
            SaveCourseInfoToFile(); // 删除后保存更新到文件
            break;
        }
        previous = current;
        current = current->next;
    }
}
void QueryAllStudentInfo() {
    StudentNode* current = heads;
    if (current == NULL) {
        printf("当前没有学生信息。\n");
        return;
    }

    printf("所有学生信息如下：\n");
    while (current != NULL) {
        printf("学号：%s\n", current->info.studentId);
        printf("姓名：%s\n", current->info.name);
        printf("学院名：%s\n", current->info.collegeName);
        printf("专业名：%s\n\n", current->info.majorName);
        current = current->next;
    }
}