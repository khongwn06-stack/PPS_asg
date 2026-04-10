// Preprocessor Directives
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Function Prototypes
void registerStudent();
void loginStudent();
void loadStudents();
void loadApplications();
void studentMenu(int studentIndex);
void submitApplication(int studentIndex);
void renewApplication(int studentIndex); 
void viewApplication(int studentIndex);
void updateStudentDetails(int studentIndex);
string generateApplicationID();
bool isWithinThreeMonths(string month); 

// Student arrays
string studentName[400];
string studentID[400];
string faculty[400];
string phoneNo[400];
string vehicleNo[400];
string password[400];
int studentCount = 0;

// Application arrays
string applicationID[400];
string applicationStudentID[400];
string applicationStatus[400];
string applicationMonth[400];
string applicationPaymentStatus[400];
int applicationCount = 0;

int main()
{
    loadStudents();
    loadApplications();

    string choice;
    do
    {
        cout << "\n======== MENU ========" << endl;
        cout << "1. Register " << endl;
        cout << "2. Login " << endl;
        cout << "3. Exit" << endl;
        cout << "\nHi! Please select an option: ";
        cin >> choice;
        cin.ignore();

        if(choice == "1") registerStudent();
        else if(choice == "2") loginStudent();
        else if(choice == "3") cout << "Thank you for using the Student System. Goodbye!" << endl;
        else cout << "Invalid choice! Please enter 1, 2, or 3. " << endl;

    } while(choice != "3");

    return 0;
}

// Load students from file 
void loadStudents()
{
    ifstream file("students.txt");
    if (!file) return;

    studentCount = 0;
    string line;
    
    while (getline(file, line))
    {
        stringstream ss(line);
        getline(ss, studentName[studentCount], ',');
        getline(ss, studentID[studentCount], ',');
        getline(ss, faculty[studentCount], ',');
        getline(ss, phoneNo[studentCount], ',');
        getline(ss, vehicleNo[studentCount], ',');
        getline(ss, password[studentCount], ',');
        studentCount++;
    }
    file.close();
}

// Load applications from file
void loadApplications()
{
    ifstream file("applications.txt");
    if (!file) return;

    applicationCount = 0;
    string line;
    
    while (getline(file, line))
    {
        stringstream ss(line);
        
        getline(ss, applicationID[applicationCount], ',');
        getline(ss, applicationStudentID[applicationCount], ',');
        getline(ss, applicationStatus[applicationCount], ',');
        getline(ss, applicationMonth[applicationCount], ',');
        getline(ss, applicationPaymentStatus[applicationCount], ',');
        applicationCount++;
    }
    file.close();
}

// Register student
void registerStudent()
{
    cout << "\n===== Register New Student =====" << endl;

    string input;
    
    cout << "Enter Full Name (or 0 to return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    studentName[studentCount] = input;

    while(true)
    {
        cout << "Enter Student ID (or 0 to return to Main Menu): ";
        getline(cin, input);
        if(input == "0") return;

        bool duplicate = false;
        for(int i = 0; i < studentCount; i++)
        {
            if(studentID[i] == input)
            {
                duplicate = true;
                cout << "This ID already exists! Please enter a different ID.\n";
                break;
            }
        }
        if(!duplicate) break;
    }
    studentID[studentCount] = input;

    cout << "Enter Faculty (or 0 to return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    faculty[studentCount] = input;

    cout << "Enter Phone Number (or 0 to return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    phoneNo[studentCount] = input;

    cout << "Enter Vehicle Number (or 0 to return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    vehicleNo[studentCount] = input;

    cout << "Set Password (or 0 to return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    password[studentCount] = input;

    ofstream file("students.txt", ios::app);
    file << studentName[studentCount] << ","
         << studentID[studentCount] << ","
         << faculty[studentCount] << ","
         << phoneNo[studentCount] << ","
         << vehicleNo[studentCount] << ","
         << password[studentCount] << endl;
    file.close();

    cout << "\nCongratulations " << studentName[studentCount] << "! Your registration was successful!" << endl;
    studentCount++;
}

