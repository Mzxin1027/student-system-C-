#include "user_management.h"
// �����û��ṹ�������ͷ�����ڴ��ļ���ͨ��utils.h����
void Register() {
    char username[50], password[50], email[100], role;
    int isValidUsername = 0; // ���������Ը����û����Ƿ���Ч
    int isValidRole = 0; // ���������Ը��ٽ�ɫ�Ƿ���Ч

    while (!isValidUsername) {
        printf("�����û������û���Ϊ���8λѧ�Ż򹤺ţ�: ");
        scanf("%s", username);
        while (strlen(username) != 8) {
            printf("\n�����������û���������Ϊ��λ���֣�:");
            scanf("%s", username);
        }

        isValidUsername = 1; // �����û�����Ч
        for (int i = 0; username[i] != '\0'; i++) {
            if (!isdigit(username[i])) { // ���ÿ���ַ��Ƿ�Ϊ����
                printf("\n��Ч���û�����ֻ����������\n");
                isValidUsername = 0; // ������ַ������ַ�������û���Ϊ��Ч
                break; // �˳�ѭ��
            }
        }

        if (isValidUsername) {
            // ����û�����Ч������Ƿ��Ѵ���
            Nodeu* current = headu;
            while (current != NULL) {
                if (strcmp(current->user.username, username) == 0) {
                    // ����ҵ���ͬ���û�������ʾ�û���Ҫ����������
                    printf("�û��� '%s' �Ѿ����ڡ� ��ѡ��ͬ���û�����\n", username);
                    isValidUsername = 0; // ����û���Ϊ��Ч�Դ�����������
                    break; // �˳�ѭ��
                }
                current = current->next;
            }
        }
    }

    // ����ע������
    printf("��������: ");
    GetPassword(password, sizeof(password)); // ʹ��GetPassword����scanf��������������
    printf("��������: ");
    scanf("%s", email);

    // ��֤��ɫ����
    while (!isValidRole) {
        printf("������� (a ��ʾ����Ա, s ��ʾѧ��): ");
        scanf(" %c", &role); // ע��ǰ��Ŀո���������ǰһ�������Ļ��з�
        if (role == 'a' || role == 's') {
            isValidRole = 1; // �����ɫ��Ч
        }
        else {
            printf("��Ч�Ľ�ɫ���롣������ a ��ʾ����Ա  ��  s ��ʾѧ��\n");
        }
    }

    Nodeu* newUser = CreateUserNodeu(username, password, email, role);
    if (newUser) {
        AddUserToList(&headu, newUser);
        SaveToFile(); // ��ע�����û����������浽�ļ�
        printf("�û��ɹ�ע�ᡣ\n");
    }
    else {
        printf("ע��ʧ�ܡ�\n");
    }
    LogOperation(username, "�û�ע�����");
}
Nodeu* CreateUserNodeu(const char* username, const char* password, const char* email, char role) {
    Nodeu* newNodeu = (Nodeu*)malloc(sizeof(Nodeu));
    if (newNodeu) {
        strncpy(newNodeu->user.username, username, sizeof(newNodeu->user.username) - 1);
        strncpy(newNodeu->user.password, password, sizeof(newNodeu->user.password) - 1);
        strncpy(newNodeu->user.email, email, sizeof(newNodeu->user.email) - 1);
        newNodeu->user.role = role; // Set the user role
        newNodeu->next = NULL;
    }
    return newNodeu;
}

void AddUserToList(Nodeu** headu, Nodeu* newUser) {
    if (*headu == NULL) {
        *headu = newUser;
    }
    else {
        Nodeu* current = *headu;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newUser;
    }
}

void SaveToFile() {
    FILE* file = fopen(filenameu, "w");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }
    Nodeu* current = headu;
    while (current != NULL) {
        fprintf(file, "%s,%s,%s,%c\n", current->user.username, current->user.password, current->user.email, current->user.role);
        current = current->next;
    }
    fclose(file);
}

