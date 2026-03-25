#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits> //limited output after enter Alphabet

using namespace std;

//Login Part
bool home_page();

//Student Part
void newReg();
void stud_login();

//Admin Part
bool admin_login();
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
	
	do{
	cout<<"\nUTAR Monthly Car Parking Pass System"<<endl<<endl;
	cout<<"--------Home Page--------"<<endl<<endl;
	cout<<"Select an option : "<<endl;
	cout<<"1. New Registration"<<endl;
	cout<<"2. Student Login"<<endl;
	cout<<"3. Admin Login"<<endl;
	cout<<"4. Exit System"<<endl<<endl;
	cout<<"Enter your option : ";
	cin>>homeChoice;
	
	switch(homeChoice){
		case 1: newReg(); break;
		case 2: stud_login(); break;
		case 3: admin_login(); break;
		case 4: exit_system(); break;
		default:limit_input(); cout<<"Invalid Option! Please Enter the Number 1-4."<<endl;
	}
	}while (homeChoice != 4);
}

//Student Part
void newReg(){
	cout<<"New Registration Page"<<endl;
}
void stud_login(){
	cout<<"Student Page"<<endl;
}

//Admin Part
bool admin_login()
{
	loading_screen();
	
	int adminChoice;
	clear_screen();
	
	do{
	cout<<"\n--------Admin Page--------"<<endl<<endl;
	cout<<"Select an option : "<<endl;
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
