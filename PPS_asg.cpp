#include <iostream>
#include <iomanip>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <ctime>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()

#define PRICE 80  //one month parking pass price
#define SLOTS 1100  //assume system capacity = 1100 parking slots

using namespace std;

// Home Page
void home_page();


// Struct
struct Student 
{
    string name, id, faculty, phone, vehicle, password;
};

struct Application 
{
    string appID, studentID, status, month, payment;
};

struct Pass
{
    string passID, studentID, startDate, endDate, status;
};

// Global arrays to store, loaded from txt.file
Student students[400];
Application applications[400];
Pass passes[400];

int studentCount = 0;
int applicationCount = 0;
int passCount = 0;

// Load from txt.file
void loadStudents()
{
    ifstream file("students.txt");
    if(!file) return;

    studentCount = 0;
    string line;

    while(getline(file, line)){
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

void loadApplications()
{
    ifstream file("applications.txt");
    if(!file) return;

    applicationCount = 0;
    string line;
    
    while(getline(file, line)){
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

void loadPasses()
{
    ifstream file("passes.txt");
    if(!file) return;

    passCount = 0;
    string line;

    while(getline(file, line)){
        stringstream ss(line);

        getline(ss, passes[passCount].passID, ',');
        getline(ss, passes[passCount].studentID, ',');
        getline(ss, passes[passCount].startDate, ',');
        getline(ss, passes[passCount].endDate, ',');
        getline(ss, passes[passCount].status, ',');

        passCount++;
    }

    file.close();
}

// Find index in array by matching ID
int findStud_Index(string id)
{
    for(int i = 0; i < studentCount; i++){
        if(students[i].id == id){
            return i;
        }
    }
    return -1;
}

int findApp_Index(string id)
{
    for(int i = 0; i < applicationCount; i++){
        if(applications[i].appID == id){
            return i;
        }
    }
    return -1;
}

int findPassIndex(string studentID)
{
    for(int i = 0; i < passCount; i++){
        if(passes[i].studentID == studentID){
            return i;
        }
    }
    return -1;
}


// ================= STATUS CONSTANTS =================
// Application Status
const string STATUS_PENDING = "Pending";
const string STATUS_APPROVED = "Approved";
const string STATUS_REJECTED = "Rejected";
// Payment Status
const string STATUS_PAID = "Paid";
const string STATUS_UNPAID = "Unpaid";
// Pass Status
const string STATUS_PENDINGPAY = "Pending Payment";
const string STATUS_ACTIVE = "Active";
const string STATUS_EXPIRED = "Expired";


// Year, Month, Day
bool parseAppMonth(const string& monthStr, int& outYear, int& outMonth);
string getMonthName(int month);
void getCurrentDate(int& day, int& month, int& year);
string trackFormatDate(int day, int month, int year);
int getDaysInMonth(int month, int year);

// Month names Display
const string MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};


//======================================== Student Module ========================================
void registerStudent();
void loginStudent();
void successLoginS(int index);
void studentMenu(int index, int studentIndex);

// Update personal details
void updateStudentDetails(int studentIndex);

// Apply New / Renew Parking Pass
void applyRenewPass(int index);
void submitApplication(int studentIndex);
void renewApplication(int studentIndex); 
string generateApplicationID();
bool isWithinThreeMonths(string month); 

// Parking Pass Details
void passDetails(int index, int studentIndex);
void viewApplication(int studentIndex);
void viewPass(int index);
bool checkRenewalAlert(int studentIndex);
void viewSpendingAnalysis(int studentIndex);

// Billing
void billMenu(int studentIndex);
void paymentMenu(int studentIndex);
bool checkApproval(string studentID);
string generatePaymentID();
string getCurrentDateTime();
string choosePaymentMethod();

// View Transaction
void transMenu(int studentIndex);
void viewAllTransactions(int studentIndex);
void viewTransactionsByYear(int studentIndex);
void viewTransactionsByMonth(int studentIndex);


//======================================== Admin Module ========================================
void admin_login();
void admin_page(int index);

// Application Validation
void view_stud(int index);
void view_app(int index);
void app_validation();
void approve_app(string targetAppID);
string validate_app(string studentID, string appID);

// Pass.txt
string generatePassID();
void createPass(string studentID, string startDate, int months);

// Summary Analytics Reports
void fullReport();
void averageApply();
void total_app();
void passUsageRate();
void monthlyIncome();



// Addition ========================================
//limited output after enter Alphabet
void limit_input(){
	if(cin.fail()){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
} 

//clear sreen after move to a new page
void clear_screen(){
	cin.get();
	#ifdef _WIN32
    system("cls");   // Windows
	#else
	    system("clear"); // Linux / macOS
	#endif
}

//loading screen for 2 second
void loading_screen(){
	cout<<"\nLoading";
    for(int i=0; i<2; i++){ //loading for 2 second
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



//=========================================================== Main Menu ===========================================================
int main()
{
	loadStudents();
    loadApplications();
    loadPasses();
	
	home_page();
	
	return 0;
}

void home_page()
{
	int homeChoice;

	do{
		cout<<"      MPKJ MONTHLY CAR PARKING PASS SYSTEM ^-^     "<<endl;
        cout<<"==================================================="<<endl;
        cout<<"|                    HOME PAGE                    |"<<endl;
        cout<<"==================================================="<<endl;
        cout<<"| 1. Student Registration                         |"<<endl;
        cout<<"| 2. Student Module                               |"<<endl;
        cout<<"| 3. Admin Module                                 |"<<endl;
        cout<<"| 4. Exit System                                  |"<<endl;
        cout<<"==================================================="<<endl;

        cout<<"Select an option: ";
		cin>>homeChoice;
		
		switch(homeChoice){
			case 1: registerStudent(); break;
			case 2: loginStudent(); break;
			case 3: admin_login(); break;
			case 4: cout<<"\n^-^ THANK YOU FOR USING CAR PARKING PASS SYSTEM! SEE YOU! ^-^"<<endl; break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-4."<<endl;
		}
	}while(homeChoice != 4);
}

// Year, Month, Day ========================================
bool parseAppMonth(const string& monthStr, int& outYear, int& outMonth) // Extract year and month from application month string "YYYY-MM"
{
    if (monthStr.length() != 7 || monthStr[4] != '-') return false;
    stringstream ss(monthStr);
    char dash;
    ss >> outYear >> dash >> outMonth;
    return (outMonth >= 1 && outMonth <= 12); // Returns true if valid, fills outYear and outMonth
}

string getMonthName(int month) // Safe month name helper
{
    if (month >= 1 && month <= 12) return MONTH_NAMES[month - 1];
    return "Unknown";
}

void getCurrentDate(int& day, int& month, int& year) // Get current system date
{
    time_t now = time(0);
    tm* timeInfo = localtime(&now);

    day = timeInfo->tm_mday;
    month = timeInfo->tm_mon + 1;
    year = timeInfo->tm_year + 1900;
}

string trackFormatDate(int day, int month, int year) // Format date as DD/MM/YYYY
{
    ostringstream oss;
    oss << setfill('0') << setw(2) << day << "/"
        << setfill('0') << setw(2) << month << "/" << year;
    return oss.str();
}

int getDaysInMonth(int month, int year) // Get days in a month
{
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
    case 4: case 6: case 9: case 11: return 30;
    case 2: return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ? 29 : 28;
    default: return 30;
    }
}


//=========================================================== Student Module ===========================================================
// Register student
void registerStudent()
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
    Student s;
    string input;
    
    cout<<"\n==================================================="<<endl;
    cout<<"|             STUDENT REGISTRATION                |"<<endl;
    cout<<"==================================================="<<endl;

    cout<<"Enter full name (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0"){
			cout<<endl;
			return;  //return = return to the last function
		}
    s.name = input;

    while(true){
        cout<<"Enter student ID (PRESS 0 to return): ";
        getline(cin, input);
        if(input == "0"){
			cout<<endl;
			return;
		}

        bool duplicate = false;
        
        for(int i = 0; i < studentCount; i++){
            if(students[i].id == input){
                duplicate = true;
                cout<<"\nTHIS STUDENT ID ALREADY EXIXTS!"<<endl;
                break;
            }
        }
        if(!duplicate) break;
    }
    s.id = input;

    cout<<"Enter faculty (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0"){
			cout<<endl;
			return;
		}
    s.faculty = input;

    cout<<"Enter phone number (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0"){
			cout<<endl;
			return;
		}
    s.phone = input;

    cout<<"Enter vehicle number (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0"){
			cout<<endl;
			return;
		}
    s.vehicle = input;

    cout<<"Set password (PRESS 0 to return): ";
    getline(cin, input);
    if(input == "0"){
			cout<<endl;
			return;
		}
    s.password = input;

    students[studentCount] = s;
    studentCount++;

    ofstream file("students.txt", ios::app);
    file<< s.name << "," << s.id << "," << s.faculty << ","
        << s.phone << "," << s.vehicle << "," << s.password <<endl;
    file.close();

    cout<<"\nREGISTERED SUCCESSFULLY!"<<endl<<endl;
}
 
