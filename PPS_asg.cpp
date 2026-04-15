#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <ctime>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()
using namespace std;

//========================================Login Part========================================
bool home_page();


//========================================Student Part========================================
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

void stud_page();
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


//========================================Admin Part========================================
void admin_login();
void admin_page();

void view_app();

//Applicatons Validation Process
void app_validation();
void approve_app();
string validate_app();

//Pass.txt
string generatePassID();
void createPass(string studentID, string startDate, int months);

//Analytics
void generateAnalytic();
void averageRenewal();
void total_app();
void utilizationRate();
void monthlyIncome();
void growthRate();
void fullReport();



//========================================Payment Part========================================
void paymentModule();

//========================================Tracking Part========================================
void trackingModule();



//===========================================================Addition===========================================================
void limit_input(){ //limited output after enter Alphabet
	if(cin.fail()){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
} 

void clear_screen(){ //clear sreen after move to the new page
	cin.get();
	system("cls"); //clear screen
}

void loading_screen(){ //loading for 3 second
	cout<<"\nLoading";
    for(int i=0; i<3; i++){ //loading for 3 second
        cout<<".";
        sleep(1); //one "." for one second
    }
    cout<<"\nDone!"<<endl<<endl;
}  

//Helper
string intToString(int num){
    stringstream ss;
    ss << num;
    return ss.str();
}

int stringToInt(string str){
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

//Exit System
void exit_system(){
	loading_screen();
	cout<<"\nTHANK YOU FOR USING CAR PARKING PASS SYSTEM! GOODBYE!"<<endl;
	exit(0);
}


//===========================================================Main Menu===========================================================
int main()
{
	//===============Student Part===============
	loadStudents();
    loadApplications();
    
	//===============Admin Part===============
	string status, reason, studentID, startDate;
	int months;
	bool incompleteDetails = false;
	bool UnpaidPass = false;
	int pass = 0;
	int activeUser = 0, parkingSlot = 100;
	
	home_page();
	
	return 0;
}

//===========================================================Login Part===========================================================
bool home_page()
{
	int homeChoice;

	do{
		cout << "        MPKJ MONTHLY CAR PARKING PASS SYSTEM       \n";
        cout << "===================================================\n";
        cout << "|                    HOME PAGE                    |\n";
        cout << "===================================================\n";

        cout << "| 1. Student Module                               |\n";
        cout << "| 2. Admin Module                                 |\n";
        cout << "| 3. Payment Module                               |\n";
        cout << "| 4. Tracking & Alert System Module               |\n";
        cout << "| 5. Exit System                                  |\n";
        cout << "===================================================\n";

        cout << "Please select an option: ";
		cin>>homeChoice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		
		switch(homeChoice){
			case 1: stud_page(); break;
			case 2: admin_login(); break;
			case 3: paymentModule(); break;
			case 4: trackingModule(); break;
			case 5: exit_system(); break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-5."<<endl;
		}
	}while (homeChoice != 5);
}

//===========================================================Student Part===========================================================
void stud_page()
{
    int student;

    do
    {
        cout << "\n========== STUDENT MODULE ==========\n";
        cout << "| 1. Register                      |\n";
        cout << "| 2. Login                         |\n";
        cout << "| 3. Back to Home Page             |\n";
        cout << "====================================\n";

        cout << "Select an option: ";
        cin >> student;
		
		switch(student){
			case 1: 
			 cin.ignore(numeric_limits<streamsize>::max(), '\n');
			 registerStudent(); 
			 break;
			case 2: loginStudent(); break;
			case 3: home_page(); break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-3."<<endl;
		}
    } while(student != 3);
}

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

    cout << "Enter full name (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0") return;
    s.name = input;

    while(true)
    {
        cout << "Enter student ID (PRESS 0 to return): ";
        getline(cin, input);
        if(input == "0") return;

        bool duplicate = false;
        
        for(int i = 0; i < studentCount; i++)
        {
            if(students[i].id == input)
            {
                duplicate = true;
                cout << "\nTHIS STUDENT ID ALREADY EXIXTS!\n";
                break;
            }
        }

        if(!duplicate) break;
    }
    s.id = input;

    cout << "Enter faculty (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0") return;
    s.faculty = input;

    cout << "Enter phone number (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0") return;
    s.phone = input;

    cout << "Enter vehicle number (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0") return;
    s.vehicle = input;

    cout << "Set password (PRESS 0 to return): ";
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
        cout << "\n=======================================================\n";
        cout << "|                    STUDENT LOGIN                    |\n";
        cout << "=======================================================\n";

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
                    cout << "\nERROR: Incorrect password!\n";
                    break;
                }
            }
        }

        if(index != -1) break;

        if(!idFound)
        {
            cout << "\nERROR: Student ID not found!\n";
        }
    }

    // login successful
    cout << "\nLOGIN SUCCESSFUL!\n";
    cout << "~ WELCOME " << students[index].name << " ~\n";
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
    while(true)
    {
        cout << "\n========= STUDENT DASHBOARD ==========\n";
        cout << "| 1. Apply New Pass                  |\n";
        cout << "| 2. Renew Pass                      |\n";
        cout << "| 3. View Application Status         |\n";
        cout << "| 4. Update Personal Details         |\n";
        cout << "| 5. Back to Student Module          |\n";
        cout << "======================================\n";

        cout << "Please select an option: ";

        string input;
        getline(cin, input);

        if(input.empty()) continue;

        stringstream ss(input);
        int studchoice;

        if (!(ss >> studchoice))
        {
            cout << "Invalid input!\n";
            continue;
        }

        switch(studchoice)
        {
            case 1: submitApplication(index); break;
            case 2: renewApplication(index); break;
            case 3: viewApplication(index); break;
            case 4: updateStudentDetails(index); break;

            case 5:
                cout << "\nBack to Student Module...\n";
                return;

            default:
                cout << "Invalid Option!\n";
        }
    }
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

        getline(cin, month);

        if(month == "0") return;

        if(month.length() != 7 || month[4] != '-')
        {
            cout << "\n[ERROR] Invalid format! Use YYYY-MM.\n";
            continue;
        }

        int inYear, inMonth;
        char dash;
        stringstream ss(month);
        ss >> inYear >> dash >> inMonth;

        if(inMonth < 1 || inMonth > 12)
        {
            cout << "\n[ERROR] Invalid month value!\n";
            continue;
        }

        time_t now = time(0);
        tm *ltm = localtime(&now);

        int curYear = 1900 + ltm->tm_year;
        int curMonth = 1 + ltm->tm_mon;

        int curTotal = curYear * 12 + curMonth;
        int inTotal = inYear * 12 + inMonth;

        if(inTotal < curTotal)
        {
            cout << "\n[ERROR] Cannot select past month!\n";
            continue;
        }

        if(inTotal - curTotal > 3)
        {
            cout << "\n[ERROR] Cannot exceed 3 months!\n";
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
            cout << "\n[ERROR] Already applied for this month!\n";
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

    applications[applicationCount++] = a;

    ofstream file("applications.txt", ios::app);
    file << a.appID << "," << a.studentID << ","
         << a.status << "," << a.month << "," << a.payment << endl;
    file.close();

    cout << "\nAPPLICATION SUCCESSFUL!\n";
    cout << "Your Application ID : " << a.appID << endl;
}

