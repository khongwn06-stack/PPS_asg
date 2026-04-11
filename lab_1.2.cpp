/*
 * ============================================================
 * MPKJ Monthly Car Parking Pass Management System for UTAR Students
 * Module: Student Transaction Tracking / Analysis / Alerts
 * ============================================================
 * This module handles:
 *   1. Students can track & analyse their historical transactions
 *      on car parking pass
 *   2. System displays an alert to user when approaching month end,
 *      to remind the user for next month renewal
 * ============================================================
 * Shared data structures and file formats are designed to integrate
 * with other team members' modules (registration, login, application,
 * admin processing).
 * ============================================================
 */

#define _CRT_SECURE_NO_WARNINGS   // Suppress MSVC deprecation warnings

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <cstdlib>

using namespace std;

// ============================================================
// Constants
// ============================================================
const int MAX_STUDENTS = 200;
const int MAX_APPLICATIONS = 500;
const int MAX_TRANSACTIONS = 1000;

const string STUDENTS_FILE = "D:\\Microsoft Visual Studio\\PROJECT\\Project1.2 groupAssignment 1104\\students.txt";
const string APPLICATIONS_FILE = "D:\\Microsoft Visual Studio\\PROJECT\\Project1.2 groupAssignment 1104\\applications.txt";
const string TRANSACTIONS_FILE = "D:\\Microsoft Visual Studio\\PROJECT\\Project1.2 groupAssignment 1104\\transactions.txt";

// UTAR Sungai Long campus faculties/institutes
const int NUM_FACULTIES = 9;
const string FACULTIES[NUM_FACULTIES] = {
    "FICT",    // Faculty of Information and Communication Technology
    "FES",     // Faculty of Engineering and Science
    "FAM",     // Faculty of Accountancy and Management
    "FAS",     // Faculty of Arts and Social Science
    "ICS",     // Institute of Chinese Studies
    "FCCI",    // Faculty of Communication and Creative Industries
    "FSC",     // Faculty of Science
    "LKCFES",  // Lee Kong Chian Faculty of Engineering and Science
    "CFS"      // Centre for Foundation Studies
};

// Month names for display (index 0 = January, index 11 = December)
const string MONTH_NAMES[12] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

// Safe helper: get month name with bounds check
string getMonthName(int month) {
    if (month >= 1 && month <= 12)
        return MONTH_NAMES[month - 1];
    return "Unknown";
}

// ============================================================
// Data Structures (shared with other modules)
// All numeric members initialized to 0 to fix C26495 warnings
// ============================================================

// Student information
struct Student {
    string studentID;
    string name;
    string password;
    string contactNo;
    string vehiclePlateNo;
    string vehicleType;     // "Car" or "Motorcycle"
    string faculty;
    string programme;
};

// Parking pass application (created by application module)
struct Application {
    string appID;
    string studentID;
    int applyMonth;         // 1-12
    int applyYear;
    int numMonths;          // 1 to 3 (advance renewal allowed)
    string status;          // "Pending", "Approved", "Rejected"
    string applyDate;       // "DD/MM/YYYY"
    double fee;

    // Constructor: initialize all numeric members to 0
    Application() : applyMonth(0), applyYear(0), numMonths(0), fee(0.0) {}
};

// Transaction record (created after payment for approved application)
struct Transaction {
    string transID;
    string studentID;
    string appID;
    double amountPaid;
    string paymentDate;     // "DD/MM/YYYY"
    int passMonth;          // Month the pass is valid for
    int passYear;           // Year the pass is valid for
    string faculty;

    // Constructor: initialize all numeric members to 0
    Transaction() : amountPaid(0.0), passMonth(0), passYear(0) {}
};

// ============================================================
// Global Arrays & Counters
// ============================================================
Student students[MAX_STUDENTS];
int studentCount = 0;

Application applications[MAX_APPLICATIONS];
int appCount = 0;

Transaction transactions[MAX_TRANSACTIONS];
int transCount = 0;

// ============================================================
// Utility Functions
// ============================================================

// Get current system date safely
void getCurrentDate(int& day, int& month, int& year) {
    time_t now = time(0);
    tm timeInfo;

    // Use localtime_s on MSVC (safe), fallback to localtime on other compilers
#ifdef _MSC_VER
    localtime_s(&timeInfo, &now);
#else
    tm* ltm = localtime(&now);
    timeInfo = *ltm;
#endif

    day = timeInfo.tm_mday;
    month = timeInfo.tm_mon + 1;
    year = timeInfo.tm_year + 1900;
}