// Login student
void loginStudent()
{
    string id, pw;
    int index = -1;
    int studentIndex = index;
    
    cout<<"\n==================================================="<<endl;
    cout<<"|                 STUDENT LOGIN                   |"<<endl;
    cout<<"==================================================="<<endl;

    while(true)
    {
        cout<<"Student ID (PRESS 0 to return): ";
        cin>>id;
        if(id == "0"){
			cout<<endl;
			return;
		}

        cout<<"Password (PRESS 0 to return): ";
        cin>>pw;
        if(pw == "0"){
			cout<<endl;
			return;
		}

        bool idFound = false;

        for(int i = 0; i < studentCount; i++){
            if(students[i].id == id){
                idFound = true;

                if(students[i].password == pw){
                    index = i;
                    break;
                }
                else{
                    cout<<"\nERROR: Incorrect password!"<<endl<<endl;
                    break;
                }
            }
        }
        if(index != -1) break;

        if(!idFound){
            cout << "\nERROR: Student ID not found!\n";
        }
    }
    loading_screen(); 
    clear_screen();
    studentMenu(index, index); 
}

// Student personal details
void successLoginS(int index)
{
	// success login 
    cout<<"LOGIN SUCCESSFUL!"<<endl;
    cout<<"~ WELCOME "<< students[index].name <<" ~ ^-^"<<endl;

    // success login message
    cout<<"================================================"<<endl;
	cout<<"|               STUDENT DETAILS                |"<<endl;
	cout<<"================================================"<<endl;
	cout<<"| Name         : "<< students[index].name << string(30-students[index].name.length(), ' ') <<"|"<<endl;
	cout<<"| Student ID   : "<< students[index].id << string(30-students[index].id.length(), ' ') <<"|"<<endl;
	cout<<"| Faculty      : "<< students[index].faculty << string(30-students[index].faculty.length(), ' ') <<"|"<<endl;
	cout<<"| Phone Number : "<< students[index].phone << string(30-students[index].phone.length(), ' ') <<"|"<<endl;
	cout<<"| Vehicle No.  : "<< students[index].vehicle << string(30-students[index].vehicle.length(), ' ') <<"|"<<endl;
	cout<<"================================================\n"<<endl;
}

// Student Menu
void studentMenu(int index, int studentIndex)
{
	successLoginS(index);
	int studchoice;
	
    do{
        cout<<"=============== STUDENT DASHBOARD ================="<<endl;
        cout<<"| 1. Update Personal Details                      |"<<endl;
        cout<<"| 2. Apply New / Renew Parking Pass               |"<<endl;
        cout<<"| 3. Parking Pass Details                         |"<<endl;
        cout<<"| 4. Billing                                      |"<<endl;
        cout<<"| 5. View Transaction                             |"<<endl;
        cout<<"| 6. Back to Home Page                            |"<<endl;
        cout<<"==================================================="<<endl;

        cout<<"Select an option: ";
        cin>>studchoice;

        switch(studchoice){
        	case 1: updateStudentDetails(index); break;
            case 2: loading_screen(); clear_screen(); applyRenewPass(index); break;
            case 3: loading_screen(); clear_screen(); passDetails(index, studentIndex); break;
            case 4: billMenu(studentIndex); break;
            case 5: loading_screen(); clear_screen(); transMenu(studentIndex); break;
            case 6: cout<<"\nReturn to Home Page...\n"; loading_screen(); clear_screen(); return;
            default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-6."<<endl; 
        }
    }while(studchoice != 6);
}

