#include "student_info.h"

void AddStudentInfoToList() {
    StudentInfo student;
    //int collegeAndMajorExist = 0;
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
            strcpy(student.collegeName, college[option1 - 1]);
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
            strcpy(student.majorName, major[option2 - 1]);
            break;
        }

    } while (1);
    LoadStudentInfoFromFile();
    printf("������ѧ��: ");
    int isValidstudentId;
    int p;
    do {
        p = 0;
        StudentNode* record = heads;
        scanf("%19s", student.studentId);
        isValidstudentId = 1; // �����û�����Ч
        while (record != NULL) {
            if (strcmp(record->info.studentId, student.studentId) == 0) {
                isValidstudentId = 0;
                p = 1;
                break;
            }
            record = record->next;
        }
        if (p == 1) {
            printf("ѧ���Ѵ��ڣ�����������:");
            continue;
        }

        if (strlen(student.studentId) != 8) {//����Ƿ�Ϊ8λ
            printf("��Ч��ѧ�ţ�ֻ������8λ����,����������\n");
            isValidstudentId = 0;
        }
        for (int i = 0; student.studentId[i] != '\0'; i++) {
            if (!isdigit(student.studentId[i])) { // ���ÿ���ַ��Ƿ�Ϊ����
                printf("��Ч��ѧ�ţ�ֻ������8λ����,����������\n");
                isValidstudentId = 0; // ������ַ������ַ�������û���Ϊ��Ч
                break; // �˳�ѭ��
            }
        }
    } while (isValidstudentId == 0);

    printf("������ѧ������: ");
    scanf(" %[^\n]", student.name);

    StudentNode* snewNode = (StudentNode*)malloc(sizeof(StudentNode));
    if (snewNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    snewNode->info = student;
    snewNode->next = heads;
    heads = snewNode;
    SaveStudentInfoToFile();
    printf("ѧ����Ϣ����ӵ�����\n");
}
void SaveStudentInfoToFile() {
    FILE* file = fopen("studentinfo.txt", "w");
    if (file == NULL) {
        printf("�޷����ļ� studentinfo.txt\n");
        return;
    }

    StudentNode* current = heads;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%s\n", current->info.studentId, current->info.name, current->info.collegeName, current->info.majorName);
        current = current->next;
    }

    fclose(file);

    printf("ѧ����Ϣ�ѱ��浽�ļ���\n");
}
void LoadStudentInfoFromFile() {
    FILE* file = fopen("studentinfo.txt", "r");
    if (file == NULL) {
        printf("�޷����ļ� studentinfo.txt ���ļ������ڡ�\n");
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
            printf("�ڴ����ʧ�ܡ�\n");
            fclose(file);
            return;
        }
        snewNode->info = student;
        snewNode->next = heads;
        heads = snewNode;
    }

    fclose(file);

    //   printf("ѧ����Ϣ�Ѵ��ļ����ء�\n");
}
void QueryStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            // �ҵ�ƥ���ѧ��ѧ�ţ���ӡѧ����Ϣ
            printf("�ҵ�ѧ����Ϣ��\n");
            printf("ѧ�ţ�%s\n", current->info.studentId);
            printf("������%s\n", current->info.name);
            printf("ѧԺ����%s\n", current->info.collegeName);
            printf("רҵ����%s\n", current->info.majorName);
            return;
        }
        current = current->next;
    }
    // �������������û���ҵ�����ʾû�и�ѧ������Ϣ
    printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����Ϣ��\n", studentId);
}
void UpdateStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    int found = 0;
    int collegeAndMajorExist = 0;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            found = 1;
            // �ҵ�ƥ���ѧ��ѧ�ţ���ʾ�û������µ�ѧԺ����רҵ��
            printf("�ҵ�ѧ����Ϣ��\n");
            printf("ѧ�ţ�%s\n", current->info.studentId);
            printf("������%s\n", current->info.name);
            printf("��ǰѧԺ����%s\n", current->info.collegeName);
            printf("��ǰרҵ����%s\n", current->info.majorName);


            while (!collegeAndMajorExist) {
                printf("������ѧԺ��: ");
                scanf(" %[^\n]", current->info.collegeName);
                printf("������רҵ��: ");
                scanf(" %[^\n]", current->info.majorName);

                if (CheckCollegeAndMajorExists(current->info.collegeName, current->info.majorName)) {
                    collegeAndMajorExist = 1; // ѧԺ����רҵ������
                }
                else {
                    printf("�����ѧԺ����רҵ�������ڣ����������롣\n");
                }
            }
            printf("ѧ����Ϣ�Ѹ��¡�\n");
            return;
        }
        current = current->next;
    }

    if (!found) {
        // �������������û���ҵ�����ʾû�и�ѧ������Ϣ
        printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����Ϣ��\n", studentId);
    }
}
int CheckCollegeAndMajorExists(const char* collegeName, const char* majorName) {
    FILE* file = fopen("info.txt", "r");
    if (file == NULL) {
        printf("�޷����ļ� info.txt ���ļ������ڡ�\n");
        return 0; // ��ʾѧԺ����רҵ��������
    }

    char line[256];
    char currentCollege[100], currentMajor[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        // ����info.txt�еĸ�ʽ��ѧԺ��,רҵ��
        if (sscanf(line, "%99[^,],%99[^\n]", currentCollege, currentMajor) == 2) {
            if (strcmp(currentCollege, collegeName) == 0 && strcmp(currentMajor, majorName) == 0) {
                fclose(file);
                return 1; // �ҵ�ƥ���ѧԺ����רҵ��������1��ʾ����
            }
        }
    }

    fclose(file);
    return 0; // �������ļ�δ�ҵ�ƥ���ѧԺ����רҵ��������0��ʾ������
}
void DeleteStudentInfoById(const char* studentId) {
    StudentNode* current = heads;
    StudentNode* previous = NULL;
    while (current != NULL) {
        if (strcmp(current->info.studentId, studentId) == 0) {
            // ����ҵ���Ҫɾ����ѧ����Ϣ
            if (previous == NULL) {
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                heads = current->next;
            }
            else {
                // ���Ҫɾ���Ľڵ㲻��ͷ�ڵ�
                previous->next = current->next;
            }
            free(current); // �ͷŽڵ��ڴ�
            printf("ѧ��ѧ��Ϊ %s ����Ϣ�ѱ�ɾ����\n", studentId);
            SaveStudentInfoToFile(); // ɾ���󱣴���µ��ļ�
            return;
        }
        previous = current;
        current = current->next;
    }
    // �������������û���ҵ�����ʾû�и�ѧ������Ϣ
    printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����Ϣ���޷�ɾ����\n", studentId);
}



