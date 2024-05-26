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
// Function to close the database connection
void close_db()
{
    if (res != NULL)
    {
        mysql_free_result(res); // Free the result set
    }
    mysql_close(conn); // Close the MySQL connection
}

// Function to register a new user
void register_user()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')"; // Inserting to database
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Registration failed: " << mysql_error(conn) << endl;
    }
    // Automatically Logs in after the user registed
    else
    {
        cout << "Registration successful! Logged in successfully" << endl;
        user_dashboard(username);
    }
}

// Function to authenticate a user
bool authenticate_user(string username, string password)
{
    string query = "SELECT * FROM users WHERE username = '" + username + "' AND password = '" + password + "'"; // Finding the user with the same name and password
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Authentication failed: " << mysql_error(conn) << endl;
        return false;
    }

    res = mysql_store_result(conn); // Store the result set
    if (res == NULL)
    {
        cout << "Error check the server: " << mysql_error(conn) << endl;
        return false;
    }

    row = mysql_fetch_row(res); // Fetch the row
    if (row == NULL)
    {
        cout << "Invalid username or password" << endl;
        return false;
    }
    else
    {
        return true;
    }
}

// Function to post a new event (admin only)
void post_event()
{
    string name, date;
    double price;
    int max_capacity;

    cout << "Enter event name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter event date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter event price: ";
    cin >> price;

    cout << "Enter event max capacity: ";
    cin >> max_capacity;

    string query = "INSERT INTO events (name, date, price, max_capacity) VALUES ('" + name + "', '" + date + "', " + string_to_int(price) + ", " + string_to_int(max_capacity) + ")"; // INserting the Event to events dataabase
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Failed to post event: " << mysql_error(conn) << endl;
    }
    else
    {
        cout << "Event posted successfully!" << endl;
    }
}

// Function to view all events
void view_events(string username)
{
    string query = "SELECT * FROM events"; // Printing all the rows in events table
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Failed to retrieve events: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn); // Store the result set
    if (res == NULL)
    {
        cout << "Failed to retrieve events: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Id" << "\t" << "Name" << "\t" << "Date" << "\t\t" << "Price" << "\t" << "Max" << "\t" << "current_capacity" << endl;
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            cout << row[i] << "\t";
        }
        cout << endl;
    }

    int choice;
    cout << "Enter the event ID to book or 0 to go back: ";
    cin >> choice;

    if (choice != 0)
    {
        book_event(username, choice);
    }
}
// Function to search for an event by name
void search_event(string &username)
{
    string name;
    cout << "Enter event name to search: ";
    cin.ignore();
    getline(cin, name);

    string query = "SELECT * FROM events WHERE name LIKE '%" + name + "%'"; // Filtering out the name the user inserted
    if (mysql_query(conn, query.c_str()))
    {
        cout << "Failed to search events: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn); // Store the result set
    if (res == NULL)
    {
        cout << "Failed Because: " << mysql_error(conn) << endl;
        return;
    }

    cout << "Id" << "\t" << "Name" << "\t" << "Date" << "\t\t" << "Price" << "\t" << "Max" << "\t" << "current_capacity" << endl;
    int num_fields = mysql_num_fields(res);
    while ((row = mysql_fetch_row(res)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            cout << row[i] << "\t";
        }
        cout << endl;
    }

    int choice;
    cout << "1. Book Event"<< endl;
    cout << "2. Go Back" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1)
    {
        int event_id;
        cout << "Enter event ID to book: ";
        cin >> event_id;
        book_event(username, event_id);
    }
    else
    {
        user_dashboard(username);
    }
}

// Function to book an event by its ID
void book_event(string username, int event_id)
{
    string query = "SELECT * FROM events WHERE id = " + string_to_int(event_id); // selecting the specifc event to check its max and curr capacity
    if (mysql_query(conn, query.c_str()))
    { 
        cout << "Failed to retrieve event beacause: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn); // Store the result set
    if (res == NULL)
    {
        cout << "Failed to the the data beacause: " << mysql_error(conn) << endl;
        return;
}
