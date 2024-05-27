# Event Organizer

## Requirements

- MySQL
- g++ compiler

## Setup Instructions

### 1. Install MySQL

Make sure MySQL is installed on your system.

### 2. Install g++ Compiler

Make sure you have the g++ compiler installed.

### 3. Create MySQL Database

Before compiling create the MySQL database. Open MySQL command line client and execute the following query:

CREATE DATABASE event_organizer_final;

### 4. Edit Code

Edit your mysql database username and password in the main.cpp on line 121

### 5. Compile the Code

Excute the following command in cmd

g++ -o EventOrganizer main.cpp -I"C:\Program Files\MySQL\MySQL Server 8.3\include" -I"C:\Program Files\MySQL\MySQL Server 8.3\include\mysql" -L"C:\Program Files\MySQL\MySQL Server 8.3\lib" -lmysql

We used "C:\Program Files\MySQL\MySQL Server 8.3\include" and "C:\Program Files\MySQL\MySQL Server 8.3\include\mysql" as include directory 

We used "C:\Program Files\MySQL\MySQL Server 8.3\lib" as Library directory

We used the linker "-lmysql"

Replace these include file Library file as they are in your file system but this is the default one 

### 5. Run Code

Excute the following command in cmd

EventOrganizer