void SaveCourseInfoToFile() {
    FILE* file = fopen("courses.txt", "w");
    if (file == NULL) {
        printf("�޷����ļ� courses.txt\n");
        return;
    }

    CourseNode* current = headc;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%.2f\n", current->info.collegeName, current->info.majorName,
            current->info.courseName, current->info.credits);
        current = current->next;
    }

    fclose(file);
    printf("�γ���Ϣ�ѱ��浽�ļ���\n");
}
void LoadCourseInfoFromFile() {
    FILE* file = fopen("courses.txt", "r");
    if (file == NULL) {
        printf("�޷����ļ� courses.txt ���ļ������ڡ�\n");
        return;
    }
    headc = NULL;
    CourseInfo course;
    while (fscanf(file, "%99[^,],%99[^,],%99[^,],%f\n", course.collegeName, course.majorName, course.courseName, &course.credits) == 4) {
        CourseNode* newNode = (CourseNode*)malloc(sizeof(CourseNode));
        if (newNode == NULL) {
            printf("�ڴ����ʧ�ܡ�\n");
            fclose(file);
            return;
        }
        newNode->info = course;
        newNode->next = headc;
        headc = newNode;
    }

    fclose(file);
    //    printf("�γ���Ϣ�Ѵ��ļ����ء�\n");
}
void QueryAllCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;
    if (current == NULL) {
        printf("��ǰû�пγ���Ϣ��\n");
        return;
    }

    printf("���пγ���Ϣ���£�\n");
    while (current != NULL) {
        printf("ѧԺ���ƣ�%s\n", current->info.collegeName);
        printf("רҵ���ƣ�%s\n", current->info.majorName);
        printf("�γ����ƣ�%s\n", current->info.courseName);
        printf("�γ�ѧ�֣�%.2f\n\n", current->info.credits);
        current = current->next;
    }
}
void QueryCourseInfoByCollege() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;
    int found = 0; // ���ڱ���Ƿ��ҵ�������һ��ƥ��Ŀγ�
    CourseInfo course;
    printf("������ѧԺ����: ");
    int i;
    char college[100][100];
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("��ѯѧԺΪ \"%s\" �Ŀγ���Ϣ���£�\n", course.collegeName);
    while (current != NULL) {
        if (strcmp(current->info.collegeName, course.collegeName) == 0) {
            // �����ǰ�ڵ��ѧԺ���������ѧԺ����ƥ�䣬��ӡ�γ���Ϣ
            printf("�γ����ƣ�%s\n", current->info.courseName);
            printf("רҵ���ƣ�%s\n", current->info.majorName);
            printf("�γ�ѧ�֣�%.2f\n\n", current->info.credits);
            found = 1;
        }
        current = current->next;
    }

    if (!found) {
        // ���û���ҵ��κ�ƥ��Ŀγ̣���ӡ��ʾ��Ϣ
        printf("δ�ҵ����� \"%s\" ѧԺ�Ŀγ���Ϣ��\n", course.collegeName);
    }
}
void AddCourseInfoToList() {
    LoadCourseInfoFromFile();
    CourseInfo course;

    printf("������ѧԺ����: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("������רҵ����: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    printf("������γ�����: ");
    scanf(" %[^\n]", course.courseName);

    printf("������γ�ѧ��: ");
    scanf("%f", &course.credits);

    CourseNode* record = headc;
    int p = 1;
    while (record != NULL) {
        p = 1;
        if (strcmp(record->info.collegeName, course.collegeName) == 0 &&
            strcmp(record->info.courseName, course.courseName) == 0 &&
            strcmp(record->info.majorName, course.majorName) == 0)
        {
            printf("�γ��Ѵ��ڣ�����������\n");
            p = 0;
            break;
        }
        record = record->next;
    }
    if (p == 0)return;

    CourseNode* newNode = (CourseNode*)malloc(sizeof(CourseNode));
    if (newNode == NULL) {
        printf("�ڴ����ʧ�ܡ�\n");
        return;
    }
    newNode->info = course;
    newNode->next = headc;
    headc = newNode;
    printf("�γ���Ϣ����ӵ�����\n");
    SaveCourseInfoToFile(); // ����γ���Ϣ���ļ�
}
void UpdateCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc;

    CourseInfo course;
    printf("������ѧԺ����: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("������רҵ����: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    char coursename[100][50];
    float credits[50];
    int k;
    printf("�γ����ƣ�");
    course_option(&k, course.collegeName, course.majorName, coursename, credits);
    for (int n = 0; n <= k; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n������ѡ�");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == k + 1) { return; }
        else if (option3 > k + 1 || option3 < 1) {
            printf("����������������룺");
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
            printf("�ҵ��γ̣�%s, ѧԺ��%s, רҵ��%s\n", course.courseName, course.collegeName, course.majorName);
            printf("��ǰ�γ�ѧ�֣�%.2f\n", current->info.credits);

            printf("�������µĿγ�ѧ��: ");
            scanf("%f", &current->info.credits);

            printf("�γ���Ϣ�Ѹ��¡�\n");
            SaveCourseInfoToFile(); // ���º󱣴�γ���Ϣ���ļ�
            break;
        }
        current = current->next;
    }
}
void DeleteCourseInfo() {
    LoadCourseInfoFromFile();
    CourseNode* current = headc, * previous = NULL;

    CourseInfo course;
    printf("������ѧԺ����: ");
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
            strcpy(course.collegeName, college[option1 - 1]);
            break;
        }

    } while (1);

    printf("������רҵ����: ");
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
            strcpy(course.majorName, major[option2 - 1]);
            break;
        }

    } while (1);

    char coursename[100][50];
    float credits[50];
    int k;
    printf("�γ����ƣ�");
    course_option(&k, course.collegeName, course.majorName, coursename, credits);
    for (int n = 0; n <= k; n++) {
        printf("%d.%s ", n + 1, coursename[n]);
    }
    int option3;
    printf("\n������ѡ�");
    do
    {
        scanf("%d", &option3);
        getchar();
        if (option3 == k + 1) { return; }
        else if (option3 > k + 1 || option3 < 1) {
            printf("����������������룺");
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
                // ���Ҫɾ���Ľڵ���ͷ�ڵ�
                headc = current->next;
            }
            else {
                // ���Ҫɾ���Ľڵ㲻��ͷ�ڵ�
                previous->next = current->next;
            }
            free(current); // �ͷŽڵ��ڴ�
            printf("�γ� \"%s\" ��Ϣ�ѱ�ɾ����\n", course.courseName);
            SaveCourseInfoToFile(); // ɾ���󱣴���µ��ļ�
            break;
        }
        previous = current;
        current = current->next;
    }
}
void QueryAllStudentInfo() {
    StudentNode* current = heads;
    if (current == NULL) {
        printf("��ǰû��ѧ����Ϣ��\n");
        return;
    }

    printf("����ѧ����Ϣ���£�\n");
    while (current != NULL) {
        printf("ѧ�ţ�%s\n", current->info.studentId);
        printf("������%s\n", current->info.name);
        printf("ѧԺ����%s\n", current->info.collegeName);
        printf("רҵ����%s\n\n", current->info.majorName);
        current = current->next;
    }
}