// Format date as DD/MM/YYYY
string formatDate(int day, int month, int year) {
    ostringstream oss;
    oss << setfill('0') << setw(2) << day << "/"
        << setfill('0') << setw(2) << month << "/"
        << year;
    return oss.str();
}

// Get number of days in a given month/year
int getDaysInMonth(int month, int year) {
    switch (month) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
            return 29;
        else
            return 28;
    default:
        return 30;
    }
}

// Generate a unique ID with prefix and number
string generateID(const string& prefix, int num) {
    ostringstream oss;
    oss << prefix << setfill('0') << setw(3) << num;
    return oss.str();
}

// Print a decorative line
void printLine(char ch = '=', int width = 65) {
    for (int i = 0; i < width; i++) cout << ch;
    cout << endl;
}

// Print section header
void printHeader(const string& title) {
    cout << endl;
    printLine('=');
    cout << "  " << title << endl;
    printLine('=');
}

// Clear input buffer safely
void clearInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

// Pause and wait for Enter key
void pauseScreen() {
    cout << "\n  Press Enter to continue...";
    clearInput();
    cin.get();
}

// ============================================================
// File I/O Functions
// ============================================================

// Load students from text file
// Format: studentID|name|password|contactNo|plateNo|vehicleType|faculty|programme
void loadStudents() {
    ifstream fin(STUDENTS_FILE.c_str());
    if (!fin.is_open()) {
        cout << "  [Info] " << STUDENTS_FILE << " not found.\n";
        return;
    }
    studentCount = 0;
    string line;
    while (getline(fin, line) && studentCount < MAX_STUDENTS) {
        if (line.empty()) continue;
        stringstream ss(line);
        Student& s = students[studentCount];
        getline(ss, s.studentID, '|');
        getline(ss, s.name, '|');
        getline(ss, s.password, '|');
        getline(ss, s.contactNo, '|');
        getline(ss, s.vehiclePlateNo, '|');
        getline(ss, s.vehicleType, '|');
        getline(ss, s.faculty, '|');
        getline(ss, s.programme, '|');
        studentCount++;
    }
    fin.close();
}

// Save students to text file
void saveStudents() {
    ofstream fout(STUDENTS_FILE.c_str());
    for (int i = 0; i < studentCount; i++) {
        Student& s = students[i];
        fout << s.studentID << "|" << s.name << "|" << s.password << "|"
            << s.contactNo << "|" << s.vehiclePlateNo << "|" << s.vehicleType << "|"
            << s.faculty << "|" << s.programme << "\n";
    }
    fout.close();
}

// Load applications from text file
// Format: appID|studentID|applyMonth|applyYear|numMonths|status|applyDate|fee
void loadApplications() {
    ifstream fin(APPLICATIONS_FILE.c_str());
    if (!fin.is_open()) {
        cout << "  [Info] " << APPLICATIONS_FILE << " not found.\n";
        return;
    }
    appCount = 0;
    string line;
    while (getline(fin, line) && appCount < MAX_APPLICATIONS) {
        if (line.empty()) continue;
        stringstream ss(line);
        Application& a = applications[appCount];
        string temp;
        getline(ss, a.appID, '|');
        getline(ss, a.studentID, '|');
        getline(ss, temp, '|'); a.applyMonth = atoi(temp.c_str());
        getline(ss, temp, '|'); a.applyYear = atoi(temp.c_str());
        getline(ss, temp, '|'); a.numMonths = atoi(temp.c_str());
        getline(ss, a.status, '|');
        getline(ss, a.applyDate, '|');
        getline(ss, temp, '|'); a.fee = atof(temp.c_str());
        appCount++;
    }
    fin.close();
}

// Save applications to text file
void saveApplications() {
    ofstream fout(APPLICATIONS_FILE.c_str());
    for (int i = 0; i < appCount; i++) {
        Application& a = applications[i];
        fout << a.appID << "|" << a.studentID << "|" << a.applyMonth << "|"
            << a.applyYear << "|" << a.numMonths << "|" << a.status << "|"
            << a.applyDate << "|" << a.fee << "\n";
    }
    fout.close();
}

