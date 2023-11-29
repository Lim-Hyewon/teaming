#include <gtk/gtk.h>
#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    gtk_init(&argc, &argv);

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

    return 0;
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