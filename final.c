#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <locale.h>
#include <ctype.h>

typedef struct {
    char name[30];
    char username[30];
    char password[30];
    char teamName[30];
} User;
User currentUser;
int loggedIn = 0;
char login_name[30];
char projects[10][30];

void login_initGTK(int argc, char *argv[]);
void registerUser();
int loginUser();
int isUserExists(const char* username);
void createTeam();
void logout();
void exitProgram();
void updateUIAfterLogin(GtkWidget *window);

// 회원가입, 로그인, 팀 생성 및 프로그램 종료 버튼에 대한 선언
static void register_button_clicked(GtkWidget *widget, gpointer data);
static void login_button_clicked(GtkWidget *widget, gpointer data);
static void create_team_button_clicked(GtkWidget *widget, gpointer data);
static void exit_program_button_clicked(GtkWidget *widget, gpointer data);


#define MAX_SCHEDULES 10
//이 구조체에 모든 스케줄 저장.
struct Schedule {
    char date[20];
    char event[50];
    char time[10];
};

struct Schedule schedules[MAX_SCHEDULES];
int scheduleCount = 0;

GtkWidget *entryDate;
GtkWidget *entryEvent;
GtkWidget *entryTime;
GtkWidget *textView;

int isValidDate(const char *date);
int compareSchedules(const void *a, const void *b);
void read_schedules();
static void viewSchedules(GtkWidget *widget, gpointer data);
void schedule_GUI();

void on_window_destroy(GtkWidget *widget, gpointer data)
{
   gtk_main_quit();
}

void end_button_clicked(GtkWidget *widget, gpointer data)
{
   gtk_main_quit();
}

void score_entry_changed(GtkEditable *editable, gpointer user_data)
{
   // Get the text from the entry
   const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(editable));

   // Convert the text to an integer
   int *target_variable = (int *)user_data;
   if (sscanf(entry_text, "%d", target_variable) != 1)
   {
      // If conversion fails, set the variable to a default value (e.g., 0)
      *target_variable = 0;
   }
}

void coment_entry_changed(GtkEditable *editable, gpointer user_data)
{
   const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(editable));
   
   strcpy(user_data, entry_text);
}

void score_GUI(int *scores, char *(*member))
{
   gtk_init(NULL, NULL);
   // Create the main window
   GtkWidget *window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window1), "동료평가");
   g_signal_connect(G_OBJECT(window1), "destroy", G_CALLBACK(on_window_destroy), NULL);

   GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
   gtk_container_add(GTK_CONTAINER(window1), box);

   // Create and add the first row (Label 1 and Entry 1)
   GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(box), row1, FALSE, FALSE, 0);

   GtkWidget *label1 = gtk_label_new(member[0]);
   GtkWidget *entry1 = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(row1), label1, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(row1), entry1, FALSE, FALSE, 0);
   
   g_signal_connect(G_OBJECT(entry1), "changed", G_CALLBACK(score_entry_changed), &scores[0]);

   // Create and add the second row (Label 2 and Entry 2)
   GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(box), row2, FALSE, FALSE, 0);

   GtkWidget *label2 = gtk_label_new(member[1]);
   GtkWidget *entry2 = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(row2), label2, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(row2), entry2, FALSE, FALSE, 0);
   
   g_signal_connect(G_OBJECT(entry2), "changed", G_CALLBACK(score_entry_changed), &scores[1]);

   // Create and add the third row (Label 3 and Entry 3)
   GtkWidget *row3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(box), row3, FALSE, FALSE, 0);

   GtkWidget *label3 = gtk_label_new(member[2]);
   GtkWidget *entry3 = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(row3), label3, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(row3), entry3, FALSE, FALSE, 0);
   
   g_signal_connect(G_OBJECT(entry3), "changed", G_CALLBACK(score_entry_changed), &scores[2]);

   // Create end_button
   GtkWidget *row4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
   gtk_box_pack_start(GTK_BOX(box), row4, FALSE, FALSE, 0);
   
   GtkWidget *button = gtk_button_new_with_label("End");
   g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(end_button_clicked), window1);
   gtk_box_pack_start(GTK_BOX(row4), button, TRUE, TRUE, 0);

   
   // Show all widgets
   gtk_widget_show_all(window1);

   // Run the GTK main loop
   gtk_main();
}

