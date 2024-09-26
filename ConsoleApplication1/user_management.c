#include "user_management.h"
// 假设用户结构体和链表头定义在此文件或通过utils.h引入
void Register() {
    char username[50], password[50], email[100], role;
    int isValidUsername = 0; // 新增变量以跟踪用户名是否有效
    int isValidRole = 0; // 新增变量以跟踪角色是否有效

    while (!isValidUsername) {
        printf("输入用户名（用户名为你的8位学号或工号）: ");
        scanf("%s", username);
        while (strlen(username) != 8) {
            printf("\n请重新输入用户名（必须为八位数字）:");
            scanf("%s", username);
        }

        isValidUsername = 1; // 假设用户名有效
        for (int i = 0; username[i] != '\0'; i++) {
            if (!isdigit(username[i])) { // 检查每个字符是否为数字
                printf("\n无效的用户名，只能输入数字\n");
                isValidUsername = 0; // 如果发现非数字字符，标记用户名为无效
                break; // 退出循环
            }
        }

        if (isValidUsername) {
            // 如果用户名有效，检查是否已存在
            Nodeu* current = headu;
            while (current != NULL) {
                if (strcmp(current->user.username, username) == 0) {
                    // 如果找到相同的用户名，提示用户并要求重新输入
                    printf("用户名 '%s' 已经存在。 请选择不同的用户名。\n", username);
                    isValidUsername = 0; // 标记用户名为无效以触发重新输入
                    break; // 退出循环
                }
                current = current->next;
            }
        }
    }

    // 继续注册流程
    printf("输入密码: ");
    GetPassword(password, sizeof(password)); // 使用GetPassword代替scanf以隐藏密码输入
    printf("输入邮箱: ");
    scanf("%s", email);

    // 验证角色输入
    while (!isValidRole) {
        printf("输入身份 (a 表示管理员, s 表示学生): ");
        scanf(" %c", &role); // 注意前面的空格，用于消耗前一个输入后的换行符
        if (role == 'a' || role == 's') {
            isValidRole = 1; // 如果角色有效
        }
        else {
            printf("无效的角色输入。请输入 a 表示管理员  或  s 表示学生\n");
        }
    }

    Nodeu* newUser = CreateUserNodeu(username, password, email, role);
    if (newUser) {
        AddUserToList(&headu, newUser);
        SaveToFile(); // 在注册新用户后立即保存到文件
        printf("用户成功注册。\n");
    }
    else {
        printf("注册失败。\n");
    }
    LogOperation(username, "用户注册操作");
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
    int passwordMatched = 0; // 用于跟踪两次密码输入是否匹配

    printf("Enter your email: ");
    scanf("%99s", email); // 限制输入长度以避免溢出

    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.email, email) == 0) {
            while (!passwordMatched) { // 循环直到用户正确输入两次相同的密码
                printf("输入新的密码: ");
                GetPassword(newPassword, sizeof(newPassword)); // 限制输入长度以避免溢出

                printf("确认密码: ");
                GetPassword(confirmPassword, sizeof(confirmPassword));  // 限制输入长度以避免溢出

                if (strcmp(newPassword, confirmPassword) != 0) {
                    printf("密码不符合，请重试。\n");
                }
                else {
                    passwordMatched = 1; // 如果两次输入的密码一致，则标记为匹配
                }
            }

            // 更新密码
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            current->user.password[sizeof(current->user.password) - 1] = '\0'; // 确保字符串以null终止
            printf("Password updated successfully.\n");
            SaveToFile(); // 保存用户信息到文件
            return;
        }
        current = current->next;
    }
    printf("Email not found.\n");
}

void ChangePassword(const char* username) {
    char newPassword[50];
    char confirmPassword[50];

    // 请求用户输入新密码
    printf("输入新的密码: ");
    GetPassword(newPassword, sizeof(newPassword)); // 限制输入长度以避免溢出

    printf("确认密码: ");
    GetPassword(confirmPassword, sizeof(confirmPassword));  // 限制输入长度以避免溢出


    // 检查两次输入的密码是否一致
    if (strcmp(newPassword, confirmPassword) != 0) {
        printf("密码不符合，请重试。\n");
        return; // 如果密码不一致，直接返回，不进行密码更改
    }

    // 寻找匹配的用户名
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0) {
            // 更新密码
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            printf("密码成功修改。\n", username);
            SaveToFile(); // 保存用户信息到文件
            return; // 密码更新成功，退出函数
        }
        current = current->next;
    }

    // 如果没有找到匹配的用户名
    printf("修改密码失败，没找到匹配的用户名。\n");
}

