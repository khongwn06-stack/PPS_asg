#include <iostream>
#include <unistd.h> //use sleep() for loading screen
#include <cstdlib>
#include <limits> //limited output after enter Alphabet
#include <fstream> //use ifstream(input txt file stream) and ofstream(output txt file stream)
#include <sstream> //extracting data from a txt file
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
		cout<<"3. Analytics"<<endl;
		cout<<"4. Reports"<<endl;
		cout<<"5. Exit"<<endl;
		
		cout<<"Enter your option : ";
		cin>>adminChoice;
		
		switch(adminChoice){
			case 1: view_app(); break;
			case 2: app_approval(); break;
			case 3: analytic(); break;
			case 4: report(); break; 
			case 5: break;
			default: cout<<"Invalid Option! Please Enter the Number."<<endl;
		}
	}while (adminChoice != 5);

	return 0;
}

void view_app(){
	cout<<"View Application"<<endl;
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
