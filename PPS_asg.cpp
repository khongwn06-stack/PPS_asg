#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits>  //limited output after enter Alphabet
#include <fstream> //required ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
#include <string>  //required for the string type used with getline()
using namespace std;

//Admin Part
void view_app();
void app_approval();
void analytic();
void report();


//Main Menu
int main()
{
	int adminChoice;
	
	cout<<"----------Admin Page----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. View Application"<<endl;
		cout<<"2. Application Approval"<<endl;
		//cout<<"3. Monthly Pass Approval"<<endl;
		cout<<"3. Analytics"<<endl;
		cout<<"4. Reports"<<endl;
		cout<<"5. Exit"<<endl;
		
		cout<<"Enter your option : ";
		cin>>adminChoice;
		
		switch(adminChoice){
			case 1: view_app(); break;
			case 2: app_approval(); break;
			//case 3: mPass_approval(); break;
			case 3: analytic(); break;
			case 4: report(); break; 
			case 5: break;
			default: cout<<"Invalid Option! Please Enter the Number."<<endl;
		}
	}while (adminChoice != 5);

	return 0;
}

void view_app(){
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

void app_approval(){
	cout<<"----------Application Approval----------"<<endl;
	
	int appID;
	char app_dec;
	string reason;  //getline() only can allow string

	cout<<"Enter Application ID: ";
    cin>>appID;
	
	/* if approve
		-
	   if reject 	
		- studID cannot be found in the txt (student detail)  */
		
    do{
    	cout<<"Approve or Reject (A/R): ";
    	cin>>app_dec;
    	
    	if(app_dec=='a' || app_dec=='A'){
			cout<<"The Application is approve"<<endl;
		}
		
		else if(app_dec=='r' || app_dec=='R'){
			cin.ignore();  //cleans leftover garbage before reading next input
		    cout<<"Enter reason: ";
			getline(cin, reason);  //getline() = captures full sentences
			cout<<"The reason is: "<<reason<<endl;
		}
		
		else{
			cout<<"Invalid Input! Enter only alphabet A or R"<<endl;
		}
		
	}while(app_dec!='a' && app_dec!='A' && app_dec!='r' && app_dec!='R');
   
//    cout<<"Approve or Reject (A/R): ";
//    cin>>app_dec;
//    	
//    while(app_dec!='a' && app_dec!='A' && app_dec!='r' && app_dec!='R'){
//    	cout<<"Invalid Input! Enter only alphabet A or R"<<endl;
//    	cout<<"Approve or Reject (A/R): ";
//    	cin>>app_dec;
//	}
//
//	if(app_dec=='a' || app_dec=='A'){
//			cout<<"The Application is approve"<<endl;
//	}
//	
//	else{
//		cin.ignore();  //cleans leftover garbage before reading next input
//	    cout<<"Enter reason: ";
//		getline(cin, reason);  //getline() = captures full sentences
//		cout<<"The reason is: "<<reason;
//	}
}

void mPass_approval(){
	cout<<"----------Monthly Pass Approval----------"<<endl;
	
	/* if approve
		- 
	   if reject
		- the student haven't completed the last payment */
}

void analytic(){
	cout<<"Analytics"<<endl;
}
void report(){
	cout<<"Reports"<<endl;
}
