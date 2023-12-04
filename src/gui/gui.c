#include "gui.h"
#include "../authorization/authorization.h"
#include "../team/team.h"
#include "../board/board.h"
#include "../schedule/schedule.h"



// 로그인 성공 후 UI 변경 함수
void updateUIAfterLogin(GtkWidget *window)
{
    GtkWidget *box;
    GtkWidget *create_team_button;
    GtkWidget *exit_program_button;

    // 기존의 모든 위젯을 제거합니다.
    GtkWidget *existingBox = gtk_bin_get_child(GTK_BIN(window));
    if (existingBox != NULL)
        gtk_widget_destroy(existingBox);

    // 새로운 박스를 생성하고 추가합니다.
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Use VERTICAL orientation for buttons below
    gtk_container_add(GTK_CONTAINER(window), box);

    create_team_button = gtk_button_new_with_label("팀 생성");
    g_signal_connect(create_team_button, "clicked", G_CALLBACK(create_team_button_clicked), window);
    gtk_box_pack_start(GTK_BOX(box), create_team_button, TRUE, TRUE, 0);

    int count = count_teams();

    int nums[count];

    GtkWidget **teams_button = malloc(count * sizeof(GtkWidget *));

    FILE *file = fopen("./data/teams.txt", "r");
    for (int i = 0; i < count; i++)
    {
        fscanf(file, "%[^:]: %*s %*s %*s %*s\n", projects[i]);
    }
    fclose(file);

    char member_data[4][10];
    for (int i = 0; i < count; i++)
    {
        nums[i] = 0;
        Invalid_teams(projects[i], member_data);
        for (int j = 0; j < 4; j++)
        {
            if (strcmp(login_name, member_data[j]) == 0)
            {
                nums[i] = 1;
                break;
            }
        }
    }

    FILE *fp = fopen("./data/teams.txt", "r");
    for (int i = 0; i < count; i++)
    {
        if (nums[i] == 1)
        {
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

void evaluate_GUI(char *member, gchar *coment)
{
    gtk_init(NULL, NULL);

    GtkWidget *window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window2), "GTK Example");
    g_signal_connect(G_OBJECT(window2), "destroy", G_CALLBACK(on_window_destroy), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_container_add(GTK_CONTAINER(window2), box);

    // label
    GtkWidget *row1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), row1, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new(member);
    gtk_box_pack_start(GTK_BOX(row1), label, FALSE, FALSE, 0);

    // entry
    GtkWidget *row2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(box), row2, FALSE, FALSE, 0);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(row2), entry, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(entry), "changed", G_CALLBACK(coment_entry_changed), coment);

    // button
    GtkWidget *row3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(box), row3, FALSE, FALSE, 0);

    GtkWidget *button = gtk_button_new_with_label("End");
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(end_button_clicked), window2);
    gtk_box_pack_start(GTK_BOX(row3), button, TRUE, TRUE, 0);

    gtk_widget_show_all(window2);

    gtk_main();
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

// GTK 초기화 함수
void login_initGTK(int argc, char *argv[])
{
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




void viewSchedules(GtkWidget *widget, gpointer data)
{
    read_schedules();
    if (scheduleCount > 0)
    {
        // 정렬
        qsort(schedules, scheduleCount, sizeof(struct Schedule), compareSchedules);

        char buffer[500];
        sprintf(buffer, "\n일정 목록:\n");
        for (int i = 1; i < scheduleCount; i++)
        {
            sprintf(buffer + strlen(buffer), "%d. 날짜: %s, 이벤트: %s, 시간: %s\n", i, schedules[i].date, schedules[i].event, schedules[i].time);
        }
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), buffer, -1);
    }
    else
    {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정이 없습니다.", -1);
    }
}