// Load transactions from text file
// Format: transID|studentID|appID|amountPaid|paymentDate|passMonth|passYear|faculty
void loadTransactions() {
    ifstream fin(TRANSACTIONS_FILE.c_str());
    if (!fin.is_open()) {
        cout << "  [Info] " << TRANSACTIONS_FILE << " not found.\n";
        return;
    }
    transCount = 0;
    string line;
    while (getline(fin, line) && transCount < MAX_TRANSACTIONS) {
        if (line.empty()) continue;
        stringstream ss(line);
        Transaction& t = transactions[transCount];
        string temp;
        getline(ss, t.transID, '|');
        getline(ss, t.studentID, '|');
        getline(ss, t.appID, '|');
        getline(ss, temp, '|'); t.amountPaid = atof(temp.c_str());
        getline(ss, t.paymentDate, '|');
        getline(ss, temp, '|'); t.passMonth = atoi(temp.c_str());
        getline(ss, temp, '|'); t.passYear = atoi(temp.c_str());
        getline(ss, t.faculty, '|');
        transCount++;
    }
    fin.close();
}

// Save transactions to text file
void saveTransactions() {
    ofstream fout(TRANSACTIONS_FILE.c_str());
    for (int i = 0; i < transCount; i++) {
        Transaction& t = transactions[i];
        fout << t.transID << "|" << t.studentID << "|" << t.appID << "|"
            << t.amountPaid << "|" << t.paymentDate << "|"
            << t.passMonth << "|" << t.passYear << "|" << t.faculty << "\n";
    }
    fout.close();
}

// Load all data files
void loadAllData() {
    loadStudents();
    loadApplications();
    loadTransactions();
}

// ============================================================
// Helper: Find student index by ID (-1 if not found)
// ============================================================
int findStudentIndex(const string& studentID) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentID == studentID)
            return i;
    }
    return -1;
}

// ============================================================
// FEATURE 1: View All Transaction History
// ============================================================
void viewAllTransactions(const string& studentID) {
    printHeader("MY TRANSACTION HISTORY");

    int sIdx = findStudentIndex(studentID);
    if (sIdx != -1) {
        cout << "  Student : " << students[sIdx].name
            << " (" << studentID << ")\n";
        cout << "  Faculty : " << students[sIdx].faculty << endl;
        cout << "  Vehicle : " << students[sIdx].vehiclePlateNo
            << " (" << students[sIdx].vehicleType << ")\n";
    }
    printLine('-');

    // Table header
    cout << "  " << left
        << setw(10) << "Trans ID"
        << setw(10) << "App ID"
        << setw(12) << "Amount(RM)"
        << setw(14) << "Payment Date"
        << setw(16) << "Pass Period" << endl;
    printLine('-');

    int myCount = 0;
    double totalSpent = 0.0;

    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID) {
            Transaction& t = transactions[i];
            cout << "  " << left
                << setw(10) << t.transID
                << setw(10) << t.appID
                << setw(12) << fixed << setprecision(2) << t.amountPaid
                << setw(14) << t.paymentDate
                << getMonthName(t.passMonth) << " " << t.passYear << endl;
            totalSpent += t.amountPaid;
            myCount++;
        }
    }

    if (myCount == 0) {
        cout << "\n  No transaction records found.\n";
        cout << "  You have not made any parking pass payments yet.\n";
    }
    else {
        printLine('-');
        cout << "  Total Records: " << myCount
            << "    |    Total Spent: RM "
            << fixed << setprecision(2) << totalSpent << endl;
    }
}

