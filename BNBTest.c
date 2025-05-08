#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//I DON'T HAVE A PARTNER

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int generateAccountNumber(void);
int checkWithdrawalLimit(int accountNumber); 
void calculateInterest(void);
void resetMonthlyWithdrawals(void);


struct User {
    char username[50];
    char password[50];
    char firstName[50];
    char lastName[50];
    char address[100];
    char phoneNumber[15];
    int pin;
    int accountNumber;  
    int isAdmin;
    float loanBalance;  
    int hasActiveLoan; 
    struct User* next;
};


struct Account {
    int accountNumber;
    char name[50];
    float balance;
    char username[50];
    char accountType[20]; 
    int monthlyWithdrawals; 
    time_t lastInterestUpdate; 
    struct Account* next;
};


struct Transaction {
    int accountNumber;
    char transactionType[20]; 
    float amount;
    float balanceAfter;
    time_t timestamp;
    int recipientAccountNumber; 
    char recipientName[50];     
    struct Transaction* next;
};


struct Message {
    int senderAccountNo;
    int recipientAccountNo;
    char senderName[50];
    char subject[100];
    char content[500];
    time_t timestamp;
    int isRead;
    struct Message* next;
};

// Global variables
struct User* userHead = NULL;
struct Account* head = NULL;
struct Transaction* transactionHead = NULL;
struct Message* messageHead = NULL;
int accountCounter = 1001;
int currentUserAccountNo = 0;  // Store logged in user's account number

// Function to validate password strength
int isStrongPassword(char* password) {
    int len = strlen(password);
    int hasUpper = 0, hasLower = 0, hasDigit = 0;
    int i;
    
    if (len < 8) return 0;
    
    for (i = 0; i < len; i++) {
        if (isupper(password[i])) hasUpper = 1;
        if (islower(password[i])) hasLower = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }
    
    return (hasUpper && hasLower && hasDigit);
}

// Function to add a transaction record
void addTransaction(int accountNumber, const char* transactionType, float amount, float balanceAfter, int recipientAccountNumber, const char* recipientName) {
    struct Transaction* newTransaction = (struct Transaction*)calloc(1, sizeof(struct Transaction));
    
    newTransaction->accountNumber = accountNumber;
    strcpy(newTransaction->transactionType, transactionType);
    newTransaction->amount = amount;
    newTransaction->balanceAfter = balanceAfter;
    newTransaction->timestamp = time(NULL);
    
    // Set recipient information for transfers
    if (strcmp(transactionType, "Transfer Out") == 0 || strcmp(transactionType, "Transfer In") == 0) {
        newTransaction->recipientAccountNumber = recipientAccountNumber;
        strcpy(newTransaction->recipientName, recipientName);
    } else {
        newTransaction->recipientAccountNumber = 0;
        strcpy(newTransaction->recipientName, "");
    }
    
    newTransaction->next = transactionHead;
    transactionHead = newTransaction;
}

