#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;


// Generate Payment ID 
string generatePaymentID()
{
    static int count = 1000;
    count++;

    stringstream ss;
    ss << "P" << count;

    return ss.str();
}


// Save to File 
void savePayment(string pid, string sid, string type,
                 int hours, int months, double amount)
{
    ofstream file("payments.txt", ios::app);

    file << pid << ","
         << sid << ","
         << type << ","
         << hours << ","
         << months << ","
         << amount << endl;

    file.close();
}


// Daily Payment 
void dailyPayment()
{
    string studentID;
    int hours;

    cout<<"\n--- DAILY PAYMENT ---\n";

    cout<<"Enter Student ID: ";
    cin>>studentID;

    cout<<"Enter hours parked (1 - 10): ";
    cin>>hours;

    // validation
    if(hours < 1 || hours > 10)
    {
        cout<<"Invalid hours!\n";
        return;
    }

    double amount = hours * 0.5;

    string pid = generatePaymentID();

    savePayment(pid, studentID, "DAILY", hours, 0, amount);

    cout<<"\nPayment Successful!\n";

    cout<<"===== RECEIPT =====\n";
    cout<<"Payment ID: "<<pid<<endl;
    cout<<"Student ID: "<<studentID<<endl;
    cout<<"Type: DAILY\n";
    cout<<"Hours: "<<hours<<endl;
    cout<<"Amount: RM"<<amount<<endl;
    cout<<"===================\n";
}


// Monthly Payment 
void monthlyPayment()
{
    string studentID;
    int months;

    cout<<"\n--- MONTHLY PAYMENT ---\n";

    cout<<"Enter Student ID: ";
    cin>>studentID;

    cout<<"Enter number of months: ";
    cin>>months;

    if(months <= 0)
    {
        cout<<"Invalid months!\n";
        return;
    }

    double amount = months * 100;

    string pid = generatePaymentID();

    savePayment(pid, studentID, "MONTHLY", 0, months, amount);

    cout<<"\nPayment Successful!\n";

    cout<<"===== RECEIPT =====\n";
    cout<<"Payment ID: "<<pid<<endl;
    cout<<"Student ID: "<<studentID<<endl;
    cout<<"Type: MONTHLY\n";
    cout<<"Months: "<<months<<endl;
    cout<<"Amount: RM"<<amount<<endl;
    cout<<"===================\n";
}


// View Payment History 
void viewPaymentHistory()
{
    string id, line;

    cout<<"\nEnter Student ID: ";
    cin>>id;

    ifstream file("payments.txt");

    cout<<"\n=== PAYMENT HISTORY ===\n";

    while(getline(file, line))
    {
        if(line.find(id) != string::npos)
        {
            cout<<line<<endl;
        }
    }

    file.close();
}


// Payment Menu
void paymentMenu()
{
    int choice;

    do
    {
        cout<<"\n===== PAYMENT MENU =====\n";
        cout<<"1. Daily Payment\n";
        cout<<"2. Monthly Payment\n";
        cout<<"3. View Payment History\n";
        cout<<"4. Exit\n";

        cout<<"Enter choice: ";
        cin>>choice;

        if(choice == 1)
            dailyPayment();

        else if(choice == 2)
            monthlyPayment();

        else if(choice == 3)
            viewPaymentHistory();

    } while(choice != 4);
}


// MAIN 
int main()
{
    paymentMenu();
    return 0;
}
