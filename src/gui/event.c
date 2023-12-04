#include "gui.h"
#include "../authorization/authorization.h"
#include "../team/team.h"
#include "../board/board.h"
#include "../schedule/schedule.h"



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



// 회원가입 버튼을 클릭했을 때 실행되는 함수
void register_button_clicked(GtkWidget *widget, gpointer data)
{
    registerUser();
}

void login_button_clicked(GtkWidget *widget, gpointer data)
{
    int loggedIn = loginUser();
    if (!loggedIn)
    {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                        GTK_DIALOG_DESTROY_WITH_PARENT,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "로그인 실패");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
    else
    {
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

void new_button1_clicked(GtkWidget *widget, gpointer data)
{
    g_print("New Button 1 clicked\n");
}

void new_button2_clicked(GtkWidget *widget, gpointer data)
{
    g_print("New Button 2 clicked\n");
}

void teams_button_clicked(GtkWidget *widget, gpointer data)
{
    GtkWidget *in_team_window;
    GtkWidget *box;
    GtkWidget *project_name_label;
    GtkWidget *schedule_button;
    GtkWidget *board_button;
    GtkWidget *peer_evaluation_button;

    in_team_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(in_team_window), box);

    char *project_name = (char *)data;
    project_name_label = gtk_label_new(project_name);
    gtk_box_pack_start(GTK_BOX(box), project_name_label, TRUE, TRUE, 0);

    schedule_button = gtk_button_new_with_label("스케줄");
    g_signal_connect(schedule_button, "clicked", G_CALLBACK(schedule_GUI), NULL);
    gtk_box_pack_start(GTK_BOX(box), schedule_button, TRUE, TRUE, 0);

    board_button = gtk_button_new_with_label("게시판");
    g_signal_connect(board_button, "clicked", G_CALLBACK(bulletinBoard), NULL); // 처리할 부분
    gtk_box_pack_start(GTK_BOX(box), board_button, TRUE, TRUE, 0);

    peer_evaluation_button = gtk_button_new_with_label("동료평가");
    g_signal_connect(peer_evaluation_button, "clicked", G_CALLBACK(end_project), g_strdup(project_name));
    gtk_box_pack_start(GTK_BOX(box), peer_evaluation_button, TRUE, TRUE, 0);

    gtk_widget_show_all(in_team_window);

    free(project_name);
}


// 팀 생성 버튼을 클릭했을 때 실행되는 함수
void create_team_button_clicked(GtkWidget *widget, gpointer data)
{
    createTeam();
}


// 프로그램 종료 함수
void exit_program_button_clicked(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

void end_project(GtkWidget *widget, gpointer data)
{
    char *project_name = (char *)data;
    char member_data[4][10];
    call_members(project_name, member_data);
    char *file_name = strcat(project_name, ".txt");

    char *member[3]; // 반복문을 사용하기 위해 멤버 하나의 변수에 할당

    for (int i = 0; i < 4; i++)
    {
        member[i] = malloc(strlen(member_data[i]) + 1);
    }

    int j = 0;
    for (int i = 0; i < 4; i++)
    {
        if (strcmp(member_data[i], login_name) == 0)
        {
            continue;
        }
        else
        {
            strcpy(member[j++], member_data[i]);
        }
    }

    char *directory = malloc(10);
    strcpy(directory, "./data/");
    char *dir_path = strcat(directory, file_name);
    member_score(member, dir_path);  // 멤버 점수 매기기
    member_coment(member, dir_path); // 코멘트 남기기
    free(directory);
    for (int i = 0; i < 4; i++)
    {
        free(member[i]);
    }
}



void member_score(char *(*member), char *file_name)
{
    // 점수 받기
    int scores[3] = {0};
    score_GUI(scores, member);

    FILE *fp = fopen(file_name, "w"); // txt 파일 열기 (생성 & 초기화)

    fprintf(fp, "<동료 점수>\n");

    for (int i = 0; i < 3; i++)
    {
        fprintf(fp, "%s : %d\n", member[i], scores[i]); // 파일에 저장
    }
    fclose(fp);
}