void LoadFromFile() {
    FILE* file = fopen(filenameu, "r");
    if (file == NULL) {
        printf("No existing user data found.\n");
        return;
    }
    char username[50], password[50], email[100];
    char role;
    while (fscanf(file, "%49[^,],%49[^,],%99[^,],%c\n", username, password, email, &role) == 4) {
        Nodeu* newUser = CreateUserNodeu(username, password, email, role);
        AddUserToList(&headu, newUser);
    }
    fclose(file);
}

void ForgotPassword() {
    char email[100], newPassword[50], confirmPassword[50];
    int passwordMatched = 0; // ���ڸ����������������Ƿ�ƥ��

    printf("Enter your email: ");
    scanf("%99s", email); // �������볤���Ա������

    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.email, email) == 0) {
            while (!passwordMatched) { // ѭ��ֱ���û���ȷ����������ͬ������
                printf("�����µ�����: ");
                GetPassword(newPassword, sizeof(newPassword)); // �������볤���Ա������

                printf("ȷ������: ");
                GetPassword(confirmPassword, sizeof(confirmPassword));  // �������볤���Ա������

                if (strcmp(newPassword, confirmPassword) != 0) {
                    printf("���벻���ϣ������ԡ�\n");
                }
                else {
                    passwordMatched = 1; // ����������������һ�£�����Ϊƥ��
                }
            }

            // ��������
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            current->user.password[sizeof(current->user.password) - 1] = '\0'; // ȷ���ַ�����null��ֹ
            printf("Password updated successfully.\n");
            SaveToFile(); // �����û���Ϣ���ļ�
            return;
        }
        current = current->next;
    }
    printf("Email not found.\n");
}

void ChangePassword(const char* username) {
    char newPassword[50];
    char confirmPassword[50];

    // �����û�����������
    printf("�����µ�����: ");
    GetPassword(newPassword, sizeof(newPassword)); // �������볤���Ա������

    printf("ȷ������: ");
    GetPassword(confirmPassword, sizeof(confirmPassword));  // �������볤���Ա������


    // �����������������Ƿ�һ��
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("���벻���ϣ������ԡ�\n");
        return; // ������벻һ�£�ֱ�ӷ��أ��������������
    }

    // Ѱ��ƥ����û���
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            // ��������
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            printf("����ɹ��޸ġ�\n", username);
            SaveToFile(); // �����û���Ϣ���ļ�
            return; // ������³ɹ����˳�����
        }
        current = current->next;
    }

    // ���û���ҵ�ƥ����û���
    printf("�޸�����ʧ�ܣ�û�ҵ�ƥ����û�����\n");
}

void Login() {
    char username[50], password[50];
    printf("�������û�����ѧ��������ѧ�š�����Ա���빤�ţ�: ");
    scanf("%49s", username);
    printf("����������: ");
    GetPassword(password, sizeof(password)); // ֱ��ʹ��scanf��ȡ����
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0 && strcmp(current->user.password, password) == 0) {
            printf("%s ��½�ɹ�����ӭ %s!\n", current->user.role == 'a' ? "Admin" : "Student", username);
            currentUserRole = current->user.role; // ���õ�ǰ�û���ɫ
            Sleep(1500);
            system("cls");
            if (currentUserRole == 's') {
                StudentMenu(username); // ѧ���˵�
            }
            else if (currentUserRole == 'a') {
                AdminMenu(username); // ����Ա�˵�
            }
            LogOperation(username, "�û���¼����");

            return;
        }
        current = current->next;
    }
    printf("��½ʧ�ܣ��û������ڻ����������\n");
}
void StudentMenu(const char* username) {
    int choice;
    do {
        PrintStudentMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
            LoadScoresFromFile();
            QueryScoreByStudentId(username); // ͨ���û�����ѯ�ɼ�
            LogOperation(username, "�ɼ���ѯ");
            break;
        case 2:   
            QueryAchievementsByStudentId(username);
            LogOperation(username, "�ɾͲ�ѯ");
            break;
        case 3:
            Stu_appeal_score(username);//���߳ɼ�
            LogOperation(username, "���߳ɼ�");
            break;
        case 4:
            ChangePassword(username); // �޸�����
            LogOperation(username, " �޸�����");
            break;
        case 5:
            printf("�˳��ɹ���\n");
            break;
        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice != 5);
}

