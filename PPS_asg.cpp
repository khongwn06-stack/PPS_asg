#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()
using namespace std;

//Login Part
bool home_page();

//Student Part
void newReg_page();
void stud_page();


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


//========================================Addition========================================
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
	cout<<"\nHave A Good Day! Thank You!"<<endl;
	exit(0);
}


//========================================Main Menu========================================
int main()
{
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

//Login Part
bool home_page()
{
	int homeChoice;
	
	cout<<"UTAR Monthly Car Parking Pass System"<<endl<<endl;
	cout<<"----------Home Page----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. New Registration Page"<<endl;
		cout<<"2. Student Page"<<endl;
		cout<<"3. Admin Page"<<endl;
		cout<<"4. Exit System"<<endl<<endl;
		cout<<"Enter your option : ";
		cin>>homeChoice;
		
		switch(homeChoice){
			case 1: newReg_page(); break;
			case 2: stud_page(); break;
			//case 3: admin_page(); break;
			case 3: admin_login(); break;
			case 4: exit_system(); break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-4."<<endl;
		}
	}while (homeChoice != 4);
}

//Student Part
void newReg_page(){
	cout<<"New Registration Page"<<endl;
}
void stud_page(){
	cout<<"Student Page"<<endl;
}

//========================================Admin Part========================================
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


