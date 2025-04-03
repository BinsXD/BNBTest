# Banco Ni Bins Banking System

A simple console-based banking system implemented in C that provides basic banking operations and user management functionality.

## Features

### User Management
- User registration with username and password
- Password strength validation (minimum 8 characters, must contain uppercase, lowercase, and numbers)
- User login system
- Admin and regular user roles

### Banking Operations
- Account creation
- Money deposit
- Money withdrawal
- Balance checking
- Account listing (admin only)

## Project Structure

### Data Structures
1. **User Structure**
   - Username
   - Password
   - Admin flag
   - Next user pointer (for linked list implementation)

2. **Account Structure**
   - Account number (auto-generated starting from 1001)
   - Account holder name
   - Balance
   - Associated username
   - Next account pointer (for linked list implementation)

### Main Functions
- `signUp()`: Handles new user registration
- `login()`: Manages user authentication
- `createAccount()`: Creates new bank accounts
- `deposit()`: Handles money deposits
- `withdraw()`: Manages money withdrawals
- `checkBalance()`: Displays account balance
- `displayAccounts()`: Shows all accounts (admin only)

## Usage

### Compilation
```bash
gcc BNBTest.c -o BNBTest
```

### Running the Program
```bash
./BNBTest
```

### Menu Options

#### Pre-Login Menu
1. Login
2. Sign Up
3. Exit

#### Post-Login Menu
1. Create Account
2. Deposit
3. Withdraw
4. Check Balance
5. Display All Accounts (Admin only)
6. Logout
7. Exit

## Security Features
- Password strength validation
- Username uniqueness check
- Admin-only access to certain features
- Balance validation for withdrawals

## Technical Details
- Written in C
- Uses linked lists for data storage
- Console-based interface
- Memory management using malloc/free

## Limitations
- Data is not persisted between sessions
- No encryption for passwords
- Limited to console interface
- No transaction history
- No interest calculation

## Future Improvements
1. Add data persistence using file storage
2. Implement password encryption
3. Add transaction history
4. Include interest calculation
5. Add account deletion functionality
6. Implement password recovery
7. Add input validation for numeric values
8. Include account transfer functionality

## Author
Vince L. Bernante

## License
This project is open source and available under the MIT License. 