// Function to generate receipt
void generateReceipt(int accountNumber) {
    struct Transaction* temp = transactionHead;
    struct Account* acc = NULL;
    struct User* user = NULL;
    int found = 0;
    char filename[50];
    FILE* receiptFile;
    time_t currentTime = time(NULL);
    
    // Find account and user information
    struct Account* accTemp = head;
    while (accTemp != NULL) {
        if (accTemp->accountNumber == accountNumber) {
            acc = accTemp;
            break;
        }
        accTemp = accTemp->next;
    }
    
    struct User* userTemp = userHead;
    while (userTemp != NULL) {
        if (userTemp->accountNumber == accountNumber) {
            user = userTemp;
            break;
        }
        userTemp = userTemp->next;
    }
    
    if (acc == NULL || user == NULL) {
        printf("Account information not found!\n");
        return;
    }
    
    // Create filename with account number and timestamp
    sprintf(filename, "receipt_%d_%ld.txt", accountNumber, currentTime);
    receiptFile = fopen(filename, "w");
    
    if (receiptFile == NULL) {
        printf("Error creating receipt file!\n");
        return;
    }
    
    // Write receipt header
    fprintf(receiptFile, "==========================================\n");
    fprintf(receiptFile, "              BANCO NI BINS              \n");
    fprintf(receiptFile, "==========================================\n\n");
    fprintf(receiptFile, "Date: %s", ctime(&currentTime));
    fprintf(receiptFile, "Account Number: %d\n", accountNumber);
    fprintf(receiptFile, "Account Holder: %s %s\n", user->firstName, user->lastName);
    fprintf(receiptFile, "Current Balance: %.2f\n\n", acc->balance);
    fprintf(receiptFile, "Transaction History:\n");
    fprintf(receiptFile, "------------------------------------------\n");
    
    // Write transaction history
    while (temp != NULL) {
        if (temp->accountNumber == accountNumber) {
            fprintf(receiptFile, "Type: %s\n", temp->transactionType);
            fprintf(receiptFile, "Amount: %.2f\n", temp->amount);
            fprintf(receiptFile, "Balance After: %.2f\n", temp->balanceAfter);
            
            // Add recipient information for transfers
            if (strcmp(temp->transactionType, "Transfer Out") == 0) {
                fprintf(receiptFile, "Recipient Account: %d\n", temp->recipientAccountNumber);
                fprintf(receiptFile, "Recipient Name: %s\n", temp->recipientName);
            } else if (strcmp(temp->transactionType, "Transfer In") == 0) {
                fprintf(receiptFile, "Sender Account: %d\n", temp->recipientAccountNumber);
                fprintf(receiptFile, "Sender Name: %s\n", temp->recipientName);
            }
            
            fprintf(receiptFile, "Time: %s", ctime(&temp->timestamp));
            fprintf(receiptFile, "------------------------------------------\n");
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        fprintf(receiptFile, "No transactions found for this session.\n");
    }
    
    fprintf(receiptFile, "\nThank you for banking with Banco Ni Bins!\n");
    fprintf(receiptFile, "==========================================\n");
    
    fclose(receiptFile);
    printf("Receipt generated successfully: %s\n", filename);
}

// Function to sign up
void signUp() {
	clearConsole();
    struct User* newUser;
    struct User* temp;
    int pin;
    
    newUser = (struct User*)malloc(sizeof(struct User));
    printf("========================\n");
    printf("        \033[1;33mSIGN UP\033[0m         \n");
    printf("========================\n");
    
    // Get personal information
    printf("\nEnter First Name: ");
    scanf("%s", newUser->firstName);
    while(getchar() != '\n');
    
    printf("Enter Last Name: ");
    scanf("%s", newUser->lastName);
    while(getchar() != '\n');
    
    printf("Enter Address: ");
    scanf("%s", newUser->address);
    while(getchar() != '\n');
    
    printf("Enter Phone Number: ");
    scanf("%s", newUser->phoneNumber);
    while(getchar() != '\n');
    
    // Get username
    printf("Create Username: ");
    scanf("%s", newUser->username);
    while(getchar() != '\n');
    
    // Check if username already exists
    temp = userHead;
    while (temp != NULL) {
        if (strcmp(temp->username, newUser->username) == 0) {
            printf("\n\033[1;31mUsername already exists! Please try again.\033[0m \n");
            free(newUser);
            printf("\nEnter any key to proceed...");
        	getch();
            return;
        }
        temp = temp->next;
    }
    
    // Get password
    do {
        printf("\nCreate Password (min 8 chars, must contain uppercase, lowercase, and number): ");
        scanf("%s", newUser->password);
        while(getchar() != '\n');
        if (!isStrongPassword(newUser->password)) {
            printf("\n\033[1;31mPassword is not strong enough! Please try again.\033[0m \n");
            
        }
    } while (!isStrongPassword(newUser->password));
    
    // Get PIN
    do {
        printf("\nCreate 4-digit PIN: ");
        scanf("%d", &pin);
        while(getchar() != '\n');
        
        if (pin < 1000 || pin > 9999) {
            printf("\n\033[1;31mPIN must be exactly 4 digits! Please try again.\033[0m\n");
        } else {
            newUser->pin = pin;
            break;
        }
    } while (1);
    
    // Generate unique account number
    int newAccNo;
    do {
        newAccNo = generateAccountNumber();
        // Check if account number already exists
        temp = userHead;
        while (temp != NULL) {
            if (temp->accountNumber == newAccNo) break;
            temp = temp->next;
        }
    } while (temp != NULL);
    
    newUser->accountNumber = newAccNo;
    newUser->isAdmin = 0;
    newUser->next = userHead;
    userHead = newUser;
    
    // Select account type
    printf("\n\033[1;33mSelect Account Type\033[0m\n");
    printf("1. Savings Account (6%% annual interest, 4 withdrawals/month limit)\n");
    printf("2. Checking Account (No interest, unlimited withdrawals)\n");
    printf("Enter your choice: ");
    int accTypeChoice;
    do {
        if (scanf("%d", &accTypeChoice) != 1) {
            printf("\n\033[1;31mError:\033[0m Invalid input! Please enter 1 or 2: ");
            while(getchar() != '\n'); // Clear input buffer
            continue;
        }
        while(getchar() != '\n'); // Clear input buffer
        
        if (accTypeChoice != 1 && accTypeChoice != 2) {
            printf("\033[1;31mError:\033[0m Invalid choice! Please enter 1 or 2: ");
            continue;
        }
        break;
    } while (1);

    struct Account* newAccount = (struct Account*)malloc(sizeof(struct Account));
    if (newAccount == NULL) {
        printf("Memory allocation failed!\n");
        free(newUser);
        return;
    }

    newAccount->accountNumber = newAccNo;
    strcpy(newAccount->name, newUser->firstName);
    strcat(newAccount->name, " ");
    strcat(newAccount->name, newUser->lastName);
    newAccount->balance = 0.0;
    strcpy(newAccount->username, newUser->username);
    newAccount->monthlyWithdrawals = 0;
    newAccount->lastInterestUpdate = time(NULL);

    if (accTypeChoice == 1) {
        strcpy(newAccount->accountType, "Savings");
    } else {
        strcpy(newAccount->accountType, "Checking");
    }

    newAccount->next = head;
    head = newAccount;
    
    // Set the current user account number
    currentUserAccountNo = newAccNo;
    
    // Record account creation as a transaction
    addTransaction(newAccNo, "Account Creation", 0.0, 0.0, 0, "");
    
    clearConsole();
    printf("\n=== \033[1;32mAccount Created Successfully!\033[0m ===\n");
    printf("\nYour Account Number: \033[1;32m%d\033[0m\n", newAccNo);
    printf("\nPlease keep your account number safe!\n");
    printf("You can now login using your account number.\n");
    printf("\nEnter any key to proceed...");
    getchar();
}

// Function to login
int login(char* username) {
    int accountNo;
    char password[50];
    struct User* temp;
    clearConsole();
    printf("========================\n");
    printf("        \033[1;33mLOGIN\033[0m         \n");
    printf("========================\n");
    printf("\nEnter Account Number: ");
    if (scanf("%d", &accountNo) != 1) {
    	printf("\033[1;31m\nInvalid account number format!\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
        getch();
        return -1;
    }
    while(getchar() != '\n');
    
    // Check for built-in admin account
    if (accountNo == 123123) {
        printf("\nEnter Password: ");
        scanf("%s", password);
        while(getchar() != '\n');
        
        if (strcmp(password, "Admin123!") == 0) {
            strcpy(username, "admin");
            currentUserAccountNo = 123123;
            printf("\033[1;32m\nAdmin login successful!\033[0m\n");
            printf("\nEnter any key to proceed...");
			getch();
            return 1;  // Return 1 for admin
        } else {
        	printf("\033[1;31m\nInvalid password!\033[0m\n");
		    printf("\nEnter any key to proceed...");
		    getch();
            return -1;
        }
    }
    
    // Regular user login
    temp = userHead;
    while (temp != NULL) {
        if (temp->accountNumber == accountNo) {
            if (temp->isAdmin == 2) {  // Check if user is restricted
                printf("\033[1;31m\nThis account has been restricted. Please contact support.\033[0m\n");
                printf("\nEnter any key to proceed...");
		    	getch();
                return -1;
            }
            
            printf("\nEnter Password: ");
            scanf("%s", password);
            while(getchar() != '\n');
            
            if (strcmp(temp->password, password) == 0) {
                strcpy(username, temp->username);
                currentUserAccountNo = accountNo;
                printf("\033[1;32m\nLogin successful!\033[0m\n");
                printf("\nWelcome, %s %s!\n", temp->firstName, temp->lastName);
			    printf("\nEnter any key to proceed...");
			    getch();
                
                return temp->isAdmin;
            } else {
                printf("\033[1;31m\nInvalid password!\033[0m\n");
			    printf("\nEnter any key to proceed...");
			    getch();
                return -1;
            }
        }
        temp = temp->next;
    }
    printf("\033[1;31m\nAccount not found!\033[0m\n");
    printf("\nEnter any key to proceed...");
    getch();
    return -1;
}

// Function to create account
void createAccount(char* username) {
    struct Account* newAccount = (struct Account*)malloc(sizeof(struct Account));
    
    // Generate unique account number
    int newAccNo;
    struct Account* temp;
    do {
        newAccNo = generateAccountNumber();
        // Check if account number already exists
        temp = head;
        while (temp != NULL) {
            if (temp->accountNumber == newAccNo) break;
            temp = temp->next;
        }
    } while (temp != NULL);  // Repeat if account number exists
    
    newAccount->accountNumber = newAccNo;
    printf("Enter name: ");
    scanf("%s", newAccount->name);
    while(getchar() != '\n'); // Clear input buffer
    
    newAccount->balance = 0.0;
    strcpy(newAccount->username, username);
    newAccount->next = head;
    head = newAccount;
    currentUserAccountNo = newAccNo;  // Store the account number for current user
    
    // Record account creation as a transaction
    addTransaction(newAccNo, "Account Creation", 0.0, 0.0, 0, "");
    
    printf("Account created successfully! Your Account Number: %d\n", newAccNo);
}

// Function to verify PIN
int verifyPIN() {
    int pin;
    struct User* temp = userHead;
    int found = 0;
    
    // First check if the user exists
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            found = 1;
            break;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("\n\033[1;31mUser account not found!\033[0m\n");
        printf("\nEnter any key to proceed...");
    	getch();
        return 0;
    }
    
    printf("\nEnter PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("\n\033[1;31mInvalid PIN format!\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
    	getch();
        return 0;
    }
    while(getchar() != '\n');
    
    if (temp->pin == pin) {
        return 1;
    } else {
        printf("\n\033[1;31mInvalid PIN!\033[0m\n");
        printf("\nEnter any key to proceed...");
    	getch();
        return 0;
    }
}

// Function to deposit money
void deposit() {
    if (!verifyPIN()) return;
    
    float amount;
    struct Account* temp = head;
    clearConsole();
    printf("============================================\n");
    printf("              \033[1;33mDEPOSIT MONEY\033[0m                \n");
    printf("============================================\n");
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("\n\033[1;31mInvalid amount! Please enter a positive number.\033[0m\n");
		        printf("\nEnter any key to proceed...");
		    	getch();
                while(getchar() != '\n'); // Clear input buffer
                return;
            }
            
            temp->balance += amount;
            printf("\n\033[1;32mAmount deposited successfully!\033[0m \n\nNew Balance: \033[1;36m%.2f\033[0m \n\n", temp->balance);
            
            // Record transaction
            addTransaction(currentUserAccountNo, "Deposit", amount, temp->balance, 0, "");
            printf("Enter to proceed...");
            getch();
            return;
        }
        temp = temp->next;
    }
    
    printf("Account not found!\n");
}

// Function to withdraw money
void withdraw() {
    if (!verifyPIN()) return;

    clearConsole();
    printf("============================================\n");
    printf("                \033[1;33mWITHDRAW MONEY\033[0m                \n");
    printf("============================================\n");

    struct Account* temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            // Immediately check withdrawal/transfer limit for savings account
            if (strcmp(temp->accountType, "Savings") == 0) {
                int left = 4 - temp->monthlyWithdrawals;
                if (left <= 0) {
                    printf("\n\033[1;31mMonthly withdrawal/transfer limit (4) reached for savings account!\033[0m\n");
                    printf("\nEnter any key to proceed...");
                    getch();
                    return;
                } else {
                    printf("\n\033[1;36mWithdrawals/Transfers left this month: \033[1;32m%d\033[0m\n\n", left);
                }
            }

            float amount;
            printf("Enter amount to withdraw: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("\n\033[1;31mInvalid amount! Please enter a positive number.\033[0m\n");
                printf("\nEnter any key to proceed...");
                getch();
                while(getchar() != '\n');
                return;
            }

            if (amount > temp->balance) {
                printf("\033[1;31mInsufficient balance!\033[0m\n");
                printf("\nEnter any key to proceed...");
                getch();
                return;
            }

            temp->balance -= amount;
            if (strcmp(temp->accountType, "Savings") == 0) {
                temp->monthlyWithdrawals++;
                printf("\nMonthly withdrawals/transfers used: \033[1;32m%d\033[0m/4\n\n", temp->monthlyWithdrawals);
                printf("Withdrawals/Transfers left this month: \033[1;36m%d\033[0m\n", 4 - temp->monthlyWithdrawals);
            }
            printf("\n\033[1;32mWithdrawal successful!\033[0m \n\nNew Balance: \033[1;36m%.2f\033[0m\n", temp->balance);
            addTransaction(currentUserAccountNo, "Withdrawal", amount, temp->balance, 0, "");
            printf("\nEnter any key to proceed...");
            getch();
            return;
        }
        temp = temp->next;
    }
    printf("\033[1;31mAccount not found!\033[0m\n");
    pauseConsole();
}

