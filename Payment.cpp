#include <iostream>
#include <string> 
#include <ctime>
using namespace std;


//========================================Payment Part========================================
void paymentModule();
void paymentMenu(int studentIndex);
bool checkApproval(string studentID);
string generatePaymentID();
string getCurrentDateTime();
string choosePaymentMethod();


//========================================Payment Part========================================
// Generate Payment ID
string generatePaymentID()
{
    static int count = 1;
    return "PID" + intToString(1000 + count++);
}


// Get current date & time
string getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int year  = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day   = ltm->tm_mday;

    int hour = ltm->tm_hour;
    int min  = ltm->tm_min;

    stringstream ss;

    ss << year << "-"
       << (month < 10 ? "0" : "") << month << "-"
       << (day < 10 ? "0" : "") << day << " "
       << (hour < 10 ? "0" : "") << hour << ":"
       << (min < 10 ? "0" : "") << min;

    return ss.str();
}


// Check if student has APPROVED application
bool checkApproval(string studentID)
{
    for(int i = 0; i < applicationCount; i++)
    {
        if(applications[i].studentID == studentID &&
           applications[i].status == STATUS_APPROVED &&
           applications[i].payment != STATUS_PAID)
        {
            return true;
        }
    }
    return false;
}


// Choose payment method
string choosePaymentMethod()
{
    int opt;

    cout << "\n========================================\n";
    cout << "|        SELECT PAYMENT METHOD         |\n";
    cout << "========================================\n";
    cout << "| 1. TNG QR                           |\n";
    cout << "| 2. DuitNow QR                       |\n";
    cout << "| 3. Credit/Debit Card                |\n";
    cout << "| 4. International Card               |\n";
    cout << "========================================\n";
    cout << "Enter choice: ";
    cin >> opt;

    switch(opt)
    {
        case 1: return "TNG QR";
        case 2: return "DuitNow QR";
        case 3: return "Card";
        case 4: return "International Card";
        default: return "Invalid Method! Please choose the payment method between 1-4";
    }
}


// MAIN PAYMENT MENU (after login)
void paymentMenu(int studentIndex)
{
    string sid = students[studentIndex].id;

    cout << "\n========================================\n";
    cout << "|               PAYMENT                |\n";
    cout << "========================================\n";

    bool found = false;

    for(int i = 0; i < applicationCount; i++)
    {
        if(applications[i].studentID == sid)
        {
            found = true;

            // ===== SHOW APPLICATION =====
            cout << "| Application ID : " << applications[i].appID << endl;
            cout << "| Month          : " << applications[i].month << endl;
            cout << "| Status         : " << applications[i].status << endl;
            cout << "| Payment        : " << applications[i].payment << endl;
            cout << "----------------------------------------\n";

            // ===== CASE 1: PENDING =====
            if(applications[i].status == STATUS_PENDING)
            {
                cout << "? Your application is still processing.\n";
                cout << "Please wait 3–5 working days.\n";
                return;
            }

            // ===== CASE 2: REJECTED =====
            if(applications[i].status == STATUS_REJECTED)
            {
                cout << "? Your application was rejected.\n";
                return;
            }

            // ===== CASE 3: APPROVED + UNPAID =====
            if(applications[i].status == STATUS_APPROVED &&
               applications[i].payment != STATUS_PAID)
            {
                string month;

                // ===== INPUT MONTH (VALIDATION LOOP) =====
                while(true)
                {
                    cout << "\nEnter the month you request for pass (YYYY-MM or 0 to cancel): ";
                    cin >> month;

                    if(month == "0") return;

                    if(month.length() == 7 && month[4] == '-')
                    {
                        int m = stoi(month.substr(5,2));

                        if(m >= 1 && m <= 12)
                        {
                            break; // valid
                        }
                    }

                    cout << "? Invalid month format! Try again.\n";
                }

                // ===== PAYMENT METHOD LOOP =====
                string method;
                while(true)
                {
                    method = choosePaymentMethod();

                    if(method != "Invalid")
                        break;

                    cout << "? Invalid method! Please choose again.\n";
                }

                // ===== PAYMENT PROCESS =====
                double amount = 40.0;
                string pid = generatePaymentID();
                string dateTime = getCurrentDateTime();

                applications[i].payment = STATUS_PAID;

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
                cout << "\n========================================\n";
                cout << "|              RECEIPT                |\n";
                cout << "========================================\n";
                cout << "| Payment ID   : " << pid << endl;
                cout << "| Student ID   : " << sid << endl;
                cout << "| Month        : " << month << endl;
                cout << "| Amount Paid  : RM" << amount << endl;
                cout << "| Method       : " << method << endl;
                cout << "| Date & Time  : " << dateTime << endl;
                cout << "========================================\n";

                return;
            }

            // ===== CASE 4: ALREADY PAID =====
            if(applications[i].payment == STATUS_PAID)
            {
                cout << "? Payment already completed.\n";
                return;
            }
        }
    }

    if(!found)
    {
        cout << "|        NO APPLICATION FOUND!         |\n";
        cout << "========================================\n";
    }
}