void AdminMenu(const char* username) {

    int choice;
    do {
        // ��ʾ���˵�����ȡ�û�ѡ��
        PrintAdminMenu();
        scanf("%d", &choice);
        getchar();
        // ����Ա�˵���ѡ������...

        switch (choice) {
        case 1:
            AdminStudentinfoMenu(username);

            break;
        case 2:
            AdminCourseManagementMenu(username);
            break;
        case 3:
            AdminScoreManagementMenu(username);

            break;
        case 4:
            AdminAchievementManagementMenu(username);

            break;
        case 5:
            AdminSystemManagementMenu(username);

            break;

        case 6:
          // printf("�˳��ɹ���\n");
            break;
        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice !=6);
}

void AdminStudentinfoMenu(const char* username) {
    int choice;
    do {
        PrintStudentinfoMenu();
        scanf("%d", &choice);
        getchar();
            switch (choice) {
            case 1:
            char college[100], major[100];
            printf("������ѧԺ����");
            scanf(" %[^\n]", college); // ʹ��" %[^\n]"��ʽ��ȡ�����ո���ַ���
            printf("������רҵ����");
            scanf(" %[^\n]", major);
            addToList(college, major);
            LogOperation(username, "���ѧԺ����רҵ����Ϣ����");
            Sleep(1500);
            system("cls");
            break;
                    case 2:
                        AddStudentInfoToList();
                        LogOperation(username, "���ѧ����Ϣ����");
                        break;
                    case 3:{
                        char studentIdToQuery[20];
                        printf("������Ҫ��ѯ��ѧ��ѧ��: ");
                        scanf("%19s", studentIdToQuery);
                        QueryStudentInfoById(studentIdToQuery);
                        LogOperation(username, "��ѯѧ����Ϣ����");
                        break;
                    }

                    case 4:
                        QueryAllStudentInfo();
                        LogOperation(username, "��ѯ����ѧ����Ϣ����");
                        break;
                    case 5:
                    {
                        char studentIdToUpdate[20];
                        printf("������Ҫ�޸ĵ�ѧ��ѧ��: ");
                        scanf("%19s", studentIdToUpdate);
                        UpdateStudentInfoById(studentIdToUpdate);
                        LogOperation(username, "�޸�ѧ����Ϣ����");
                        break;
            
                    }   
                    case 6:
                          UpdateTransferInfo();//�޸� score.txt�� ��ѧ����ѧԺ�� רҵ��
                          LogOperation(username, "���³ɼ���ѧ����Ϣ����");
                     break;
                    case 7: {
                        char studentIdToDelete[20];
                        printf("������Ҫɾ����ѧ��ѧ��: ");
                        scanf("%19s", studentIdToDelete);
                        DeleteStudentInfoById(studentIdToDelete);
                        LogOperation(username, "ɾ��ѧ����Ϣ");
                        break;
                    }
                    case 8:
                    {
                        
                            char studentId[20];
                            printf("������Ҫɾ���ɼ���¼��ѧ��ѧ��: ");
                            scanf("%19s", studentId);
                            DeleteAllScoresForStudent(studentId);
                            LogOperation(username, "ɾ���ɼ���ѧ����Ϣ");
                            break;
                     
                    }
                    case 9:
                        printf("�˳��ɹ���\n");
                        break;

        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice !=9);
}
void AdminScoreManagementMenu(const char* username) {
    int choice;
    do {
        PrintScoreManagementMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
             case 1:
                 AddNewScoreRecord();
                 LogOperation(username, "��ӳɼ�����");
                 break;
             case 2:
                 ModifyScoreRecord();
                 LogOperation(username, "�޸ĳɼ�����");
                 break;
             case 3:
                 QueryAllScores();
                 LogOperation(username, "����ѧ���ɼ���ѯ");
                 break;
             case 4:
                 QueryStudentInfo();
                 LogOperation(username, "ѧ���ɼ���ѯ");
                 break;
             case 5:
                 char studentId[20], courseName[100];
                 printf("������Ҫɾ���ɼ���¼��ѧ��ѧ��: ");
                 scanf("%19s", studentId);
                 printf("������Ҫɾ���ɼ���¼�Ŀγ�����: ");
                 scanf(" %[^\n]", courseName);
                 DeleteScoreRecord(studentId, courseName);
                 LogOperation(username, "ѧ���ɼ���ѯ");
                 break;
             case 6:

             // Ϊÿ��ѧԺ��רҵ������ɲ�����GPA�ļ�
                 //RankAllGPAFiles();
                    sortGPAbycollegeAndmajor();
                    LogOperation(username, "��������ѧԺ����רҵ����");
                    break;
             case 7:
                 SaveScoresByCollegeToFile();
                    LogOperation(username, "����ƽ��GPA�ļ�");
                    break;

             case 8:
                 SaveScoresByCollege();
                 LogOperation(username, "���ɰ�ѧԺ��רҵ����ĳɼ��ļ�");
                 
                 break;
             case 9:
                 QueryMajorRankingProcess();
                 LogOperation(username, "��ѧԺ��רҵ����������ļ�");
                 break;
             case 10:
                 QueryScoresByCollege();
                 LogOperation(username, "�鿴��ѧԺ�ɼ��ļ�");
                 break;
             case 11:
                 {
                  char courseName[100];
                   printf("������Ҫ�����Ŀγ�����: ");
                   scanf("%99s", courseName); // ��ȡ�γ�����
                   AnalyzeCourseStatistics(courseName);
                   LogOperation(username, "�����γ̳ɼ�ͳ����Ϣ");// �����γ̳ɼ�ͳ����Ϣ
                      break;
                    }
           case 12:
                 {char studentId[100];
                  printf("������Ҫ������ѧ��ѧ��: ");
                  scanf("%99s", studentId);
                   PredictPostgraduateOpportunity(studentId);
                   LogOperation(username, "�ɼ�Ԥ��"); 
                   break;
                  }
           case 13:
               QueryappealsInfo();
               LogOperation(username, "�ɼ�����");
               break;
             case 14:
                 break;

        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice != 14);
}

void AdminAchievementManagementMenu(const char* username) {
    int choice;
    do {
        PrintAchievementManagementMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
                case 1:
                        AddNewAchievementRecord();
                        LogOperation(username, "��ӳɾͲ���");
                        break;
                case 2:
                    QueryAllAchievements();
                    LogOperation(username, "��ѯ���гɾͲ���");
                    break;
                case 3:
                        char a[100];
                        printf("������Ҫ��ѯ��ѧ��ѧ��: ");
                        scanf("%99s", a);
                        QueryAchievementsByStudentId(a);
                        LogOperation(username, "��ѯ�ɾͲ���");
                        break;
                case 4:
                  ModifyAchievementRecord();
                  LogOperation(username, "�޸ĳɾͲ���");
                    break;
                case 5:
                    DeleteAchievementRecord();
                    LogOperation(username, "ɾ���ɾͲ���");
                    break;
                case 6:
                  {char studentId[100];
                    printf("������ɾ����ѧ��ѧ��: ");
                    scanf("%99s", studentId);
                    DeleteAllAchievementsForStudent(studentId);
                    LogOperation(username, "ɾ���ɾͲ���");
                    break;
                    }
                  
                case 7:
                    break;

        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice != 7);
}
void AdminSystemManagementMenu(const char* username) {
    int choice;
    do {
        PrintSystemManagementMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice) {
        case 1:
         ShowOperationLogs();
         LogOperation(username, "�鿴�����û�����");
         break;
        case 2:UpdateStudentPassword();
            break;
        case 3:
            ChangePassword(username);
            break;
        case 4:
            break;
        default:
            printf("��Ч��ѡ�������ԡ�\n");
        }
    } while (choice != 4);
}
void AdminCourseManagementMenu(const char* username) {
    int choice;
    do {
        PrintCourseinfoMenu();
      
            scanf("%d", &choice);

            switch (choice) {
            case 1:
                AddCourseInfoToList();
                LogOperation(username, "��ӿγ���Ϣ");
                break;
            case 2:
                QueryCourseInfoByCollege();
                LogOperation(username, "��ѧԺ����");
                break;
            case 3:
                UpdateCourseInfo();
                LogOperation(username, "���¿γ���Ϣ");
                break;
            case 4:
                QueryAllCourseInfo();
                LogOperation(username, "���ҿγ���Ϣ");
                break;
            case 5:
                DeleteCourseInfo();
                LogOperation(username, "ɾ���γ���Ϣ");
                break;
            case 6:
                printf("���سɹ���\n");
                break;
            default:
                printf("��Ч��ѡ�����������롣\n");
            }
    } while (choice != 6);
}

void LoadCurrentUserRole() {
        char currentUser[50];
        printf("�����������û���: ");
        scanf("%49s", currentUser);

        FILE* file = fopen("user.txt", "r");
        if (!file) {
            printf("�޷����û��ļ���\n");
            return;
        }

        UserInfo user;
        char currentUserRole = '\0';
        while (fscanf(file, "%49[^,],%49[^,],%99[^,],%c\n",
            user.username, user.password, user.email, &user.role) == 4) {
            if (strcmp(user.username, currentUser) == 0) {
                currentUserRole = user.role;
                break;
            }
        }

        fclose(file);

        if (currentUserRole == '\0') {
            printf("δ�ҵ���ǰ�û���ɫ��\n");
        }
        else if (currentUserRole == 'a') {
            printf("��ǰ�û��ǹ���Ա��\n");
        }
        else if (currentUserRole == 's') {
            printf("��ǰ�û���ѧ����\n");
        }
    }
void JudgeRole() {
    if (currentUserRole != 'a') {
        printf("ֻ�й���Ա��Ȩ���޸ĳɼ���\n");
        return;
    }
}
void UpdateStudentPassword() {
    if (currentUserRole != 'a') {
        printf("ֻ�й���Ա��Ȩ���޸����롣\n");
        return;
    }

    char studentId[50], newPassword[50], confirmPassword[50];
    printf("������Ҫ�޸������ѧ��ѧ��: ");
    scanf("%49s", studentId);

    // Ѱ��ƥ���ѧ���˺�
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, studentId) == 0) {
            // ����Ƿ�Ϊѧ���˺�
            if (current->user.role != 's') {
                printf("ֻ���޸�ѧ���˺ŵ����롣\n");
                return;
            }

            // ��������������
            printf("�����µ�����: ");
            GetPassword(newPassword, sizeof(newPassword));
            printf("ȷ������: ");
            GetPassword(confirmPassword, sizeof(confirmPassword));
            // �����������������Ƿ�һ��
            if (strcmp(newPassword, confirmPassword) != 0) {
                printf("������������벻һ�£������²�����\n");
                return;
            }

            // ��������
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            current->user.password[sizeof(current->user.password) - 1] = '\0'; // ȷ���ַ�����null��ֹ

            SaveToFile(); // �����û���Ϣ���ļ�
            printf("������³ɹ���\n");
            return;
        }
        current = current->next;
    }

    printf("δ�ҵ�ѧ��Ϊ %s ��ѧ����\n", studentId);
}

