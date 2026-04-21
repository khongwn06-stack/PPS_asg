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
           applications[i].status == "Approved" &&
           applications[i].payment == "Unpaid")
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

	while(true)
	{
    cout << "\nSelect Payment Method:\n";
    cout << "1. TNG QR\n";
    cout << "2. DuitNow QR\n";
    cout << "3. Credit/Debit Card\n";
    cout << "4. International Card\n";
    cout << "Enter choice: ";
    cin >> opt;
    
    if(cin.fail())
    {
    	cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a number between 1-4.\n";
        continue;	
	}

    switch(opt)
    {
        case 1: return "TNG QR";
        case 2: return "DuitNow QR";
        case 3: return "Card";
        case 4: return "International Card";
        default: cout << "Invalid method! Please try again.\n";
    }
	}	
}


// MAIN PAYMENT MENU (after login)
void paymentMenu(int studentIndex)
{
    string sid = students[studentIndex].id;

    cout << "\n====================================\n";
    cout << "|           PAYMENT MODULE         |\n";
    cout << "====================================\n";

    // ===== CHECK APPROVAL =====
    if(!checkApproval(sid))
    {
        cout << "No APPROVED application or already PAID!\n";
        return;
    }

    cout << "Application Approved! Proceeding...\n";

    int months;

    cout << "Enter number of months: ";
    cin >> months;

    if(months <= 0)
    {
        cout << "Invalid input!\n";
        return;
    }

    // ===== MONTHLY PAYMENT =====
    double monthlyPayment = months * 30;

    // ===== DAILY COMPARISON (6–9 HOURS) =====
    int minH = 6, maxH = 9;

    double minDaily = minH * 0.5 * 30 * months;
    double maxDaily = maxH * 0.5 * 30 * months;

    double minSave = minDaily - monthlyPayment;
    double maxSave = maxDaily - monthlyPayment;

    // ===== PAYMENT METHOD =====
    string method = choosePaymentMethod();

    if(method == "Invalid")
    {
        cout << "Invalid method!\n";
        return;
    }

    // ===== GENERATE PAYMENT ID =====
    string pid = generatePaymentID();

    // ===== DATE TIME =====
    string dateTime = getCurrentDateTime();

    // ===== UPDATE APPLICATION STATUS =====
    for(int i = 0; i < applicationCount; i++)
    {
        if(applications[i].studentID == sid &&
           applications[i].status == "Approved" &&
           applications[i].payment == "Unpaid")
        {
            applications[i].payment = "Paid";
        }
    }

    // ===== SAVE BACK TO FILE =====
    ofstream file("applications.txt");

    for(int i = 0; i < applicationCount; i++)
    {
        file << applications[i].appID << ","
             << applications[i].studentID << ","
             << applications[i].status << ","
             << applications[i].month << ","
             << applications[i].payment << endl;
    }

    file.close();

    // ===== SAVE PAYMENT RECORD =====
    ofstream payFile("payments.txt", ios::app);

    payFile << pid << ","
            << sid << ","
            << method << ","
            << months << ","
            << monthlyPayment << ","
            << dateTime << endl;

    payFile.close();

    // ============= RECEIPT ================
    cout << "\n====================================\n";
    cout << "|              RECEIPT             |\n";
    cout << "====================================\n";

    cout << "Payment ID   : " << pid << endl;
    cout << "Student ID   : " << sid << endl;
    cout << "Months       : " << months << endl;
    cout << "Amount Paid  : RM" << monthlyPayment << endl;
    cout << "Method       : " << method << endl;
    cout << "Date & Time  : " << dateTime << endl;

    cout << "\n--- Cost Comparison ---\n";
    cout << "Daily (6 to 9 hours): RM" << minDaily << " - RM" << maxDaily << endl;
    cout << "You saved: RM" << minSave << " - RM" << maxSave << endl;

    cout << "====================================\n";
}

// Home page direct access 
void paymentModule()
{
    cout << "\nPlease login through Student Module to proceed.\n";
}
