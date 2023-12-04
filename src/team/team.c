#include "team.h"
#include "../authorization/authorization.h"
#include "../gui/gui.h"

extern user_t currentUser;

// 팀 생성 함수
void createTeam()
{
    char teamName[30], memberId[30], continueAdding;
    int teamCreated = 0;

    printf("팀 이름을 입력하세요: ");
    scanf("%s", teamName);
    printf("'%s' 팀이 생성되었습니다.\n", teamName);
    teamCreated = 1;

    FILE *file = fopen("./data/teams.txt", "a"); // 팀 정보를 저장할 파일 열기
    if (file == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    fprintf(file, "%s: %s ", teamName, currentUser.name); // 팀명 및 로그인한 사용자 이름 저장

    while (teamCreated)
    {
        printf("추가할 사람의 아이디를 입력하세요: ");
        scanf("%s", memberId);

        if (isUserExists(memberId))
        {
            printf("팀에 %s님이 추가되었습니다.\n", memberId);
            fprintf(file, "%s ", memberId); // 팀원 아이디 저장
        }
        else
        {
            printf("다시 입력해주세요.\n");
        }

        printf("멤버 추가를 계속하시겠습니까? (y/n): ");
        scanf(" %c", &continueAdding); // 공백을 추가하여 이전 입력의 개행 문자를 무시합니다.

        if (continueAdding == 'n' || continueAdding == 'N')
        {
            fprintf(file, "\n"); // 한 팀의 정보를 줄바꿈으로 구분
            break;
        }
    }

    fclose(file); // 파일 닫기
}

// 팀원 추가 함수
void addTeamMember()
{
    char memberName[30];
    printf("추가할 팀원의 이름을 입력하세요: ");
    scanf("%s", memberName);

    FILE *file = fopen(currentUser.teamName, "a");
    if (file == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    fprintf(file, "%s\n", memberName); // 팀원 이름 저장
    fclose(file);

    printf("%s님이 팀에 추가되었습니다.\n", memberName);
}

// 팀 보기 함수
void viewTeam()
{
    char line[60];
    FILE *file = fopen(currentUser.teamName, "r");
    if (file == NULL)
    {
        printf("팀 정보를 불러올 수 없습니다.\n");
        return;
    }
    printf("팀원 목록:\n");
    while (fgets(line, sizeof(line), file) != NULL)
    {
        printf("%s", line);
    }
    fclose(file);
}

int count_teams()
{
    FILE *fp = fopen("./data/teams.txt", "r");

    int count = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            count++;
        }
    }
    return count;
}

void Invalid_teams(char project_name[30], char m[4][10])
{
    FILE *fp = fopen("./data/teams.txt", "r");
    int member_num = 4;
    char name[30];
    if (fp == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    while (fscanf(fp, "%[^:]: %s %s %s %s", name, m[0], m[1], m[2], m[3]) == 5)
    {
        if (strcmp(name, project_name) == 0)
        {
            return;
        }
    }
}



void member_coment(char *(*member), char *file_name)
{
    FILE *fp = fopen(file_name, "a, ccs = UTF-8"); // txt 파일 열기 (내용 추가)

    gchar *coment;
    int MAX_COMENT_LEN = 3001; // 최대 입력 가능 크기. 한글기준 1000자. (한글 1글자 = 3, '\0' = 1)
    fprintf(fp, "<동료 코멘트>");

    for (int i = 0; i < 3; i++)
    {
        coment = malloc(MAX_COMENT_LEN);
        evaluate_GUI(member[i], coment);
        fprintf(fp, "\n %s님에 관한 코멘트 : %s \n", member[i], coment);
        free(coment);
    }

    fclose(fp);
}

void call_members(char project_name[30], char m[4][10])
{
    FILE *fp = fopen("./data/teams.txt", "r");
    int member_num = 4;
    char name[30];
    if (fp == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    while (fscanf(fp, "%[^:]: %s %s %s %s", name, m[0], m[1], m[2], m[3]) == 5)
    {
        if (strcmp(name, project_name) == 0)
        {
            return;
        }
    }
    printf("해당하는 팀 이름이 없습니다.");
}
