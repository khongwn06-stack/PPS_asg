#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

// Struct
struct Student 
{
    string name, id, faculty, phone, vehicle, password;
};

struct Application 
{
    string appID, studentID, status, month, payment;
};

// Global array 
Student students[400];
Application applications[400];

int studentCount = 0;
int applicationCount = 0;

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


int main()
{
    loadStudents();
    loadApplications();

    string choice;

	do
	{
    	cout << "\n========================================\n";
    	cout << "|            STUDENT SYSTEM            |\n";
    	cout << "========================================\n";

    	cout << "| 1. Register                          |\n";
    	cout << "| 2. Login                             |\n";
    	cout << "| 3. Exit                              |\n";
    	cout << "========================================\n";

    	cout << "Select an option: ";
    	cin >> choice;
    	cin.ignore();

    	if(choice == "1")
        	registerStudent();

    	else if(choice == "2")
        	loginStudent();

    	else if(choice == "3")
        	cout << "\nTHANK YOU FOR USING THE STUDENT SYSTEMS. GOODBYE!\n";

    	else
    	{
        	cout << "\nERROR: Invalid choice\n";
        	cout << "PLEASE ENTER 1, 2 or 3\n";
    	}

	} while(choice != "3");
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

        getline(ss, students[studentCount].name, ',');
        getline(ss, students[studentCount].id, ',');
        getline(ss, students[studentCount].faculty, ',');
        getline(ss, students[studentCount].phone, ',');
        getline(ss, students[studentCount].vehicle, ',');
        getline(ss, students[studentCount].password, ',');
        
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

        getline(ss, applications[applicationCount].appID, ',');
        getline(ss, applications[applicationCount].studentID, ',');
        getline(ss, applications[applicationCount].status, ',');
        getline(ss, applications[applicationCount].month, ',');
        getline(ss, applications[applicationCount].payment, ',');

        applicationCount++;
    }
    file.close();
}

// Register student
void registerStudent()
{
    Student s;
    string input;
    
    cout << "\n=======================================================================\n";
    cout << "|                         STUDENT REGISTRATION                        |\n";
    cout << "=======================================================================\n";

    cout << "Enter full name (PRESS 0 return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    s.name = input;

    while(true)
    {
        cout << "Enter student ID (PRESS 0 return to Main Menu): ";
        getline(cin, input);
        if(input == "0") return;

        bool duplicate = false;
        for(int i = 0; i < studentCount; i++)
        {
            if(students[i].id == input)
            {
                duplicate = true;
                cout << "\nTHIS ID ALREADY EXIXTS!\n";
                break;
            }
        }

        if(!duplicate) break;
    }
    s.id = input;

    cout << "Enter faculty (PRESS 0 return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    s.faculty = input;

    cout << "Enter phone number (PRESS 0 return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    s.phone = input;

    cout << "Enter vehicle number (PRESS 0 return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    s.vehicle = input;

    cout << "Set password (PRESS 0 return to Main Menu): ";
    getline(cin, input);
    if(input == "0") return;
    s.password = input;

    students[studentCount] = s;
    studentCount++;

    ofstream file("students.txt", ios::app);
    file << s.name << "," << s.id << "," << s.faculty << ","
         << s.phone << "," << s.vehicle << "," << s.password << endl;
    file.close();

    cout << "\nREGISTERED SUCCESSFULLY!\n";
}
 
// Login student
void loginStudent()
{
    string id, pw;
    int index = -1;

    while(true)
    {
        cout << "\n========================================\n";
        cout << "|           STUDENT LOGIN              |\n";
        cout << "========================================\n";

        cout << "Student ID (PRESS 0 to return): ";
        cin >> id;

        if(id == "0") return;

        cout << "Password (PRESS 0 to return): ";
        cin >> pw;

        if(pw == "0") return;

        bool idFound = false;

        for(int i = 0; i < studentCount; i++)
        {
            if(students[i].id == id)
            {
                idFound = true;

                if(students[i].password == pw)
                {
                    index = i;
                    break;
                }
                else
                {
                    cout << "\nERROR: Incorrect password\n";
                    break;
                }
            }
        }

        if(index != -1) break;

        if(!idFound)
        {
            cout << "\nERROR: Student ID not found\n";
        }
    }

    // login successful
    cout << "\nLOGIN SUCCESSFUL!\n";
    cout << "~ WELCOME " << students[index].name << " ~\n";

    // success login message
    cout << "====================================\n";
	cout << "|           YOUR DETAILS           |\n";
	cout << "====================================\n";

	cout << "| Name         : " << students[index].name;
	cout << string(18 - students[index].name.length(), ' ') << "|\n";

	cout << "| Student ID   : " << students[index].id;
	cout << string(18 - students[index].id.length(), ' ') << "|\n";

	cout << "| Faculty      : " << students[index].faculty;
	cout << string(18 - students[index].faculty.length(), ' ') << "|\n";

	cout << "| Phone Number : " << students[index].phone;
	cout << string(18 - students[index].phone.length(), ' ') << "|\n";

	cout << "| Vehicle No.  : " << students[index].vehicle;
	cout << string(18 - students[index].vehicle.length(), ' ') << "|\n";

	cout << "====================================\n";
	
	studentMenu(index);
}

