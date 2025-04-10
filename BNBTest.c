#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//I DON'T HAVE A PARTNER

// Define structure for user credentials
struct User {
    char username[50];
    char password[50];
    char firstName[50];
    char lastName[50];
    char address[100];
    char phoneNumber[15];
    int pin;
    int accountNumber;  // Store account number with user info
    int isAdmin;
    struct User* next;
};

// Define structure for bank account
struct Account {
    int accountNumber;
    char name[50];
    float balance;
    char username[50];
    struct Account* next;
};

// Define structure for transactions
struct Transaction {
    int accountNumber;
    char transactionType[20];  // "Deposit", "Withdrawal", "Transfer"
    float amount;
    float balanceAfter;
    time_t timestamp;
    int recipientAccountNumber;  // For transfer transactions
    char recipientName[50];      // For transfer transactions
    struct Transaction* next;
};

// Global variables
struct User* userHead = NULL;
struct Account* head = NULL;
struct Transaction* transactionHead = NULL;
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
    struct Transaction* newTransaction = (struct Transaction*)malloc(sizeof(struct Transaction));
    
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
    struct User* newUser;
    struct User* temp;
    int pin;
    
    newUser = (struct User*)malloc(sizeof(struct User));
    printf("\n=== Sign Up ===\n");
    
    // Get personal information
    printf("Enter First Name: ");
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
            printf("Username already exists! Please try again.\n");
            free(newUser);
            return;
        }
        temp = temp->next;
    }
    
    // Get password
    do {
        printf("Create Password (min 8 chars, must contain uppercase, lowercase, and number): ");
        scanf("%s", newUser->password);
        while(getchar() != '\n');
        if (!isStrongPassword(newUser->password)) {
            printf("Password is not strong enough! Please try again.\n");
        }
    } while (!isStrongPassword(newUser->password));
    
    // Get PIN
    do {
        printf("Create 4-digit PIN: ");
        scanf("%d", &pin);
        while(getchar() != '\n');
        
        if (pin < 1000 || pin > 9999) {
            printf("PIN must be exactly 4 digits! Please try again.\n");
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
    
    // Create a corresponding account in the head list
    struct Account* newAccount = (struct Account*)malloc(sizeof(struct Account));
    newAccount->accountNumber = newAccNo;
    strcpy(newAccount->name, newUser->firstName);
    strcat(newAccount->name, " ");
    strcat(newAccount->name, newUser->lastName);
    newAccount->balance = 0.0;
    strcpy(newAccount->username, newUser->username);
    newAccount->next = head;
    head = newAccount;
    
    // Set the current user account number
    currentUserAccountNo = newAccNo;
    
    // Record account creation as a transaction
    addTransaction(newAccNo, "Account Creation", 0.0, 0.0, 0, "");
    
    printf("\n=== Account Created Successfully ===\n");
    printf("Your Account Number: %d\n", newAccNo);
    printf("Please keep your account number safe!\n");
    printf("You can now login using your account number.\n");
}

// Function to login
int login(char* username) {
    int accountNo, pin;
    struct User* temp;
    
    printf("\n=== Login ===\n");
    printf("Enter Account Number: ");
    if (scanf("%d", &accountNo) != 1) {
        printf("Invalid account number format!\n");
        while(getchar() != '\n');
        return -1;
    }
    while(getchar() != '\n');
    
    temp = userHead;
    while (temp != NULL) {
        if (temp->accountNumber == accountNo) {
            printf("Enter PIN: ");
            if (scanf("%d", &pin) != 1) {
                printf("Invalid PIN format!\n");
                while(getchar() != '\n');
                return -1;
            }
            while(getchar() != '\n');
            
            if (temp->pin == pin) {
                strcpy(username, temp->username);
                currentUserAccountNo = accountNo;
                printf("Login successful!\n");
                printf("Welcome, %s %s!\n", temp->firstName, temp->lastName);
                return temp->isAdmin;
            } else {
                printf("Invalid PIN!\n");
                return -1;
            }
        }
        temp = temp->next;
    }
    printf("Account not found!\n");
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
        printf("User account not found!\n");
        return 0;
    }
    
    printf("Enter PIN: ");
    if (scanf("%d", &pin) != 1) {
        printf("Invalid PIN format!\n");
        while(getchar() != '\n');
        return 0;
    }
    while(getchar() != '\n');
    
    if (temp->pin == pin) {
        return 1;
    } else {
        printf("Invalid PIN!\n");
        return 0;
    }
}

