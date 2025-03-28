#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//I DON'T HAVE A PARTNER

// Define structure for user credentials
struct User {
    char username[50];
    char password[50];
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

// Global variables
struct User* userHead = NULL;
struct Account* head = NULL;
int accountCounter = 1001;

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

// Function to sign up
void signUp() {
    struct User* newUser;
    struct User* temp;
    
    newUser = (struct User*)malloc(sizeof(struct User));
    printf("\n=== Sign Up ===\n");
    printf("Enter username: ");
    fflush(stdin);
    gets(newUser->username);
    
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
    
    do {
        printf("Enter password (min 8 chars, must contain uppercase, lowercase, and number): ");
        fflush(stdin);
        gets(newUser->password);
        if (!isStrongPassword(newUser->password)) {
            printf("Password is not strong enough! Please try again.\n");
        }
    } while (!isStrongPassword(newUser->password));
    
    newUser->isAdmin = 0;
    newUser->next = userHead;
    userHead = newUser;
    printf("Sign up successful!\n");
}

// Function to login
int login(char* username) {
    char password[50];
    struct User* temp;
    
    printf("\n=== Login ===\n");
    printf("Enter username: ");
    fflush(stdin);
    gets(username);
    printf("Enter password: ");
    fflush(stdin);
    gets(password);
    
    temp = userHead;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && 
            strcmp(temp->password, password) == 0) {
            printf("Login successful!\n");
            return temp->isAdmin;
        }
        temp = temp->next;
    }
    printf("Invalid credentials!\n");
    return -1;
}

// Function to create account
void createAccount(char* username) {
    struct Account* newAccount = (struct Account*)malloc(sizeof(struct Account));
    newAccount->accountNumber = accountCounter++;
    printf("Enter name: ");
    fflush(stdin);
    gets(newAccount->name);
    newAccount->balance = 0.0;
    strcpy(newAccount->username, username);
    newAccount->next = head;
    head = newAccount;
    printf("Account created successfully! Account Number: %d\n", newAccount->accountNumber);
}

// Function to deposit money
void deposit() {
    int accNo;
    float amount;
    struct Account* temp;
    
    printf("Enter account number: ");
    scanf("%d", &accNo);
    
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            temp->balance += amount;
            printf("Amount deposited successfully! New Balance: %.2f\n", temp->balance);
            return;
        }
        temp = temp->next;
    }
    printf("Account not found!\n");
}

// Function to withdraw money
void withdraw() {
    int accNo;
    float amount;
    struct Account* temp;
    
    printf("Enter account number: ");
    scanf("%d", &accNo);
    
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (amount > temp->balance) {
                printf("Insufficient balance!\n");
            } else {
                temp->balance -= amount;
                printf("Withdrawal successful! New Balance: %.2f\n", temp->balance);
            }
            return;
        }
        temp = temp->next;
    }
    printf("Account not found!\n");
}

// Function to check balance
void checkBalance() {
    int accNo;
    struct Account* temp;
    
    printf("Enter account number: ");
    scanf("%d", &accNo);
    
    temp = head;
    while (temp != NULL) {
        if (temp->accountNumber == accNo) {
            printf("Account Balance: %.2f\n", temp->balance);
            return;
        }
        temp = temp->next;
    }
    printf("Account not found!\n");
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

int main() {
    int choice, loggedIn = 0;
    char username[50];
    int isAdmin;
    
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
            printf("1. Create Account\n");
            printf("2. Deposit\n");
            printf("3. Withdraw\n");
            printf("4. Check Balance\n");
            printf("5. Display All Accounts\n");
            printf("6. Logout\n");
            printf("7. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: 
                    createAccount(username); 
                    break;
                case 2: 
                    deposit(); 
                    break;
                case 3: 
                    withdraw(); 
                    break;
                case 4: 
                    checkBalance(); 
                    break;
                case 5: 
                    if (isAdmin) 
                        displayAccounts();
                    else 
                        printf("Only admins can view all accounts!\n");
                    break;
                case 6: 
                    loggedIn = 0; 
                    printf("Logged out successfully!\n");
                    break;
                case 7: 
                    printf("Exiting...\n"); 
                    return 0;
                default: 
                    printf("Invalid choice! Please try again.\n");
            }
        }
    }
    return 0;
}