// ============================================================
// FEATURE 2: Filter Transactions by Year
// ============================================================
void viewTransactionsByYear(const string& studentID) {
    printHeader("TRANSACTION HISTORY BY YEAR");

    // Collect all unique years this student has transactions in
    const int MAX_YEARS = 50;
    int years[MAX_YEARS];
    // Initialize array to zero to prevent uninitialized memory warnings
    for (int i = 0; i < MAX_YEARS; i++) years[i] = 0;
    int yearCount = 0;

    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID) {
            bool exists = false;
            for (int j = 0; j < yearCount; j++) {
                if (years[j] == transactions[i].passYear) {
                    exists = true;
                    break;
                }
            }
            if (!exists && yearCount < MAX_YEARS) {
                years[yearCount++] = transactions[i].passYear;
            }
        }
    }

    if (yearCount == 0) {
        cout << "  No transaction records found.\n";
        return;
    }

    // Sort years ascending using bubble sort
    for (int i = 0; i < yearCount - 1; i++) {
        for (int j = 0; j < yearCount - i - 1; j++) {
            if (years[j] > years[j + 1]) {
                int temp = years[j];
                years[j] = years[j + 1];
                years[j + 1] = temp;
            }
        }
    }

    // Show available years
    cout << "  Available years: ";
    for (int i = 0; i < yearCount; i++) {
        cout << years[i];
        if (i < yearCount - 1) cout << ", ";
    }
    cout << endl;

    int targetYear;
    cout << "\n  Enter year to view: ";
    cin >> targetYear;

    // Validate year exists in records
    bool validYear = false;
    for (int i = 0; i < yearCount; i++) {
        if (years[i] == targetYear) {
            validYear = true;
            break;
        }
    }
    if (!validYear) {
        cout << "  No transactions found for year " << targetYear << ".\n";
        return;
    }

    // Display filtered results
    cout << "\n  Transactions for Year " << targetYear << ":\n";
    printLine('-');
    cout << "  " << left
        << setw(10) << "Trans ID"
        << setw(10) << "App ID"
        << setw(12) << "Amount(RM)"
        << setw(14) << "Payment Date"
        << setw(16) << "Pass Month" << endl;
    printLine('-');

    int count = 0;
    double yearTotal = 0.0;
    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID &&
            transactions[i].passYear == targetYear) {
            Transaction& t = transactions[i];
            cout << "  " << left
                << setw(10) << t.transID
                << setw(10) << t.appID
                << setw(12) << fixed << setprecision(2) << t.amountPaid
                << setw(14) << t.paymentDate
                << getMonthName(t.passMonth) << endl;
            yearTotal += t.amountPaid;
            count++;
        }
    }
    printLine('-');
    cout << "  Records: " << count
        << "    |    Year Total: RM "
        << fixed << setprecision(2) << yearTotal << endl;
}

// ============================================================
// FEATURE 3: Filter Transactions by Specific Month
// ============================================================
void viewTransactionsByMonth(const string& studentID) {
    printHeader("TRANSACTION HISTORY BY MONTH");

    int targetMonth, targetYear;
    cout << "  Enter month (1-12): ";
    cin >> targetMonth;
    if (targetMonth < 1 || targetMonth > 12) {
        cout << "  Invalid month. Please enter 1-12.\n";
        return;
    }
    cout << "  Enter year (e.g., 2026): ";
    cin >> targetYear;

    cout << "\n  Transactions for " << getMonthName(targetMonth)
        << " " << targetYear << ":\n";
    printLine('-');
    cout << "  " << left
        << setw(10) << "Trans ID"
        << setw(10) << "App ID"
        << setw(12) << "Amount(RM)"
        << setw(14) << "Payment Date" << endl;
    printLine('-');

    int count = 0;
    double monthTotal = 0.0;
    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID &&
            transactions[i].passMonth == targetMonth &&
            transactions[i].passYear == targetYear) {
            Transaction& t = transactions[i];
            cout << "  " << left
                << setw(10) << t.transID
                << setw(10) << t.appID
                << setw(12) << fixed << setprecision(2) << t.amountPaid
                << setw(14) << t.paymentDate << endl;
            monthTotal += t.amountPaid;
            count++;
        }
    }

    if (count == 0) {
        cout << "  No transactions found for this period.\n";
    }
    else {
        printLine('-');
        cout << "  Records: " << count
            << "    |    Month Total: RM "
            << fixed << setprecision(2) << monthTotal << endl;
    }
}