// Student Menu
void studentMenu(int index)
{
    string choice;

    do
    {
        cout << "\n============ STUDENT MENU ============\n";
        cout << "| 1. Apply New Pass                  |\n";
        cout << "| 2. Renew Pass                      |\n";
        cout << "| 3. View Application Status         |\n";
        cout << "| 4. Update Personal Details         |\n";
        cout << "|------------------------------------|\n";;
        cout << "|     * Press 0 to Log out *         |\n";
        cout << "--------------------------------------\n";

        cout << "Select an option: ";
        cin >> choice;

        if(choice == "1") submitApplication(index);
        else if(choice == "2") renewApplication(index);
        else if(choice == "3") viewApplication(index);
        else if(choice == "4") updateStudentDetails(index);
        else if(choice == "0") cout << "\nLOG OUT...\n";
        else
        {
            cout << "\nINVALID CHOICE! PLEASE TRY AGAIN!\n";
        }

    } while(choice != "0");
}

// Apply New Pass
void submitApplication(int index)
{
    string month;

    while(true)
    {
        cout << "\n===================================================================\n";
        cout << "|                      APPLY NEW PASS                             |\n";
        cout << "===================================================================\n";
        cout << "Enter month (YYYY-MM or 0 to return): ";
        cin >> month;
        

        if(month == "0") return;

        if(!isWithinThreeMonths(month))
        {
            cout << "\nERROR: Only within 3 months allowed!\n";
            continue;
        }

        bool duplicate = false;

        for(int i = 0; i < applicationCount; i++)
        {
            if(applications[i].studentID == students[index].id &&
               applications[i].month == month)
            {
                duplicate = true;
                break;
            }
        }

        if(duplicate)
        {
            cout << "\nERROR: Already applied for this month!\n";
            continue;
        }

        break;
    }

    Application a;
    a.appID = generateApplicationID();
    a.studentID = students[index].id;
    a.status = "Pending";
    a.month = month;
    a.payment = "Unpaid";

    applications[applicationCount] = a;
    applicationCount++;

    ofstream file("applications.txt", ios::app);
    file << a.appID << "," << a.studentID << ","
         << a.status << "," << a.month << "," << a.payment << endl;
    file.close();

    cout << "\nAPPLICATION SUCCESSFUL!" << endl;
	cout << "Your Application ID : " << a.appID<<endl;
}

// Renew Pass
void renewApplication(int index)
{
    bool found = false;
    
    cout << "\n========================================\n";
	cout << "|         EXISTING APPLICATIONS        |\n";
	cout << "========================================\n";

	for(int i = 0; i < applicationCount; i++)
	{
    	if(applications[i].studentID == students[index].id)
    	{
        	cout << "| Application ID : " << applications[i].appID;
        	cout << string(20 - applications[i].appID.length(), ' ') << "|\n";

        	cout << "| Month          : " << applications[i].month;
        	cout << string(20 - applications[i].month.length(), ' ') << "|\n";

        	cout << "----------------------------------------\n";

        	found = true;
    	}
	}
	
    // If no application exists, exit the function
    if(!found)
    {
        cout << "|NO APPLICATION FOUND!		       |\n";
        cout << "========================================\n";
        return;
    }

    string month;

    // Input validation loop for new application month
    while(true)
    {
        cout << "\nRENEW YOUR PASS HERE: "<<endl;
		cout<<"Enter new month (YYYY-MM or 0 to cancel): ";
        cin >> month;

        if(month == "0") return;

        // Prevent selection of past months
        if(month < "2026-04")
        {
            cout << "\nCANNOT SELECT PASS MONTHS.\n";
            continue;
        }

        // Ensure month is within allowed 3-month range
        if(!isWithinThreeMonths(month))
        {
            cout << "\nONLY WITHIN 3 MONTHS ALLOWED.\n";
            continue;
        }

        bool duplicate = false;

        // Check if student already applied for this month
        for(int i = 0; i < applicationCount; i++)
        {
            if(applications[i].studentID == students[index].id &&
               applications[i].month == month)
            {
                duplicate = true;
                break;
            }
        }

        if(duplicate)
        {
            cout << "\nALREADY APPLIED FOR THIS MONTH.\n";
            continue;
        }

        break;
    }

    // Create new application record
    Application a;
    a.appID = generateApplicationID();
    a.studentID = students[index].id;
    a.status = "Pending";
    a.month = month;
    a.payment = "Unpaid";

    // Save application to file
    ofstream file("applications.txt", ios::app);
    file << a.appID << "," << a.studentID
         << ",Pending," << month << ",Unpaid" << endl;
    file.close();

	cout << "\nRENEW SUCCESSFUL!" << endl;
	cout << " Your Application ID : " << a.appI<<endl;
}