// Renew Pass
void renewApplication(int index)
{
    bool found = false;

    cout << "\n==========================================================================\n";
    cout << "|                         EXISTING APPLICATIONS                          |\n";
    cout << "==========================================================================\n";

    int list[400];
    int count = 0;

    for(int i = 0; i < applicationCount; i++)
    {
        if(applications[i].studentID == students[index].id)
        {
            cout << "Index number: " << count << " | ";
            cout << "Application ID: " << applications[i].appID << " | "
                 << "Month: " << applications[i].month << " | "
                 << "Status: " << applications[i].status << endl;

            list[count] = i;
            count++;
            found = true;
        }
    }

    if(!found)
    {
        cout << "\nNO APPLICATION FOUND! PLEASE APPLY FIRST.\n";
        return;
    }

    int choice;

    while(true)
    {
        cout << "\nSelect index (press -1 to cancel): ";

        string input;
        getline(cin, input);

        stringstream ss(input);
        if(!(ss >> choice))
        {
            cout << "Invalid input format!\n";
            continue;
        }

        if(choice == -1)
        {
            cout << "Cancelled\n";
            return;
        }

        if(choice >= 0 && choice < count)
            break;

        cout << "INVALID SELECTION!\n";
    }

    int realIndex = list[choice];

    string month;

    while(true)
    {
        cout << "\nEnter new month (YYYY-MM or 0 to cancel): ";

        getline(cin, month);

        if(month == "0") return;

        if(month.length() != 7 || month[4] != '-')
        {
            cout << "[ERROR] Invalid format!\n";
            continue;
        }

        int inYear, inMonth;
        char dash;
        stringstream ss(month);
        ss >> inYear >> dash >> inMonth;

        if(inMonth < 1 || inMonth > 12)
        {
            cout << "[ERROR] Invalid month!\n";
            continue;
        }

        time_t now = time(0);
        tm *ltm = localtime(&now);

        int curYear = 1900 + ltm->tm_year;
        int curMonth = 1 + ltm->tm_mon;

        int curTotal = curYear * 12 + curMonth;
        int inTotal = inYear * 12 + inMonth;

        if(inTotal < curTotal)
        {
            cout << "[ERROR] Cannot select past month!\n";
            continue;
        }

        if(inTotal - curTotal > 3)
        {
            cout << "[ERROR] Cannot exceed 3 months!\n";
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
            cout << "[ERROR] Already applied!\n";
            continue;
        }

        break;
    }

    applications[realIndex].month = month;
    applications[realIndex].status = "Pending";
    applications[realIndex].payment = "Unpaid";

    ofstream file("applications.txt");

    for(int i = 0; i < applicationCount; i++)
    {
        file << applications[i].appID << ","
             << applications[i].studentID << ","
             << applications[i].status << ","
             << applications[i].month << ","
             << applications[i].payment << endl;
    }

    file.close();

    cout << "\nRENEW SUCCESSFUL!\n";
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
        cout << "\n===========================================\n";
		cout << "|           UPDATE PERSONAL DETAILS       |\n";
		cout << "===========================================\n";

		cout << "| 1. Full Name   : " << students[index].name;
		cout << string(23 - students[index].name.length(), ' ') << "|\n";

		cout << "| 2. Faculty     : " << students[index].faculty;
		cout << string(23 - students[index].faculty.length(), ' ') << "|\n";

		cout << "| 3. Phone No.   : " << students[index].phone;
		cout << string(23 - students[index].phone.length(), ' ') << "|\n";

		cout << "| 4. Vehicle No. : " << students[index].vehicle;
		cout << string(23 - students[index].vehicle.length(), ' ') << "|\n";

		cout << "|-----------------------------------------|\n";
		cout << "| Press 0 Return to Student Dashboard     |\n";
		cout << "===========================================\n";

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
            cout << "\nRETURNING TO STUDENT DASHBOARD...\n";
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


//===========================================================Admin Part===========================================================
void admin_page()
{
	int adminChoice;
	
	cout<<"----------Admin Page----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. View Application"<<endl;
		cout<<"2. Application Validation"<<endl;
		cout<<"3. Generate Analytics"<<endl;
		cout<<"4. Full Analytics Reports"<<endl;
		cout<<"5. Return to Home Page"<<endl<<endl;
		
		cout<<"Enter your option : ";
		cin>>adminChoice;
		
		switch(adminChoice){
			case 1: view_app(); break;
			case 2: app_validation(); break;
			case 3: generateAnalytic(); break;
			case 4: fullReport(); break; 
			case 5: cout<<"\nReturn to Admin Page\n"; loading_screen(); clear_screen(); home_page(); break;
			default: cout<<"Invalid Option! Please Enter the Number 1-5."<<endl;
		}
	}while(adminChoice != 5);
}