void member_score(char *(*member), char *file_name)
{
   // 점수 받기
   int scores[3] = {0};
   score_GUI(scores, member);
   
   FILE* fp = fopen(file_name, "w"); // txt 파일 열기 (생성 & 초기화)
   
   fprintf(fp, "<동료 점수>\n");
   
   for (int i=0; i<3; i++) {
      fprintf(fp, "%s : %d\n", member[i], scores[i]); // 파일에 저장 
   }
   fclose(fp);
}

void evaluate_GUI(char *member, gchar *coment)
{
   gtk_init(NULL, NULL);
   
   GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window2), "GTK Example");
   g_signal_connect(G_OBJECT(window2), "destroy", G_CALLBACK(on_window_destroy), NULL);
   
   GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
   gtk_container_add(GTK_CONTAINER(window2), box);
   
   //label
   GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(box), row1, FALSE, FALSE, 0);

   GtkWidget *label = gtk_label_new(member);
   gtk_box_pack_start(GTK_BOX(row1), label, FALSE, FALSE, 0);
   
   //entry
   GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
   gtk_box_pack_start(GTK_BOX(box), row2, FALSE, FALSE, 0);
   
   GtkWidget *entry = gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(row2), entry, FALSE, FALSE, 0);
   
   g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(coment_entry_changed), coment);
    
   //button
   GtkWidget *row3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
   gtk_box_pack_start(GTK_BOX(box), row3, FALSE, FALSE, 0);
   
   GtkWidget *button = gtk_button_new_with_label("End");
   g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(end_button_clicked), window2);
   gtk_box_pack_start(GTK_BOX(row3), button, TRUE, TRUE, 0);
   
   gtk_widget_show_all(window2);

   gtk_main();
}

void member_coment(char *(*member), char *file_name)
{   
   FILE* fp=fopen(file_name, "a, ccs = UTF-8"); // txt 파일 열기 (내용 추가)
   
   gchar *coment;
   int MAX_COMENT_LEN = 3001; // 최대 입력 가능 크기. 한글기준 1000자. (한글 1글자 = 3, '\0' = 1)
   fprintf(fp, "<동료 코멘트>");
   
   for (int i = 0; i < 3; i++) {
      coment = malloc(MAX_COMENT_LEN);
      evaluate_GUI(member[i], coment);
      fprintf(fp, "\n %s님에 관한 코멘트 : %s \n", member[i], coment);
      free(coment);
   }
   
   fclose(fp);
}

void call_members(char project_name[30], char m[4][10])
{
   FILE *fp = fopen("teams.txt", "r");
   int member_num = 4;
   char name[30];
   if (fp == NULL) {
      printf("파일을 열 수 없습니다.\n");
      return;
   }
   
   while (fscanf(fp, "%[^:]: %s %s %s %s", name, m[0], m[1], m[2], m[3]) == 5) {
      if (strcmp(name, project_name)==0) {
         return;
      }
   }
   printf("해당하는 팀 이름이 없습니다.");
}

void end_project(GtkWidget *widget, gpointer data)
{
   char *project_name = (char*) data;
   char member_data[4][10];
   call_members(project_name, member_data);
   char *file_name = strcat(project_name, ".txt");
   
   char *member[3]; // 반복문을 사용하기 위해 멤버 하나의 변수에 할당 
   
   for (int i=0; i<4; i++) {
      member[i] = malloc(strlen(member_data[i]) + 1);
   }
   
   int j = 0;
   for (int i=0; i<4; i++) {
      if (strcmp(member_data[i], login_name)==0) {
         continue;
      } else {
         strcpy(member[j++], member_data[i]);
      }
   }
   
   member_score(member, file_name); // 멤버 점수 매기기
   member_coment(member, file_name); // 코멘트 남기기 
   
   for (int i=0; i<4; i++) {
      free(member[i]);
   }
}

int isValidDate(const char *date) {
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-') {
        return 0;
    }

    for (int i = 0; i < 10; i++) {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
            return 0;
        }
    }

    return 1;
}

