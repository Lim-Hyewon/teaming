#include "schedule.h"

int isValidDate(const char *date)
{
    if (strlen(date) != 10 || date[4] != '-' || date[7] != '-')
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9'))
        {
            return 0;
        }
    }

    return 1;
}

// 비교 함수 정의
int compareSchedules(const void *a, const void *b)
{
    return strcmp(((struct Schedule *)a)->date, ((struct Schedule *)b)->date);
}

void addScheduleCallback(GtkWidget *widget, gpointer data)
{
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entryDate));
    const gchar *event = gtk_entry_get_text(GTK_ENTRY(entryEvent));
    const gchar *time = gtk_entry_get_text(GTK_ENTRY(entryTime));

    if (!isValidDate(date))
    { // 유효한 날짜 데이터인지 확인.
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "유효하지 않은 날짜 형식입니다. 다시 입력해주세요.", -1);
        return;
    }

    if (scheduleCount < MAX_SCHEDULES)
    { // MAX_SCHEDULES = 10
        strcpy(schedules[scheduleCount].date, date);
        strcpy(schedules[scheduleCount].event, event);
        strcpy(schedules[scheduleCount].time, time);
        FILE *file = fopen("./data/schedule.txt", "a");
        fprintf(file, "Date: %s, Event: %s, Time: %s\n", date, event, time);
        fclose(file);
        scheduleCount++;
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정이 추가되었습니다.", -1);
    }
    else
    {
        gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView)), "일정을 더 이상 추가할 수 없습니다. 최대 개수에 도달했습니다.", -1);
    }
}

void read_schedules()
{
    char date[20];
    char event[50];
    char time[10];

    scheduleCount = 1;
    FILE *file = fopen("./data/schedule.txt", "r");
    while (fscanf(file, "Date: %19[^,], Event: %49[^,], Time: %9[^\n]\n", date, event, time) == 3)
    {
        strcpy(schedules[scheduleCount].date, date);
        strcpy(schedules[scheduleCount].event, event);
        strcpy(schedules[scheduleCount].time, time);
        scheduleCount++;
    }

    fclose(file);
}
