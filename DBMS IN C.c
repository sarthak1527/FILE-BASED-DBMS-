#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* OS Compatibility */
#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
    #define PATH_SEP "\\"
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0777)
    #define PATH_SEP "/"
#endif

#define MAX_TOKENS 50
#define MAX_LEN 100

char basePath[200] = "CProject";
char currentDB[300] = "";

/* STRUCTURES */
typedef struct {
    char name[30];
    char dataType[10];
} Column;

typedef struct {
    char name[50];
    int columnCount;
    Column *columns;
} Table;

/* ---------------- HELP ---------------- */
void showHelp() {
    printf("\n===== AVAILABLE COMMANDS =====\n\n");

    printf("CREATE DATABASE <database_name>\n");
    printf("USE <database_name>\n");
    printf("CREATE TABLE <table_name> col1,col2,col3\n");
    printf("INSERT <table_name> val1,val2,val3\n");
    printf("SELECT * FROM <table_name>\n");
    printf("DELETE <table_name> WHERE column value\n");
    printf("DROP TABLE <table_name>\n");
    printf("HELP\n");
    printf("EXIT\n");

    printf("\n===============================\n\n");
}

/* ---------------- TOKENIZER ---------------- */
int tokenize(char *input, char tokens[MAX_TOKENS][MAX_LEN]) {
    int count = 0;
    char *tok = strtok(input, " ");
    while (tok != NULL && count < MAX_TOKENS) {
        strcpy(tokens[count++], tok);
        tok = strtok(NULL, " ");
    }
    return count;
}

/* ---------------- CREATE DATABASE ---------------- */
void createDatabase(char *name) {
    char path[300];
    sprintf(path, "%s%s%s", basePath, PATH_SEP, name);
    if (MKDIR(path) == 0)
        printf("Database '%s' created.\n", name);
    else
        printf("Database exists or error.\n");
}

/* ---------------- USE DATABASE ---------------- */
void useDatabase(char *name) {
    char path[300];
    sprintf(path, "%s%s%s", basePath, PATH_SEP, name);

    if (access(path, F_OK) != 0) {
        printf("Database does not exist.\n");
        return;
    }

    strcpy(currentDB, path);
    printf("Using database '%s'\n", name);
}

/* ---------------- CREATE TABLE ---------------- */
void createTable(char *name, char *columnsStr) {

    if (strlen(currentDB) == 0) {
        printf("Select a database first.\n");
        return;
    }

    Table tbl;
    strcpy(tbl.name, name);

    int i;
    tbl.columnCount = 1;
    for (i = 0; columnsStr[i]; i++)
        if (columnsStr[i] == ',') tbl.columnCount++;

    tbl.columns = (Column*)calloc(tbl.columnCount, sizeof(Column));

    int idx = 0;
    char *tok = strtok(columnsStr, ",");
    while (tok) {
        strcpy(tbl.columns[idx].name, tok);
        strcpy(tbl.columns[idx].dataType, "TEXT");
        idx++;
        tok = strtok(NULL, ",");
    }

    char filePath[300];
    sprintf(filePath, "%s%s%s.txt", currentDB, PATH_SEP, name);
FILE *check = fopen(filePath, "r");
if (check) {
    fclose(check);
    printf("Table already exists.\n");
    free(tbl.columns);
    return;
}
    FILE *fp = fopen(filePath, "w");
    for (i = 0; i < tbl.columnCount; i++)
        fprintf(fp, "%s,", tbl.columns[i].name);

    fprintf(fp, "\n");
    fclose(fp);

    free(tbl.columns);
    printf("Table '%s' created.\n", name);
}

/* ---------------- INSERT ---------------- */
void insertInto(char *table, char *values) {

    if (strlen(currentDB) == 0) {
        printf("Select a database first.\n");
        return;
    }

    char filePath[300];
    sprintf(filePath, "%s%s%s.txt", currentDB, PATH_SEP, table);

    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        printf("Table not found.\n");
        return;
    }

    char header[500];
    fgets(header, sizeof(header), fp);

    int colCount = 1;
    for (int i = 0; header[i]; i++)
        if (header[i] == ',') colCount++;

    fclose(fp);

    int valCount = 1;
    for (int i = 0; values[i]; i++)
        if (values[i] == ',') valCount++;

    if (colCount != valCount) {
        printf("Column count mismatch.\n");
        return;
    }

    fp = fopen(filePath, "a");
    fprintf(fp, "%s\n", values);
    fclose(fp);

    printf("Row inserted into '%s'.\n", table);
}
/* ---------------- SELECT ---------------- */
void selectAll(char *table) {
    char filePath[300];
    sprintf(filePath, "%s%s%s.txt", currentDB, PATH_SEP, table);

    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        printf("Table not found.\n");
        return;
    }

    char line[500];
    while (fgets(line, sizeof(line), fp))
        printf("%s", line);

    fclose(fp);
}