// 비교 함수 정의
int compareSchedules(const void *a, const void *b) {
    return strcmp(((struct Schedule *)a)->date, ((struct Schedule *)b)->date);
}

static void addScheduleCallback(GtkWidget *widget, gpointer data) {
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entryDate));
    const gchar *event = gtk_entry_get_text(GTK_ENTRY(entryEvent));
    const gchar *time = gtk_entry_get_text(GTK_ENTRY(entryTime));
    
    if (!isValidDate(date)) { // 유효한 날짜 데이터인지 확인.
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "유효하지 않은 날짜 형식입니다. 다시 입력해주세요.", -1);
        return;
    }

    if (scheduleCount < MAX_SCHEDULES) { // MAX_SCHEDULES = 10
        strcpy(schedules[scheduleCount].date, date);
        strcpy(schedules[scheduleCount].event, event);
        strcpy(schedules[scheduleCount].time, time);
        FILE *file = fopen("schedule.txt", "a");
        fprintf(file, "Date: %s, Event: %s, Time: %s\n", date, event, time);
   fclose(file);
        scheduleCount++;
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정이 추가되었습니다.", -1);
    } else {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정을 더 이상 추가할 수 없습니다. 최대 개수에 도달했습니다.", -1);
    }
}

void read_schedules()
{
    char date[20];
    char event[50];
    char time[10];
    
    scheduleCount = 1;
    FILE *file = fopen("schedule.txt", "r");
    while (fscanf(file, "Date: %19[^,], Event: %49[^,], Time: %9[^\n]\n", date, event, time)==3) {
        strcpy(schedules[scheduleCount].date, date);
        strcpy(schedules[scheduleCount].event, event);
        strcpy(schedules[scheduleCount].time, time);
       scheduleCount++;
    }
    
    fclose(file);
}

static void viewSchedules(GtkWidget *widget, gpointer data) {
    read_schedules();
    if (scheduleCount > 0) {
        // 정렬
        qsort(schedules, scheduleCount, sizeof(struct Schedule), compareSchedules);

        char buffer[500];
        sprintf(buffer, "\n일정 목록:\n");
        for (int i = 1; i < scheduleCount; i++) {
            sprintf(buffer + strlen(buffer), "%d. 날짜: %s, 이벤트: %s, 시간: %s\n", i, schedules[i].date, schedules[i].event, schedules[i].time);
        }
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), buffer, -1);
    } else {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정이 없습니다.", -1);
    }
}

void schedule_GUI(GtkWidget *widget, gpointer data)
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "일정 조회 프로그램");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *addButton = gtk_button_new_with_label("일정 추가");
    GtkWidget *viewButton = gtk_button_new_with_label("일정 조회");

    entryDate = gtk_entry_new();
    entryEvent = gtk_entry_new();
    entryTime = gtk_entry_new();
    textView = gtk_text_view_new();

    // 예시를 표시하기 위한 라벨 추가
    GtkWidget *exampleDateLabel = gtk_label_new("예시: YYYY-MM-DD");
    GtkWidget *exampleEventLabel = gtk_label_new("예시: 회의");
    GtkWidget *exampleTimeLabel = gtk_label_new("예시: 14:00");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), addButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), viewButton, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), entryDate, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), exampleDateLabel, TRUE, TRUE, 0); // 예시 추가
    gtk_box_pack_start(GTK_BOX(box), entryEvent, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), exampleEventLabel, TRUE, TRUE, 0); // 예시 추가
    gtk_box_pack_start(GTK_BOX(box), entryTime, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), exampleTimeLabel, TRUE, TRUE, 0); // 예시 추가
    gtk_box_pack_start(GTK_BOX(box), textView, TRUE, TRUE, 0);

    g_signal_connect(addButton, "clicked", G_CALLBACK(addScheduleCallback), NULL);
    g_signal_connect(viewButton, "clicked", G_CALLBACK(viewSchedules), NULL);

    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
    gtk_main();
}

