#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Transaction {
    char date[20];
    char type[20];
    float amount;
    float balance;
};

void getDate(char *dateStr) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

float getCurrentBalance() {
    FILE *fp = fopen("passbook.txt", "r");
    if (!fp) return 0;

    struct Transaction t;
    float lastBalance = 0;

    while (fscanf(fp, "%s %s %f %f", t.date, t.type, &t.amount, &t.balance) != EOF) {
        lastBalance = t.balance;
    }

    fclose(fp);
    return lastBalance;
}

void addTransaction(char type[], float amount) {
    FILE *fp = fopen("passbook.txt", "a");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    struct Transaction t;
    getDate(t.date);
    strcpy(t.type, type);
    t.amount = amount;

    float currentBalance = getCurrentBalance();

    if (strcmp(type, "DEPOSIT") == 0) {
        t.balance = currentBalance + amount;
    } else if (strcmp(type, "WITHDRAW") == 0) {
        if (amount > currentBalance) {
            printf("\n Not enough balance!\n");
            fclose(fp);
            return;
        }
        t.balance = currentBalance - amount;
    }

    fprintf(fp, "%s %s %.2f %.2f\n", t.date, t.type, t.amount, t.balance);
    fclose(fp);

    printf("\n✔ Transaction Successful!\n");
}

void printPassbook() {
    FILE *fp = fopen("passbook.txt", "r");
    if (!fp) {
        printf("\nNo transactions found.\n");
        return;
    }

    struct Transaction t;
    printf("\n===== PASSBOOK =====\n");
    printf("DATE        TYPE        AMOUNT    BALANCE\n");

    while (fscanf(fp, "%s %s %f %f", t.date, t.type, &t.amount, &t.balance) != EOF) {
        printf("%s  %-10s  %.2f    %.2f\n", t.date, t.type, t.amount, t.balance);
    }

    fclose(fp);
}

void searchByDate() {
    char searchDate[20];
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", searchDate);

    FILE *fp = fopen("passbook.txt", "r");
    if (!fp) {
        printf("No transactions found.\n");
        return;
    }

    struct Transaction t;
    int found = 0;

    printf("\nResults for %s\n", searchDate);

    while (fscanf(fp, "%s %s %f %f", t.date, t.type, &t.amount, &t.balance) != EOF) {
        if (strcmp(t.date, searchDate) == 0) {
            printf("%s  %-10s  %.2f    %.2f\n", t.date, t.type, t.amount, t.balance);
            found = 1;
        }
    }

    if (!found) {
        printf("No records found on this date.\n");
    }

    fclose(fp);
}

/* --------- Bank Passbook Managing System (Option 2) ---------- */