//======================================== Update personal details ========================================
void updateStudentDetails(int index)
{
	int studDchoice;
    bool updated = false;
    string temp;

    do{
        cout<<"\n================================================"<<endl;
		cout<<"|            UPDATE PERSONAL DETAILS           |"<<endl;
		cout<<"================================================"<<endl;
		cout<<"| 1. Full Name   : " << students[index].name << string(28-students[index].name.length(), ' ') << "|"<<endl;
		cout<<"| 2. Faculty     : " << students[index].faculty << string(28-students[index].faculty.length(), ' ') << "|"<<endl;
		cout<<"| 3. Phone No.   : " << students[index].phone << string(28-students[index].phone.length(), ' ') << "|"<<endl;
		cout<<"| 4. Vehicle No. : " << students[index].vehicle << string(28-students[index].vehicle.length(), ' ') << "|"<<endl;
		cout<<"|----------------------------------------------|"<<endl;
		cout<<"| 0. Return to Student Dashboard               |"<<endl;
		cout<<"================================================"<<endl;

		cout<<"Select an option: ";
		cin>>studDchoice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	
		switch(studDchoice){
			case 1: cout<<"\nEnter new Full Name (or 0 to cancel): ";
		            getline(cin, temp);
		            if(temp != "0"){
		                students[index].name = temp;
		                updated = true;
		            }; break;
			case 2: cout << "\nEnter new Faculty (or 0 to cancel): ";
		            getline(cin, temp);
		            if(temp != "0")
		            {
		                students[index].faculty = temp;
		                updated = true;
		            }; break;
			case 3: cout << "\nEnter new Phone Number (or 0 to cancel): ";
		            getline(cin, temp);
		            if(temp != "0")
		            {
		                students[index].phone = temp;
		                updated = true;
		            }; break;
			case 4: cout << "\nEnter new Vehicle No (or 0 to cancel): ";
		            getline(cin, temp);
		            if(temp != "0")
		            {
		                students[index].vehicle = temp;
		                updated = true;
		            }; break; 
			case 0: cout<<endl; return;
			default: limit_input(); cout<<"Invalid Option! Please Enter the Number 0-4."<<endl;
		}

        // save file
        if(updated){
            ofstream file("students.txt");

            for(int i = 0; i < studentCount; i++){
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
    }while(studDchoice != 0);
}


//======================================== Apply New / Renew Parking Pass ========================================
// Apply New / Renew Parking Pass Menu
void applyRenewPass(int index)
{
	int aRpassChoice;
	
	do{
        cout<<"========================================="<<endl;
        cout<<"|         APPLY NEW / RENEW PASS        |"<<endl;
        cout<<"========================================="<<endl;
        cout<<"| 1. Apply New Parking Pass             |"<<endl;
        cout<<"| 2. Renew Parking Pass                 |"<<endl;
        cout<<"| 3. Back to Student Dashboard          |"<<endl;
        cout<<"========================================="<<endl;

        cout<<"Select an option: ";
        cin>>aRpassChoice;

        switch(aRpassChoice){
        	case 1: submitApplication(index); break;
            case 2: renewApplication(index); break;
            case 3: cout<<"\nReturn to Student Dashboard...\n"; loading_screen(); clear_screen(); return;
            default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-3."<<endl; 
        }
    }while(aRpassChoice != 3);
}

// Apply New Pass
void submitApplication(int index)
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string month;

    while(true)
    {
        cout<<"\n==================================================="<<endl;
        cout<<"|                 APPLY NEW PASS                  |"<<endl;
        cout<<"==================================================="<<endl;
        cout<<"Enter month (YYYY-MM or 0 to return): ";

        getline(cin, month);
        if(month == "0"){
			cout<<endl;
			return;
		}

        if(month.length() != 7 || month[4] != '-'){
            cout<<"\n[ERROR] Invalid format! Use YYYY-MM."<<endl;
            continue;
        }

        int inYear, inMonth;
        char dash;
        stringstream ss(month);
        ss >> inYear >> dash >> inMonth;

        if(inMonth < 1 || inMonth > 12){
            cout<<"\n[ERROR] Invalid month value!"<<endl;
            continue;
        }

        time_t now = time(0);
        tm *ltm = localtime(&now);

        int curYear = 1900 + ltm->tm_year;
        int curMonth = 1 + ltm->tm_mon;

        int curTotal = curYear * 12 + curMonth;
        int inTotal = inYear * 12 + inMonth;

        if(inTotal < curTotal){
            cout << "\n[ERROR] Cannot select past month!\n";
            continue;
        }

        if(inTotal - curTotal > 3){
            cout << "\n[ERROR] Cannot exceed 3 months!\n";
            continue;
        }

        bool duplicate = false;

        for(int i = 0; i < applicationCount; i++){
            if(applications[i].studentID == students[index].id && applications[i].month == month){
                duplicate = true;
                break;
            }
        }

        if(duplicate){
            cout<<"\n[ERROR] Already applied for this month!\n";
            continue;
        }
        break;
    }

    Application a;
    a.appID = generateApplicationID();
    a.studentID = students[index].id;
    a.status = STATUS_PENDING;
    a.month = month;
    a.payment = STATUS_UNPAID;

    applications[applicationCount++] = a;

    ofstream file("applications.txt", ios::app);
    file<< a.appID << "," << a.studentID << "," 
		<< a.status << "," << a.month << "," << a.payment <<endl;
    file.close();

    cout<<"\nAPPLICATION SUCCESSFUL!"<<endl;
    cout<<"Your Application ID : "<< a.appID <<endl<<endl;
}

// Renew Pass
void renewApplication(int index)
{
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	ifstream in_file("applications.txt");
    bool found = false;

    cout<<"\n==========================================================="<<endl;
    cout<<"|                 EXISTING APPLICATIONS                   |"<<endl;
    cout<<"==========================================================="<<endl;

    int list[400];
    int count = 0;

    for(int i = 0; i < applicationCount; i++){
        if(applications[i].studentID == students[index].id){
        	cout<<"| "<<left
				<< setw(15) <<"Index number"
				<< setw(20) <<"Application ID"
				<< setw(10) <<"Month"
				<< setw(10) <<"Status" <<" |"<<endl;
			cout<<"|---------------------------------------------------------|"<<endl;
	
          	cout<< "| " << left
          		<< setw(15) << count
	            << setw(20) << applications[i].appID
	            << setw(10) << applications[i].month
	            << setw(10) << applications[i].status <<" |"<<endl;
	        cout<<"==========================================================="<<endl;

            list[count] = i;
            count++;
            found = true;
        }
    }
    
    if(!found){
		cout<<"|      --NO APPLICATION FOUND! PLEASE APPLY FIRST--       |"<<endl;
    	cout<<"===========================================================\n"<<endl;
    	return;
	}

    int choice;

    while(true){
        cout << "Select index (press -1 to cancel): ";

        string input;
        getline(cin, input);

        stringstream ss(input);
        if(!(ss >> choice)){
            cout << "Invalid input format!\n";
            continue;
        }

        if(choice == -1){
            cout << "Cancelled\n\n";
            return;
        }

        if(choice >= 0 && choice < count) break;
        cout << "INVALID SELECTION!\n";
    }

    int realIndex = list[choice];
    string month;

    while(true){
        cout << "\nEnter new month (YYYY-MM or 0 to cancel): ";

        getline(cin, month);
        if(month == "0") return;

        if(month.length() != 7 || month[4] != '-'){
            cout << "[ERROR] Invalid format!\n";
            continue;
        }

        int inYear, inMonth;
        char dash;
        stringstream ss(month);
        ss >> inYear >> dash >> inMonth;

        if(inMonth < 1 || inMonth > 12){
            cout << "[ERROR] Invalid month!\n";
            continue;
        }

        time_t now = time(0);
        tm *ltm = localtime(&now);

        int curYear = 1900 + ltm->tm_year;
        int curMonth = 1 + ltm->tm_mon;

        int curTotal = curYear * 12 + curMonth;
        int inTotal = inYear * 12 + inMonth;

        if(inTotal < curTotal){
            cout << "[ERROR] Cannot select past month!\n";
            continue;
        }

        if(inTotal - curTotal > 3){
            cout << "[ERROR] Cannot exceed 3 months!\n";
            continue;
        }

        bool duplicate = false;

        for(int i = 0; i < applicationCount; i++){
            if(applications[i].studentID == students[index].id &&
               applications[i].month == month){
                duplicate = true;
                break;
            }
        }

        if(duplicate){
            cout << "[ERROR] Already applied!\n";
            continue;
        }
        break;
    }

    applications[realIndex].month = month;
    applications[realIndex].status = STATUS_PENDING;
	applications[realIndex].payment = STATUS_UNPAID;

    ofstream file("applications.txt");

    for(int i = 0; i < applicationCount; i++){
        file << applications[i].appID << ","
             << applications[i].studentID << ","
             << applications[i].status << ","
             << applications[i].month << ","
             << applications[i].payment << endl;
    }
    file.close();
    cout << "RENEW SUCCESSFUL!\n\n";
}

// Generate Application ID
string generateApplicationID()
{
    ifstream file("applications.txt");
    string line, lastID = "A1000";

    while(getline(file, line)){
        stringstream ss(line);
        getline(ss, lastID, ',');
    }

    int num = stringToInt(lastID.substr(1));
    return "A" + intToString(num + 1);
}

// Validation
bool isWithinThreeMonths(string month)
{
    if(month.length() != 7) return false;
    if(month[4] != '-') return false;

    for(int i = 0; i < 7; i++){
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


//======================================== Parking Pass Details ========================================
// Parking Pass Details Menu
void passDetails(int index, int studentIndex)
{
	int passDChoice;
	
	do{
        cout<<"========================================="<<endl;
        cout<<"|          PARKING PASS DETAILS         |"<<endl;
        cout<<"========================================="<<endl;
        cout<<"| 1. View Application Status            |"<<endl;
        cout<<"| 2. View Parking Pass                  |"<<endl;
        cout<<"| 3. Check Renewal Alert                |"<<endl;
        cout<<"| 4. Spending Analysis & Statistics     |"<<endl;
        cout<<"| 5. Back to Student Dashboard          |"<<endl;
        cout<<"========================================="<<endl;

        cout<<"Select an option: ";
        cin>>passDChoice;

        switch(passDChoice){
        	case 1: viewApplication(index); break;
            case 2: viewPass(index); break;
            case 3: if (!checkRenewalAlert(studentIndex)){
		                int d, m, y;
		                getCurrentDate(d, m, y);
		                int daysLeft = getDaysInMonth(m, y) - d;
		                cout << "\nNo renewal alert at this time.\n";
		                cout << "There are " << daysLeft << " days remaining in " << getMonthName(m) << ".\n";
		                cout << "Alerts appear in the last 5 days of the month." << endl << endl;
		            }
		            break;
            case 4: viewSpendingAnalysis(studentIndex); break;
            case 5: cout<<"\nReturn to Student Dashboard...\n"; loading_screen(); clear_screen(); return;
            default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-5."<<endl; 
        }
    }while(passDChoice != 5);
}

// View Application Status
void viewApplication(int index)
{
    cout<<"\n========================================"<<endl;
	cout<<"|          APPLICATION STATUS          |"<<endl;
	cout<<"========================================"<<endl;

	bool found = false;

	for(int i = 0; i < applicationCount; i++){
    	if(applications[i].studentID == students[index].id){
        	cout << "| Application ID : " << applications[i].appID << string(20 - applications[i].appID.length(), ' ') << "|\n";
        	cout << "| Month          : " << applications[i].month << string(20 - applications[i].month.length(), ' ') << "|\n";
        	cout << "| Status         : " << applications[i].status << string(20 - applications[i].status.length(), ' ') << "|\n";
        	cout << "| Payment        : " << applications[i].payment << string(20 - applications[i].payment.length(), ' ') << "|\n";
        	cout<<"========================================\n"<<endl;
        	found = true;
    	}
	}

	if(!found){
    	cout<<"|      --NO APPLICATION FOUND!--       |"<<endl;
    	cout<<"========================================\n"<<endl;
	}
}

// View Parking Pass
void viewPass(int index)
{
    bool found = false;
    
    cout<<"\n========================================"<<endl;
	cout<<"|             PARKING PASS             |"<<endl;
	cout<<"========================================"<<endl;

	for(int i = 0; i < passCount; i++){
        if(passes[i].studentID == students[index].id){
        	cout << "| Pass ID   : " << passes[i].passID << string(30-passes[i].passID.length(), ' ') << "|\n";
        	cout << "| Start Date: " << passes[i].startDate << string(30-passes[i].startDate.length(), ' ') << "|\n";
        	cout << "| End Date  : " << passes[i].endDate << string(30-passes[i].endDate.length(), ' ') << "|\n";
        	cout << "| Status    : " << passes[i].status << string(30-passes[i].status.length(), ' ') << "|\n";
        	cout<<"========================================\n"<<endl;
        	found = true;
    	}
	}

	if(!found){
    	cout<<"|           --UNPAID PASS--            |"<<endl;
    	cout<<"========================================\n"<<endl;
	}
}

// Check Renewal Alert
bool checkRenewalAlert(int studentIndex)
{
    int currentDay, currentMonth, currentYear;
    getCurrentDate(currentDay, currentMonth, currentYear);

    int daysInMonth = getDaysInMonth(currentMonth, currentYear);
    int daysRemaining = daysInMonth - currentDay;

    if (daysRemaining > 5) return false;

    string sid = students[studentIndex].id;

    // Determine next month
    int nextMonth = currentMonth + 1;
    int nextYear = currentYear;
    if (nextMonth > 12) { nextMonth = 1; nextYear++; }

    // Build next month key "YYYY-MM"
    string nextMonthKey = intToString(nextYear) + "-" + (nextMonth < 10 ? "0" : "") + intToString(nextMonth);

    // Check if student already has an application for next month
    bool hasApprovedNext = false;
    bool hasPendingNext = false;
    bool hasAnyNext = false;

    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid && applications[i].month == nextMonthKey) {
            hasAnyNext = true;
            if (applications[i].status == STATUS_APPROVED) hasApprovedNext = true;
            else if (applications[i].status == STATUS_PENDING) hasPendingNext = true;
            break;
        }
    }

    // Check if student has an active pass covering next month
    bool hasActivePass = false;
    for (int i = 0; i < passCount; i++) {
        if (passes[i].studentID == sid && passes[i].status == "Active") {
            // Parse end date "YYYY-MM-DD"
            int eYear = 0, eMonth = 0;
            if (passes[i].endDate.length() >= 7) {
                eYear = stringToInt(passes[i].endDate.substr(0, 4));
                eMonth = stringToInt(passes[i].endDate.substr(5, 2));
            }
            // If pass end date is in or after next month, covered
            if (eYear > nextYear || (eYear == nextYear && eMonth >= nextMonth)) {
                hasActivePass = true;
                break;
            }
        }
    }

    if (hasActivePass) return false; // Already covered

    // Display alert
    cout << endl;
    cout << "\n===========================================================" << endl;
    cout << "|          MONTHLY PARKING PASS RENEWAL REMINDER          |" << endl;
    cout << "===========================================================" << endl;

    cout << "Dear " << students[studentIndex].name << ",\n\n";
    cout << "Today's date     : " << trackFormatDate(currentDay, currentMonth, currentYear) << endl;
    cout << "Days remaining   : " << daysRemaining << " day(s) left in " << getMonthName(currentMonth) << endl;
    cout << "Next month       : " << getMonthName(nextMonth) << " " << nextYear << endl;
    cout << endl;

    if (hasApprovedNext) {
        cout << "STATUS: Your application for " << getMonthName(nextMonth) << " has been APPROVED!\n";
        cout << "ACTION: Please complete your PAYMENT to activate the pass.\n";
    }
    else if (hasPendingNext) {
        cout << "STATUS: Your application for " << getMonthName(nextMonth) << " is still PENDING review.\n";
        cout << "ACTION: Please wait for admin approval. Check back soon.\n";
    }
    else {
        cout << "STATUS: You have NO application for " << getMonthName(nextMonth) << " " << nextYear << ".\n";
        cout << "ACTION: Please APPLY or RENEW your monthly parking pass\n";
        cout << "        before the end of this month to avoid disruption!\n";
    }

    cout << endl << endl;
    return true;
}

// Spending Analysis & Statistics
void viewSpendingAnalysis(int studentIndex)
{
    cout << "\n===================================================" << endl;
    cout << "|        MY SPENDING ANALYSIS & STATISTICS        |" << endl;
    cout << "===================================================" << endl;

    string sid = students[studentIndex].id;
    cout << "  Student : " << students[studentIndex].name << " (" << sid << ")\n";
    cout << "  Faculty : " << students[studentIndex].faculty << endl;

    // Count applications by status
    int totalApps = 0, approved = 0, rejected = 0, pending = 0;
    int paidCount = 0;

    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid) {
            totalApps++;
            if (applications[i].status == STATUS_APPROVED) approved++;
            else if (applications[i].status == STATUS_REJECTED) rejected++;
            else pending++;
            if (applications[i].payment == STATUS_PAID) paidCount++;
        }
    }

    // Count passes
    int activePass = 0, expiredPass = 0, pendingPayPass = 0;
    for (int i = 0; i < passCount; i++) {
        if (passes[i].studentID == sid) {
            if (passes[i].status == STATUS_ACTIVE) activePass++;
            else if (passes[i].status == STATUS_EXPIRED) expiredPass++;
            else if (passes[i].status == STATUS_PENDINGPAY) pendingPayPass++;
        }
    }

    if (totalApps == 0) {
        cout << "\n  No records found for analysis.\n";
        return;
    }

    cout << "|-------------------------------------------------|" << endl;
    cout << "\n  [A] APPLICATION SUMMARY\n\n";
    cout << "  Total Applications  : " << totalApps << endl;
    cout << "  Approved            : " << approved << endl;
    cout << "  Rejected            : " << rejected << endl;
    cout << "  Pending             : " << pending << endl;
    if (totalApps > 0)
        cout << "  Approval Rate       : " << fixed << setprecision(1)
        << ((double)approved / totalApps * 100.0) << "%" << endl;
    cout << "  Paid Applications   : " << paidCount << endl;
    cout << "\n|-------------------------------------------------|\n";
    cout << "\n  [B] PASS SUMMARY\n\n";
    cout << "  Active Passes       : " << activePass << endl;
    cout << "  Expired Passes      : " << expiredPass << endl;
    cout << "  Pending Payment     : " << pendingPayPass << endl;

    // Monthly calendar for current year
    int d, m, y;
    getCurrentDate(d, m, y);
    cout << "\n|-------------------------------------------------|\n";
    cout << "\n  [C] MONTHLY PASS CALENDAR FOR " << y << "\n\n";

    for (int mon = 1; mon <= 12; mon++) {
        string monthKey = intToString(y) + "-" + (mon < 10 ? "0" : "") + intToString(mon);

        bool hasApp = false;
        string appStatus = "";
        for (int i = 0; i < applicationCount; i++) {
            if (applications[i].studentID == sid && applications[i].month == monthKey) {
                hasApp = true;
                appStatus = applications[i].status + "/" + applications[i].payment;
                break;
            }
        }

        cout << "  " << left << setw(12) << getMonthName(mon);
        if (hasApp) {
            cout << "[APPLIED] " << appStatus;
        }
        else {
            cout << "[  ---  ] No application";
        }
        cout << endl;
    }
    cout << "===================================================" << endl << endl;
}