// Function to check balance
void checkBalance() {
    if (!verifyPIN()) return;
    clearConsole();
    struct Account* temp = head;
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("============================================\n");
		    printf("             \033[1;33mACCOUNT INFORMATION\033[0m                \n");
		    printf("============================================\n");
            printf("\nAccount Number: \033[1;36m%d\033[0m\n", temp->accountNumber);
            printf("\nCurrent Balance: \033[1;36m%.2f\033[0m\n", temp->balance);
            printf("\nEnter any key to proceed...");
            getch();
            // Record balance check as a transaction
            addTransaction(currentUserAccountNo, "Balance Check", 0.0, temp->balance, 0, "");
            return;
        }
        temp = temp->next;
    }
}

// Function to display all accounts
void displayAccounts() {
    struct Account* temp = head;
    if (temp == NULL) {
        printf("No accounts found!\n");
        return;
    }
    printf("\nList of Bank Accounts:\n");
    printf("------------------------------------\n");
    while (temp != NULL) {
        printf("Account Number: %d\n", temp->accountNumber);
        printf("Name: %s\n", temp->name);
        printf("Username: %s\n", temp->username);
        printf("Balance: %.2f\n", temp->balance);
        printf("------------------------------------\n");
        temp = temp->next;
    }
}

// Function to transfer money between accounts
void transfer() {
    if (!verifyPIN()) return;

    clearConsole();
    printf("============================================\n");
    printf("               \033[1;33mTRANSFER MONEY\033[0m                \n");
    printf("============================================\n");

    struct Account *fromAcc = NULL, *toAcc = NULL, *temp = head;
    char recipientName[50] = "";

    // Find the sender's account
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            fromAcc = temp;
            break;
        }
        temp = temp->next;
    }

    if (fromAcc == NULL) {
        printf("\033[1;31mAccount not found!\033[0m\n");
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    // Immediately check withdrawal/transfer limit for savings account
    if (strcmp(fromAcc->accountType, "Savings") == 0) {
        int left = 4 - fromAcc->monthlyWithdrawals;
        if (left <= 0) {
            printf("\n\033[1;31mMonthly withdrawal/transfer limit (4) reached for savings account!\033[0m\n");
            printf("\nEnter any key to proceed...");
            getch();
            return;
        } else {
            printf("\n\033[1;36mWithdrawals/Transfers left this month: \033[1;32m%d\033[0m\n\n", left);
        }
    }

    int toAccNo;
    float amount;

    printf("Enter recipient's account number: ");
    if (scanf("%d", &toAccNo) != 1) {
        printf("\n\033[1;31mInvalid account number!\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    if (toAccNo == currentUserAccountNo) {
        printf("\n\033[1;31mCannot transfer to the same account!\033[0m\n");
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    // Find the recipient's account
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == toAccNo) {
            toAcc = temp;
            strcpy(recipientName, temp->name);
            break;
        }
        temp = temp->next;
    }

    if (toAcc == NULL) {
        printf("\n\033[1;31mRecipient account not found!\033[0m\n");
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    printf("\nEnter amount to transfer: ");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("\n\033[1;31mInvalid amount! Please enter a positive number.\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    if (amount > fromAcc->balance) {
        printf("\n\033[1;31mInsufficient balance!\033[0m\n");
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }

    // Update withdrawal/transfer count for savings account
    if (strcmp(fromAcc->accountType, "Savings") == 0) {
        fromAcc->monthlyWithdrawals++;
        printf("\nMonthly withdrawals/transfers used: \033[1;32m%d\033[0m/4\n\n", fromAcc->monthlyWithdrawals);
        printf("Withdrawals/Transfers left this month: \033[1;36m%d\033[0m\n", 4 - fromAcc->monthlyWithdrawals);
    }

    fromAcc->balance -= amount;
    toAcc->balance += amount;
    printf("\n\033[1;32mTransfer successful!\033[0m\n");
    printf("\nYour new balance: \033[1;36m%.2f\033[0m\n", fromAcc->balance);
    printf("\nEnter any key to proceed...");
    getch();

    // Get sender's name for the recipient's receipt
    char senderName[50] = "";
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            strcpy(senderName, temp->name);
            break;
        }
        temp = temp->next;
    }

    // Record transactions for both accounts
    addTransaction(currentUserAccountNo, "Transfer Out", amount, fromAcc->balance, toAccNo, recipientName);
    addTransaction(toAccNo, "Transfer In", amount, toAcc->balance, currentUserAccountNo, senderName);
}

// Function to generate random account number
int generateAccountNumber() {
    return 100000 + (rand() % 900000);  // Generates number between 100000-999999
}

// --- File Handling Functions ---

// Save users to file
void saveUsersToFile() {
    FILE *fp = fopen("users.txt", "w");
    if (!fp) return;
    struct User *temp = userHead;
    while (temp) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%d|%d|%d|%.2f|%d\n",
            temp->username, temp->password, temp->firstName, temp->lastName,
            temp->address, temp->phoneNumber, temp->pin, temp->accountNumber, 
            temp->isAdmin, temp->loanBalance, temp->hasActiveLoan);
        temp = temp->next;
    }
    fclose(fp);
}

// Load users from file
void loadUsersFromFile() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return;
    char buf[400];
    while (fgets(buf, sizeof(buf), fp)) {
        struct User *newUser = (struct User*)calloc(1, sizeof(struct User));
        sscanf(buf, "%49[^|]|%49[^|]|%49[^|]|%49[^|]|%99[^|]|%14[^|]|%d|%d|%d|%f|%d",
            newUser->username, newUser->password, newUser->firstName, newUser->lastName,
            newUser->address, newUser->phoneNumber, &newUser->pin, &newUser->accountNumber, 
            &newUser->isAdmin, &newUser->loanBalance, &newUser->hasActiveLoan);
        newUser->next = userHead;
        userHead = newUser;
    }
    fclose(fp);
}

// Save accounts to file
void saveAccountsToFile() {
    FILE *fp = fopen("accounts.txt", "w");
    if (!fp) return;
    struct Account *temp = head;
    while (temp) {
        fprintf(fp, "%d|%s|%.2f|%s|%s|%d|%ld\n", 
            temp->accountNumber, temp->name, temp->balance, 
            temp->username, temp->accountType, temp->monthlyWithdrawals,
            (long)temp->lastInterestUpdate);
        temp = temp->next;
    }
    fclose(fp);
}

// Load accounts from file
void loadAccountsFromFile() {
    FILE *fp = fopen("accounts.txt", "r");
    if (!fp) return;
    char buf[300];
    while (fgets(buf, sizeof(buf), fp)) {
        struct Account *newAcc = (struct Account*)calloc(1, sizeof(struct Account));
        long t;
        sscanf(buf, "%d|%49[^|]|%f|%49[^|]|%19[^|]|%d|%ld", 
            &newAcc->accountNumber, newAcc->name, &newAcc->balance, 
            newAcc->username, newAcc->accountType, &newAcc->monthlyWithdrawals, &t);
        newAcc->lastInterestUpdate = (time_t)t;
        newAcc->next = head;
        head = newAcc;
    }
    fclose(fp);
}

// Save transactions to file
void saveTransactionsToFile() {
    FILE *fp = fopen("transactions.txt", "w");
    if (!fp) return;
    struct Transaction *temp = transactionHead;
    while (temp) {
        fprintf(fp, "%d|%s|%.2f|%.2f|%ld|%d|%s\n", temp->accountNumber, temp->transactionType, temp->amount, temp->balanceAfter, (long)temp->timestamp, temp->recipientAccountNumber, temp->recipientName);
        temp = temp->next;
    }
    fclose(fp);
}

// Load transactions from file
void loadTransactionsFromFile() {
    FILE *fp = fopen("transactions.txt", "r");
    if (!fp) return;
    char buf[300];
    while (fgets(buf, sizeof(buf), fp)) {
        struct Transaction *newT = (struct Transaction*)calloc(1, sizeof(struct Transaction));
        long t;
        int n = sscanf(buf, "%d|%19[^|]|%f|%f|%ld|%d|%49[^\n]", &newT->accountNumber, newT->transactionType, &newT->amount, &newT->balanceAfter, &t, &newT->recipientAccountNumber, newT->recipientName);
        if (n < 7) strcpy(newT->recipientName, "");
        newT->timestamp = (time_t)t;
        newT->next = transactionHead;
        transactionHead = newT;
    }
    fclose(fp);
}

// Save all data
void saveAllData() {
    saveUsersToFile();
    saveAccountsToFile();
    saveTransactionsToFile();
    saveMessagesToFile();  // Add this line
}

// Load all data
void loadAllData() {
    loadUsersFromFile();
    loadAccountsFromFile();
    loadTransactionsFromFile();
    loadMessagesFromFile();  // Add this line
}

