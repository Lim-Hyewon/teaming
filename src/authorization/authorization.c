#include "authorization.h"

extern char login_name[30];

void registerUser()
{
    user_t user;
    FILE *file = fopen("./data/users.txt", "a");
    if (file == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        exit(1);
    }

    printf("회원가입\n");
    printf("이름: ");
    scanf("%s", user.name);
    printf("아이디: ");
    scanf("%s", user.username);
    printf("비밀번호: ");
    scanf("%s", user.password);

    fprintf(file, "%s %s %s\n", user.name, user.username, user.password);
    fclose(file);
}

int loginUser()
{
    char userID[30], password[30];
    char line[100];
    char fileUsername[30], fileID[30], filePassword[30];

    printf("로그인\n");
    printf("아이디: ");
    scanf("%29s", userID); // 버퍼 오버플로 방지
    getchar();             // 개행 문자 제거

    printf("비밀번호: ");
    scanf("%29s", password); // 버퍼 오버플로 방지
    getchar();               // 개행 문자 제거

    FILE *file = fopen("./data/users.txt", "r");
    if (file == NULL)
    {
        perror("파일을 열 수 없습니다.");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // 한 줄에서 아이디와 비밀번호를 추출합니다.
        sscanf(line, "%s %s %s", fileUsername, fileID, filePassword);

        // 입력된 아이디와 비밀번호가 파일에 있는 데이터와 일치하는지 확인합니다.
        if (strcmp(userID, fileID) == 0 && strcmp(password, filePassword) == 0)
        {
            strcpy(login_name, fileUsername);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int isUserExists(const char *username)
{
    user_t user;
    FILE *file = fopen("./data/users.txt", "r");
    if (file == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }

    while (fscanf(file, "%s %s", user.username, user.password) != EOF)
    {
        if (strcmp(username, user.username) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}