/* ---------------- DELETE WHERE ---------------- */
void deleteWhere(char *table, char *column, char *value) {

    char filePath[300];
    sprintf(filePath, "%s%s%s.txt", currentDB, PATH_SEP, table);

    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        printf("Table not found.\n");
        return;
    }

    char tempPath[300];
    sprintf(tempPath, "%s%s%s_temp.txt", currentDB, PATH_SEP, table);
    FILE *temp = fopen(tempPath, "w");

    char header[500];
    fgets(header, sizeof(header), fp);
    fprintf(temp, "%s", header);

    char headerCopy[500];
    strcpy(headerCopy, header);

    char *tok = strtok(headerCopy, ",");
    int colIdx = -1, colCount = 0;

    while (tok) {
        tok[strcspn(tok, "\n")] = 0;
        if (strcmp(tok, column) == 0)
            colIdx = colCount;
        colCount++;
        tok = strtok(NULL, ",");
    }
if (colIdx == -1) {
    printf("Column not found.\n");
    fclose(fp);
    fclose(temp);
    remove(tempPath);
    return;
}
    char line[500];
    int deleted = 0;

    while (fgets(line, sizeof(line), fp)) {

        char tempLine[500];
        strcpy(tempLine, line);

        char *vals[20];
        int i = 0;
        tok = strtok(line, ",");

        while (tok) {
            tok[strcspn(tok, "\n")] = 0;
            vals[i++] = tok;
            tok = strtok(NULL, ",");
        }

        if (colIdx != -1 && strcmp(vals[colIdx], value) == 0) {
            deleted++;
            continue;
        }

        fprintf(temp, "%s", tempLine);
    }

    fclose(fp);
    fclose(temp);

    remove(filePath);
    rename(tempPath, filePath);

    printf("Deleted %d row(s).\n", deleted);
}

/* ---------------- DROP TABLE ---------------- */
void dropTable(char *table) {
    char filePath[300];
    sprintf(filePath, "%s%s%s.txt", currentDB, PATH_SEP, table);

    if (remove(filePath) == 0)
        printf("Table '%s' dropped.\n", table);
    else
        printf("Table not found.\n");
}

/* ---------------- MAIN ---------------- */
int main() {

    MKDIR(basePath);

    char input[500];
    char tokens[MAX_TOKENS][MAX_LEN];

    printf("WELCOME TO C BASED DBMS\n");

    while (1) {

        if (strlen(currentDB) == 0)
            printf("DBMS> ");
        else
            printf("%s> ", currentDB);

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        int count = tokenize(input, tokens);
        if (count == 0)
            continue;

        if (strcmp(tokens[0], "CREATE") == 0 && strcmp(tokens[1], "DATABASE") == 0)
            createDatabase(tokens[2]);

        else if (strcmp(tokens[0], "USE") == 0)
            useDatabase(tokens[1]);

        else if (strcmp(tokens[0], "CREATE") == 0 && strcmp(tokens[1], "TABLE") == 0)
            createTable(tokens[2], tokens[3]);

        else if (strcmp(tokens[0], "INSERT") == 0)
            insertInto(tokens[1], tokens[2]);

        else if (strcmp(tokens[0], "SELECT") == 0)
            selectAll(tokens[3]);

        else if (strcmp(tokens[0], "DELETE") == 0 && strcmp(tokens[2], "WHERE") == 0)
            deleteWhere(tokens[1], tokens[3], tokens[5]);

        else if (strcmp(tokens[0], "DROP") == 0)
            dropTable(tokens[2]);

        else if (strcmp(tokens[0], "HELP") == 0)
            showHelp();

        else if (strcmp(tokens[0], "EXIT") == 0)
            break;

        else
            printf("Invalid command.\n");
    }

    return 0;
}