//======================================== Billing ========================================
// Billing Menu
void billMenu(int studentIndex)
{
    cout<<"====================================="<<endl;
    cout<<"|              BILLING              |"<<endl;
    cout<<"====================================="<<endl;
    
    paymentMenu(studentIndex);

}

// Payment Menu
void paymentMenu(int studentIndex)
{
    string sid = students[studentIndex].id;

    cout << "\n========================================\n";
    cout << "|               PAYMENT                |\n";
    cout << "========================================\n";

    bool found = false;

    for(int i = 0; i < applicationCount; i++){
        if(applications[i].studentID == sid){
            found = true;

            // ===== SHOW APPLICATION =====
            cout << "| Application ID : " << applications[i].appID << endl;
            cout << "| Month          : " << applications[i].month << endl;
            cout << "| Status         : " << applications[i].status << endl;
            cout << "| Payment        : " << applications[i].payment << endl;
            cout << "========================================\n";

            // ===== CASE 1: PENDING =====
            if(applications[i].status == STATUS_PENDING){
                
				cout << "Your application is still processing.\n";
                cout << "Please wait 3-5 working days.\n";
                return;
            }

            // ===== CASE 2: REJECTED =====
            if(applications[i].status == STATUS_REJECTED){
                
				cout << "Your application was rejected.\n";
                return;
            }

            // ===== CASE 3: APPROVED + UNPAID =====
            if(applications[i].status == STATUS_APPROVED && applications[i].payment != STATUS_PAID){
            	
				string month;

                // ===== INPUT MONTH =====
                while(true){
                    cout << "\nEnter the month you request for pass (YYYY-MM or 0 to cancel): ";
                    cin >> month;

                    if(month == "0") return;

                    if(month.length() == 7 && month[4] == '-'){
						int m = stringToInt(month.substr(5,2));

                        if(m >= 1 && m <= 12){
                            break; // valid
                        }
                    }
                    cout << "Invalid month format! Try again.\n";
                }

                // ===== PAYMENT METHOD =====
                string method;
                while(true){
                    method = choosePaymentMethod();

                    if(method != "Invalid") break;
                    cout << "Invalid method! Please choose again.\n";
                }

                // ===== PAYMENT PROCESS =====
                double amount = 40.0;
                string pid = generatePaymentID();
                string dateTime = getCurrentDateTime();

                applications[i].payment = STATUS_PAID;
                
                // Activate pass after payment
				for(int p = 0; p < passCount; p++){
				    if(passes[p].studentID == sid && passes[p].status == STATUS_PENDINGPAY){
					    passes[p].status = STATUS_ACTIVE;
					}
				}
				
				ofstream passFile("passes.txt");

				for(int p = 0; p < passCount; p++){
				    passFile << passes[p].passID << ","
				             << passes[p].studentID << ","
				             << passes[p].startDate << ","
				             << passes[p].endDate << ","
				             << passes[p].status << endl;
				}
				passFile.close();
                
                // ===== DAILY COMPARISON (8–10 HOURS) =====
                int minH = 8, maxH = 10;

                double minDaily = minH * 0.5 * 30;
                double maxDaily = maxH * 0.5 * 30;

                double minSave = minDaily - amount;
                double maxSave = maxDaily - amount;

                // ===== SAVE APPLICATION =====
                ofstream file("applications.txt");

                for(int j = 0; j < applicationCount; j++)
                {
                    file << applications[j].appID << ","
                         << applications[j].studentID << ","
                         << applications[j].status << ","
                         << applications[j].month << ","
                         << applications[j].payment << endl;
                }
                file.close();

                // ===== SAVE PAYMENT RECORD =====
                ofstream payFile("payments.txt", ios::app);

                payFile << pid << ","
                        << sid << ","
                        << method << ","
                        << month << ","
                        << amount << ","
                        << dateTime << endl;

                payFile.close();

                // ===== RECEIPT =====
                cout<<"\n================================================="<<endl;
                cout<<"|                     RECEIPT                   |"<< endl;
                cout<<"================================================="<<endl;
                cout<<"| Payment ID   : " << left << setw(15) << pid << "|" << endl;
	            cout<<"| Student ID   : " << left << setw(15) << sid << "|" << endl;
	            cout<<"| Months       : " << left << setw(15) << month << "|" << endl;
	            cout<<"| Amount Paid  : RM " << left << setw(10) << amount << "|" << endl;
	            cout<<"| Method       : " << left << setw(15) << method << "|" << endl;
	            cout<<"| Date & Time  : " << left << setw(15) << dateTime << "|" << endl;
                cout<<"|" << setw(30) << "|" << endl;
                cout<<"|--------------- Cost Comparison ---------------|" <<endl;
                cout<<"| Daily (6 to 9 hours): RM" << minDaily << " - RM" << maxDaily << endl;
                cout<<"| You saved           : RM" << minSave << " - RM" << maxSave << endl;
                cout<<"================================================="<<endl;

                return;
            }

            // ===== CASE 4: ALREADY PAID =====
            if(applications[i].payment == STATUS_PAID){
                cout << "Payment already completed.\n";
                return;
            }
        }
    }
    cout<<endl;

    if(!found)
    {
        cout << "|        NO APPLICATION FOUND!         |\n";
        cout << "========================================\n\n";
    }
}