// Function to check loan eligibility
int checkLoanEligibility() {
    struct User* temp = userHead;
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            if (temp->hasActiveLoan) {
			    printf("\n=== \033[1;32mAPPLY LOAN\033[0m \033[1;31mDENIED\033[0m ===\n");
                printf("\n\033[1;31mYou already have an active loan. Please pay it off first.\033[0m \n");
                printf("\nEnter any key to proceed...");
        		getch();
                return 0;
            }
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Function to apply for a loan
void applyLoan() {
    if (!verifyPIN()) return;
    clearConsole();
    
    float loanAmount;
    struct User* temp = userHead;
    struct Account* acc = NULL;
    
    // Check if user is eligible for a loan
    if (!checkLoanEligibility()) {
        return;
    }
    
    // Find user's account
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            break;
        }
        temp = temp->next;
    }
    
    // Find user's bank account
    struct Account* accTemp = head;
    while (accTemp != NULL) {
        if (accTemp->accountNumber == currentUserAccountNo) {
            acc = accTemp;
            break;
        }
        accTemp = accTemp->next;
    }
    
    if (temp == NULL || acc == NULL) {
        printf("Account not found!\n");
        return;
    }
    
    printf("========================\n");
    printf("      \033[1;33mAPPLY LOAN\033[0m         \n");
    printf("========================\n");
    printf("\nEnter loan amount: ");
    if (scanf("%f", &loanAmount) != 1 || loanAmount <= 0) {
        printf("\n\033[1;31mInvalid amount! Please enter a positive number.\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }
    
    // Calculate loan with 20% interest
    float totalLoan = loanAmount * 1.20;
    
    // Update user's loan information
    temp->loanBalance = totalLoan;
    temp->hasActiveLoan = 1;
    
    // Add loan amount to user's balance
    acc->balance += loanAmount;
    
    // Record loan transaction
    addTransaction(currentUserAccountNo, "Loan", loanAmount, acc->balance, 0, "");
    
    printf("\n\033[1;32mLoan Approved\033[0m \n");
    printf("\nLoan Amount: \033[1;36m%.2f\033[0m\n", loanAmount);
    printf("\nTotal Amount to Pay (with 20%% interest): \033[1;36m%.2f\033[0m\n", totalLoan);
    printf("\n\033[1;32mAmount has been added to your account balance.\033[0m\n");
    printf("\nEnter any key to proceed...");
    getch();
    
    // Save changes to file
    saveAllData();
}

// Function to pay loan
void payLoan() {
    if (!verifyPIN()) return;
    clearConsole();
    float paymentAmount;
    struct User* temp = userHead;
    struct Account* acc = NULL;
    
    // Find user's account
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            break;
        }
        temp = temp->next;
    }
    
    // Find user's bank account
    struct Account* accTemp = head;
    while (accTemp != NULL) {
        if (accTemp->accountNumber == currentUserAccountNo) {
            acc = accTemp;
            break;
        }
        accTemp = accTemp->next;
    }
    
    if (temp == NULL || acc == NULL) {
        printf("Account not found!\n");
        return;
    }
    
    if (!temp->hasActiveLoan) {
        printf("You don't have any active loans.\n");
        return;
    }
    
    printf("========================\n");
    printf("    \033[1;33mLOAN PAYMENT\033[0m         \n");
    printf("========================\n");
    printf("\nCurrent Loan Balance: \033[1;36m%.2f\033[0m\n", temp->loanBalance);
    printf("\nEnter payment amount: ");
    if (scanf("%f", &paymentAmount) != 1 || paymentAmount <= 0) {
        printf("\n\033[1;33mInvalid amount! Please enter a positive number.\033[0m\n");
        while(getchar() != '\n');
        return;
    }
    
    if (paymentAmount > acc->balance) {
        printf("\n\033[1;31mInsufficient balance!\033[0m\n");
        return;
    }
    
    // Process payment
    acc->balance -= paymentAmount;
    
    // Check if payment exceeds loan balance
    if (paymentAmount > temp->loanBalance) {
        float excessAmount = paymentAmount - temp->loanBalance;
        // Return excess amount to user's balance
        acc->balance += excessAmount;
        paymentAmount = temp->loanBalance; // Adjust payment amount to actual loan balance
        printf("\n\033[1;32mExcess payment of %.2f has been returned to your account.\033[0m\n", excessAmount);
    }
    
    temp->loanBalance -= paymentAmount;
    
    // Record loan payment transaction
    addTransaction(currentUserAccountNo, "Loan Payment", paymentAmount, acc->balance, 0, "");
    
    printf("\n\033[1;32mPayment successful!\033[0m\n");
    printf("\nRemaining loan balance: \033[1;36m%.2f\033[0m \n", temp->loanBalance);
    printf("\nCurrent account balance: \033[1;36m%.2f\033[0m \n", acc->balance);
    
    // Check if loan is fully paid
    if (temp->loanBalance <= 0) {
        temp->hasActiveLoan = 0;
        temp->loanBalance = 0;
        printf("\n\033[1;32mCongratulations! Your loan has been fully paid.\033[0m\n");
    }
    
    // Save changes to file
    saveAllData();
}

// Function to check loan status
void checkLoanStatus() {
    if (!verifyPIN()) return;
    clearConsole();
    struct User* temp = userHead;
    struct Account* acc = head;

    // Find the user
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            break;
        }
        temp = temp->next;
    }

    // Find the user's account
    while (acc != NULL) {
        if (acc->accountNumber == currentUserAccountNo) {
            break;
        }
        acc = acc->next;
    }

    printf("========================\n");
    printf("      \033[1;33mLOAN STATUS\033[0m         \n");
    printf("========================\n");

    

    if (temp != NULL) {
        if (temp->hasActiveLoan) {
            printf("\n\033[1;33mYou have an active loan.\033[0m\n");
            if (acc != NULL) {
		        printf("\nCurrent Balance: \033[1;36m%.2f\033[0m\n", acc->balance);
		    } else {
		        printf("\n\033[1;31mAccount not found!\033[0m\n");
		        return;
		    }
            printf("\nCurrent Loan Balance: \033[1;36m%.2f\033[0m\n", temp->loanBalance);
        } else {
            printf("\n\033[1;32mYou don't have any active loans\033[0m.\n");
        }
    } else {
        printf("\n\033[1;31mUser not found!\033[0m\n");
    }
}

// Function to view transaction history
void viewTransactionHistory() {
    if (!verifyPIN()) return;
    clearConsole();
    struct Transaction* temp = transactionHead;
    int found = 0;
    time_t currentTime = time(NULL);
    
    printf("===================================\n");
    printf("      \033[1;33mTRANSACTION HISTORY\033[0m         \n");
    printf("===================================\n");
    printf("\nDate: \033[1;37m%s\033[0m\n", ctime(&currentTime));
    printf("\033[1;37m------------------------------------------\033[0m\n");
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("Type: \033[1;33m%s\033[0m\n", temp->transactionType);
            printf("Amount: \033[1;36m%.2f\033[0m\n", temp->amount);
            printf("Balance After: \033[1;36m%.2f\033[0m\n", temp->balanceAfter);
            
            // Add recipient information for transfers
            if (strcmp(temp->transactionType, "Transfer Out") == 0) {
                printf("Recipient Account: \033[1;33m%d\033[0m\n", temp->recipientAccountNumber);
                printf("Recipient Name: \033[1;33m%s\033[0m\n", temp->recipientName);
            } else if (strcmp(temp->transactionType, "Transfer In") == 0) {
                printf("Sender Account: %d\n", temp->recipientAccountNumber);
                printf("Sender Name: %s\n", temp->recipientName);
            }
            
            printf("Time: \033[1;37m%s\033[0m", ctime(&temp->timestamp));
            printf("\033[1;37m------------------------------------------\033[0m\n");
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("No transactions found.\n");
    }
}

// Function to view all users' information
void viewAllUsers() {
    struct User* temp = userHead;
    printf("============================================\n");
    printf("         \033[1;32mALL USERS INFORMATION\033[0m              \n");
    printf("============================================\n");
    printf("------------------------------------------\n");
    
    while (temp != NULL) {
        printf("Username: \033[1;33m%s\033[0m\n", temp->username);
        printf("Full Name: \033[1;33m%s\033[0m \033[1;33m%s\033[0m\n", temp->firstName, temp->lastName);
        printf("Address: \033[1;33m%s\033[0m\n", temp->address);
        printf("Phone: \033[1;33m%s\033[0m\n", temp->phoneNumber);
        printf("Account Number: \033[1;36m%d\033[0m\n\n", temp->accountNumber);
        printf("Has Active Loan: \033[1;32m%s\033[0m\n\n", temp->hasActiveLoan ? "Yes" : "No");
        if (temp->hasActiveLoan) {
            printf("Loan Balance: \033[1;36m%.2f\033[0m\n\n", temp->loanBalance);
        }
        printf("\033[1;37m------------------------------------------\033[0m\n");
        temp = temp->next;
    }
}