void Login() {
    char username[50], password[50];
    printf("请输入用户名（学生请输入学号、管理员输入工号）: ");
    scanf("%49s", username);
    printf("请输入密码: ");
    GetPassword(password, sizeof(password)); // 直接使用scanf读取密码
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, username) == 0 && strcmp(current->user.password, password) == 0) {
            printf("%s 登陆成功，欢迎 %s!\n", current->user.role == 'a' ? "Admin" : "Student", username);
            currentUserRole = current->user.role; // 设置当前用户角色
            Sleep(1500);
            system("cls");
            if (currentUserRole == 's') {
                StudentMenu(username); // 学生菜单
            }
            else if (currentUserRole == 'a') {
                AdminMenu(username); // 管理员菜单
            }
            LogOperation(username, "用户登录操作");

            return;
        }
        current = current->next;
    }
    printf("登陆失败，用户不存在或者密码错误\n");
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
            QueryScoreByStudentId(username); // 通过用户名查询成绩
            LogOperation(username, "成绩查询");
            break;
        case 2:   
            QueryAchievementsByStudentId(username);
            LogOperation(username, "成就查询");
            break;
        case 3:
            Stu_appeal_score(username);//申诉成绩
            LogOperation(username, "申诉成绩");
            break;
        case 4:
            ChangePassword(username); // 修改密码
            LogOperation(username, " 修改密码");
            break;
        case 5:
            printf("退出成功。\n");
            break;
        default:
            printf("无效的选择，请重试。\n");
        }
    } while (choice != 5);
}

void AdminMenu(const char* username) {

    int choice;
    do {
        // 显示主菜单并获取用户选择
        PrintAdminMenu();
        scanf("%d", &choice);
        getchar();
        // 管理员菜单的选项处理代码...

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
          // printf("退出成功。\n");
            break;
        default:
            printf("无效的选择，请重试。\n");
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
            printf("请输入学院名：");
            scanf(" %[^\n]", college); // 使用" %[^\n]"格式读取包含空格的字符串
            printf("请输入专业名：");
            scanf(" %[^\n]", major);
            addToList(college, major);
            LogOperation(username, "添加学院名、专业名信息操作");
            Sleep(1500);
            system("cls");
            break;
                    case 2:
                        AddStudentInfoToList();
                        LogOperation(username, "添加学生信息操作");
                        break;
                    case 3:{
                        char studentIdToQuery[20];
                        printf("请输入要查询的学生学号: ");
                        scanf("%19s", studentIdToQuery);
                        QueryStudentInfoById(studentIdToQuery);
                        LogOperation(username, "查询学生信息操作");
                        break;
                    }

                    case 4:
                        QueryAllStudentInfo();
                        LogOperation(username, "查询所有学生信息操作");
                        break;
                    case 5:
                    {
                        char studentIdToUpdate[20];
                        printf("请输入要修改的学生学号: ");
                        scanf("%19s", studentIdToUpdate);
                        UpdateStudentInfoById(studentIdToUpdate);
                        LogOperation(username, "修改学生信息操作");
                        break;
            
                    }   
                    case 6:
                          UpdateTransferInfo();//修改 score.txt中 该学生的学院名 专业名
                          LogOperation(username, "更新成绩中学生信息操作");
                     break;
                    case 7: {
                        char studentIdToDelete[20];
                        printf("请输入要删除的学生学号: ");
                        scanf("%19s", studentIdToDelete);
                        DeleteStudentInfoById(studentIdToDelete);
                        LogOperation(username, "删除学生信息");
                        break;
                    }
                    case 8:
                    {
                        
                            char studentId[20];
                            printf("请输入要删除成绩记录的学生学号: ");
                            scanf("%19s", studentId);
                            DeleteAllScoresForStudent(studentId);
                            LogOperation(username, "删除成绩中学生信息");
                            break;
                     
                    }
                    case 9:
                        printf("退出成功。\n");
                        break;

        default:
            printf("无效的选择，请重试。\n");
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
                 LogOperation(username, "添加成绩操作");
                 break;
             case 2:
                 ModifyScoreRecord();
                 LogOperation(username, "修改成绩操作");
                 break;
             case 3:
                 QueryAllScores();
                 LogOperation(username, "所有学生成绩查询");
                 break;
             case 4:
                 QueryStudentInfo();
                 LogOperation(username, "学生成绩查询");
                 break;
             case 5:
                 char studentId[20], courseName[100];
                 printf("请输入要删除成绩记录的学生学号: ");
                 scanf("%19s", studentId);
                 printf("请输入要删除成绩记录的课程名称: ");
                 scanf(" %[^\n]", courseName);
                 DeleteScoreRecord(studentId, courseName);
                 LogOperation(username, "学生成绩查询");
                 break;
             case 6:

             // 为每个学院和专业组合生成并排名GPA文件
                 //RankAllGPAFiles();
                    sortGPAbycollegeAndmajor();
                    LogOperation(username, "排名所有学院所有专业操作");
                    break;
             case 7:
                 SaveScoresByCollegeToFile();
                    LogOperation(username, "生成平均GPA文件");
                    break;

             case 8:
                 SaveScoresByCollege();
                 LogOperation(username, "生成按学院和专业分类的成绩文件");
                 
                 break;
             case 9:
                 QueryMajorRankingProcess();
                 LogOperation(username, "按学院和专业分类的排名文件");
                 break;
             case 10:
                 QueryScoresByCollege();
                 LogOperation(username, "查看按学院成绩文件");
                 break;
             case 11:
                 {
                  char courseName[100];
                   printf("请输入要分析的课程名称: ");
                   scanf("%99s", courseName); // 获取课程名称
                   AnalyzeCourseStatistics(courseName);
                   LogOperation(username, "分析课程成绩统计信息");// 分析课程成绩统计信息
                      break;
                    }
           case 12:
                 {char studentId[100];
                  printf("请输入要分析的学生学号: ");
                  scanf("%99s", studentId);
                   PredictPostgraduateOpportunity(studentId);
                   LogOperation(username, "成绩预测"); 
                   break;
                  }
           case 13:
               QueryappealsInfo();
               LogOperation(username, "成绩申诉");
               break;
             case 14:
                 break;

        default:
            printf("无效的选择，请重试。\n");
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
                        LogOperation(username, "添加成就操作");
                        break;
                case 2:
                    QueryAllAchievements();
                    LogOperation(username, "查询所有成就操作");
                    break;
                case 3:
                        char a[100];
                        printf("请输入要查询的学生学号: ");
                        scanf("%99s", a);
                        QueryAchievementsByStudentId(a);
                        LogOperation(username, "查询成就操作");
                        break;
                case 4:
                  ModifyAchievementRecord();
                  LogOperation(username, "修改成就操作");
                    break;
                case 5:
                    DeleteAchievementRecord();
                    LogOperation(username, "删除成就操作");
                    break;
                case 6:
                  {char studentId[100];
                    printf("请输入删除的学生学号: ");
                    scanf("%99s", studentId);
                    DeleteAllAchievementsForStudent(studentId);
                    LogOperation(username, "删除成就操作");
                    break;
                    }
                  
                case 7:
                    break;

        default:
            printf("无效的选择，请重试。\n");
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
         LogOperation(username, "查看所有用户操作");
         break;
        case 2:UpdateStudentPassword();
            break;
        case 3:
            ChangePassword(username);
            break;
        case 4:
            break;
        default:
            printf("无效的选择，请重试。\n");
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
                LogOperation(username, "添加课程信息");
                break;
            case 2:
                QueryCourseInfoByCollege();
                LogOperation(username, "按学院查找");
                break;
            case 3:
                UpdateCourseInfo();
                LogOperation(username, "更新课程信息");
                break;
            case 4:
                QueryAllCourseInfo();
                LogOperation(username, "查找课程信息");
                break;
            case 5:
                DeleteCourseInfo();
                LogOperation(username, "删除课程信息");
                break;
            case 6:
                printf("返回成功！\n");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
            }
    } while (choice != 6);
}

