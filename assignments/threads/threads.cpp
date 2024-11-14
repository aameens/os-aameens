/*
Simple threading program, uses multiple threads to make simultaneous
deposits and withdrawals in a "bank account", using mutex to ensure
that the balance remains accurate.

To run:
    g++ -o threads threads.cpp -pthread
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Structure to represent a bank account
typedef struct {
    double balance;
    pthread_mutex_t lock;  // Mutex to protect the balance
} BankAccount;

// Function to deposit money into the account
void deposit(BankAccount *account, double amount) {
    pthread_mutex_lock(&account->lock);  // Lock the mutex
    account->balance += amount;
    printf("Deposited %.2f, new balance: %.2f\n", amount, account->balance);
    pthread_mutex_unlock(&account->lock);  // Unlock the mutex
}

// Function to withdraw money from the account
void withdraw(BankAccount *account, double amount) {
    pthread_mutex_lock(&account->lock);  // Lock the mutex
    if (account->balance >= amount) {
        account->balance -= amount;
        printf("Withdrew %.2f, new balance: %.2f\n", amount, account->balance);
    } else {
        printf("Insufficient funds to withdraw %.2f, balance: %.2f\n", amount, account->balance);
    }
    pthread_mutex_unlock(&account->lock);  // Unlock the mutex
}

// Thread function for deposit
void* deposit_thread(void* arg) {
    BankAccount* account = (BankAccount*)arg;
    for (int i = 0; i < 5; i++) {
        deposit(account, 100.0);  // Simulate a deposit of 100
        usleep(100000);  // Sleep for 100ms
    }
    return NULL;
}

// Thread function for withdrawal
void* withdraw_thread(void* arg) {
    BankAccount* account = (BankAccount*)arg;
    for (int i = 0; i < 5; i++) {
        withdraw(account, 50.0);  // Simulate a withdrawal of 50
        usleep(150000);  // Sleep for 150ms
    }
    return NULL;
}

int main() {
    BankAccount account = {0.0, PTHREAD_MUTEX_INITIALIZER};  // Initialize account with 0 balance and mutex

    // Create threads for deposit and withdrawal
    pthread_t deposit_threads[3], withdraw_threads[3];

    // Create deposit threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&deposit_threads[i], NULL, deposit_thread, &account);
    }

    // Create withdrawal threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&withdraw_threads[i], NULL, withdraw_thread, &account);
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; i++) {
        pthread_join(deposit_threads[i], NULL);
        pthread_join(withdraw_threads[i], NULL);
    }

    printf("Final balance: %.2f\n", account.balance);
    
    // Destroy the mutex
    pthread_mutex_destroy(&account.lock);
    
    return 0;
}