// Function to edit user information
void editUserInfo() {
    int accNo;
    struct User* temp = userHead;
    
    printf("\nEnter account number to edit: ");
    scanf("%d", &accNo);
    while(getchar() != '\n');
    
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("===================================\n");
            printf("      \033[1;32mEDIT USER INFORMATION\033[0m         \n");
            printf("===================================\n");
            printf("\nCurrent Information:\n");
            printf("Username: \033[1;33m%s\033[0m\n", temp->username);
            printf("First Name: \033[1;33m%s\033[0m\n", temp->firstName);
            printf("Last Name: \033[1;33m%s\033[0m\n", temp->lastName);
            printf("Address: \033[1;33m%s\033[0m\n", temp->address);
            printf("Phone: \033[1;33m%s\033[0m\n", temp->phoneNumber);
            printf("PIN: \033[1;33m****\033[0m\n");
            
            printf("\nEnter new information (press Enter to keep current value):\n");
            
            char input[100];
            char newUsername[50];
            char newPassword[50];
            int newPin;
            
            // Username change
            printf("\nNew Username: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                strcpy(newUsername, input);
                // Check if username is unique
                struct User* checkUser = userHead;
                int isUnique = 1;
                while (checkUser != NULL) {
                    if (strcmp(checkUser->username, newUsername) == 0 && checkUser != temp) {
                        isUnique = 0;
                        break;
                    }
                    checkUser = checkUser->next;
                }
                if (isUnique) {
                    strcpy(temp->username, newUsername);
                } else {
                    printf("\n\033[1;31mUsername already exists! Username not changed.\033[0m\n");
                }
            }
            
            // Password change
            printf("\nNew Password (press Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                if (isStrongPassword(input)) {
                    strcpy(temp->password, input);
                } else {
                    printf("\n\033[1;31mPassword is not strong enough! Password not changed.\033[0m\n");
                }
            }
            
            // Other information changes
            printf("\nNew First Name: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->firstName, input);
            
            printf("\nNew Last Name: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->lastName, input);
            
            printf("\nNew Address: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->address, input);
            
            printf("\nNew Phone Number: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->phoneNumber, input);
            
            // Add PIN change
            printf("\nNew PIN (press Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                newPin = atoi(input);
                if (newPin >= 1000 && newPin <= 9999) {
                    temp->pin = newPin;
                } else {
                    printf("\n\033[1;31mPIN must be exactly 4 digits! PIN not changed.\033[0m\n");
                }
            }
            
            printf("\n\033[1;32mUser information updated successfully!\033[0m\n");
            saveAllData();
            return;
        }
        temp = temp->next;
    }
    printf("\n\033[1;31mAccount not found!\033[0m\n");
}

// Function to view user's transaction history (admin version)
void viewUserTransactions() {
    int accNo;
    struct Transaction* temp = transactionHead;
    int found = 0;

    printf("\nEnter account number to view transactions: ");
    scanf("%d", &accNo);
    while(getchar() != '\n');

    // Check if account exists
    struct Account* accCheck = head;
    int accExists = 0;
    while (accCheck != NULL) {
        if (accCheck->accountNumber == accNo) {
            accExists = 1;
            break;
        }
        accCheck = accCheck->next;
    }
    if (!accExists) {
        printf("\n\033[1;31mInvalid account number! No such account exists.\033[0m\n");
        return;
    }

    printf("\n=== \033[1;33mTransaction History for Account\033[0m \033[1;36m%d\033[0m ===\n", accNo);
    printf("\n------------------------------------------\n");

    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("Type: \033[1;33m%s\033[0m\n", temp->transactionType);
            printf("Amount: \033[1;36m%.2f\033[0m\n", temp->amount);
            printf("Balance After: \033[1;36m%.2f\033[0m\n", temp->balanceAfter);

            if (strcmp(temp->transactionType, "Transfer Out") == 0) {
                printf("Recipient Account: \033[1;36m%d\033[0m\n", temp->recipientAccountNumber);
                printf("Recipient Name: \033[1;33m%s\033[0m\n", temp->recipientName);
            } else if (strcmp(temp->transactionType, "Transfer In") == 0) {
                printf("Sender Account: \033[1;36m%d\033[0m\n", temp->recipientAccountNumber);
                printf("Sender Name: \033[1;33m%s\033[0m\n", temp->recipientName);
            }

            printf("Time: \033[1;37m%s\033[0m", ctime(&temp->timestamp));
            printf("------------------------------------------\n");
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("\033[1;31mNo transactions found for this account.\033[0m\n");
    }
}

// Function to restrict/unrestrict user
void toggleUserRestriction() {
    int accNo;
    struct User* temp = userHead;
    
    printf("\nEnter account number to restrict/unrestrict: ");
    scanf("%d", &accNo);
    while(getchar() != '\n');
    
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            if (accNo == 123123) {  // Prevent restricting admin account
                printf("Cannot restrict admin account!\n");
                return;
            }
            temp->isAdmin = temp->isAdmin == 2 ? 0 : 2;  // 2 represents restricted
            printf("\nUser \033[1;33m%s\033[0m has been %s\n", temp->username, 
                   temp->isAdmin == 2 ? "\033[1;31mrestricted\033[0m" : "\033[1;32munrestricted\033[0m");
            saveAllData();
            return;
        }
        temp = temp->next;
    }
    printf("\033[1;31mAccount not found!\033[0m\n");
}

// Function to delete user account
void deleteUserAccount() {
    int accNo;
    struct User *temp = userHead, *prev = NULL;
    struct Account *accTemp = head, *accPrev = NULL;

    printf("\nEnter account number to delete: ");
    scanf("%d", &accNo);
    while(getchar() != '\n');

    if (accNo == 123123) {  // Prevent deleting admin account
        printf("\n\033[1;31mCannot delete admin account!\033[0m\n");
        return;
    }

    // Check if account exists
    struct Account* accCheck = head;
    int accExists = 0;
    while (accCheck != NULL) {
        if (accCheck->accountNumber == accNo) {
            accExists = 1;
            break;
        }
        accCheck = accCheck->next;
    }
    if (!accExists) {
        printf("\n\033[1;31mInvalid account number! No such account exists.\033[0m\n");
        return;
    }

    // Delete from user list
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            if (prev == NULL) {
                userHead = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    // Delete from account list
    while (accTemp != NULL) {
        if (accTemp->accountNumber == accNo) {
            if (accPrev == NULL) {
                head = accTemp->next;
            } else {
                accPrev->next = accTemp->next;
            }
            free(accTemp);
            break;
        }
        accPrev = accTemp;
        accTemp = accTemp->next;
    }

    printf("\033[1;32mAccount deleted successfully!\033[0m\n");
    saveAllData();
}

// Function to view specific user information
void viewSpecificUser() {
    int accNo;
    struct User* temp = userHead;
    
    printf("\nEnter account number to view: ");
    scanf("%d", &accNo);
    while(getchar() != '\n');
    
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("\n============================================\n");
			    printf("             \033[1;32mUSER INFORMATION\033[0m              \n");
			    printf("============================================\n");
            printf("------------------------------------------\n");
            printf("Username: \033[1;33m%s\033[0m\n", temp->username);
            printf("\nFull Name: \033[1;33m%s\033[0m \033[1;33m%s\033[0m\n", temp->firstName, temp->lastName);
            printf("\nAddress: \033[1;33m%s\033[0m\n", temp->address);
            printf("\nPhone: \033[1;33m%s\033[0m\n", temp->phoneNumber);
            printf("\nAccount Number: \033[1;36m%d\033[0m\n", temp->accountNumber);
            printf("\nHas Active Loan: \033[1;32m%s\033[0m\n", temp->hasActiveLoan ? "Yes" : "No");
            if (temp->hasActiveLoan) {
                printf("\nLoan Balance: \033[1;36m%.2f\033[0m\n", temp->loanBalance);
            }
            printf("\nAccount Status: %s\n", temp->isAdmin == 2 ? "\033[1;31mRestricted\033[0m" : "\033[1;32mActive\033[0m");
            printf("------------------------------------------\n");
            return;
        }
        temp = temp->next;
    }
    printf("\n\033[1;31mAccount not found!\033[0m\n");
}

// Function to check withdrawal limit for savings account
int checkWithdrawalLimit(int accountNumber) {
    struct Account* temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accountNumber) {
            if (strcmp(temp->accountType, "Savings") == 0) {
                if (temp->monthlyWithdrawals >= 4) {
                    printf("\n\033[1;31mMonthly withdrawal/Transfer limit reached for savings account!\033[0m\n");
                    return 0;
                }
            }
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

// Function to display account information
void displayAccountInfo() {
    if (!verifyPIN()) return;
    clearConsole();
    
    struct Account* temp = head;
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("===================================\n");
		    printf("      \033[1;33mACCOUNT INFORMATION\033[0m         \n");
		    printf("===================================\n");
            printf("\nAccount Number: \033[1;33m%d\033[0m\n", temp->accountNumber);
            printf("\nAccount Type: \033[1;33m%s\033[0m\n", temp->accountType);
            printf("\nCurrent Balance: \033[1;36m%.2f\033[0m\n", temp->balance);
            
            if (strcmp(temp->accountType, "Savings") == 0) {
                printf("\nMonthly Withdrawals Used: \033[1;32m%d\033[0m/4\n", temp->monthlyWithdrawals);
                printf("\nNext Interest Update: \033[1;37m%s\033[0m", ctime(&temp->lastInterestUpdate));
            }
            
            return;
        }
        temp = temp->next;
    }
    printf("\033[1;31mAccount not found!\033[0m\n");
}

