#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <locale.h>
#include <ctype.h>


typedef struct {
    char name[30];
    char username[30];
    char password[30];
    char teamName[30];
} user_t;


void registerUser();
int loginUser();
int isUserExists(const char* username);
void logout();