#include <iostream>
#include <iomanip>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <ctime>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()
using namespace std;

//========================================Login Part========================================
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
    string passID, studentID, startDate, endDate, status; // status = Active / Expired / PendingPayment
};

// Global arrays to store, loaded from txt.file
Student students[400];
Application applications[400];
Pass passes[400];

int studentCount = 0;
int applicationCount = 0;
int passCount = 0;

// Load from txt.file
void loadStudents();
void loadApplications();
void loadPasses();

// Find index in array by matching ID
int findStud_Index(string id);
int findApp_Index(string id);
int findPassIndex(string studentID);

//========================================Student Part========================================
void stud_page();
void registerStudent();
void loginStudent();
void successLoginS(int index);
void studentMenu(int studentIndex);

void submitApplication(int studentIndex);
void renewApplication(int studentIndex); 
void viewApplication(int studentIndex);
void viewPass(int index);
void payment(int index);
void updateStudentDetails(int studentIndex);

// Applications.txt
string generateApplicationID();
bool isWithinThreeMonths(string month); 


//========================================Admin Part========================================
void admin_login();
void admin_page(int index);

// Applicatons Validation Process
void view_stud(int index);
void view_app(int index);
void app_validation();
void approve_app(string targetAppID);
string validate_app(string studentID, string appID);

// Pass.txt
string generatePassID();
void createPass(string studentID, string startDate, int months);

// Generate Analytics
void fullReport();
void averageRenewal();
void total_app();
void utilizationRate();
void monthlyIncome();
void growthRate();


//========================================Tracking Part========================================
void trackingModule();



//===========================================================Addition===========================================================
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
	system("cls"); //clear screen
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



//===========================================================Main Menu===========================================================
int main()
{
	loadStudents();
    loadApplications();
    loadPasses();
	
	home_page();
	
	return 0;
}

//===========================================================Login Part===========================================================
void home_page()
{
	int homeChoice;

	do{
		cout<<"        MPKJ MONTHLY CAR PARKING PASS SYSTEM       "<<endl;
        cout<<"==================================================="<<endl;
        cout<<"|                    HOME PAGE                    |"<<endl;
        cout<<"==================================================="<<endl;
        cout<<"| 1. Student Module                               |"<<endl;
        cout<<"| 2. Admin Module                                 |"<<endl;
        cout<<"| 3. Tracking & Alert System Module               |"<<endl;
        cout<<"| 4. Exit System                                  |"<<endl;
        cout<<"==================================================="<<endl;

        cout<<"Select an option: ";
		cin>>homeChoice;
		
		switch(homeChoice){
			case 1: loading_screen(); clear_screen(); stud_page(); break;
			case 2: admin_login(); break;
			case 3: trackingModule(); break;
			case 4: cout<<"\nTHANK YOU FOR USING CAR PARKING PASS SYSTEM! SEE YOU!"<<endl; break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-4."<<endl;
		}
	}while(homeChoice != 4);
}

// Load students from txt.file
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

// Load applications from txt.file
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

// Load passes from txt.file
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

//===========================================================Find index in array by matching ID===========================================================
// Find student index in array by matching Student ID
int findStud_Index(string id)
{
    for(int i = 0; i < studentCount; i++){
        if(students[i].id == id){
            return i;
        }
    }
    return -1;
}

// Find application index in array by Application ID
int findApp_Index(string id)
{
    for(int i = 0; i < applicationCount; i++){
        if(applications[i].appID == id){
            return i;
        }
    }
    return -1;
}

// Find pass index in array by matching Student ID
int findPassIndex(string studentID)
{
    for(int i = 0; i < passCount; i++){
        if(passes[i].studentID == studentID){
            return i;
        }
    }
    return -1;
}


//===========================================================Student Part===========================================================
void stud_page()
{
    int studChoice;

    do
    {
        cout<<"========== STUDENT MODULE =========="<<endl;
        cout<<"| 1. Register                      |"<<endl;
        cout<<"| 2. Login                         |"<<endl;
        cout<<"| 3. Back to Home Page             |"<<endl;
        cout<<"===================================="<<endl;

        cout<<"Select an option: ";
        cin>>studChoice;
		
		switch(studChoice){
			case 1: registerStudent(); break;
			case 2: loginStudent(); break;
			case 3: cout<<"\nReturn to Home Page...\n"; loading_screen(); clear_screen(); return;
			default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-3."<<endl;
		}
    }while(studChoice != 3);
}

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
    successLoginS(index); 
}

// Student personal details
void successLoginS(int index)
{
	// success login 
    cout<<"LOGIN SUCCESSFUL!"<<endl;
    cout<<"~ WELCOME "<< students[index].name <<" ~"<<endl;

    // success login message
    cout<<"================================================"<<endl;
	cout<<"|               STUDENT DETAILS                |"<<endl;
	cout<<"================================================"<<endl;
	cout<<"| Name         : "<< students[index].name << string(30-students[index].name.length(), ' ') <<"|"<<endl;
	cout<<"| Student ID   : "<< students[index].id << string(30-students[index].id.length(), ' ') <<"|"<<endl;
	cout<<"| Faculty      : "<< students[index].faculty << string(30-students[index].faculty.length(), ' ') <<"|"<<endl;
	cout<<"| Phone Number : "<< students[index].phone << string(30-students[index].phone.length(), ' ') <<"|"<<endl;
	cout<<"| Vehicle No.  : "<< students[index].vehicle << string(30-students[index].vehicle.length(), ' ') <<"|"<<endl;
	cout<<"================================================"<<endl;
	
	studentMenu(index);
}