void registerUser() {
    User user;
    FILE* file = fopen("users.txt", "a");
    if (file == NULL) {
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

int loginUser() {
    char userID[30], password[30];
    char line[100];
    char fileUsername[30], fileID[30], filePassword[30];

    printf("로그인\n");
    printf("아이디: ");
    scanf("%29s", userID); // 버퍼 오버플로 방지
    getchar(); // 개행 문자 제거

    printf("비밀번호: ");
    scanf("%29s", password); // 버퍼 오버플로 방지
    getchar(); // 개행 문자 제거

    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        perror("파일을 열 수 없습니다.");
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        // 한 줄에서 아이디와 비밀번호를 추출합니다.
        sscanf(line, "%s %s %s", fileUsername, fileID, filePassword);

        // 입력된 아이디와 비밀번호가 파일에 있는 데이터와 일치하는지 확인합니다.
        if (strcmp(userID, fileID) == 0 && strcmp(password, filePassword) == 0) {
            strcpy(login_name, fileUsername);
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

int isUserExists(const char* username) {
    User user;
    FILE* file = fopen("users.txt", "r");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }

    while (fscanf(file, "%s %s", user.username, user.password) != EOF) {
        if (strcmp(username, user.username) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// 팀 생성 함수
void createTeam() {
    char teamName[30], memberId[30], continueAdding;
    int teamCreated = 0;

    printf("팀 이름을 입력하세요: ");
    scanf("%s", teamName);
    printf("'%s' 팀이 생성되었습니다.\n", teamName);
    teamCreated = 1;

    FILE *file = fopen("teams.txt", "a"); // 팀 정보를 저장할 파일 열기
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    fprintf(file, "%s: %s ", teamName, currentUser.name); // 팀명 및 로그인한 사용자 이름 저장

    while (teamCreated) {
        printf("추가할 사람의 아이디를 입력하세요: ");
        scanf("%s", memberId);

        if (isUserExists(memberId)) {
            printf("팀에 %s님이 추가되었습니다.\n", memberId);
            fprintf(file, "%s ", memberId); // 팀원 아이디 저장
        }
        else {
            printf("다시 입력해주세요.\n");
        }

        printf("멤버 추가를 계속하시겠습니까? (y/n): ");
        scanf(" %c", &continueAdding); // 공백을 추가하여 이전 입력의 개행 문자를 무시합니다.

        if (continueAdding == 'n' || continueAdding == 'N') {
            fprintf(file, "\n"); // 한 팀의 정보를 줄바꿈으로 구분
            break;
        }
    }

    fclose(file); // 파일 닫기
}


// 팀원 추가 함수
void addTeamMember() {
    char memberName[30];
    printf("추가할 팀원의 이름을 입력하세요: ");
    scanf("%s", memberName);

    FILE *file = fopen(currentUser.teamName, "a");
    if (file == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }
    fprintf(file, "%s\n", memberName); // 팀원 이름 저장
    fclose(file);

    printf("%s님이 팀에 추가되었습니다.\n", memberName);
}

// 팀 보기 함수
void viewTeam() {
    char line[60];
    FILE *file = fopen(currentUser.teamName, "r");
    if (file == NULL) {
        printf("팀 정보를 불러올 수 없습니다.\n");
        return;
    }
    printf("팀원 목록:\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }
    fclose(file);
}

// GTK 초기화 함수
void login_initGTK(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *register_button;
    GtkWidget *login_button;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "회원가입 및 로그인");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    register_button = gtk_button_new_with_label("회원가입");
    g_signal_connect(register_button, "clicked", G_CALLBACK(register_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), register_button, 0, 0, 1, 1);

    login_button = gtk_button_new_with_label("로그인");
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), login_button, 1, 0, 1, 1);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();
}

static void register_button_clicked(GtkWidget *widget, gpointer data) {
    // 회원가입 버튼을 클릭했을 때 실행되는 함수
    registerUser();
}

static void login_button_clicked(GtkWidget *widget, gpointer data) {
    int loggedIn = loginUser();
    if (!loggedIn) {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "로그인 실패");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_INFO,
                                        GTK_BUTTONS_OK,
                                        "로그인 성공");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        updateUIAfterLogin(data);
    }
}

void new_button1_clicked(GtkWidget *widget, gpointer data) {
    g_print("New Button 1 clicked\n");
}

void new_button2_clicked(GtkWidget *widget, gpointer data) {
    g_print("New Button 2 clicked\n");
}

int count_teams() {
    FILE *fp = fopen("teams.txt", "r");
    
    int count = 0;
    int ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}

void teams_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *in_team_window;
    GtkWidget *box;
    GtkWidget *project_name_label;
    GtkWidget *schedule_button;
    GtkWidget *board_button;
    GtkWidget *peer_evaluation_button;
    
    in_team_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(in_team_window), box);
    
    char *project_name = (char*) data;
    project_name_label = gtk_label_new(project_name);
    gtk_box_pack_start(GTK_BOX(box), project_name_label, TRUE, TRUE, 0);
    
    schedule_button = gtk_button_new_with_label("스케줄");
    g_signal_connect(schedule_button, "clicked", G_CALLBACK(schedule_GUI), NULL);
    gtk_box_pack_start(GTK_BOX(box), schedule_button, TRUE, TRUE, 0);
    
    board_button = gtk_button_new_with_label("게시판");
    g_signal_connect(board_button, "clicked", G_CALLBACK(bulletinboard), NULL); // 처리할 부분
    gtk_box_pack_start(GTK_BOX(box), board_button, TRUE, TRUE, 0);
    
    peer_evaluation_button = gtk_button_new_with_label("동료평가");
    g_signal_connect(peer_evaluation_button, "clicked", G_CALLBACK(end_project), g_strdup(project_name));
    gtk_box_pack_start(GTK_BOX(box), peer_evaluation_button, TRUE, TRUE, 0);
    
    gtk_widget_show_all(in_team_window);
    
    free(project_name);
    
}