void LoadCurrentUserRole() {
        char currentUser[50];
        printf("请输入您的用户名: ");
        scanf("%49s", currentUser);

        FILE* file = fopen("user.txt", "r");
        if (!file) {
            printf("无法打开用户文件。\n");
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
            printf("未找到当前用户角色。\n");
        }
        else if (currentUserRole == 'a') {
            printf("当前用户是管理员。\n");
        }
        else if (currentUserRole == 's') {
            printf("当前用户是学生。\n");
        }
    }
void JudgeRole() {
    if (currentUserRole != 'a') {
        printf("只有管理员有权限修改成绩。\n");
        return;
    }
}
void UpdateStudentPassword() {
    if (currentUserRole != 'a') {
        printf("只有管理员有权限修改密码。\n");
        return;
    }

    char studentId[50], newPassword[50], confirmPassword[50];
    printf("请输入要修改密码的学生学号: ");
    scanf("%49s", studentId);

    // 寻找匹配的学生账号
    Nodeu* current = headu;
    while (current != NULL) {
        if (strcmp(current->user.username, studentId) == 0) {
            // 检查是否为学生账号
            if (current->user.role != 's') {
                printf("只能修改学生账号的密码。\n");
                return;
            }

            // 请求输入新密码
            printf("输入新的密码: ");
            GetPassword(newPassword, sizeof(newPassword));
            printf("确认密码: ");
            GetPassword(confirmPassword, sizeof(confirmPassword));
            // 检查两次输入的密码是否一致
            if (strcmp(newPassword, confirmPassword) != 0) {
                printf("两次输入的密码不一致，请重新操作。\n");
                return;
            }

            // 更新密码
            strncpy(current->user.password, newPassword, sizeof(current->user.password) - 1);
            current->user.password[sizeof(current->user.password) - 1] = '\0'; // 确保字符串以null终止

            SaveToFile(); // 保存用户信息到文件
            printf("密码更新成功。\n");
            return;
        }
        current = current->next;
    }

    printf("未找到学号为 %s 的学生。\n", studentId);
}