// Student Menu
void studentMenu(int index)
{
	int studchoice;
	
    do{
        cout<<"\n=============== STUDENT DASHBOARD ================="<<endl;
        cout<<"| 1. Apply New Pass                               |"<<endl;
        cout<<"| 2. Renew Pass                                   |"<<endl;
        cout<<"| 3. View Application Status                      |"<<endl;
        cout<<"| 4. View Parking Pass                            |"<<endl;
        cout<<"| 5. Payment                                      |"<<endl;
        cout<<"| 6. Update Personal Details                      |"<<endl;
        cout<<"| 7. Back to Student Module                       |"<<endl;
        cout<<"==================================================="<<endl;

        cout<<"Select an option: ";
        cin>>studchoice;

        switch(studchoice){
            case 1: submitApplication(index); break;
            case 2: renewApplication(index); break;
            case 3: viewApplication(index); break;
            case 4: viewPass(index); break;
            case 5: payment(index); break;
			case 6: updateStudentDetails(index); break;
            case 7: cout<<"\nReturn to Student Module...\n"; loading_screen(); clear_screen(); return;
            default:limit_input(); cout<<"\nInvalid Option! Please Enter the Number 1-7."<<endl; 
        }
    }while(studchoice != 7);
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
        if(month == "0") return;

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
    a.status = "Pending";
    a.month = month;
    a.payment = "Unpaid";

    applications[applicationCount++] = a;

    ofstream file("applications.txt", ios::app);
    file<< a.appID << "," << a.studentID << "," 
		<< a.status << "," << a.month << "," << a.payment <<endl;
    file.close();

    cout<<"\nAPPLICATION SUCCESSFUL!"<<endl;
    cout<<"Your Application ID : "<< a.appID <<endl;
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
    	cout<<"==========================================================="<<endl;
    	return;
	}

    int choice;

    while(true){
        cout << "\nSelect index (press -1 to cancel): ";

        string input;
        getline(cin, input);

        stringstream ss(input);
        if(!(ss >> choice)){
            cout << "Invalid input format!\n";
            continue;
        }

        if(choice == -1){
            cout << "Cancelled\n";
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
    applications[realIndex].status = "Pending";
    applications[realIndex].payment = "Unpaid";

    ofstream file("applications.txt");

    for(int i = 0; i < applicationCount; i++){
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
        	cout<<"========================================"<<endl;
        	found = true;
    	}
	}

	if(!found){
    	cout<<"|      --NO APPLICATION FOUND!--       |"<<endl;
    	cout<<"========================================"<<endl;
	}
}

// Payment
void payment(int index)
{
	cout << "\n[PAYMENT MODULE - NOT IMPLEMENTED YET]\n";
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
        	cout<<"========================================"<<endl;
        	found = true;
    	}
	}

	if(!found){
    	cout<<"|      --NO PARKING PASS FOUND!--      |"<<endl;
    	cout<<"========================================"<<endl;
	}
}

// Update personal details
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
			case 0: return;
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


//===========================================================Admin Part===========================================================
// Login admin
void admin_login()
{
	int index = 0;
	string id, username, password;
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
        loginSuccess = false;

		//If file doesn't exist, create default admin
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
		
		    if(adminID == id && adminPassword == password){
		        loginSuccess = true;
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
			case 2: view_app(index); app_validation(); break;
			case 3: fullReport(); break; 
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
    	cout<<"==================================================================================================="<<endl;
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
		cout<<"|                      --ERROR FILE FOUND!--                        |"<<endl;
    	cout<<"====================================================================="<<endl;
	}
	
	else{	
		cout<<"| "<<left
			<< setw(15) <<"Student ID"
			<< setw(20) <<"Application ID"
			<< setw(10) <<"Month"
			<< setw(10) <<"Status"
			<< setw(10) <<"Payment" <<" |"<<endl;
		cout<<"|-------------------------------------------------------------------|"<<endl;

		for(int i = 0; i < applicationCount; i++){
	        int idx = findStud_Index(applications[i].studentID);
	        cout<<"| ";
	        
	        if(idx != -1){
		        cout << setw(15) << students[idx].id;
		    } else {
		        cout << setw(15) << applications[i].studentID;
		    }
	        
	        cout << left
	             << setw(20) << applications[i].appID
	             << setw(10) << applications[i].month
	             << setw(10) << applications[i].status
	             << setw(10) << applications[i].payment << " |\n";
		}
		cout<<"====================================================================="<<endl;
	}
}