void Invalid_teams(char project_name[30], char m[4][10])
{
   FILE *fp = fopen("teams.txt", "r");
   int member_num = 4;
   char name[30];
   if (fp == NULL) {
      printf("파일을 열 수 없습니다.\n");
      return;
   }
   
   while (fscanf(fp, "%[^:]: %s %s %s %s", name, m[0], m[1], m[2], m[3]) == 5) {
      if (strcmp(name, project_name)==0) {
         return;
      }
   }
}

// 로그인 성공 후 UI 변경 함수
void updateUIAfterLogin(GtkWidget *window) {
    GtkWidget *box;
    GtkWidget *create_team_button;
    GtkWidget *exit_program_button;

    // 기존의 모든 위젯을 제거합니다.
    GtkWidget *existingBox = gtk_bin_get_child(GTK_BIN(window));
    if (existingBox != NULL) {
        gtk_widget_destroy(existingBox);
    }

    // 새로운 박스를 생성하고 추가합니다.
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Use VERTICAL orientation for buttons below
    gtk_container_add(GTK_CONTAINER(window), box);

    create_team_button = gtk_button_new_with_label("팀 생성");
    g_signal_connect(create_team_button, "clicked", G_CALLBACK(create_team_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(box), create_team_button, TRUE, TRUE, 0);
    
    int count = count_teams();
    
    int nums[count];
    
    GtkWidget **teams_button = malloc(count * sizeof(GtkWidget*));
    
    FILE *file = fopen("teams.txt", "r");
    for (int i=0; i<count; i++) {
        fscanf(file, "%[^:]: %*s %*s %*s %*s\n", projects[i]);
    }
    fclose(file);
    
    char member_data[4][10];
    for (int i=0; i<count; i++) {
        nums[i] = 0;
        Invalid_teams(projects[i], member_data);
       for (int j=0; j<4; j++) {
           if (strcmp(login_name, member_data[j]) == 0) {
               nums[i] = 1;
               break;
           }
       }
    }
    
    FILE *fp = fopen("teams.txt", "r");
    for (int i=0; i<count; i++) {
       if (nums[i] == 1) {
            teams_button[i] = gtk_button_new_with_label(projects[i]);
            g_signal_connect(teams_button[i], "clicked", G_CALLBACK(teams_button_clicked), g_strdup(projects[i]));
            gtk_box_pack_start(GTK_BOX(box), teams_button[i], TRUE, TRUE, 0);
        }
    }
    fclose(fp);
    
    exit_program_button = gtk_button_new_with_label("프로그램 종료");
    g_signal_connect(exit_program_button, "clicked", G_CALLBACK(exit_program_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(box), exit_program_button, TRUE, TRUE, 0);

    gtk_widget_show_all(window);
    
    free(teams_button);
    
    return;
}

static void create_team_button_clicked(GtkWidget *widget, gpointer data) {
    // 팀 생성 버튼을 클릭했을 때 실행되는 함수
    createTeam();
}

static void exit_program_button_clicked(GtkWidget *widget, gpointer data) {
    //프로그램 종료 함수
    gtk_main_quit();
}

//메인 함수
int main(int argc, char *argv[]) {
    login_initGTK(argc, argv);
    return 0;
}

// 게시판 함수 시작
#define MAX_POSTS 100
#define MAX_TITLE 100
#define MAX_CONTENT 500

typedef struct {
    char title[MAX_TITLE];
    char content[MAX_CONTENT];
    char author[MAX_TITLE];
} Post;

Post posts[MAX_POSTS];
int postCount = 0;

void writePost(GtkWidget *widget, gpointer data);
void readPosts(GtkWidget *widget, gpointer data);
void searchPosts(GtkWidget *widget, gpointer data);
char *strstr_case_insensitive_word(const char *haystack, const char *needle);
void editPost(GtkWidget *widget, gpointer data);
void deletePost(GtkWidget *widget, gpointer data);
void saveEdit(GtkWidget *widget, gpointer data);
void saveData();
void loadData();

GtkWidget *titleEntry, *contentEntry, *authorEntry, *outputLabel, *keywordEntry;

void bulletinboard() {
    setlocale(LC_ALL, "");

    gtk_init(0, "");

    GtkWidget *window, *box, *button, *grid, *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "게시판 프로그램");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(box), grid, TRUE, TRUE, 0);

    label = gtk_label_new("제목:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    titleEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), titleEntry, 1, 0, 1, 1);

    label = gtk_label_new("내용:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    contentEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), contentEntry, 1, 1, 1, 1);

    label = gtk_label_new("작성자:");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    authorEntry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), authorEntry, 1, 2, 1, 1);

    button = gtk_button_new_with_label("글 작성");
    g_signal_connect(button, "clicked", G_CALLBACK(writePost), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 2, 1);

    button = gtk_button_new_with_label("글 읽기");
    g_signal_connect(button, "clicked", G_CALLBACK(readPosts), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 4, 2, 1);

    button = gtk_button_new_with_label("글 검색");
    g_signal_connect(button, "clicked", G_CALLBACK(searchPosts), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 2, 1);

    button = gtk_button_new_with_label("글 수정");
    g_signal_connect(button, "clicked", G_CALLBACK(editPost), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 2, 1);

    button = gtk_button_new_with_label("글 삭제");
    g_signal_connect(button, "clicked", G_CALLBACK(deletePost), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 7, 2, 1);

    outputLabel = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(box), outputLabel, TRUE, TRUE, 0);

    // 파일에서 데이터 로드
    loadData();

    gtk_widget_show_all(window);
    gtk_main();
}