// ============================================================
// FEATURE 4: Spending Analysis & Statistics
// ============================================================
void viewSpendingAnalysis(const string& studentID) {
    printHeader("MY SPENDING ANALYSIS & STATISTICS");

    int sIdx = findStudentIndex(studentID);
    if (sIdx != -1) {
        cout << "  Student : " << students[sIdx].name << " (" << studentID << ")\n";
        cout << "  Faculty : " << students[sIdx].faculty << endl;
    }

    // Gather all transactions for this student
    int myTransCount = 0;
    double totalSpent = 0.0;
    double highestPayment = 0.0;
    double lowestPayment = 999999.0;
    int earliestMonth = 13, earliestYear = 9999;
    int latestMonth = 0, latestYear = 0;

    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID) {
            Transaction& t = transactions[i];
            myTransCount++;
            totalSpent += t.amountPaid;

            if (t.amountPaid > highestPayment) highestPayment = t.amountPaid;
            if (t.amountPaid < lowestPayment)  lowestPayment = t.amountPaid;

            // Track earliest pass period
            if (t.passYear < earliestYear ||
                (t.passYear == earliestYear && t.passMonth < earliestMonth)) {
                earliestMonth = t.passMonth;
                earliestYear = t.passYear;
            }
            // Track latest pass period
            if (t.passYear > latestYear ||
                (t.passYear == latestYear && t.passMonth > latestMonth)) {
                latestMonth = t.passMonth;
                latestYear = t.passYear;
            }
        }
    }

    if (myTransCount == 0) {
        cout << "\n  No transaction records found for analysis.\n";
        return;
    }

    double averageSpending = totalSpent / myTransCount;

    // ---- Section A: Overall Summary ----
    printLine('-');
    cout << "\n  [A] OVERALL SUMMARY\n\n";
    cout << "  Total Passes Purchased : " << myTransCount << endl;
    cout << "  Total Amount Spent     : RM " << fixed << setprecision(2) << totalSpent << endl;
    cout << "  Average Monthly Cost   : RM " << fixed << setprecision(2) << averageSpending << endl;
    cout << "  Highest Single Payment : RM " << fixed << setprecision(2) << highestPayment << endl;
    cout << "  Lowest Single Payment  : RM " << fixed << setprecision(2) << lowestPayment << endl;
    cout << "  First Pass Period      : " << getMonthName(earliestMonth) << " " << earliestYear << endl;
    cout << "  Latest Pass Period     : " << getMonthName(latestMonth) << " " << latestYear << endl;

    // Calculate coverage / gaps
    int totalPossibleMonths = (latestYear - earliestYear) * 12 + (latestMonth - earliestMonth) + 1;
    int missedMonths = totalPossibleMonths - myTransCount;
    if (missedMonths < 0) missedMonths = 0;

    cout << "\n  Months in Range        : " << totalPossibleMonths << endl;
    cout << "  Months with Pass       : " << myTransCount << endl;
    if (missedMonths > 0) {
        cout << "  Months WITHOUT Pass    : " << missedMonths
            << " (gaps in coverage)" << endl;
    }
    else {
        cout << "  Coverage               : Full (no gaps!)" << endl;
    }

    // ---- Section B: Yearly Breakdown Table ----
    cout << "\n  [B] YEARLY BREAKDOWN\n\n";

    // Collect unique years (initialized to zero)
    const int MAX_YEARS = 50;
    int years[MAX_YEARS];
    for (int i = 0; i < MAX_YEARS; i++) years[i] = 0;
    int yearCount = 0;

    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID) {
            bool exists = false;
            for (int j = 0; j < yearCount; j++) {
                if (years[j] == transactions[i].passYear) {
                    exists = true;
                    break;
                }
            }
            if (!exists && yearCount < MAX_YEARS) {
                years[yearCount++] = transactions[i].passYear;
            }
        }
    }
    // Sort ascending
    for (int i = 0; i < yearCount - 1; i++) {
        for (int j = 0; j < yearCount - i - 1; j++) {
            if (years[j] > years[j + 1]) {
                int temp = years[j];
                years[j] = years[j + 1];
                years[j + 1] = temp;
            }
        }
    }

    cout << "  " << left
        << setw(8) << "Year"
        << setw(10) << "Passes"
        << setw(16) << "Total Spent(RM)"
        << setw(14) << "Avg/Month(RM)" << endl;
    printLine('-', 50);

    for (int y = 0; y < yearCount; y++) {
        int cnt = 0;
        double spent = 0.0;
        for (int i = 0; i < transCount; i++) {
            if (transactions[i].studentID == studentID &&
                transactions[i].passYear == years[y]) {
                cnt++;
                spent += transactions[i].amountPaid;
            }
        }
        cout << "  " << left
            << setw(8) << years[y]
            << setw(10) << cnt
            << setw(16) << fixed << setprecision(2) << spent
            << setw(14) << fixed << setprecision(2) << (cnt > 0 ? spent / cnt : 0.0) << endl;
    }

    // ---- Section C: Monthly Calendar View (latest year) ----
    if (yearCount > 0) {
        int displayYear = years[yearCount - 1];
        cout << "\n  [C] MONTHLY PASS CALENDAR FOR " << displayYear << "\n\n";

        for (int m = 1; m <= 12; m++) {
            bool hasTrans = false;
            double mAmount = 0.0;
            for (int i = 0; i < transCount; i++) {
                if (transactions[i].studentID == studentID &&
                    transactions[i].passMonth == m &&
                    transactions[i].passYear == displayYear) {
                    hasTrans = true;
                    mAmount = transactions[i].amountPaid;
                    break;
                }
            }
            cout << "  " << left << setw(12) << getMonthName(m);
            if (hasTrans) {
                cout << "[ACTIVE] RM " << fixed << setprecision(2) << mAmount;
            }
            else {
                cout << "[  ---  ] No pass";
            }
            cout << endl;
        }
    }

    // ---- Section D: Application Status Summary ----
    cout << "\n  [D] MY APPLICATION STATUS SUMMARY\n\n";

    int appTotal = 0, appApproved = 0, appRejected = 0, appPending = 0;
    for (int i = 0; i < appCount; i++) {
        if (applications[i].studentID == studentID) {
            appTotal++;
            if (applications[i].status == "Approved")      appApproved++;
            else if (applications[i].status == "Rejected")  appRejected++;
            else                                            appPending++;
        }
    }
    if (appTotal > 0) {
        cout << "  Total Applications  : " << appTotal << endl;
        cout << "  Approved            : " << appApproved << endl;
        cout << "  Rejected            : " << appRejected << endl;
        cout << "  Pending             : " << appPending << endl;
        cout << "  Approval Rate       : "
            << fixed << setprecision(1)
            << ((double)appApproved / appTotal * 100.0) << "%" << endl;
    }
    else {
        cout << "  No applications on record.\n";
    }
}