// View Application Status
void viewApplication(int index)
{
    cout << "\n====================================\n";
	cout << "|         APPLICATION STATUS       |\n";
	cout << "====================================\n";

	bool found = false;

	for(int i = 0; i < applicationCount; i++)
	{
    	if(applications[i].studentID == students[index].id)
    	{
        	cout << "| Application ID : " << applications[i].appID;
        	cout << string(16 - applications[i].appID.length(), ' ') << "|\n";

        	cout << "| Month          : " << applications[i].month;
        	cout << string(16 - applications[i].month.length(), ' ') << "|\n";

        	cout << "| Status         : " << applications[i].status;
        	cout << string(16 - applications[i].status.length(), ' ') << "|\n";

        	cout << "| Payment        : " << applications[i].payment;
        	cout << string(16 - applications[i].payment.length(), ' ') << "|\n";

        	cout << "====================================\n";

        	found = true;
    	}
	}

	if(!found)
	{
    	cout << "| NO APPLICATION FOUND!            |\n";
    	cout << "====================================\n";
	}
}

// Update personal details
void updateStudentDetails(int index)
{
    string choice;

    do
    {
        cout << "\n====================================\n";
		cout << "|     UPDATE PERSONAL DETAILS      |\n";
		cout << "====================================\n";

		cout << "| 1. Full Name   : " << students[index].name;
		cout << string(16 - students[index].name.length(), ' ') << "|\n";

		cout << "| 2. Faculty     : " << students[index].faculty;
		cout << string(16 - students[index].faculty.length(), ' ') << "|\n";

		cout << "| 3. Phone No.   : " << students[index].phone;
		cout << string(16 - students[index].phone.length(), ' ') << "|\n";

		cout << "| 4. Vehicle No. : " << students[index].vehicle;
		cout << string(16 - students[index].vehicle.length(), ' ') << "|\n";

		cout << "|----------------------------------|\n";
		cout << "| Press 0 Return to Student Menu   |\n";
		cout << "====================================\n";

		cout << "Select an option: ";
		cin >> choice;
		cin.ignore();
      
        bool updated = false;
        string temp;

        if(choice == "1")
        {
            cout << "\nEnter new Full Name (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0")
            {
                students[index].name = temp;
                updated = true;
            }
        }
        else if(choice == "2")
        {
            cout << "\nEnter new Faculty (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0")
            {
                students[index].faculty = temp;
                updated = true;
            }
        }
        else if(choice == "3")
        {
            cout << "\nEnter new Phone Number (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0")
            {
                students[index].phone = temp;
                updated = true;
            }
        }
        else if(choice == "4")
        {
            cout << "\nEnter new Vehicle No (or 0 to cancel): ";
            getline(cin, temp);
            if(temp != "0")
            {
                students[index].vehicle = temp;
                updated = true;
            }
        }
        else if(choice == "0")
        {
            cout << "RETURNING TO STUDENT MENU...\n";
            break;
        }
        else
        {
            cout << "\nINVALID OPTION! PLEASE SELECT AGAIN.\n";
        }

        // save file
        if(updated)
        {
            ofstream file("students.txt");

            for(int i = 0; i < studentCount; i++)
            {
                file << students[i].name << ","
                     << students[i].id << ","
                     << students[i].faculty << ","
                     << students[i].phone << ","
                     << students[i].vehicle << ","
                     << students[i].password << endl;
            }

            file.close();
            cout << "UPDATE SAVED SUCCESSFULLY.\n";
        }

    } while(choice != "0");
}

// Generate Application ID
string generateApplicationID()
{
    stringstream ss;
    ss << "A" << (applicationCount + 1);
    return ss.str();
}

// validation
bool isWithinThreeMonths(string month)
{
  
    if(month.length() != 7) return false;
    if(month[4] != '-') return false;

    for(int i = 0; i < 7; i++)
    {
        if(i == 4) continue;
        if(!isdigit(month[i])) return false;
    }

  
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int curYear = 1900 + ltm->tm_year;
    int curMonth = 1 + ltm->tm_mon;

  
    int inYear, inMonth;
    char dash;

    stringstream ss(month);
    ss >> inYear >> dash >> inMonth;

  
    if(inMonth < 1 || inMonth > 12) return false;

   
    int curTotal = curYear * 12 + curMonth;
    int inTotal = inYear * 12 + inMonth;

    int diff = inTotal - curTotal;

 
    if(diff < 0) return false;
    if(diff > 3) return false;

    return true;
}
