#include "header.h"

char homeDir[MAX];
char relativePath[MAX];
char hostname[MAX];
char username[MAX];
char path[MAX];
char command[MAX];
char input[MAX];
char list[MAX][MAX];
char logPath[MAX+20];
int lineCount = 0;
char andList[MAX][MAX];
int size;
int timeFlag = 0;
char processInfo[MAX];
double timeTaken = 0;
char prevDir[MAX];
char flags[MAX];
char revealpath[MAX];
char myshline[MAX];
int alias_count = 0;
char shrcPath[MAX+20];
int activity_count = 0;
Activity activities[MAX];
pid_t bg_processes[100];
int bg_process_count = 0;
int pipeStartArea = 0;
int real_size;
int fg_proc;
char manCommand[MAX];
int latest_pid;
int neonate_time;
int mk_hop_flag;
int hop_seek_flag;
volatile sig_atomic_t stop;