// ============================================================
// FEATURE 5: Month-End Renewal Alert System
// ============================================================
// Checks if the student needs to renew for next month.
// Triggered automatically on login + callable from menu.
// Returns true if an alert was displayed, false otherwise.
bool checkRenewalAlert(const string& studentID) {
    int currentDay, currentMonth, currentYear;
    getCurrentDate(currentDay, currentMonth, currentYear);

    int daysInMonth = getDaysInMonth(currentMonth, currentYear);
    int daysRemaining = daysInMonth - currentDay;

    // Only show alert if within last 5 days of the month
    if (daysRemaining > 5) {
        return false;
    }

    // Determine next month/year
    int nextMonth = currentMonth + 1;
    int nextYear = currentYear;
    if (nextMonth > 12) {
        nextMonth = 1;
        nextYear++;
    }

    // Check if student already has a paid pass for next month
    bool hasNextMonthPass = false;
    for (int i = 0; i < transCount; i++) {
        if (transactions[i].studentID == studentID &&
            transactions[i].passMonth == nextMonth &&
            transactions[i].passYear == nextYear) {
            hasNextMonthPass = true;
            break;
        }
    }

    // If already covered, no alert needed
    if (hasNextMonthPass) {
        return false;
    }

    // Check if there is a pending or approved (but unpaid) application
    // that covers next month
    bool hasPendingForNextMonth = false;
    bool hasApprovedForNextMonth = false;

    for (int i = 0; i < appCount; i++) {
        if (applications[i].studentID == studentID) {
            for (int m = 0; m < applications[i].numMonths; m++) {
                int coveredMonth = applications[i].applyMonth + m;
                int coveredYear = applications[i].applyYear;
                if (coveredMonth > 12) {
                    coveredMonth -= 12;
                    coveredYear++;
                }
                if (coveredMonth == nextMonth && coveredYear == nextYear) {
                    if (applications[i].status == "Pending")
                        hasPendingForNextMonth = true;
                    else if (applications[i].status == "Approved")
                        hasApprovedForNextMonth = true;
                    break;
                }
            }
        }
    }

    // Display the alert box
    cout << endl;
    printLine('!');
    cout << "  *** MONTHLY PARKING PASS RENEWAL REMINDER ***\n";
    printLine('!');

    int sIdx = findStudentIndex(studentID);
    if (sIdx != -1) {
        cout << "  Dear " << students[sIdx].name << ",\n\n";
    }

    cout << "  Today's date     : "
        << formatDate(currentDay, currentMonth, currentYear) << endl;
    cout << "  Days remaining   : " << daysRemaining << " day(s) left in "
        << getMonthName(currentMonth) << endl;
    cout << "  Next month       : "
        << getMonthName(nextMonth) << " " << nextYear << endl;
    cout << endl;

    if (hasApprovedForNextMonth) {
        cout << "  STATUS: Your application for " << getMonthName(nextMonth)
            << " has been APPROVED!\n";
        cout << "  ACTION: Please complete your PAYMENT to activate the pass.\n";
    }
    else if (hasPendingForNextMonth) {
        cout << "  STATUS: Your application for " << getMonthName(nextMonth)
            << " is still PENDING review.\n";
        cout << "  ACTION: Please wait for admin approval. Check back soon.\n";
    }
    else {
        cout << "  STATUS: You have NO pass or application for "
            << getMonthName(nextMonth) << " " << nextYear << ".\n";
        cout << "  ACTION: Please APPLY or RENEW your monthly parking pass\n";
        cout << "          before the end of this month to avoid disruption!\n";
    }

    printLine('!');
    cout << endl;
    return true;
}