// Login student
void loginStudent()
{
    string id, psword;
    int studentIndex = -1;

    while(true)
    {
        cout << "\n===== Student Login =====" << endl;
        cout << "Enter Student ID (or 0 to return to Main Menu): ";
        cin >> id;
        if(id == "0") return;
        cout << "Enter Password (or 0 to return to Main Menu): ";
        cin >> psword;
        if(psword == "0") return;

        bool idFound = false;
        for(int i = 0; i < studentCount; i++)
        {
            if(studentID[i] == id)
            {
                idFound = true;
                if(password[i] == psword)
                {
                    studentIndex = i;
                    break;
                }
                else
                {
                    cout << "Incorrect password. Please try again.\n";
                    break;
                }
            }
        }

        if(studentIndex != -1) break;
        if(!idFound)
        {
            cout << "Student ID not found. Please register first.\n";
        }
    }

    // Show login success and student details
    cout << "\nLogin Successful! Welcome " << studentName[studentIndex] << "!" << endl;
    cout << "====== Your Details ======" << endl;
    cout << "Name         : " << studentName[studentIndex] << endl;
    cout << "Student ID   : " << studentID[studentIndex] << endl;
    cout << "Faculty      : " << faculty[studentIndex] << endl;
    cout << "Phone Number : " << phoneNo[studentIndex] << endl;
    cout << "Vehicle No.  : " << vehicleNo[studentIndex] << endl;
    cout << "---------------------------" << endl;

    studentMenu(studentIndex);
}

// Student Menu
void studentMenu(int studentIndex)
{
    string choice;
    do
    {
        cout << "\n===== Student Menu =====" << endl;
        cout << "1. Apply New Pass" << endl;
        cout << "2. Renew Pass" << endl;
        cout << "3. View Application Status" << endl;
        cout << "4. Update Personal Details" << endl;
        cout << "5. Logout" << endl;
        cout << "Select an option: ";
        cin >> choice;

        if(choice == "1") submitApplication(studentIndex);
        else if(choice == "2") renewApplication(studentIndex);
        else if(choice == "3") viewApplication(studentIndex);
        else if(choice == "4") updateStudentDetails(studentIndex);
        else if(choice == "5") cout << "Logout...\n";
        else cout << "Invalid choice!\n";

    } while(choice != "5");
}

// Apply New Pass
void submitApplication(int studentIndex)
{
    string month;
    
    while(true)
    {
        cout << "Enter month (YYYY-MM) or 0 to return: ";
        cin >> month;
        if(month == "0") return;

        if(!isWithinThreeMonths(month))
        {
            cout << "Only future months within 3 months allowed.\n";
            continue;
        }

        bool duplicate = false;
        for(int i = 0; i < applicationCount; i++)
        {
            if(applicationStudentID[i] == studentID[studentIndex] &&
               applicationMonth[i] == month)
            {
                duplicate = true;
                break;
            }
        }

        if(duplicate)
        {
            cout << "Already applied. Choose another month.\n";
            continue;
        }

        break;
    }

    string id = generateApplicationID();

    applicationID[applicationCount] = id;
    applicationStudentID[applicationCount] = studentID[studentIndex];
    applicationStatus[applicationCount] = "Pending";
    applicationMonth[applicationCount] = month;
    applicationPaymentStatus[applicationCount] = "Unpaid";

    ofstream file("applications.txt", ios::app);
    file << id << "," << studentID[studentIndex] << ",Pending," << month << ",Unpaid" << endl;
    file.close();

    applicationCount++;
    cout << "Application success! Your Application is ID is " << id << endl;
}