void admin_login()
{
	string id, username, password;
	string adminUsername, adminPassword;
	bool loginSuccess = false;

	cout<<"\n---------Admin Login---------"<<endl<<endl;
	do{
		cout<<"Username: ";
		cin>>adminUsername;
		cout<<"Password: ";
		cin>>adminPassword;
		
		ifstream file("admin.txt");
		//file.open("admin.txt");
        loginSuccess = false;
		
		//if file doesn't exist, create default admin
	    if(!file){
	        ofstream newFile("admin.txt");
	        newFile<<"A001|admin|1234\n";
	        newFile.close();
	        file.open("admin.txt"); // reopen for reading
	    }
			
		string line;
		while(getline(file, line)){
		    stringstream ss(line);
		    string id, username, password;
		    
		    getline(ss, id, '|');
		    getline(ss, username, '|');
		    getline(ss, password, '|');
		
		    if(adminUsername == username && adminPassword == password){
		        loginSuccess = true;
		    }
		}
		file.close();
		
		if(!loginSuccess){
		    cout<<"Login Failed! Please try again.\n";
		}
	}while(!loginSuccess)/*(adminUsername != username || adminPassword != password)*/;
	cout<<"Login Successful!"<<endl<<endl;
	
	loading_screen();
	clear_screen();
	admin_page();
}