// Application Validation
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
	    if(studentIndex != -1){
		    cout<<"| Student Name  : " << students[studentIndex].name << string(30-students[studentIndex].name.length(), ' ') <<"|"<<endl;
		} else {
			string placeholder = "-";
		    cout<<"| Student Name  : - " << string(30-placeholder.length(), ' ') <<"|"<<endl;
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
	            applications[appIndex].status = "Approved";
	            cout << "\nApplication APPROVED!" <<endl<<endl;
	            
	            createPass(
	                applications[appIndex].studentID,
	                applications[appIndex].month + "-01", // safe default date
	                1 // default 1 month (you can extend later)
	            );
	        }
	        else{
	            applications[appIndex].status = "Rejected";
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

    applications[idx].status = "Approved";

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

    return "Approved";
}

// Generate Pass ID
string generatePassID()
{
	ifstream file("passes.txt");
    string line, lastID = "P1000";

    while(getline(file, line)){
        stringstream ss(line);
        getline(ss, lastID, ',');
    }

    int num = stringToInt(lastID.substr(1));
    return "P" + intToString(num + 1);
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
    passes[passCount].status = "PendingPayment";

    passCount++;

    // into file
    outFile << passID << "," << studentID << ","
            << startDate << "," << endDate << ",PendingPayment" << endl;

    outFile.close();
}

// Generate Analytics
void fullReport()
{
    cout<<"\n==================================================="<<endl;
	cout<<"|          SUMMARY OF ANALYTICS REPORTS           |"<<endl;
	cout<<"==================================================="<<endl;
    total_app();
	averageRenewal();
    utilizationRate();
    monthlyIncome();
    growthRate();
    cout<<"==================================================="<<endl<<endl;
}

// Average Renewal
void averageRenewal()
{
	/* Average Renewal = total number of applications / number of students who applied
	
	   Exp:	Student A -> 2 applications
			Student B -> 1 application
			Student C -> 3 applications
	
	   		average = (2+1+3) / 3 = 2
	*/
		
	if(applicationCount == 0){
        cout << "No applications found.\n";
        return;
    }

    int totalApplications = 0;
    int totalStudents = 0;

    for(int i = 0; i < studentCount; i++){
        int count = 0;

        for(int j = 0; j < applicationCount; j++){
            if(applications[j].studentID == students[i].id){
                count++;
            }
        }

        if(count > 0){
            totalStudents++;
            totalApplications += count;
        }
    }

    double avgRenew = (double)totalApplications / totalStudents;

    cout << "Average Renewal: " << fixed << setprecision(2) << avgRenew << endl;
}

// Total Applications
void total_app()
{
	/*	Total Applications = total number of records in applications.txt (new + renew)
	
		Exp:	application.txt = 120 row
				total = 120
	*/
		
	cout << "Total Applications: " << applicationCount << endl;
}

// Car Park Utilization Rate
void utilizationRate()
{
	/*	Utilization Rate = total usage units / maximum parking capacity

		Assumption:	- Each approved application represents 1 unit of usage
					- Total capacity = parking slots ?operating hours
		
		Exp:	1000 slots ?10.5 hours = 10500 total capacity units
				If 100 approved applications:
					Utilization Rate = 100 / 10500 ?100%
	*/
			
	const int slots = 1000;
	const double hoursPerDay = 10.5;

    double maxCapacity = slots * hoursPerDay;
    double used = 0;

//    for(int i = 0; i < applicationCount; i++){
//        if(applications[i].status == "Approved"){
//            used += 1; // simplified usage unit
//        }
//    }

    double rate = (used / maxCapacity) * 100;

    cout << "Utilization Rate: " << fixed << setprecision(2) << rate << "%" << endl;
}

// Monthly Income
void monthlyIncome()
{
	/*	Monthly Income = total payment collected within a specific month
	
		- Read payments.txt
		- Filter by selected month
		- Sum all payment amounts
	*/
		
	double monthIncome = 0;
	
//	for(int i = 0; i < applicationCount; i++){
//	    if(applications[i].status == "Approved"){
//	        monthIncome += 50; // per pass
//	    }
//	}

    cout << "Monthly Income: RM " << monthIncome << endl;
}

// Growth Rate
void growthRate()
{
	/*	Growth Rate = (current month income - previous month income) / previous month ?100%
	
		Current implementation uses a simplified estimation:
		- First half of records is treated as previous month
		- Second half is treated as current month
	
		Exp:	previous = RM1000
				current = RM1200
				growth = 20%
	*/
		
	double current = 0;
    double previous = 0;

    for(int i = 0; i < applicationCount; i++){
        if(applications[i].status == "Approved"){
            current += 50;

            // fake split for demo (first half = previous)
            if(i < applicationCount / 2){
                previous += 50;
            }
        }
    }

    if(previous == 0){
        cout << "Growth Rate: N/A (no previous data)" << endl;
        return;
    }

    double growth = ((current - previous) / previous) * 100;

    cout << "Growth Rate: " << fixed << setprecision(2) << growth << "%" << endl;
}


//========================================Tracking Part========================================
void trackingModule(){
	cout << "\n[TRACKING MODULE - NOT IMPLEMENTED YET]\n";
}
