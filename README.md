
## Project Title: File-Based Mini Database Management System in C

## Overview
This project is a command-line based Mini Database Management System (DBMS) implemented in C programming language.
It simulates basic database operations such as creating databases, creating tables, inserting records, selecting data, deleting records with conditions, and dropping tables — using text files for persistent storage.
The system is lightweight, file-based, and designed to demonstrate core DBMS concepts like query parsing, file handling, and dynamic memory allocation.

## Objectives
- Implement fundamental DBMS operations in C
-	Practice file handling in C
-	Use dynamic memory allocation (malloc(), calloc( ), free( ))
-	Implement command parsing using strtok( )
-	Use enum and switch-case for structured command execution
-	Support cross-platform compilation (Windows & macOS/Linux)

## Features
The system supports the following commands:
-	CREATE DATABASE <name> 
-	USE <database_name>
-	CREATE TABLE <table_name> column1,column2,column3
-	INSERT <table_name> value1,value2,value3
-	SELECT * FROM <table_name>
-	DELETE <table_name> WHERE column=value
-	DROP TABLE <table_name>
-	EXIT

## Working Principle
-	The program reads user input using fgets().
-	The input string is tokenized using strtok().
-	The first token is mapped to a command using enum.
-	A switch-case block executes the corresponding function.
-	Tables are stored as .txt files inside database folders.
-	Data persists in files even after program termination. 

## Data Structures
-	Columns are dynamically allocated using calloc().
-	Memory is released using free() after use.  <br>

typedef struct { <br>
    char name[30]; <br>
    char dataType[10]; <br>
} Column; <br>
 <br>
typedef struct { <br>
    char name[50]; <br>
    int columnCount; <br>
    Column *columns; <br>
} Table;

## File Storage Structure
- Each database is a folder.
- Each table is a text file.
- First line of file stores column names.
- Subsequent lines store row data.

## Cross-Platform Support
The project uses conditional compilation: <br>
#ifdef _WIN32 <br>
    #define MKDIR(path) _mkdir(path) <br>
#else <br>
    #define MKDIR(path) mkdir(path, 0777) <br>
#endif <br>
This allows us to compile the project in Windows Mac Os and LINUX <br>
 
 ## Advantages
 -	Lightweight and simple
 -  Demonstrates core DBMS logic
 - Uses dynamic memory
 - Cross-platform compatible
 - Easy to extend

## Limitations
-	No indexing
-	No JOIN operations
-	No transaction management
-	No strict data type validation
-	Single-user syste

## Future Enhancements
-	Implement UPDATE command
-	Add multiple WHERE conditions
-	Add data type enforcement
-	Implement indexing for faster search
-	Develop GUI interface

 ## Concepts Demonstrated
- File Handling in C
- Dynamic Memory Allocation
- String Tokenization
- Conditional Compilation
- Structured Programming

## Author
Sarthak Acharya  <br>
Thapathali Campus 