// Generate Payment ID
string generatePaymentID()
{
    ifstream file("payments.txt");
    string line, lastID = "PID1000";
 
    while(getline(file, line)){
        stringstream ss(line);
        getline(ss, lastID, ',');
    }

    int num = stringToInt(lastID.substr(3));
    return "PID" + intToString(num + 1);
}

// Get current date & time
string getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << (1 + ltm->tm_mon) << "-"
       << ltm->tm_mday << " "
       << ltm->tm_hour << ":"
       << ltm->tm_min;

    return ss.str();
}

// Check if student has APPROVED application
bool checkApproval(string studentID)
{
    for(int i = 0; i < applicationCount; i++){
        if(applications[i].studentID == studentID &&
           applications[i].status == STATUS_APPROVED &&
           applications[i].payment != STATUS_PAID){
            return true;
        }
    }
    return false;
}

// Payment Choice
string choosePaymentMethod()
{
    int opt;
    
    while(true){
        cout<<"---------- Payment Choice ----------"<<endl;
        cout<<"| 1. TNG QR                        |"<<endl;
        cout<<"| 2. DuitNow QR                    |"<<endl;
        cout<<"| 3. Credit / Debit Card           |"<<endl;
        cout<<"| 4. International Card            |"<<endl;
        cout<<"------------------------------------"<<endl;
        
        cout << "Enter choice: ";
        cin >> opt;

	    switch(opt){
	        case 1: return "TNG QR";
	        case 2: return "DuitNow QR";
	        case 3: return "Card";
	        case 4: return "International Card";
	        default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-4."<<endl;
	    }
    }   
}


//======================================== View Transaction ========================================
// View Transaction Menu
void transMenu(int studentIndex)
{
	int transChoice;
	
	do{
        cout<<"======================================="<<endl;
        cout<<"|          VIEW TRANSACTION           |"<<endl;
        cout<<"======================================="<<endl;
        cout<<"| 1. View Transaction Historys        |"<<endl;
        cout<<"| 2. View Transactions by Year        |"<<endl;
        cout<<"| 3. View Transactions by Month       |"<<endl;
        cout<<"| 4. Back to Student Dashboard        |"<<endl;
        cout<<"======================================="<<endl;

        cout<<"Select an option: ";
        cin>>transChoice;

        switch(transChoice){
        	case 1: viewAllTransactions(studentIndex); break;
            case 2: viewTransactionsByYear(studentIndex); break;
            case 3: viewTransactionsByMonth(studentIndex); break;
            case 4: cout<<"\nReturn to Student Dashboard...\n"; loading_screen(); clear_screen(); return;
            default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-4."<<endl; 
        }
    }while(transChoice != 4);
}