void view_app()
{
	cout<<"----------View Application----------"<<endl;
//	ifstream file("applications.txt");
//    string line;
//
//    while(getline(file, line)) {  //getline() = captures full sentences
//        if(line.find("Pending") != string::npos) {
//            cout << line << endl;
//        }
//    }
//
//    file.close();
}

//Pass.txt
string generatePassID()
{
    static int id = 1;
    return "P" + intToString(1000 + id++);
}

void createPass(string studentID, string startDate, int months)
{
	ofstream outFile("passes.txt", ios::app);
	string passID = generatePassID();
	
	int month = stringToInt(startDate.substr(5,2));
    month += months;

    string endDate = startDate.substr(0,5) +
                     (month < 10 ? "0" : "") + intToString(month) +
                     startDate.substr(7);

    outFile << passID << "|" << studentID << "|"
            << startDate << "|" << endDate << "|PendingPayment" << endl;

    outFile.close();
}

//Applicatons Validation Process
void app_validation()
{
	view_app();
		
	string appID, reason;  //getline() only can allow string
	char validation, app_dec;
	
	cout<<"----------Application Validation----------"<<endl;
	do{
		cout<<"Enter Application ID: ";
	    cin>>appID;
	    
	    cout<<"\nDetail of the Application ID"<<endl;
		cout<<"Application ID: "<<appID<<endl
			<<"Student ID: "<<endl
			<<"..."<<endl<<endl;
			
		do{		
			cout<<"Start Application Validation (y/n): ";
			cin>>validation;
			
			if(validation=='y' || validation=='Y'){
				cout<<"start validation..."<<endl;
				string reason = validate_app();

				if(reason == "Approved"){
				    approve_app();
				}
				else {
				    cout << "Rejected reason: " << reason << endl;
				}
			}
			
			else if(validation!='y' && validation!='Y' && validation!='n' && validation!='N'){
				cout<<"Invalid Input! Enter only alphabet 'y' for yes or 'n' for no"<<endl<<endl;
			}
			
			else{
				cout<<"\nReturn to Admin Page";
				loading_screen();
				admin_page();
			}
			
		}while(validation!='y' && validation!='Y' && validation!='n' && validation!='N');
		
	}while(validation=='n' || validation=='N');
}

void approve_app()
{
//	ifstream inFile("applications.txt");
//    ofstream tempFile("temp.txt");
//
//	string line;
//
//    while(getline(inFile, line)) {
//        string appID, studentID, type, months, status, applyDate, startDate, reason;
//
//        stringstream ss(line);
//        getline(ss, appID, '|');
//        getline(ss, studentID, '|');
//        getline(ss, type, '|');
//        getline(ss, months, '|');
//        getline(ss, status, '|');
//        getline(ss, applyDate, '|');
//        getline(ss, startDate, '|');
//        getline(ss, reason, '|');
//	
//		if(appID == targetAppID && status == "Pending") {
//            cout << "AppID: " << appID << ", Student: " << studentID << endl;
//            createPass(studentID, startDate, stringToInt(months));
//        }
//        tempFile << appID << "|" << studentID << "|" << type << "|"
//                 << months << "|" << status << "|" << applyDate
//                 << "|" << startDate << "|" << reason << endl;
//    }
//    
//    inFile.close();
//    tempFile.close();
//
//    remove("applications.txt");
//    rename("temp.txt", "applications.txt");
    
    cout << "\nApproved!\nAll applications processed.\n";
}

