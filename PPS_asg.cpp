#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits> //limited output after enter Alphabet
#include <fstream> //use ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
using namespace std;

//Login Part
bool home_page();

//Student Part
void newReg_page();
void stud_page();

//Admin Part
bool admin_login();
bool admin_page();
void app_approval();
void analytic();
void report();

//Exit System
void exit_system();

//Addition
void limit_input();
void clear_screen(); //clear sreen after move to the new page
void loading_screen(); //loading for 3 second 

//Main Menu
int main()
{
	home_page();
	return 0;
}

//Addition
void limit_input(){ //limited output after enter Alphabet
	if(cin.fail()){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
} 

void clear_screen(){
	cin.get();
	system("cls"); //clear screen
}

void loading_screen(){
	cout<<"\nLoading";
    for(int i=0; i<3; i++){ //loading for 3 second
        cout<<".";
        sleep(1); //one "." for one second
    }
    cout<<"\nDone!"<<endl;
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
			case 3: admin_page(); break;
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

//Admin Part
bool admin_login()
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
	    while(getline(file, line)){
	        stringstream ss(line);
	        getline(ss, id, '|');
	        getline(ss, username, '|');
	        getline(ss, password, '|');
	
	        if(adminUsername != username || adminPassword != password){
	            cout<<"Login Failed! Please enter again: "<<endl<<endl;
	        }
	    }
	}while(adminUsername != username || adminPassword != password);
	cout<<"Login Successful!"<<endl;
}

bool admin_page()
{
	admin_login();
	loading_screen();
	
	int adminChoice;
	clear_screen();
	
	cout<<"----------Admin Page----------"<<endl;
	do{
		cout<<"\nSelect an option : "<<endl;
		cout<<"1. Application Approval"<<endl;
		cout<<"2. Analytics"<<endl;
		cout<<"3. Reports"<<endl;
		cout<<"4. Back to Home Page"<<endl;
		cout<<"5. Exit System"<<endl<<endl;
		
		cout<<"Enter your option : ";
		cin>>adminChoice;
		
		switch(adminChoice){
			case 1: app_approval(); break;
			case 2: analytic(); break;
			case 3: report(); break;
			case 4:	loading_screen(); clear_screen(); home_page(); break;
			case 5: exit_system(); break;
			default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-5."<<endl;
		}
	}while (adminChoice != 5);
}

void app_approval(){
	cout<<"Application Approval"<<endl;
}
void analytic(){
	cout<<"Analytics"<<endl;
}
void report(){
	cout<<"Reports"<<endl;
}

//Exit System
void exit_system(){
	loading_screen();
	cout<<"\nHave A Good Day! Thank You!"<<endl;
	exit(0);
}