// Renew Pass
void renewApplication(int studentIndex)
{
    cout << "\n===== Renew Pass =====\n";

    bool found = false;

    cout << "Your existing applications:\n";

    for(int i = 0; i < applicationCount; i++)
    {
        if(applicationStudentID[i] == studentID[studentIndex])
        {
            cout << applicationID[i] << " | "
                 << applicationMonth[i] << " | "
                 << applicationStatus[i] << endl;
            found = true;
        }
    }

    if(!found)
    {
        cout << "No application found.\n";
        return;
    }

    string month;

    while(true)
    {
        cout << "\nEnter new month (YYYY-MM or 0 to cancel): ";
        cin >> month;

        if(month == "0") return;

        if(month < "2026-04")
        {
            cout << "Cannot select past months.\n";
            continue;
        }

        if(!isWithinThreeMonths(month))
        {
            cout << "Only within 3 months allowed.\n";
            continue;
        }

        bool duplicate = false;
        for(int i = 0; i < applicationCount; i++)
        {
            if(applicationStudentID[i] == studentID[studentIndex] &&
               applicationMonth[i] == month)
            {
                duplicate = true;
                break;
            }
        }

        if(duplicate)
        {
            cout << "Already applied for this month.\n";
            continue;
        }

        break;
    }

    string id = generateApplicationID();

    applicationID[applicationCount] = id;
    applicationStudentID[applicationCount] = studentID[studentIndex];
    applicationStatus[applicationCount] = "Pending";
    applicationMonth[applicationCount] = month;
    applicationPaymentStatus[applicationCount] = "Unpaid";

    ofstream file("applications.txt", ios::app);
    file << id << "," << studentID[studentIndex]
         << ",Pending," << month << ",Unpaid" << endl;
    file.close();

    applicationCount++;

    cout << "Renew successful! Your Application ID is " << id << endl;
}

// View Application Status
void viewApplication(int studentIndex)
{
    cout << "\n===== Your Applications =====" << endl;
    bool found = false;
    for(int i = 0; i < applicationCount; i++)
    {
        if(applicationStudentID[i] == studentID[studentIndex])
        {
            cout << "Application ID: " << applicationID[i]
                 << " | Month: " << applicationMonth[i]
                 << " | Status: " << applicationStatus[i]
                 << " | Payment: " << applicationPaymentStatus[i]
                 << endl;
                 
            found = true;
        }
    }
    if(!found) cout << "No applications found." << endl;
}

// Update personal details
void updateStudentDetails(int studentIndex)
{
   string choice;
    do
    {
        cout << "\n===== Update Personal Details =====" << endl;
        cout << "1. Edit Full Name (current: " << studentName[studentIndex] << ")" << endl;
        cout << "2. Edit Faculty (current: " << faculty[studentIndex] << ")" << endl;
        cout << "3. Edit Phone Number (current: " << phoneNo[studentIndex] << ")" << endl;
        cout << "4. Edit Vehicle No (current: " << vehicleNo[studentIndex] << ")" << endl;
        cout << "0. Return to Student Menu" << endl;
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore();

        bool updated = false; // Track if any field was updated
        string temp;

        if(choice == "1")
        {
            cout << "Enter new Full Name (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0") { studentName[studentIndex] = temp; updated = true; }
        }
        else if(choice == "2")
        {
            cout << "Enter new Faculty (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0") { faculty[studentIndex] = temp; updated = true; }
        }
        else if(choice == "3")
        {
            cout << "Enter new Phone Number (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0") { phoneNo[studentIndex] = temp; updated = true; }
        }
        else if(choice == "4")
        {
            cout << "Enter new Vehicle No (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0") { vehicleNo[studentIndex] = temp; updated = true; }
        }
        else if(choice == "0")
        {
            cout << "Returning to Student Menu..." << endl;
            break;
        }
        else
        {
            cout << "Invalid option! Please select again." << endl;
        }

        if(updated)
        {
            // Save updates after each successful edit
            ofstream file("students.txt");
            for(int i = 0; i < studentCount; i++)
            {
                file << studentName[i] << "," << studentID[i] << "," << faculty[i] << ","
                     << phoneNo[i] << "," << vehicleNo[i] << "," << password[i] << endl;
            }
            file.close();
            cout << "Update saved successfully.\n";
        }

    } while(choice != "0");
}

// Generate Application ID
string generateApplicationID()
{
    int num = applicationCount + 1;
    stringstream ss;
    ss << "A" << num;
    return ss.str();
}

// validation
bool isWithinThreeMonths(string month)
{
    string current = "2026-04";

    // must be valid format YYYY-MM
    if(month.length() != 7) return false;

    // allow current month and next 3 months only
    if(month < current) return false;
    if(month > "2026-07") return false;

    return true;
}