// View Transaction Historys 
void viewAllTransactions(int studentIndex)
{
    // In PPS, "transactions" = passes with status "Active" or paid.
    // We use passes[] array and applications[] to show the student's history.
    cout << "\n==========================================================" << endl;
    cout << "|                 MY TRANSACTION HISTORY                 |" << endl;
    cout << "==========================================================" << endl;

    string sid = students[studentIndex].id;
    cout << "| Student Name : " << students[studentIndex].name << string(40 - students[studentIndex].name.length(), ' ') << "|" << endl;
    cout << "| Student ID   : " << students[studentIndex].id << string(40 - students[studentIndex].id.length(), ' ') << "|" << endl;
    cout << "| Faculty      : " << students[studentIndex].faculty << string(40 - students[studentIndex].faculty.length(), ' ') << "|" << endl;
    cout << "| Vehicle      : " << students[studentIndex].vehicle << string(40 - students[studentIndex].vehicle.length(), ' ') << "|" << endl;
    cout << "==========================================================" << endl;

    // Application Table
    cout << "| " << left
        << setw(10) << "App ID"
        << setw(14) << "Month"
        << setw(14) << "Status"
        << setw(16) << "Payment" << " |" << endl;
    cout << "|--------------------------------------------------------|" << endl;

    int count = 0;

    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid) {
            cout << "| " << left
                << setw(10) << applications[i].appID
                << setw(14) << applications[i].month
                << setw(14) << applications[i].status
                << setw(16) << applications[i].payment << " |" << endl;
            count++;
        }
    }

    if (count == 0) {
        cout << "\nNo transaction records found.\n";
    }
    else {
        cout << "==========================================================" << endl;
        cout << "| Total Records: " << count <<"\t\t\t\t\t |"<< endl;
    }

    // Pass Section
    bool hasPass = false;

    for (int i = 0; i < passCount; i++) {
        if (passes[i].studentID == sid) {
            if (!hasPass) {
                cout << "\n|------------------- Parking Passes ---------------------|" << endl;

                cout << "| " << left
                    << setw(10) << "Pass ID"
                    << setw(14) << "Start Date"
                    << setw(14) << "End Date"
                    << setw(16) << "Status" << " |" << endl;

                cout << "|--------------------------------------------------------|" << endl;

                hasPass = true;
            }

            cout << "| " << left
                << setw(10) << passes[i].passID
                << setw(14) << passes[i].startDate
                << setw(14) << passes[i].endDate
                << setw(16) << passes[i].status << " |" << endl;
        }
    }
    cout << "==========================================================" << endl << endl;
}

// View Transactions by Year
void viewTransactionsByYear(int studentIndex)
{
    cout << "\n===================================================" << endl;
    cout << "|           TRANSACTION HISTORY BY YEAR           |" << endl;
    cout << "===================================================" << endl;

    string sid = students[studentIndex].id;

    // Collect unique years from application months
    const int MAX_YEARS = 50;
    int years[MAX_YEARS];
    for (int i = 0; i < MAX_YEARS; i++) years[i] = 0;
    int yearCount = 0;

    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid) {
            int yr, mn;
            if (parseAppMonth(applications[i].month, yr, mn)) {
                bool exists = false;
                for (int j = 0; j < yearCount; j++) {
                    if (years[j] == yr) { exists = true; break; }
                }
                if (!exists && yearCount < MAX_YEARS) {
                    years[yearCount++] = yr;
                }
            }
        }
    }

    if (yearCount == 0) {
        cout << "No transaction records found.\n";
        return;
    }

    // Sort ascending
    for (int i = 0; i < yearCount - 1; i++)
        for (int j = 0; j < yearCount - i - 1; j++)
            if (years[j] > years[j + 1]) { int t = years[j]; years[j] = years[j + 1]; years[j + 1] = t; }

    cout << "Available years: ";
    for (int i = 0; i < yearCount; i++) {
        cout << years[i];
        if (i < yearCount - 1) cout << ", ";
    }
    cout << endl;

    int targetYear;
    cout << "\nEnter year to view: ";
    cin >> targetYear;

    bool validYear = false;
    for (int i = 0; i < yearCount; i++)
        if (years[i] == targetYear) { validYear = true; break; }

    if (!validYear) {
        cout << "No transactions found for year " << targetYear << "." << endl << endl;
        return;
    }
    
    cout << "=================================================" ;
    cout << "\n|\t   Transactions for Year " << targetYear << "\t\t|\n";
    cout << "=================================================" << endl;
    cout << "|  " << left
        << setw(12) << "App ID"
        << setw(12) << "Month"
        << setw(12) << "Status"
        << setw(12) << "Payment\t|" << endl;
    cout << "|-----------------------------------------------|" << endl;

    int count = 0;
    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid) {
            int yr, mn;
            if (parseAppMonth(applications[i].month, yr, mn) && yr == targetYear) {
                cout << "|  " << left
                    << setw(12) << applications[i].appID
                    << setw(12) << applications[i].month
                    << setw(12) << applications[i].status
                    << setw(9) << applications[i].payment <<"|" << endl;
                count++;
            }
        }
    }
    cout << "=================================================" << endl;
    cout << "Records: " << count << endl << endl;
}

// View Transactions by Month
void viewTransactionsByMonth(int studentIndex)
{
    cout << "\n===================================================" << endl;
    cout << "|          TRANSACTION HISTORY BY MONTH           |" << endl;
    cout << "===================================================" << endl;

    string sid = students[studentIndex].id;
    string targetMonth;

    cout << "Enter month (YYYY-MM): ";
    cin >> targetMonth;

    int yr, mn;
    if (!parseAppMonth(targetMonth, yr, mn)) {
        cout << "Invalid format. Use YYYY-MM.\n\n";
        return;
    }
    cout << "\n===================================================";
    cout << "\n|\t    Transactions for" << targetMonth << "\t\t  |\n";
    cout << "===================================================" << endl;

    cout << "|\t " << left
        << setw(12) << "App ID"
        << setw(12) << "Status"
        << setw(12) << "Payment" <<"\t  |" << endl;
    cout << "|-------------------------------------------------|" << endl;

    int count = 0;
    for (int i = 0; i < applicationCount; i++) {
        if (applications[i].studentID == sid && applications[i].month == targetMonth) {
            cout << "|\t " << left
                << setw(12) << applications[i].appID
                << setw(12) << applications[i].status
                << setw(12) << applications[i].payment <<"\t  |" << endl;
            count++;
        }
    }

    if (count == 0) {
        cout << "No transactions found for this period.\n\n";
    }
    else {
        cout << "===================================================" << endl;
        cout << "Records: " << count <<endl<<endl;
    }
}


//=========================================================== Admin Module ===========================================================
// Login admin
void admin_login()
{
	int index = 0;
	string adminID, adminPassword;
	bool loginSuccess = false;

	cout<<"\n==================================================="<<endl;
	cout<<"|                   ADMIN LOGIN                   |"<<endl;
	cout<<"==================================================="<<endl;
	
	do{
		cout<<"Admin ID (PRESS 0 to return): ";
		cin>>adminID;
		if(adminID == "0"){
			cout<<endl;
			return;
		}
		
		cout<<"Password (PRESS 0 to return): ";
		cin>>adminPassword;
		if(adminPassword == "0"){
			cout<<endl;
			return;
		}
		
		ifstream file("admin.txt");

		//If file doesn't exist, create default admin
		if(!file){
		    ofstream newFile("admin.txt");
		    newFile<<"A001,admin,1234\n";
		    newFile.close();
		    
		    file.open("admin.txt");
		}

		loginSuccess = false;
		string line;
		while(getline(file, line)){
		    stringstream ss(line);
		    string id, username, password;

		    getline(ss, id, ',');
		    getline(ss, username, ',');
		    getline(ss, password, ',');
		
		    if(adminID == id && adminPassword == password){
		        loginSuccess = true;
		        break;
		    }
		}
		file.close();
		
		if(!loginSuccess){
		    cout<<"\nLogin Failed! Please try again."<<endl<<endl;
		}
	}while(!loginSuccess);
	
	cout<<"\nLOGIN SUCCESSFULLY!"<<endl;
	loading_screen();
	clear_screen();
	admin_page(index);
}

