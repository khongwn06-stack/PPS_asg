#include <iostream>
#include <string> 
#include <ctime>
using namespace std;


const string STATUS_PENDING  = "Pending";
const string STATUS_APPROVED = "Approved";
const string STATUS_REJECTED = "Rejected";
const string STATUS_PAID     = "Paid";


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

// Payment Choice
string choosePaymentMethod()
{
    int opt;

        cout<<"---------- Payment Choice ----------"<<endl;
        cout<<"| 1. TNG QR                        |"<<endl;
        cout<<"| 2. DuitNow QR                    |"<<endl;
        cout<<"| 3. Credit / Debit Card           |"<<endl;
        cout<<"| 4. International Card            |"<<endl;
        cout<<"------------------------------------"<<endl;
        
        cout << "Enter choice: ";
        cin >> opt;

    switch(opt)
    {
        case 1: return "TNG QR";
        case 2: return "DuitNow QR";
        case 3: return "Card";
        case 4: return "International Card";
        default: return "Invalid Method! Please choose the payment method between 1-4.";
    }
}

// Payment Menu
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
                cout << "Your application is still processing.\n";
                cout << "Please wait 3–5 working days.\n";
                return;
            }

            // ===== CASE 2: REJECTED =====
            if(applications[i].status == STATUS_REJECTED)
            {
                cout << "Your application was rejected.\n";
                return;
            }

            // ===== CASE 3: APPROVED + UNPAID =====
            if(applications[i].status == STATUS_APPROVED &&
               applications[i].payment != STATUS_PAID)
            {
                string month;

                // ===== INPUT MONTH =====
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

                    cout << "Invalid month format! Try again.\n";
                }

                // ===== PAYMENT METHOD =====
                string method;
                while(true)
                {
                    method = choosePaymentMethod();

                    if(method != "Invalid")
                        break;

                    cout << "Invalid method! Please choose again.\n";
                }

                // ===== PAYMENT PROCESS =====
                double amount = 40.0;
                string pid = generatePaymentID();
                string dateTime = getCurrentDateTime();

                applications[i].payment = STATUS_PAID;
                
                
                // ===== DAILY COMPARISON (6–9 HOURS) =====
                int minH = 6, maxH = 9;

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
                
                // Header 
    			/*if(isFileEmpty())
    			{
        		file << "No | PID | SID | Type | Hours | Months | Amount | Method | DateTime\n";
        		file << "--------------------------------------------------------------------------\n";
    			}

    			file << no << " | "
         		<< pid << " | "
         		<< studentID << " | "
         		<< "MONTHLY" << " | "
         		<< hours << " | "
         		<< months << " | "
         		<< amount << " | "
         		<< method << " | "
         		<< datetime;

    			file.close();
				}*/


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
            if(applications[i].payment == STATUS_PAID)
            {
                cout << "Payment already completed.\n";
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

