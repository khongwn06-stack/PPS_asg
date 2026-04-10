#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()
using namespace std;

//Admin Part
void admin_login();
void admin_page();

void view_app();

//Validation Process
void app_validation();
void approve_app();
string reject_app();

string generatePassID();
void createPass(string studentID, string startDate, int months);

//Analytics
void generateAnalytic();
void averageRenewal();
void total_app();
void utilizationRate();
void monthlyIncome();
void growthRate();
void report();

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



//Main Menu
int main()
{
	string status, reason, studentID, startDate;
	int months;
	bool incompleteDetails = false;
	bool UnpaidPass = false;
	int pass = 0;
	int activeUser = 0, parkingSlot = 100;

	admin_page();
	return 0;
}

void admin_login()
{
	string id, username, password;
	string adminUsername, adminPassword;

	cout<<"\n\n---------Admin Login---------"<<endl<<endl;
	do{
		cout<<"Username: ";
		cin>>adminUsername;
		cout<<"Password: ";
		cin>>adminPassword;
		
		ifstream file("admin.txt");
		
		//If file doesn't exist, create default admin
	    if(!file){
	        ofstream newFile("admin.txt");
	        newFile<<"A001|admin|1234\n";
	        newFile.close();
	        file.open("admin.txt"); // reopen for reading
	    }
		string line;
		bool loginSuccess = false;

		while(getline(file, line)){
		    stringstream ss(line);
		    getline(ss, id, '|');
		    getline(ss, username, '|');
		    getline(ss, password, '|');
		
		    if(adminUsername == username && adminPassword == password){
		        loginSuccess = true;
		        break;
		    }
		}
		
		if(!loginSuccess){
		    cout<<"Login Failed! Please try again.\n";
		}
	}while/*(!loginSuccess)*/(adminUsername != username || adminPassword != password);
	cout<<"Login Successful!"<<endl<<endl;
}

void admin_page()
{
	admin_login();
	//loading_screen();
	
	int adminChoice;
	//clear_screen();
	
	cout<<"----------Admin Page----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. View Application"<<endl;
		cout<<"2. Application Validation"<<endl;
		cout<<"3. Generate Analytics"<<endl;
		cout<<"4. Reports"<<endl;
		cout<<"5. Exit"<<endl;
		
		cout<<"Enter your option : ";
		cin>>adminChoice;
		
		switch(adminChoice){
			case 1: view_app(); break;
			case 2: app_validation(); break;
			case 3: generateAnalytic(); break;
			case 4: report(); break; 
			case 5: break;
			default: cout<<"Invalid Option! Please Enter the Number."<<endl;
		}
	}while(adminChoice != 5);
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
				string reason = reject_app();

				if(reason == "Approved"){
				    approve_app();
				}
				else {
				    cout << "Rejected reason: " << reason << endl;
				}
//				reject_app();
//				if(status == "Rejected") {
//					reject_app();
//                	cout << "Application Rejected: " << reason << endl;
//            	}
			}
			
			else if(validation!='y' && validation!='Y' && validation!='n' && validation!='N'){
				cout<<"Invalid Input! Enter only alphabet 'y' for yes or 'n' for no"<<endl;
			}
			
		}while(validation!='y' && validation!='Y' && validation!='n' && validation!='N');
		
	}while(validation=='n' || validation=='N');
}

void approve_app()
{
	ifstream inFile("applications.txt");
    ofstream tempFile("temp.txt");

	string line;

    while(getline(inFile, line)) {
        string appID, studentID, type, months, status, applyDate, startDate, reason;

        stringstream ss(line);
        getline(ss, appID, '|');
        getline(ss, studentID, '|');
        getline(ss, type, '|');
        getline(ss, months, '|');
        getline(ss, status, '|');
        getline(ss, applyDate, '|');
        getline(ss, startDate, '|');
        getline(ss, reason, '|');
	
		if(status == "Pending") {
            cout << "AppID: " << appID << ", Student: " << studentID << endl;
            createPass(studentID, startDate, stringToInt(months));
        }
        tempFile << appID << "|" << studentID << "|" << type << "|"
                 << months << "|" << status << "|" << applyDate
                 << "|" << startDate << "|" << reason << endl;
    }
    
    inFile.close();
    tempFile.close();

    remove("applications.txt");
    rename("temp.txt", "applications.txt");
    
    cout << "\nAll applications processed.\n";
}

string reject_app()
{
	int pass=0, months=4, activeUser=50, parkingSlot=100;
	
    //if(incompleteDetails) return "Incomplete details";
    if(pass != 0) return "Pass overlap";
    //if(UnpaidPass) return "Unpaid pass";
    if(months > 3) return "Exceeded limit";
    if(activeUser == parkingSlot) return "Parking full";

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
		cout<<"6. Full Analytics Reports"<<endl;
		cout<<"7. Exit"<<endl;
		
		cout<<"Enter your option : ";
		cin>>analyticChoice;
		
		switch(analyticChoice){
			case 1: averageRenewal(); break;
			case 2: total_app(); break;
			case 3: utilizationRate(); break;
			case 4: monthlyIncome(); break; 
			case 5: growthRate(); break;
			case 6: report(); break;
			case 7: break;
			default: cout<<"Invalid Option! Please Enter the Number."<<endl;
		}
	}while(analyticChoice != 7);
}

void averageRenewal(){
	cout<<"----------Average Renewal----------"<<endl;
}
void total_app(){
	cout<<"----------Total Applications----------"<<endl;
}
void utilizationRate(){
	cout<<"----------Car Park Utilization Rate----------"<<endl;
}
void monthlyIncome(){
	cout<<"----------Monthly Income----------"<<endl;
}
void growthRate(){
	cout<<"----------Growth Rate----------"<<endl;
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
