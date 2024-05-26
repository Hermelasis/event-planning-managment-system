#include <iostream>
#include <mysql.h>
#include <sstream>
#include <string>

using namespace std;

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

// Creating connections and database function
void connect_db();
void close_db();
void create_db();

// Autuntication functions
void register_user();
bool authenticate_user(string username, string password);

// Users Privilage
void view_events(string username);
void search_event(string &username);
void book_event(string username, int event_id);

// Dashbords
void admin_dashboard();
void user_dashboard(string username);

// Admin Privilages
void post_event();
void view_events_with_registrations();
void view_event_registrants();

void create_admin_user();

// Some helper functions
string string_to_int(int x);
int string_to_int(const string &str);

// Main function
int main()
{
    connect_db();
    create_db();
    create_admin_user();

    int choice;
    do
    {
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            register_user();
            break;
        case 2:
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            if (username == "admin" && password == "admin")
            {
                admin_dashboard();
            }
            else if (authenticate_user(username, password))
            {
                user_dashboard(username);
            }
            else
            {
                cout << "No such user!" << endl;
            }
            break;
        }
        case 3:
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 3);

    close_db();
    return 0;
}

int string_to_int(const string &str)
{
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

string string_to_int(int x)
{
    stringstream ss;
    ss << x;
    return ss.str();
}

// Function to connect to the database
void connect_db()
{
    conn = mysql_init(NULL);
    if (conn == NULL)
    {
        cout << "mysql_init() failed" << endl;
        exit(1);
    }

    // Connect to the MySQL database
    if (mysql_real_connect(conn, "localhost", "username", "password", "event_organizer_final", 0, NULL, 0) == NULL) // Replace with ur own username and password 
    {
        cout << "mysql_real_connect() failed\n"
             << mysql_error(conn) << endl;
        mysql_close(conn);
        exit(1);
    }
}