string validate_app()
{
	int pass=0, months=2, activeUser=50, parkingSlot=100;
	
    if(pass != 0) return "Pass overlap";
    else if(months > 3) return "Exceeded limit";
    else if(activeUser == parkingSlot) return "Parking full";

    return "Approved";
}

void generateAnalytic()
{
	int analyticChoice;
	
	cout<<"----------Generate Analytics----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. Average Renewal"<<endl;
		cout<<"2. Total Applications"<<endl;
		cout<<"3. Car Park Utilization Rate"<<endl;
		cout<<"4. Monthly Income"<<endl;
		cout<<"5. Growth Rate"<<endl;
		cout<<"6. Return to Admin Page"<<endl<<endl;
		//cout<<"6. Full Analytics Reports"<<endl;
		//cout<<"7. Return to Admin Page"<<endl<<endl;
		
		cout<<"Enter your option : ";
		cin>>analyticChoice;
		
		switch(analyticChoice){
			case 1: averageRenewal(); break;
			case 2: total_app(); break;
			case 3: utilizationRate(); break;
			case 4: monthlyIncome(); break; 
			case 5: growthRate(); break;
			case 6: admin_page(); break;
			//case 6: fullReport(); break;
			//case 7: admin_page(); break;
			default: cout<<"Invalid Option! Please Enter the Number 1-6."<<endl;
		}
	}while(analyticChoice != 6);
}

void averageRenewal(){
	cout<<"----------Average Renewal----------"<<endl;
	/*	int avgRenew = total of month apply from stud / total apply from stud
		A1 -> 1 month
		A2 -> 3 months
		A3 -> 2 months
		average = (1+3+2)/3 = 2
		
		cout<<"Average Renewal: "<<avgRenew<<endl;	*/
}
void total_app(){
	cout<<"----------Total Applications----------"<<endl;
	/*	int totalApp = new + renew
		if application.txt = 120 row
					total = 120
					
		cout<<"Total Applications: "<<totalApp<<endl;	*/
}
void utilizationRate(){
	cout<<"----------Car Park Utilization Rate----------"<<endl;
	/*	double uRate = total parking time / mix parking time
		if 1000 parking slots, 10.5hours/day, student total park 7000 hours
			mix = 1000 × 10.5 = 10500 hours
			Utilization = 7000 / 10500 = 66.7%
		
		cout<<"Car Park Utilization Rate: "<<uRate<<endl;	*/
}
void monthlyIncome(){
	cout<<"----------Monthly Income----------"<<endl;
	/*	int monthIn = total of payments.txt
		A1 -> RM50
		A2 -> RM50
		A3 -> RM30
		Total = RM130
		
		cout<<"Monthly Income: "<<monthIn<<endl;	*/
}
void growthRate(){
	cout<<"----------Growth Rate----------"<<endl;
	/*	double gRate = (this month - last month) / last month * 100%
		last month = RM1000
		this month = RM1200
		Growth = (1200-1000)/1000 × 100 = 20%
		
		cout<<"Growth Rate: "<<gRate<<endl;	*/
}

void fullReport(){
	cout<<"----------Full Analytics Reports----------"<<endl;
	averageRenewal();
    total_app();
    utilizationRate();
    monthlyIncome();
    growthRate();
    cout<<"=========================================="<<endl;
}


//========================================Payment Part========================================
void paymentModule(){
	cout << "\n[PAYMENT MODULE - NOT IMPLEMENTED YET]\n";
}

//========================================Tracking Part========================================
void trackingModule(){
	cout << "\n[TRACKING MODULE - NOT IMPLEMENTED YET]\n";
}