// Admin Menu
void admin_page(int index)
{
	int adminChoice;
	
	do{
		cout<<"=========== ADMIN MODULE ==========="<<endl;
        cout<<"| 1. View Students Details         |"<<endl;
        cout<<"| 2. Application Validation        |"<<endl;
        cout<<"| 3. Summary Analytics Reports     |"<<endl;
        cout<<"| 4. Back to Home Page             |"<<endl;
        cout<<"===================================="<<endl;
		
		cout << "Enter your option : ";
		cin >> adminChoice;
		
		switch(adminChoice){
			case 1: view_stud(index); break;
			case 2: view_app(index); break;
			case 3: loading_screen(); clear_screen(); fullReport(); break; 
			case 4: cout<<"\nReturn to Home Page...\n"; loading_screen(); clear_screen(); return;
			default: limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-4."<<endl;
		}
	}while(adminChoice != 4);
}

// View Students Details from students.txt
void view_stud(int index)
{
	cout<<"\n==================================================================================================="<<endl;
	cout<<"|                                        STUDENTS DETAILS                                         |"<<endl;
	cout<<"==================================================================================================="<<endl;
	
	ifstream in_file("students.txt");
	
	if(!in_file){
		cout<<"|                                     --ERROR FILE FOUND!--                                       |"<<endl;
    	cout<<"===================================================================================================\n"<<endl;
	}
	
	else{
		string name, id, faculty, phone, vehicle, password;
		
		cout<<"| "<<left
			<< setw(30) <<"Student Name"
			<< setw(15) <<"Student ID"
			<< setw(20) <<"Faculty"
			<< setw(15) <<"Phone No."
			<< setw(15) <<"Vehicle No." <<" |"<<endl;
		cout<<"|-------------------------------------------------------------------------------------------------|"<<endl;

	    while(getline(in_file, name, ',') &&
	          getline(in_file, id, ',') &&
	          getline(in_file, faculty, ',') &&
	          getline(in_file, phone, ',') &&
	          getline(in_file, vehicle, ',') &&
	          getline(in_file, password))
		{
	          	cout<< "| " << left
	            << setw(30) << name
	            << setw(15) << id
	            << setw(20) << faculty
	            << setw(15) << phone
	            << setw(15) << vehicle <<" |"<<endl;
	    }
		cout<<"==================================================================================================="<<endl<<endl;
	}
	in_file.close();
}

// View Students Applications details from applications.txt
void view_app(int index)
{
	cout<<"\n====================================================================="<<endl;
	cout<<"|                       STUDENTS APPLICATIONS                       |"<<endl;
	cout<<"====================================================================="<<endl;
	
	if(applicationCount == 0){
		cout<<"|                        --ERROR FILE FOUND!--                      |"<<endl;
    	cout<<"=====================================================================\n"<<endl;
    	return;
	}
	
	else{	
		cout<<"| "<<left
			<< setw(20) <<"Application ID"
			<< setw(15) <<"Student ID"
			<< setw(10) <<"Month"
			<< setw(10) <<"Status"
			<< setw(10) <<"Payment" <<" |"<<endl;
		cout<<"|-------------------------------------------------------------------|"<<endl;

		for(int i = 0; i < applicationCount; i++){
	        int idx = findStud_Index(applications[i].studentID);
			
			// application ID
	        cout<<"| "<<left
	        	<< setw(20) << applications[i].appID;
	        
	        // student ID
	        if(idx != -1){
		        cout << setw(15) << students[idx].id;
		    } 
			else{
		        cout << setw(15) << applications[i].studentID;
		    }
	        
	        // month, status, payment
	        cout<< left
	            << setw(10) << applications[i].month
	            << setw(10) << applications[i].status
	            << setw(10) << applications[i].payment <<" |\n";
		}
		cout<<"====================================================================="<<endl;
		app_validation();
	}
}

// Application Validation ========================================
void app_validation()
{
	string appID;  //getline() only can allow string
	char validation;

	do{
		cout<<"\n==================================================="<<endl;
		cout<<"|             APPLICATION VALIDATION              |"<<endl;
		cout<<"==================================================="<<endl;
		
		cout<<"Enter Application ID (PRESS 0 to return): ";
	    cin>>appID;
	    if(appID == "0"){
			cout<<endl;
			return;
		}
	    
	    int appIndex = findApp_Index(appID);
	
	    if(appIndex == -1){
	        cout << "Application NOT FOUND!\n" << endl;
	        return;
	    }
	    
	    cout<<"\n================================================="<<endl;
	    cout<<"|            Detail of Application ID           |"<< endl;
	    cout<<"================================================="<<endl;
	    cout<<"| Application ID: " << applications[appIndex].appID << string(30-applications[appIndex].appID.length(), ' ') <<"|"<<endl;
	    cout<<"| Student ID    : " << applications[appIndex].studentID << string(30-applications[appIndex].studentID.length(), ' ') <<"|"<<endl;
	    
	    int studentIndex = findStud_Index(applications[appIndex].studentID);
	    string vehicle = "-";
	    // student name
	    if(studentIndex != -1){
		    cout<<"| Student Name  : " << students[studentIndex].name << string(30-students[studentIndex].name.length(), ' ') <<"|"<<endl;
		} else {
			string placeholder = "-";
		    cout<<"| Student Name  : - " << string(29-placeholder.length(), ' ') <<"|"<<endl;
		}
		// vehicle
		if(studentIndex != -1){
		    cout<<"| Vehicle No.   : " << students[studentIndex].vehicle << string(30-students[studentIndex].vehicle.length(), ' ') <<"|"<<endl;
		} else {
			string placeholder = "-";
		    cout<<"| Vehicle No.   : - " << string(29-placeholder.length(), ' ') <<"|"<<endl;
		}
		
	    cout<<"| Month         : " << applications[appIndex].month << string(30-applications[appIndex].month.length(), ' ') <<"|"<<endl;
	    cout<<"| Status        : " << applications[appIndex].status << string(30-applications[appIndex].status.length(), ' ') <<"|"<<endl;
	    cout<<"================================================="<<endl;
	
	    cout << "Start Validation? (y/n): ";
	    cin >> validation;
			
		if(validation == 'y' || validation == 'Y'){
	        string reason = validate_app(
	            applications[appIndex].studentID,
	            applications[appIndex].appID
        	);
	
	        if(reason == "Approved"){
	            applications[appIndex].status = STATUS_APPROVED;
	            cout << "\nApplication APPROVED!" <<endl<<endl;
	            
	            createPass(
	                applications[appIndex].studentID,
	                applications[appIndex].month + "-01", // safe default date
	                1 // default 1 month (you can extend later)
	            );
	        }
	        else{
	            applications[appIndex].status = STATUS_REJECTED;
	            cout << "\nRejected Reason: " << reason <<endl<<endl;
	        }
	        // save back to file
	        ofstream file("applications.txt");
	        for(int i = 0; i < applicationCount; i++){
	            file << applications[i].appID << ","
	                 << applications[i].studentID << ","
	                 << applications[i].status << ","
	                 << applications[i].month << ","
	                 << applications[i].payment << endl;
	        }
	        file.close();
		}
	}while(validation == 'n' || validation == 'N');
}

// Student details found
void approve_app(string targetAppID)
{
    int idx = findApp_Index(targetAppID);
    if(idx == -1){
        cout << "Application not found\n";
        return;
    }

    applications[idx].status = STATUS_APPROVED;

    createPass(
        applications[idx].studentID,
        applications[idx].month + "-01",
        1
    );

    cout << "Approved and pass created!\n";
}

// Validate application for checking details
string validate_app(string studentID, string appID)
{
    // Check student exists in students.txt
    ifstream stuFile("students.txt");
    if(!stuFile) return "Student file missing";

    bool studentFound = false;
    string line;

    while(getline(stuFile, line)){
        stringstream ss(line);
        string name, id, faculty, phone, vehicle, password;

        getline(ss, name, ',');
        getline(ss, id, ',');
        getline(ss, faculty, ',');
        getline(ss, phone, ',');
        getline(ss, vehicle, ',');
        getline(ss, password, ',');

        if(id == studentID){
            studentFound = true;

            // basic completeness check
            if(name.empty() || faculty.empty() || phone.empty() || vehicle.empty()){
                return "Incomplete student details";
            }
            break;
        }
    }
    stuFile.close();

    if(!studentFound){
        return "Student ID not found";
    }

    // Check application exists + valid data
    int appIndex = findApp_Index(appID);
    if(appIndex == -1){
        return "Application not found";
    }

    if(applications[appIndex].month.empty()){
        return "Incomplete application month";
    }

    return STATUS_APPROVED;
}