// Function to calculate and add interest for savings accounts
void calculateInterest() {
    struct Account* temp = head;
    time_t currentTime = time(NULL);
    
    while (temp != NULL) {
        if (strcmp(temp->accountType, "Savings") == 0) {
            // Check if it's been a year since last interest update
            if (difftime(currentTime, temp->lastInterestUpdate) >= 31536000) { // 31536000 seconds = 1 year
                float interest = temp->balance * 0.06; // 6% interest
                temp->balance += interest;
                temp->lastInterestUpdate = currentTime;
                
                // Record interest transaction
                addTransaction(temp->accountNumber, "Interest", interest, temp->balance, 0, "");
                
                printf("Interest of %.2f added to account %d\n", interest, temp->accountNumber);
            }
        }
        temp = temp->next;
    }
}

// Function to reset monthly withdrawal count for all savings accounts
void resetMonthlyWithdrawals(void) {
    struct Account* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->accountType, "Savings") == 0) {
            temp->monthlyWithdrawals = 0;
        }
        temp = temp->next;
    }
}

void pauseConsole() {
	
    printf("\nPress Enter to continue...");
    while(getchar() != '\n');
}

void printWelcomeBanner() {
    printf("============================================\n");
    printf("        \033[1;36mWELCOME TO BANCO NI BINS\033[0m         \n");
    printf("============================================\n");
    printf("        _.-'''''-._\n");
    printf("      .'  _     _  '.\n");
    printf("     /   (_)   (_)   \\\n");
    printf("    |  ,           ,  |\n");
    printf("    |  \\`.       .`/  |\n");
    printf("     \\  '.`'\"\"'`.'  /\n");
    printf("      '.  `'---'`  .'\n");
    printf("        '-._____.-'\n");
    printf("============================================\n");
}

void loadingAnimation() {
    printf("Processing");
    for (int i = 0; i < 3; i++) {
        fflush(stdout);
        sleep(1); // or Sleep(1000) on Windows
        printf(".");
    }
    printf("\n");
}

// Add this new function for password reset
void forgotPassword() {
    clearConsole();
    printf("========================\n");
    printf("    \033[1;33mFORGOT PASSWORD\033[0m         \n");
    printf("========================\n");
    
    int accountNo;
    char newPassword[50];
    struct User* temp = userHead;
    
    printf("\nEnter your Account Number: ");
    if (scanf("%d", &accountNo) != 1) {
        printf("\n\033[1;31mInvalid account number format!\033[0m\n");
        while(getchar() != '\n');
        printf("\nEnter any key to proceed...");
        getch();
        return;
    }
    while(getchar() != '\n');
    
    // Find user
    while (temp != NULL) {
        if (temp->accountNumber == accountNo) {
            // Verify PIN for security
            int pin;
            printf("\nEnter your PIN: ");
            if (scanf("%d", &pin) != 1) {
                printf("\n\033[1;31mInvalid PIN format!\033[0m\n");
                while(getchar() != '\n');
                printf("\nEnter any key to proceed...");
                getch();
                return;
            }
            while(getchar() != '\n');
            
            if (temp->pin != pin) {
                printf("\n\033[1;31mInvalid PIN!\033[0m\n");
                printf("\nEnter any key to proceed...");
                getch();
                return;
            }
            
            // Get new password
            do {
                printf("\nEnter new password (min 8 chars, must contain uppercase, lowercase, and number): ");
                scanf("%s", newPassword);
                while(getchar() != '\n');
                
                if (!isStrongPassword(newPassword)) {
                    printf("\n\033[1;31mPassword is not strong enough! Please try again.\033[0m\n");
                } else {
                    strcpy(temp->password, newPassword);
                    printf("\n\033[1;32mPassword changed successfully!\033[0m\n");
                    saveAllData();
                    printf("\nEnter any key to proceed...");
                    getch();
                    return;
                }
            } while (1);
        }
        temp = temp->next;
    }
    printf("\n\033[1;31mAccount not found!\033[0m\n");
    printf("\nEnter any key to proceed...");
    getch();
}

