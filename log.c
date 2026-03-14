#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define LOG_FILE "log.txt"

void log_msg(char *message) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (fp == NULL) {
        printf("Error opening log file");
        return;
    }
    fprintf(fp, "%s", message);
    fclose(fp);
}

void show_logs() {
    FILE *fp = fopen(LOG_FILE, "r");
    if (fp == NULL) {
        printf("No logs found or unable to open log file.\n");
        return;
    }
    char line[512];
    printf("\n---- System Logs ----\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("\n---------------------\n");
    fclose(fp);
}

void clear_logs() {
    FILE *fp = fopen(LOG_FILE, "w"); // 'w' mode truncates the file to 0 length
    if (fp == NULL) {
        printf("Unable to clear logs");
        return;
    }
    fclose(fp);
    printf("Logs cleared successfully.\n");
}