void writePost(GtkWidget *widget, gpointer data) {
    if (postCount < MAX_POSTS) {
        Post newPost;
        const gchar *title = gtk_entry_get_text(GTK_ENTRY(titleEntry));
        const gchar *content = gtk_entry_get_text(GTK_ENTRY(contentEntry));
        const gchar *author = gtk_entry_get_text(GTK_ENTRY(authorEntry));

        g_strlcpy(newPost.title, title, MAX_TITLE);
        g_strlcpy(newPost.content, content, MAX_CONTENT);
        g_strlcpy(newPost.author, author, MAX_TITLE);

        posts[postCount++] = newPost;

        gtk_label_set_text(GTK_LABEL(outputLabel), "글이 성공적으로 작성되었습니다.");

        // 데이터를 파일에 저장
        saveData();
    } else {
        gtk_label_set_text(GTK_LABEL(outputLabel), "더 이상 글을 작성할 수 없습니다. 최대 글 개수를 초과했습니다.");
    }
}

void readPosts(GtkWidget *widget, gpointer data) {
    if (postCount > 0) {
        char buffer[2000];
        snprintf(buffer, sizeof(buffer), "\n=== 글 목록 ===\n");

        for (int i = 0; i < postCount; ++i) {
            char postInfo[800];
            snprintf(postInfo, sizeof(postInfo), "%d. 제목: %s\n   작성자: %s\n   내용: %s\n", i + 1, posts[i].title, posts[i].author, posts[i].content);
            strncat(buffer, postInfo, sizeof(buffer) - strlen(buffer) - 1);
        }

        gtk_label_set_text(GTK_LABEL(outputLabel), buffer);
    } else {
        gtk_label_set_text(GTK_LABEL(outputLabel), "현재 작성된 글이 없습니다.");
    }
}