// Add new function for user profile management
void manageUserProfile() {
    if (!verifyPIN()) return;
    clearConsole();
    
    struct User* temp = userHead;
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("===================================\n");
            printf("      \033[1;32mMANAGE PROFILE\033[0m         \n");
            printf("===================================\n");
            printf("\nCurrent Information:\n");
            printf("Username: \033[1;33m%s\033[0m\n", temp->username);
            printf("First Name: \033[1;33m%s\033[0m\n", temp->firstName);
            printf("Last Name: \033[1;33m%s\033[0m\n", temp->lastName);
            printf("Address: \033[1;33m%s\033[0m\n", temp->address);
            printf("Phone: \033[1;33m%s\033[0m\n", temp->phoneNumber);
            printf("PIN: \033[1;33m****\033[0m\n");
            
            printf("\nEnter new information (press Enter to keep current value):\n");
            
            char input[100];
            char newUsername[50];
            int newPin;
            
            // Username change
            printf("\nNew Username: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                strcpy(newUsername, input);
                // Check if username is unique
                struct User* checkUser = userHead;
                int isUnique = 1;
                while (checkUser != NULL) {
                    if (strcmp(checkUser->username, newUsername) == 0 && checkUser != temp) {
                        isUnique = 0;
                        break;
                    }
                    checkUser = checkUser->next;
                }
                if (isUnique) {
                    strcpy(temp->username, newUsername);
                } else {
                    printf("\n\033[1;31mUsername already exists! Username not changed.\033[0m\n");
                }
            }
            
            // Password change
            printf("\nNew Password (press Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                if (isStrongPassword(input)) {
                    strcpy(temp->password, input);
                } else {
                    printf("\n\033[1;31mPassword is not strong enough! Password not changed.\033[0m\n");
                }
            }
            
            // Other information changes
            printf("\nNew First Name: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->firstName, input);
            
            printf("\nNew Last Name: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->lastName, input);
            
            printf("\nNew Address: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->address, input);
            
            printf("\nNew Phone Number: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) strcpy(temp->phoneNumber, input);
            
            // Add PIN change
            printf("\nNew PIN (press Enter to skip): ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) > 0) {
                newPin = atoi(input);
                if (newPin >= 1000 && newPin <= 9999) {
                    temp->pin = newPin;
                } else {
                    printf("\n\033[1;31mPIN must be exactly 4 digits! PIN not changed.\033[0m\n");
                }
            }
            
            printf("\n\033[1;32mProfile updated successfully!\033[0m\n");
            saveAllData();
            printf("\nEnter any key to proceed...");
            getch();
            return;
        }
        temp = temp->next;
    }
}

// Function to send a message
void sendMessage(int recipientAccountNo) {
    struct Message* newMessage = (struct Message*)calloc(1, sizeof(struct Message));
    struct User* sender = userHead;
    struct User* recipient = userHead;
    char subject[100];
    char content[500];
    
    // Find sender's information
    while (sender != NULL) {
        if (sender->accountNumber == currentUserAccountNo) {
            break;
        }
        sender = sender->next;
    }
    
    // Special handling for admin account
    if (recipientAccountNo == 123123) {
        printf("\nEnter subject: ");
        fgets(subject, sizeof(subject), stdin);
        subject[strcspn(subject, "\n")] = 0;
        
        printf("\nEnter message content: ");
        fgets(content, sizeof(content), stdin);
        content[strcspn(content, "\n")] = 0;
        
        newMessage->senderAccountNo = currentUserAccountNo;
        newMessage->recipientAccountNo = 123123; // Admin account number
        strcpy(newMessage->senderName, sender->firstName);
        strcat(newMessage->senderName, " ");
        strcat(newMessage->senderName, sender->lastName);
        strcpy(newMessage->subject, subject);
        strcpy(newMessage->content, content);
        newMessage->timestamp = time(NULL);
        newMessage->isRead = 0;
        
        newMessage->next = messageHead;
        messageHead = newMessage;
        
        printf("\n\033[1;32mMessage sent successfully to admin!\033[0m\n");
        return;
    }
    
    // Regular user message handling
    while (recipient != NULL) {
        if (recipient->accountNumber == recipientAccountNo) {
            break;
        }
        recipient = recipient->next;
    }
    
    if (recipient == NULL) {
        printf("\n\033[1;31mRecipient account not found!\033[0m\n");
        return;
    }
    
    printf("\nEnter subject: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;
    
    printf("\nEnter message content: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    
    newMessage->senderAccountNo = currentUserAccountNo;
    newMessage->recipientAccountNo = recipientAccountNo;
    strcpy(newMessage->senderName, sender->firstName);
    strcat(newMessage->senderName, " ");
    strcat(newMessage->senderName, sender->lastName);
    strcpy(newMessage->subject, subject);
    strcpy(newMessage->content, content);
    newMessage->timestamp = time(NULL);
    newMessage->isRead = 0;
    
    newMessage->next = messageHead;
    messageHead = newMessage;
    
    printf("\n\033[1;32mMessage sent successfully!\033[0m\n");
}

// Function to send message to admin
void messageAdmin() {
    sendMessage(123123); // Admin account number
}

// Function to view inbox
void viewInbox() {
    struct Message* temp = messageHead;
    int found = 0;
    
    printf("\n============================================\n");
    printf("                \033[1;33mINBOX\033[0m                \n");
    printf("============================================\n");
    
    while (temp != NULL) {
        if (temp->recipientAccountNo == currentUserAccountNo) {
            printf("\nFrom: \033[1;33m%s\033[0m\n", temp->senderName);
            printf("Subject: \033[1;36m%s\033[0m\n", temp->subject);
            printf("Time: \033[1;37m%s\033[0m", ctime(&temp->timestamp));
            printf("Content: \033[1;32m%s\033[0m\n", temp->content);
            printf("------------------------------------------\n");
            temp->isRead = 1;
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("\n\033[1;31mNo messages in inbox.\033[0m\n");
    }
}

// Function to save messages to file
void saveMessagesToFile() {
    FILE *fp = fopen("messages.txt", "w");
    if (!fp) return;
    struct Message *temp = messageHead;
    while (temp) {
        fprintf(fp, "%d|%d|%s|%s|%s|%ld|%d\n",
            temp->senderAccountNo, temp->recipientAccountNo,
            temp->senderName, temp->subject, temp->content,
            (long)temp->timestamp, temp->isRead);
        temp = temp->next;
    }
    fclose(fp);
}

// Function to load messages from file
void loadMessagesFromFile() {
    FILE *fp = fopen("messages.txt", "r");
    if (!fp) return;
    char buf[1000];
    while (fgets(buf, sizeof(buf), fp)) {
        struct Message *newMsg = (struct Message*)calloc(1, sizeof(struct Message));
        long t;
        sscanf(buf, "%d|%d|%49[^|]|%99[^|]|%499[^|]|%ld|%d",
            &newMsg->senderAccountNo, &newMsg->recipientAccountNo,
            newMsg->senderName, newMsg->subject, newMsg->content,
            &t, &newMsg->isRead);
        newMsg->timestamp = (time_t)t;
        newMsg->next = messageHead;
        messageHead = newMsg;
    }
    fclose(fp);
}

// Add this new function for admin message system
void adminMessageSystem() {
    clearConsole();
    printf("============================================\n");
    printf("             \033[1;32mADMIN MESSAGE SYSTEM\033[0m              \n");
    printf("============================================\n");
    printf("1. View All Messages\n");
    printf("2. View Admin Inbox\n");
    printf("3. Send Message to User\n");
    printf("4. Broadcast Message to All Users\n");
    printf("5. Back to Main Menu\n");
    printf("Enter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    while(getchar() != '\n');
    
    switch (choice) {
        case 1:
            viewAllMessages();
            break;
        case 2:
            viewAdminMessages();
            break;
        case 3:
            sendMessageToUser();
            break;
        case 4:
            broadcastMessage();
            break;
        case 5:
            return;
        default:
            printf("\n\033[1;31mInvalid choice!\033[0m\n");
    }
}

// Function to view all messages in the system
void viewAllMessages() {
    struct Message* temp = messageHead;
    int found = 0;
    
    printf("\n============================================\n");
    printf("                \033[1;33mALL MESSAGES\033[0m                \n");
    printf("============================================\n");
    
    while (temp != NULL) {
        printf("\nFrom: \033[1;33m%s\033[0m\n", temp->senderName);
        printf("To: \033[1;36mAccount #%d\033[0m\n", temp->recipientAccountNo);
        printf("Subject: \033[1;36m%s\033[0m\n", temp->subject);
        printf("Time: \033[1;37m%s\033[0m", ctime(&temp->timestamp));
        printf("Content: \033[1;32m%s\033[0m\n", temp->content);
        printf("Status: %s\n", temp->isRead ? "\033[1;32mRead\033[0m" : "\033[1;31mUnread\033[0m");
        printf("------------------------------------------\n");
        found = 1;
        temp = temp->next;
    }
    
    if (!found) {
        printf("\n\033[1;31mNo messages in the system.\033[0m\n");
    }
}

// Function for admin to send message to specific user
void sendMessageToUser() {
    int recipientAccNo;
    struct Message* newMessage = (struct Message*)calloc(1, sizeof(struct Message));
    char subject[100];
    char content[500];
    
    printf("\nEnter recipient's account number: ");
    if (scanf("%d", &recipientAccNo) != 1) {
        printf("\n\033[1;31mInvalid account number!\033[0m\n");
        while(getchar() != '\n');
        return;
    }
    while(getchar() != '\n');
    
    // Verify account exists
    struct User* recipient = userHead;
    while (recipient != NULL) {
        if (recipient->accountNumber == recipientAccNo) {
            break;
        }
        recipient = recipient->next;
    }
    
    if (recipient == NULL) {
        printf("\n\033[1;31mRecipient account not found!\033[0m\n");
        return;
    }
    
    printf("\nEnter subject: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;
    
    printf("\nEnter message content: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    
    newMessage->senderAccountNo = 123123; // Admin account number
    newMessage->recipientAccountNo = recipientAccNo;
    strcpy(newMessage->senderName, "Admin");
    strcpy(newMessage->subject, subject);
    strcpy(newMessage->content, content);
    newMessage->timestamp = time(NULL);
    newMessage->isRead = 0;
    
    newMessage->next = messageHead;
    messageHead = newMessage;
    
    printf("\n\033[1;32mMessage sent successfully!\033[0m\n");
}

// Function for admin to broadcast message to all users
void broadcastMessage() {
    struct User* temp = userHead;
    char subject[100];
    char content[500];
    
    printf("\nEnter broadcast subject: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = 0;
    
    printf("\nEnter broadcast content: ");
    fgets(content, sizeof(content), stdin);
    content[strcspn(content, "\n")] = 0;
    
    while (temp != NULL) {
        if (temp->accountNumber != 123123) { // Don't send to admin account
            struct Message* newMessage = (struct Message*)calloc(1, sizeof(struct Message));
            newMessage->senderAccountNo = 123123; // Admin account number
            newMessage->recipientAccountNo = temp->accountNumber;
            strcpy(newMessage->senderName, "Admin");
            strcpy(newMessage->subject, subject);
            strcpy(newMessage->content, content);
            newMessage->timestamp = time(NULL);
            newMessage->isRead = 0;
            
            newMessage->next = messageHead;
            messageHead = newMessage;
        }
        temp = temp->next;
    }
    
    printf("\n\033[1;32mBroadcast message sent to all users!\033[0m\n");
}

// Add this new function to view messages sent to admin
void viewAdminMessages() {
    struct Message* temp = messageHead;
    int found = 0;
    
    printf("\n============================================\n");
    printf("            \033[1;33mADMIN INBOX\033[0m                \n");
    printf("============================================\n");
    
    while (temp != NULL) {
        if (temp->recipientAccountNo == 123123) { // Check for admin account number
            printf("\nFrom: \033[1;33m%s\033[0m\n", temp->senderName);
            printf("Subject: \033[1;36m%s\033[0m\n", temp->subject);
            printf("Time: \033[1;37m%s\033[0m", ctime(&temp->timestamp));
            printf("Content: \033[1;32m%s\033[0m\n", temp->content);
            printf("Status: %s\n", temp->isRead ? "\033[1;32mRead\033[0m" : "\033[1;31mUnread\033[0m");
            printf("------------------------------------------\n");
            temp->isRead = 1; // Mark message as read
            found = 1;
        }
        temp = temp->next;
    }
    
    if (!found) {
        printf("\n\033[1;31mNo messages in admin inbox.\033[0m\n");
    }
}

// Add these new analytics functions after the existing functions but before main()

// Function to calculate total deposits
float calculateTotalDeposits() {
    struct Transaction* temp = transactionHead;
    float total = 0.0;
    
    while (temp != NULL) {
        if (strcmp(temp->transactionType, "Deposit") == 0) {
            total += temp->amount;
        }
        temp = temp->next;
    }
    return total;
}

// Function to calculate total withdrawals
float calculateTotalWithdrawals() {
    struct Transaction* temp = transactionHead;
    float total = 0.0;
    
    while (temp != NULL) {
        if (strcmp(temp->transactionType, "Withdrawal") == 0) {
            total += temp->amount;
        }
        temp = temp->next;
    }
    return total;
}

// Function to calculate total transfers
float calculateTotalTransfers() {
    struct Transaction* temp = transactionHead;
    float total = 0.0;
    
    while (temp != NULL) {
        if (strcmp(temp->transactionType, "Transfer Out") == 0) {
            total += temp->amount;
        }
        temp = temp->next;
    }
    return total;
}

// Function to count active loans
int countActiveLoans() {
    struct User* temp = userHead;
    int count = 0;
    
    while (temp != NULL) {
        if (temp->hasActiveLoan) {
            count++;
        }
        temp = temp->next;
    }
    return count;
}

// Function to calculate total loan amount
float calculateTotalLoanAmount() {
    struct User* temp = userHead;
    float total = 0.0;
    
    while (temp != NULL) {
        if (temp->hasActiveLoan) {
            total += temp->loanBalance;
        }
        temp = temp->next;
    }
    return total;
}

// Function to count account types
void countAccountTypes(int* savings, int* checking) {
    struct Account* temp = head;
    *savings = 0;
    *checking = 0;
    
    while (temp != NULL) {
        if (strcmp(temp->accountType, "Savings") == 0) {
            (*savings)++;
        } else if (strcmp(temp->accountType, "Checking") == 0) {
            (*checking)++;
        }
        temp = temp->next;
    }
}

// Function to display analytics
void displayAnalytics() {
    clearConsole();
    printf("============================================\n");
    printf("             \033[1;32mBANKING ANALYTICS\033[0m              \n");
    printf("============================================\n\n");
    
    // Transaction Analytics
    printf("\033[1;33mTransaction Analytics:\033[0m\n");
    printf("------------------------------------------\n");
    printf("Total Deposits: \033[1;36m%.2f\033[0m\n", calculateTotalDeposits());
    printf("Total Withdrawals: \033[1;36m%.2f\033[0m\n", calculateTotalWithdrawals());
    printf("Total Transfers: \033[1;36m%.2f\033[0m\n", calculateTotalTransfers());
    
    // Account Analytics
    int savingsCount, checkingCount;
    countAccountTypes(&savingsCount, &checkingCount);
    printf("\n\033[1;33mAccount Analytics:\033[0m\n");
    printf("------------------------------------------\n");
    printf("Total Savings Accounts: \033[1;36m%d\033[0m\n", savingsCount);
    printf("Total Checking Accounts: \033[1;36m%d\033[0m\n", checkingCount);
    printf("Total Accounts: \033[1;36m%d\033[0m\n", savingsCount + checkingCount);
    
    // Loan Analytics
    printf("\n\033[1;33mLoan Analytics:\033[0m\n");
    printf("------------------------------------------\n");
    printf("Active Loans: \033[1;36m%d\033[0m\n", countActiveLoans());
    printf("Total Loan Amount: \033[1;36m%.2f\033[0m\n", calculateTotalLoanAmount());
    
    // Message Analytics
    struct Message* temp = messageHead;
    int totalMessages = 0;
    int unreadMessages = 0;
    
    while (temp != NULL) {
        totalMessages++;
        if (!temp->isRead) {
            unreadMessages++;
        }
        temp = temp->next;
    }
    
    printf("\n\033[1;33mMessage Analytics:\033[0m\n");
    printf("------------------------------------------\n");
    printf("Total Messages: \033[1;36m%d\033[0m\n", totalMessages);
    printf("Unread Messages: \033[1;36m%d\033[0m\n", unreadMessages);
    
    printf("\nEnter any key to proceed...");
    getch();
}

int main() {
    srand(time(NULL));  // Initialize random number generator
    int choice, loggedIn = 0;
    char username[50];
    int isAdmin;
    char receiptChoice;

    // Load data from files
    loadAllData();
    
    time_t lastInterestCheck = time(NULL);
    
    while (1) {
        if (!loggedIn) {
            clearConsole();
            printWelcomeBanner();
            printf("1. Login\n");
            printf("2. Sign Up\n");
            printf("3. Forgot Password\n");
            printf("4. Exit\n");
            printf("--------------------------------------------\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            while(getchar() != '\n');
            
            switch (choice) {
                case 1:
                    isAdmin = login(username);
                    if (isAdmin >= 0) loggedIn = 1;
                    break;
                case 2:
                    signUp();
                    saveAllData();
                    break;
                case 3:
                    forgotPassword();
                    break;
                case 4:
                    printf("Exiting...\n");
                    return 0;
                default:
                    printf("\n\033[1;31mInvalid choice! Please try again.\033[0m\n");
                    printf("\nEnter any key to proceed...");
			    	getch();
            }
        } else {
            if (isAdmin == 1) {  // Admin menu
			    clearConsole();
			    printf("============================================\n");
			    printf("             \033[1;32mBANK SYSTEM MENU\033[0m              \n");
			    printf("============================================\n");
			    printf("1. View All Users\n");
			    printf("2. View Specific User\n");
			    printf("3. Edit User Information\n");
			    printf("4. View User Transactions\n");
			    printf("5. Restrict/Unrestrict User\n");
			    printf("6. Delete User Account\n");
			    printf("7. Admin Message System\n");
			    printf("8. Banking Analytics\n");
			    printf("9. Logout\n");
			    printf("Enter your choice: ");
			    scanf("%d", &choice);
				clearConsole();
			    switch (choice) {
			        case 1:
			            viewAllUsers();
			            break;
			        case 2:
			            viewSpecificUser();
			            break;
			        case 3:
			            editUserInfo();
			            break;
			        case 4:
			            viewUserTransactions();
			            break;
			        case 5:
			            toggleUserRestriction();
			            break;
			        case 6:
			            deleteUserAccount();
			            break;
			        case 7:
			            adminMessageSystem();
			            break;
			        case 8:
			            displayAnalytics();
			            break;
			        case 9:
			            loggedIn = 0;
			            printf("Logged out successfully!\n");
			            break;
			        default:
			            printf("Invalid choice! Please try again.\n");
			    }
			    printf("\nEnter any key to proceed...");
    			getch();
			} else {  // Regular user menu
                clearConsole();
                printf("============================================\n");
                printf("             \033[1;32mBANK SYSTEM MENU\033[0m              \n");
                printf("============================================\n");
                printf("1. Deposit\n\n");
                printf("2. Withdraw\n\n");
                printf("3. Transfer\n\n");
                printf("4. Check Balance\n\n");
                printf("5. Apply for Loan\n\n");
                printf("6. Pay Loan\n\n");
                printf("7. Check Loan Status\n\n");
                printf("8. View Transaction History\n\n");
                printf("9. Display Account Information\n\n");
                printf("10. Manage Profile\n\n");
                printf("11. Message System\n\n");
                printf("12. Logout\n\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);
            
                time_t currentTime = time(NULL);
                if (difftime(currentTime, lastInterestCheck) >= 86400) { // Check daily
                    calculateInterest();
                    lastInterestCheck = currentTime;
                    
                    // Reset monthly withdrawals on the first day of each month
                    struct tm* timeinfo = localtime(&currentTime);
                    if (timeinfo->tm_mday == 1) {
                        resetMonthlyWithdrawals();
                    }
                }
            
                switch (choice) {
                    case 1: 
                        deposit(); 
                        saveAllData();
                        break;
                    case 2: 
                        withdraw(); 
                        saveAllData();
                        break;
                    case 3: 
                        transfer(); 
                        saveAllData();
                        break;
                    case 4: 
                        checkBalance(); 
                        saveAllData();
                        break;
                    case 5:
                        applyLoan();
                        saveAllData();
                        break;
                    case 6:
                        payLoan();
                        printf("\nEnter any key to proceed...");
    					getch();
                        saveAllData();
                        break;
                    case 7:
                        checkLoanStatus();
                        printf("\nEnter any key to proceed...");
    					getch();
                        break;
                    case 8:
                        viewTransactionHistory();
                        printf("\nEnter any key to proceed...");
    					getch();
                        break;
                    case 9:
                        displayAccountInfo();
                        printf("\nEnter any key to proceed...");
    					getch();
                        break;
                    case 10:
                        manageUserProfile();
                        break;
                    case 11:
                        clearConsole();
                        printf("============================================\n");
                        printf("             \033[1;32mMESSAGE SYSTEM\033[0m              \n");
                        printf("============================================\n");
                        printf("1. Send Message to User\n");
                        printf("2. Message Admin\n");
                        printf("3. View Inbox\n");
                        printf("4. Back to Main Menu\n");
                        printf("Enter your choice: ");
                        scanf("%d", &choice);
                        while(getchar() != '\n');
                        
                        switch (choice) {
                            case 1:
                                printf("\nEnter recipient's account number: ");
                                int recipientAccNo;
                                scanf("%d", &recipientAccNo);
                                while(getchar() != '\n');
                                sendMessage(recipientAccNo);
                                break;
                            case 2:
                                messageAdmin();
                                break;
                            case 3:
                                viewInbox();
                                break;
                            case 4:
                                break;
                            default:
                                printf("\n\033[1;31mInvalid choice!\033[0m\n");
                        }
                        printf("\nEnter any key to proceed...");
                        getch();
                        break;
                    case 12: 
                        printf("Would you like a receipt of your transactions? (Y/N): ");
                        scanf(" %c", &receiptChoice);
                        while(getchar() != '\n');
                        
                        if (toupper(receiptChoice) == 'Y') {
                            generateReceipt(currentUserAccountNo);
                        }
                        
                        loggedIn = 0; 
                        saveAllData();
                        printf("Logged out successfully!\n");
                        break;
                    default: 
                    printf("\n\033[1;31mInvalid choice! Please try again.\033[0m\n");
                    printf("\nEnter any key to proceed...");
			    	getch();
                }
            }
        }
    }
    return 0;
}



