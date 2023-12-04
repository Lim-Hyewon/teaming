#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <locale.h>
#include <ctype.h>

void createTeam();
void addTeamMember();
void viewTeam();
int count_teams();
void Invalid_teams(char project_name[30], char m[4][10]);

void call_members(char project_name[30], char m[4][10]);
void member_coment(char *(*member), char *file_name);