void searchPosts(GtkWidget *widget, gpointer data) {
    if (postCount > 0) {
        // 검색할 키워드를 입력받기 위한 다이얼로그 생성
        GtkWidget *dialog, *grid, *label, *entry;
        dialog = gtk_dialog_new_with_buttons("검색", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL,
                                             "확인", GTK_RESPONSE_ACCEPT,
                                             "취소", GTK_RESPONSE_CANCEL,
                                             NULL);
        gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_ACCEPT);


        // 그리드를 다이얼로그에 추가
        grid = gtk_grid_new();
        gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
        gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
        gtk_container_set_border_width(GTK_CONTAINER(grid), 10);
        gtk_box_pack_start(GTK_BOX(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), grid, TRUE, TRUE, 0);

        // 레이블을 그리드에 추가
        label = gtk_label_new("검색할 키워드를 입력하세요:");
        gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

        // 엔트리를 그리드에 추가
        entry = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry, 1, 0, 1, 1);

        // 다이얼로그 실행 및 응답 처리
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        if (response == GTK_RESPONSE_ACCEPT) {
            const gchar *keyword = gtk_entry_get_text(GTK_ENTRY(entry));

            // 검색 결과 출력을 위한 버퍼 생성
            char buffer[2000];
            snprintf(buffer, sizeof(buffer), "\n=== 검색 결과 ===\n");

            for (int i = 0; i < postCount; ++i) {
                // strstr_case_insensitive_word 함수를 사용하여 대소문자를 무시하고 단어로 검색
                if (strstr_case_insensitive_word(posts[i].title, keyword) ||
                    strstr_case_insensitive_word(posts[i].content, keyword) ||
                    strstr_case_insensitive_word(posts[i].author, keyword)) {
                    // 검색 결과를 버퍼에 추가
                    char postInfo[800];
                    snprintf(postInfo, sizeof(postInfo), "제목: %s\n   작성자: %s\n   내용: %s\n", posts[i].title, posts[i].author, posts[i].content);
                    strncat(buffer, postInfo, sizeof(buffer) - strlen(buffer) - 1);
                }
            }

            // 기존의 텍스트에 누적하여 출력
            const gchar *existingText = gtk_label_get_text(GTK_LABEL(outputLabel));
            gchar *markupBuffer = g_markup_printf_escaped("%s%s", existingText, buffer);
            gtk_label_set_markup(GTK_LABEL(outputLabel), markupBuffer);
            g_free(markupBuffer);
        }

        gtk_widget_destroy(dialog);  // 다이얼로그 파기
    } else {
        gtk_label_set_text(GTK_LABEL(outputLabel), "현재 작성된 글이 없습니다.");
    }
}

// 대소문자를 무시하고 부분 문자열을 찾는 함수
char* strstr_case_insensitive_word(const char* haystack, const char* needle) {
    while (*haystack) {
        const char* h = haystack;
        const char* n = needle;

        while (*h && *n && (tolower((unsigned char)*h) == tolower((unsigned char)*n))) {
            ++h;
            ++n;
        }

        if (!*n && (*h == '\0' || !isalnum((unsigned char)*h))) {
            return (char*)haystack;  // 부분 문자열 발견
        }

        ++haystack;
    }

    return NULL;  // 부분 문자열을 찾을 수 없음
}