void bankPassbookSystem() {
    int choice;
    float amount;

    while (1) {
        printf("\n===== BANK PASSBOOK MANAGING SYSTEM =====\n");
        printf("1. Deposit Amount\n");
        printf("2. Withdraw Amount\n");
        printf("3. Check Balance\n");
        printf("4. Print Passbook\n");
        printf("5. Search by Date\n");
        printf("6. Back to Bank Login\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter deposit amount: ");
                scanf("%f", &amount);
                addTransaction("DEPOSIT", amount);
                break;

            case 2:
                printf("Enter withdrawal amount: ");
                scanf("%f", &amount);
                addTransaction("WITHDRAW", amount);
                break;

            case 3:
                printf("\nCurrent Balance: %.2f\n", getCurrentBalance());
                break;

            case 4:
                printPassbook();
                break;

            case 5:
                searchByDate();
                break;

            case 6:
                printf("\nReturning to Bank Login...\n");
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

/* --------- Create New Account (Option 1) ---------- */

void createNewAccount() {
    char name[50], dob[20], natId[30];

    printf("\n========= New Account =========\n");
    printf("Fill the details\n");
    printf("Name : ");
    scanf("%s", name);
    printf("Date of Birth : ");
    scanf("%s", dob);
    printf("National ID Number : ");
    scanf("%s", natId);

    printf("Press Enter to open this account ");
    getchar();  // consume leftover '\n'
    getchar();  // wait for Enter

    // Read last account number and pin from accounts.txt
    long long lastAccNo = 0, accNo;
    int lastPin = -1, pin;

    FILE *fp = fopen("accounts.txt", "r");
    if (fp != NULL) {
        long long tAcc;
        int tPin;
        char tName[50], tDob[20], tNat[30];

        while (fscanf(fp, "%lld %d %s %s %s", &tAcc, &tPin, tName, tDob, tNat) == 5) {
            lastAccNo = tAcc;
            lastPin = tPin;
        }
        fclose(fp);
    }

    if (lastAccNo == 0)
        accNo = 1;           // 00000000001
    else
        accNo = lastAccNo + 1;

    if (lastPin < 0)
        pin = 0;             // 0000
    else
        pin = lastPin + 1;

    fp = fopen("accounts.txt", "a");
    if (!fp) {
        printf("Error opening accounts file!\n");
        return;
    }

    fprintf(fp, "%011lld %04d %s %s %s\n", accNo, pin, name, dob, natId);
    fclose(fp);

    printf("\nThe account number provided is %011lld and Pin is %04d\n", accNo, pin);

    printf("\nPress Enter to get back main menu Bank Login");
    getchar();  // consume leftover '\n'
    getchar();  // wait for Enter
}

/* --------- Close Account (Option 3) ---------- */

void closeAccount() {
    char name[50], dob[20], natId[30];

    printf("\n========= Close Account =========\n");
    printf("Fill the details\n");
    printf("Name : ");
    scanf("%s", name);
    printf("Date of Birth : ");
    scanf("%s", dob);
    printf("National ID Number : ");
    scanf("%s", natId);

    printf("Press Enter to continue ");
    getchar();  // consume leftover '\n'
    getchar();  // wait for Enter

    char ch;
    printf("\nIf You are sure\n");
    printf("Press Y to Permanently delete your account\n");
    printf("Press N to Cancel request\n");
    scanf(" %c", &ch);

    if (ch == 'Y' || ch == 'y') {
        FILE *fp = fopen("accounts.txt", "r");
        if (!fp) {
            printf("\nNo account data found.\n");
            return;
        }

        FILE *temp = fopen("temp_accounts.txt", "w");
        if (!temp) {
            printf("Error opening temp file!\n");
            fclose(fp);
            return;
        }

        long long accNo;
        int pin;
        char rName[50], rDob[20], rNatId[30];
        int deleted = 0;

        while (fscanf(fp, "%lld %d %s %s %s", &accNo, &pin, rName, rDob, rNatId) == 5) {
            if (strcmp(name, rName) == 0 &&
                strcmp(dob, rDob) == 0 &&
                strcmp(natId, rNatId) == 0) {
                deleted = 1; // skip writing this one
                continue;
            }
            fprintf(temp, "%011lld %04d %s %s %s\n", accNo, pin, rName, rDob, rNatId);
        }

        fclose(fp);
        fclose(temp);

        remove("accounts.txt");
        rename("temp_accounts.txt", "accounts.txt");

        if (deleted) {
            printf("\nYour account has been permanently deleted.\n");
        } else {
            printf("\nAccount not found. Nothing deleted.\n");
        }
    } else {
        printf("\nRequest cancelled. Your account is safe.\n");
    }
}

/* ---------------- MAIN : BANK LOGIN MENU ---------------- */

int main() {
    int choice;

    while (1) {
        printf("\n========== Bank Login ==========\n");
        printf("1. Create New Account\n");
        printf("2. Accounts\n");
        printf("3. Close Account\n");
        printf("4. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createNewAccount();
                break;

            case 2:
                bankPassbookSystem();
                break;

            case 3:
                closeAccount();
                break;

            case 4:
                printf("\nExiting...\n");
                exit(0);

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