// ============================================================
// Student Tracking & Alerts Menu
// ============================================================
void studentTrackingMenu(const string& studentID) {
    // Show renewal alert automatically upon entering the menu
    checkRenewalAlert(studentID);

    int choice;
    do {
        printHeader("TRANSACTION TRACKING & ANALYSIS");

        int sIdx = findStudentIndex(studentID);
        if (sIdx != -1) {
            cout << "  Logged in as: " << students[sIdx].name
                << " (" << studentID << ")\n";
        }
        printLine('-');

        cout << "  1. View All Transaction History\n";
        cout << "  2. View Transactions by Year\n";
        cout << "  3. View Transactions by Month\n";
        cout << "  4. Spending Analysis & Statistics\n";
        cout << "  5. Check Renewal Alert\n";
        cout << "  0. Back / Logout\n";
        printLine('-');
        cout << "  Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            viewAllTransactions(studentID);
            pauseScreen();
            break;
        case 2:
            viewTransactionsByYear(studentID);
            pauseScreen();
            break;
        case 3:
            viewTransactionsByMonth(studentID);
            pauseScreen();
            break;
        case 4:
            viewSpendingAnalysis(studentID);
            pauseScreen();
            break;
        case 5:
            if (!checkRenewalAlert(studentID)) {
                int d, m, y;
                getCurrentDate(d, m, y);
                int daysLeft = getDaysInMonth(m, y) - d;
                cout << "\n  No renewal alert at this time.\n";
                cout << "  There are " << daysLeft
                    << " days remaining in " << getMonthName(m) << ".\n";
                cout << "  Alerts appear in the last 5 days of the month.\n";
            }
            pauseScreen();
            break;
        case 0:
            cout << "  Returning...\n";
            break;
        default:
            cout << "  Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

// ============================================================
// Sample Data Generator (105 students for testing)
// ============================================================
void generateSampleData() {
    cout << "\n  Generating sample data with 105 students...\n";

    string firstNames[] = {
        "Cheng", "Siti", "Wei Jie", "Kumar", "Mei Ling",
        "Raj", "Ling Hui", "Ali", "Farah", "Jun Wei",
        "Priya", "Xin Yi", "Amir", "Nurul", "Hao Yu",
        "Deepa", "Zhi Hao", "Hassan", "Aina", "Yong Sheng"
    };
    string lastNames[] = {
        "Li", "Binti Ismail", "Lim", "A/L Muthu", "Tan",
        "A/L Raju", "Wong", "Bin Osman", "Binti Yusof", "Chen",
        "A/P Krishnan", "Lee", "Bin Hassan", "Binti Ahmad", "Ng",
        "A/P Suresh", "Ong", "Bin Ali", "Binti Zainal", "Goh"
    };
    string programmes[] = {
        "BSc Software Engineering",
        "BSc Computer Science",
        "BEng Electrical Engineering",
        "BBA Accounting",
        "BA English Language",
        "BA Chinese Studies",
        "BSc Biotechnology",
        "BSc Data Science",
        "Foundation in Science"
    };

    studentCount = 0;
    srand(42); // Fixed seed for reproducible data

    for (int i = 0; i < 105 && studentCount < MAX_STUDENTS; i++) {
        Student& s = students[studentCount];

        ostringstream idoss;
        idoss << "23" << setfill('0') << setw(5) << (10001 + i);
        s.studentID = idoss.str();

        s.name = firstNames[i % 20] + " " + lastNames[i % 20];
        s.password = "pass" + s.studentID;

        ostringstream phoneoss;
        phoneoss << "01" << (rand() % 9 + 1) << "-"
            << setfill('0') << setw(8) << (rand() % 90000000 + 10000000);
        s.contactNo = phoneoss.str();

        string states[] = { "W", "B", "V", "J", "A", "N", "D" };
        ostringstream plateoss;
        plateoss << states[rand() % 7]
            << (rand() % 9000 + 1000) << " "
            << (char)('A' + rand() % 26)
            << (char)('A' + rand() % 26)
            << (char)('A' + rand() % 26);
        s.vehiclePlateNo = plateoss.str();

        s.vehicleType = (rand() % 5 == 0) ? "Motorcycle" : "Car";
        s.faculty = FACULTIES[i % NUM_FACULTIES];
        s.programme = programmes[i % NUM_FACULTIES];

        studentCount++;
    }

    // Generate 100 applications across months
    appCount = 0;
    for (int i = 0; i < 100 && appCount < MAX_APPLICATIONS; i++) {
        Application& a = applications[appCount];
        a.appID = generateID("APP", appCount + 1);
        a.studentID = students[i % studentCount].studentID;
        a.applyMonth = (i % 12) + 1;
        a.applyYear = 2026;
        a.numMonths = (rand() % 3) + 1;

        int statusRand = rand() % 10;
        if (statusRand < 6)      a.status = "Approved";
        else if (statusRand < 8) a.status = "Pending";
        else                     a.status = "Rejected";

        a.applyDate = formatDate(rand() % 28 + 1, a.applyMonth, a.applyYear);

        int sIdx = findStudentIndex(a.studentID);
        double rate = (sIdx != -1 && students[sIdx].vehicleType == "Motorcycle") ? 30.00 : 50.00;
        a.fee = rate * a.numMonths;

        appCount++;
    }

    // Generate transactions for approved applications
    transCount = 0;
    for (int i = 0; i < appCount && transCount < MAX_TRANSACTIONS; i++) {
        if (applications[i].status == "Approved") {
            int sIdx = findStudentIndex(applications[i].studentID);
            double rate = (sIdx != -1 && students[sIdx].vehicleType == "Motorcycle") ? 30.00 : 50.00;

            for (int m = 0; m < applications[i].numMonths && transCount < MAX_TRANSACTIONS; m++) {
                Transaction& t = transactions[transCount];
                t.transID = generateID("TXN", transCount + 1);
                t.studentID = applications[i].studentID;
                t.appID = applications[i].appID;
                t.amountPaid = rate;

                t.passMonth = applications[i].applyMonth + m;
                t.passYear = applications[i].applyYear;
                if (t.passMonth > 12) {
                    t.passMonth -= 12;
                    t.passYear++;
                }

                t.paymentDate = formatDate(rand() % 28 + 1,
                    applications[i].applyMonth,
                    applications[i].applyYear);
                t.faculty = (sIdx != -1) ? students[sIdx].faculty : "Unknown";
                transCount++;
            }
        }
    }

    saveStudents();
    saveApplications();
    saveTransactions();

    cout << "  Sample data generated successfully!\n";
    printLine('-', 45);
    cout << "  Students     : " << studentCount << endl;
    cout << "  Applications : " << appCount << endl;
    cout << "  Transactions : " << transCount << endl;
    printLine('-', 45);
    cout << "\n  Try logging in as:\n";
    cout << "  ID: 2310001  |  Password: pass2310001\n";
}

// ============================================================
// Main Function
// ============================================================
int main() {
    loadAllData();

    int choice;
    do {
        printHeader("MPKJ MONTHLY CAR PARKING PASS MANAGEMENT SYSTEM");
        cout << "  (Student Transaction Tracking & Alerts Module)\n";
        printLine('-');
        cout << "  1. Student Login (Tracking & Alerts)\n";
        cout << "  2. Generate Sample Data (105 students)\n";
        cout << "  0. Exit\n";
        printLine('-');
        cout << "  Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string studentID, password;
            cout << "\n  Enter Student ID : ";
            cin >> studentID;

            int idx = findStudentIndex(studentID);
            if (idx == -1) {
                cout << "  Student ID not found in system.\n";
                break;
            }

            cout << "  Enter Password   : ";
            cin >> password;

            if (students[idx].password != password) {
                cout << "  Incorrect password. Access denied.\n";
                break;
            }

            cout << "\n  Welcome, " << students[idx].name << "!\n";
            studentTrackingMenu(studentID);
            break;
        }
        case 2:
            generateSampleData();
            pauseScreen();
            break;
        case 0:
            cout << "\n  Thank you for using MPKJ Parking System. Goodbye!\n\n";
            break;
        default:
            cout << "  Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}