void editPost(GtkWidget *widget, gpointer data) {
    if (postCount > 0) {
        int postIndex;
        const gchar *postIndexText = gtk_entry_get_text(GTK_ENTRY(titleEntry));
        sscanf(postIndexText, "%d", &postIndex);

        if (postIndex >= 1 && postIndex <= postCount) {
            char buffer[800];
            snprintf(buffer, sizeof(buffer), "현재 글 내용:\n제목: %s\n작성자: %s\n내용: %s\n", posts[postIndex - 1].title, posts[postIndex - 1].author, posts[postIndex - 1].content);

            // 작성자 확인
            const gchar *authorInput = gtk_entry_get_text(GTK_ENTRY(authorEntry));

            if (strcmp(posts[postIndex - 1].author, authorInput) == 0) {
                gtk_label_set_text(GTK_LABEL(outputLabel), buffer);

                // 수정 폼 표시
                gtk_entry_set_text(GTK_ENTRY(titleEntry), posts[postIndex - 1].title);
                gtk_entry_set_text(GTK_ENTRY(contentEntry), posts[postIndex - 1].content);

                // 수정 버튼에 새로운 콜백 함수 연결
                g_signal_connect(widget, "clicked", G_CALLBACK(saveEdit), GINT_TO_POINTER(postIndex - 1));
            } else {
                gtk_label_set_text(GTK_LABEL(outputLabel), "작성자가 일치하지 않아 글을 수정할 수 없습니다.");
            }
        } else {
            gtk_label_set_text(GTK_LABEL(outputLabel), "잘못된 글 번호입니다.");
        }
    } else {
        gtk_label_set_text(GTK_LABEL(outputLabel), "현재 작성된 글이 없습니다.");
    }
}

void saveEdit(GtkWidget *widget, gpointer data) {
    int postIndex = GPOINTER_TO_INT(data);

    // 수정된 내용 저장
    const gchar *newTitle = gtk_entry_get_text(GTK_ENTRY(titleEntry));
    const gchar *newContent = gtk_entry_get_text(GTK_ENTRY(contentEntry));

    g_strlcpy(posts[postIndex].title, newTitle, MAX_TITLE);
    g_strlcpy(posts[postIndex].content, newContent, MAX_CONTENT);

    // 수정 완료 메시지 표시
    gtk_label_set_text(GTK_LABEL(outputLabel), "글이 성공적으로 수정되었습니다.");

    // 데이터를 파일에 저장
    saveData();
}

void deletePost(GtkWidget *widget, gpointer data) {
    if (postCount > 0) {
        int postIndex;
        const gchar *postIndexText = gtk_entry_get_text(GTK_ENTRY(titleEntry));
        sscanf(postIndexText, "%d", &postIndex);

        if (postIndex >= 1 && postIndex <= postCount) {
            // 작성자 확인
            const gchar *authorInput = gtk_entry_get_text(GTK_ENTRY(authorEntry));

            if (strcmp(posts[postIndex - 1].author, authorInput) == 0) {
                for (int i = postIndex - 1; i < postCount - 1; ++i) {
                    posts[i] = posts[i + 1];
                }
                postCount--;

                // 삭제 완료 메시지 표시
                gtk_label_set_text(GTK_LABEL(outputLabel), "글이 성공적으로 삭제되었습니다.");

                // 데이터를 파일에 저장
                saveData();
            } else {
                gtk_label_set_text(GTK_LABEL(outputLabel), "작성자가 일치하지 않아 글을 삭제할 수 없습니다.");
            }
        } else {
            gtk_label_set_text(GTK_LABEL(outputLabel), "잘못된 글 번호입니다.");
        }
    } else {
        gtk_label_set_text(GTK_LABEL(outputLabel), "현재 작성된 글이 없습니다.");
    }
}

void saveData() {
    FILE *file = fopen("posts.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < postCount; ++i) {
            fprintf(file, "%s\n%s\n%s\n", posts[i].title, posts[i].content, posts[i].author);
        }
        fclose(file);
    }
}

void loadData() {
    // 기존의 데이터 초기화
    postCount = 0;

    FILE *file = fopen("posts.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%99[^\n]\n%499[^\n]\n%99[^\n]\n", posts[postCount].title, posts[postCount].content, posts[postCount].author) == 3) {
            postCount++;
            if (postCount >= MAX_POSTS) {
                break;
            }
        }
        fclose(file);
    }
}