// Generate Pass ID
string generatePassID()
{
	ifstream file("passes.txt");
    string line;
    int maxNum = 1000;

    while (getline(file, line)) {
        stringstream ss(line);
        string id;
        getline(ss, id, ',');

        if (id.length() > 1) {
            int num = stringToInt(id.substr(1));
            if (num > maxNum) maxNum = num;
        }
    }

    return "P" + intToString(maxNum + 1);
}

// Create parking pass (passes.txt)
void createPass(string studentID, string startDate, int months)
{
	ofstream outFile("passes.txt", ios::app);
    string passID = generatePassID();

    int year = stringToInt(startDate.substr(0,4));
    int month = stringToInt(startDate.substr(5,2));
    int day = stringToInt(startDate.substr(8,2));

    month += months;

    while(month > 12){
        month -= 12;
        year++;
    }

	string endDate = intToString(year) + "-" + (month < 10 ? "0" : "") + intToString(month) + "-" + (day < 10 ? "0" : "") + intToString(day);

    // into struct
    passes[passCount].passID = passID;
    passes[passCount].studentID = studentID;
    passes[passCount].startDate = startDate;
    passes[passCount].endDate = endDate;
    passes[passCount].status = STATUS_PENDINGPAY;

    passCount++;

    // into file
    outFile << passID << "," << studentID << ","
            << startDate << "," << endDate << "," << STATUS_PENDINGPAY << endl;

    outFile.close();
}

// Summary Analytics Reports ========================================
void fullReport()
{
	int reportChoice;
	
	do{
		cout<<"============== ANALYTICS REPORTS ==============="<<endl;
        cout<<"| 1. Total Applications                        |"<<endl;
        cout<<"| 2. Average Appl per Student (by Faculty)     |"<<endl;
        cout<<"| 3. Parking Pass Usage Rate                   |"<<endl;
        cout<<"| 4. Monthly Income per Year                   |"<<endl;
        cout<<"| 5. Back to Admin Module                      |"<<endl;
        cout<<"================================================"<<endl;
		
		cout << "Enter your option : ";
		cin >> reportChoice;
		
		switch(reportChoice){
			case 1: total_app(); break;
			case 2: averageApply(); break;
			case 3: passUsageRate(); break;
			case 4: monthlyIncome(); break;
			case 5: cout<<"\nReturn to Admin Module...\n"; loading_screen(); clear_screen(); return;
			default: limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-5."<<endl;
		}
	}while(reportChoice != 5);
}

// Average Apply per Student (by Faculty)
void averageApply()
{
	// Average Apply per Student (by Faculty) = total applications in that faculty / total students in that faculty
	cout<<"\n============================================"<<endl;
	cout<<"|     Average Applications per Student     |"<<endl;
	cout<<"============================================"<<endl;
	
	if(applicationCount == 0 || studentCount == 0){
        cout<<"|          --No data available--           |"<<endl;
        cout<<"============================================\n"<<endl;
        return;
    }

    // store unique faculties
    string faculties[100];
    int facultyCount = 0;

    // Step 1: collect unique faculty names
    for(int i = 0; i < studentCount; i++){
        bool exists = false;

        for(int j = 0; j < facultyCount; j++){
            if(faculties[j] == students[i].faculty){
                exists = true;
                break;
            }
        }

        if(!exists){
            faculties[facultyCount++] = students[i].faculty;
        }
    }

    cout<<"| "<<left
		<< setw(20) <<"Faculty"
		<< setw(20) <<"Average Apply" <<" |"<<endl;
	cout<<"|------------------------------------------|"<<endl;

    // Step 2: calculate for each faculty
    for(int i = 0; i < facultyCount; i++){
        string currentFaculty = faculties[i];

        int totalStudents = 0;
        int totalApplications = 0;

        for(int j = 0; j < studentCount; j++){
            if(students[j].faculty == currentFaculty){
                totalStudents++;

                // count applications for this student
                for(int k = 0; k < applicationCount; k++){
                    if(applications[k].studentID == students[j].id){
                        totalApplications++;
                    }
                }
            }
        }

        double avg = 0;
        if(totalStudents > 0){
            avg = (double)totalApplications / totalStudents;
        }
        
		cout<<"| " << left
            << setw(20) << currentFaculty
            << setw(20) << fixed << setprecision(2) << avg <<" |"<<endl;
    }
    cout<<"============================================"<<endl<<endl;
    return;
}

// Total Applications
void total_app()
{
	cout << "\nTotal Applications: " << applicationCount <<endl<<endl;
	return;
}

// Parking Pass Usage Rate
void passUsageRate()
{
	// Parking Pass Usage Rate = (Number of Active Parking Pass / Total Parking Slots) x 100%
	const int totalSlots = SLOTS;
    int activePass = 0;

    for(int i = 0; i < passCount; i++){
        if(passes[i].status == STATUS_ACTIVE){
            activePass++;
        }
    }
    
    if(activePass > totalSlots) activePass = totalSlots;
    double rate = (double)activePass / totalSlots * 100;
    
    ostringstream ss;
	ss << fixed << setprecision(2) << rate;
	string rateStr = ss.str();

	cout<<"\n============================================="<<endl;
	cout<<"|       Parking Pass Utilization Rate       |"<<endl;
	cout<<"============================================="<<endl;
    cout<<"| Active Parking Pass : " << activePass << string(20 - intToString(activePass).length(), ' ') << "|" << endl;
	cout<<"| Total Parking Slots : " << totalSlots << string(20 - intToString(totalSlots).length(), ' ') << "|" << endl;
	cout<<"| Usage Rate          : " << fixed << rateStr << "%" << string(19 - rateStr.length(), ' ') << "|" << endl;    
	cout<<"============================================="<<endl<<endl;
    return;
}

// Monthly Income per Year 
void monthlyIncome()
{
	// Monthly Income per Year = Total Payment Collected in that Month
	cout<<"\n==================================="<<endl;
	cout<<"|     Monthly Income per Year     |"<<endl;
	cout<<"==================================="<<endl;
	
	if(applicationCount == 0){
        cout<<"|      --No data available--      |"<<endl;
        cout<<"===================================\n"<<endl;
        return;
    }

    // store unique years
    int years[10];
    int yearCount = 0;

    // Step 1: collect unique years
    for(int i = 0; i < applicationCount; i++){
        string ym = applications[i].month;

        int year = stringToInt(ym.substr(0,4));

        bool exists = false;
        for(int j = 0; j < yearCount; j++){
            if(years[j] == year){
                exists = true;
                break;
            }
        }

        if(!exists){
            years[yearCount++] = year;
        }
    }
    
    cout<<"| "<<left
		<< setw(8) <<"Year"
		<< setw(8) <<"Month"
		<< setw(15) <<"Income (RM)" <<" |"<<endl;
	
    // Step 2: calculate income per year & month
    for(int y = 0; y < yearCount; y++){
        int year = years[y];

        double monthly[13] = {0}; // index 1–12

        for(int i = 0; i < applicationCount; i++){
            if(applications[i].status == STATUS_APPROVED && applications[i].payment == STATUS_PAID){
			    int yr, mn;
			    if(parseAppMonth(applications[i].month, yr, mn)){
			        if(yr == year){
			            monthly[mn] += PRICE;
			        }
			    }
			}
        }
        
        for(int m = 1; m <= 12; m++){
		
		    if(m == 1){
		    	cout<<"|---------------------------------|"<<endl;
		        cout<<"| " << left << setw(8) << year;   // only first row show year
		    } 
			else{
		        cout<<"| " << left << setw(8) << " ";    // others empty
		    }
		
		    cout << setw(8) << (m < 10 ? "0" + intToString(m) : intToString(m))
		         << setw(15) << fixed << setprecision(2) << monthly[m] 
		         << " |"<<endl;
		}
    }
    cout<<"==================================="<<endl<<endl;
    return;
}