// Function to deposit money
void deposit() {
    if (!verifyPIN()) return;
    
    float amount;
    struct Account* temp = head;
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("Enter amount to deposit: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount! Please enter a positive number.\n");
                while(getchar() != '\n'); // Clear input buffer
                return;
            }
            
            temp->balance += amount;
            printf("Amount deposited successfully! New Balance: %.2f\n", temp->balance);
            
            // Record transaction
            addTransaction(currentUserAccountNo, "Deposit", amount, temp->balance, 0, "");
            return;
        }
        temp = temp->next;
    }
    
    printf("Account not found!\n");
}

// Function to withdraw money
void withdraw() {
    if (!verifyPIN()) return;
    
    float amount;
    struct Account* temp = head;
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("Enter amount to withdraw: ");
            if (scanf("%f", &amount) != 1 || amount <= 0) {
                printf("Invalid amount! Please enter a positive number.\n");
                while(getchar() != '\n'); // Clear input buffer
                return;
            }
            
            if (amount > temp->balance) {
                printf("Insufficient balance!\n");
            } else {
                temp->balance -= amount;
                printf("Withdrawal successful! New Balance: %.2f\n", temp->balance);
                
                // Record transaction
                addTransaction(currentUserAccountNo, "Withdrawal", amount, temp->balance, 0, "");
            }
            return;
        }
        temp = temp->next;
    }
    
    printf("Account not found!\n");
}

// Function to check balance
void checkBalance() {
    if (!verifyPIN()) return;
    
    struct Account* temp = head;
    
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) {
            printf("\n=== Account Information ===\n");
            printf("Account Number: %d\n", temp->accountNumber);
            printf("Current Balance: %.2f\n", temp->balance);
            
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
    
    int toAccNo;
    float amount;
    struct Account *fromAcc = NULL, *toAcc = NULL;
    struct Account* temp;
    char recipientName[50] = "";
    
    printf("Enter recipient's account number: ");
    if (scanf("%d", &toAccNo) != 1) {
        printf("Invalid account number!\n");
        while(getchar() != '\n'); // Clear input buffer
        return;
    }
    
    // Check if trying to transfer to the same account
    if (toAccNo == currentUserAccountNo) {
        printf("Cannot transfer to the same account!\n");
        return;
    }
    
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == currentUserAccountNo) fromAcc = temp;
        if (temp->accountNumber == toAccNo) {
            toAcc = temp;
            strcpy(recipientName, temp->name);
        }
        temp = temp->next;
    }
    
    if (toAcc == NULL) {
        printf("Recipient account not found!\n");
        return;
    }
    
    printf("Enter amount to transfer: ");
    if (scanf("%f", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Please enter a positive number.\n");
        while(getchar() != '\n'); // Clear input buffer
        return;
    }
    
    if (amount > fromAcc->balance) {
        printf("Insufficient balance!\n");
        return;
    }
    
    fromAcc->balance -= amount;
    toAcc->balance += amount;
    printf("Transfer successful!\n");
    printf("Your new balance: %.2f\n", fromAcc->balance);
    
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

int main() {
    srand(time(NULL));  // Initialize random number generator
    int choice, loggedIn = 0;
    char username[50];
    int isAdmin;
    char receiptChoice;
    
    while (1) {
        if (!loggedIn) {
            printf("\n=== Welcome to Banco Ni Bins ===\n");
            printf("1. Login\n");
            printf("2. Sign Up\n");
            printf("3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            
            switch (choice) {
                case 1:
                    isAdmin = login(username);
                    if (isAdmin >= 0) loggedIn = 1;
                    break;
                case 2:
                    signUp();
                    break;
                case 3:
                    printf("Exiting...\n");
                    return 0;
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } else {
            printf("\n===== Bank System Menu =====\n");
            printf("1. Deposit\n");
            printf("2. Withdraw\n");
            printf("3. Transfer\n");
            printf("4. Check Balance\n");
            printf("5. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: 
                    deposit(); 
                    break;
                case 2: 
                    withdraw(); 
                    break;
                case 3: 
                    transfer(); 
                    break;
                case 4: 
                    checkBalance(); 
                    break;
                case 5: 
                    printf("Would you like a receipt of your transactions? (Y/N): ");
                    scanf(" %c", &receiptChoice);
                    while(getchar() != '\n');
                    
                    if (toupper(receiptChoice) == 'Y') {
                        generateReceipt(currentUserAccountNo);
                    }
                    
                    loggedIn = 0; 
                    printf("Logged out successfully!\n");
                    break;
                default: 
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }
    return 0;
}

