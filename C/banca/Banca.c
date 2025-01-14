#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "accounts.txt"
#define KEY "mysecretkey"

void byteToHex(unsigned char byte, char *hex) {
    sprintf(hex, "%02X", byte);
}

unsigned char hexToByte(const char *hex) {
    unsigned char byte;
    sscanf(hex, "%2hhX", &byte);
    return byte;
}

void xorEncryptToHex(const char *input, char *output, const char *key) {
    int keyLen = strlen(key);
    int inputLen = strlen(input);

    for (int i = 0; i < inputLen; i++) {
        unsigned char xorByte = input[i] ^ key[i % keyLen];
        byteToHex(xorByte, &output[i * 2]);
    }
    output[inputLen * 2] = '\0'; 
}

void xorDecryptFromHex(const char *input, char *output, const char *key) {
    int keyLen = strlen(key);
    int inputLen = strlen(input) / 2;

    for (int i = 0; i < inputLen; i++) {
        unsigned char xorByte = hexToByte(&input[i * 2]);
        output[i] = xorByte ^ key[i % keyLen];
    }
    output[inputLen] = '\0'; 
}

void saveAccount(const char *email, const char *password, int money, int moneyInBank) {
    char encryptedEmail[100];
    char encryptedPassword[100];

    xorEncryptToHex(email, encryptedEmail, KEY);
    xorEncryptToHex(password, encryptedPassword, KEY);

    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf("Error opening file for saving!\n");
        return;
    }
    fprintf(file, "%s %s %d %d\n", encryptedEmail, encryptedPassword, money, moneyInBank);
    fclose(file);
    printf("Account saved successfully.\n");
}

int loadAccount(const char *email, const char *password, int *money, int *moneyInBank) {
    char encryptedEmail[100], encryptedPassword[100];
    xorEncryptToHex(email, encryptedEmail, KEY);
    xorEncryptToHex(password, encryptedPassword, KEY);

    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("No accounts found. Please register first.\n");
        return 0;
    }

    char fileEmail[100], filePassword[100];
    int fileMoney, fileMoneyInBank;

    while (fscanf(file, "%s %s %d %d", fileEmail, filePassword, &fileMoney, &fileMoneyInBank) != EOF) {
        if (strcmp(encryptedEmail, fileEmail) == 0 && strcmp(encryptedPassword, filePassword) == 0) {
            *money = fileMoney;
            *moneyInBank = fileMoneyInBank;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void updateAccountDetails(const char *email, const char *password, int money, int moneyInBank) {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (!file || !tempFile) {
        printf("Error opening file!\n");
        return;
    }

    char fileEmail[100], filePassword[100];
    int fileMoney, fileMoneyInBank;

    while (fscanf(file, "%s %s %d %d", fileEmail, filePassword, &fileMoney, &fileMoneyInBank) != EOF) {
        if (strcmp(fileEmail, email) == 0 && strcmp(filePassword, password) == 0) {
            char encryptedEmail[100], encryptedPassword[100];
            xorEncryptToHex(email, encryptedEmail, KEY);
            xorEncryptToHex(password, encryptedPassword, KEY);
            fprintf(tempFile, "%s %s %d %d\n", encryptedEmail, encryptedPassword, money, moneyInBank);
        } else {
            fprintf(tempFile, "%s %s %d %d\n", fileEmail, filePassword, fileMoney, fileMoneyInBank);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);
}

void depositMoney(int *money, int *moneyInBank) {
    int deposit;
    printf("How much money would you like to deposit? ");
    scanf("%d", &deposit);

    if (deposit > *money) {
        printf("You do not have enough money to deposit.\n");
    } else {
        *money -= deposit;
        *moneyInBank += deposit;
        printf("You have successfully deposited %d money.\n", deposit);
    }
}

void withdrawMoney(int *money, int *moneyInBank) {
    int withdraw;
    printf("How much money would you like to withdraw? ");
    scanf("%d", &withdraw);

    if (withdraw > *moneyInBank) {
        printf("You cannot withdraw more than you have in the bank.\n");
    } else {
        *money += withdraw;
        *moneyInBank -= withdraw;
        printf("You have successfully withdrawn %d money.\n", withdraw);
    }
}

void transferMoney(int *moneyInBank) {
    char transferEmail[50];
    int transferAmount;
    printf("Enter the email of the user you want to transfer money to: ");
    scanf("%s", transferEmail);
    printf("Enter the amount to transfer: ");
    scanf("%d", &transferAmount);

    if (transferAmount > *moneyInBank) {
        printf("You cannot transfer more than you have in the bank.\n");
        return;
    }

    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile) {
        printf("Error opening file!\n");
        return;
    }

    char tempEmail[50], tempPassword[50];
    int tempMoney, tempMoneyInBank;
    int found = 0;

    while (fscanf(file, "%s %s %d %d", tempEmail, tempPassword, &tempMoney, &tempMoneyInBank) != EOF) {
        if (strcmp(tempEmail, transferEmail) == 0) {
            tempMoneyInBank += transferAmount;
            found = 1;
        }
        fprintf(tempFile, "%s %s %d %d\n", tempEmail, tempPassword, tempMoney, tempMoneyInBank);
    }

    fclose(file);
    fclose(tempFile);
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found) {
        *moneyInBank -= transferAmount;
        printf("Successfully transferred %d money to %s.\n", transferAmount, transferEmail);
    } else {
        printf("Account with email %s not found.\n", transferEmail);
    }
}

int main() {
    char choice;
    char email[50], password[50];
    int money = 100, moneyInBank = 500;

    while (1) {
        printf("Welcome to the banking system\n");
        printf("Do you want to Register (R) or Login (L)? ");
        scanf(" %c", &choice);

        if (choice == 'R' || choice == 'r') {
            printf("Enter email: ");
            scanf("%s", email);
            printf("Enter password: ");
            scanf("%s", password);
            saveAccount(email, password, money, moneyInBank);
        } else if (choice == 'L' || choice == 'l') {
            printf("Enter email: ");
            scanf("%s", email);
            printf("Enter password: ");
            scanf("%s", password);

            if (loadAccount(email, password, &money, &moneyInBank)) {
                printf("Login successful! Welcome back, %s.\n", email);
                printf("Account balance: %d, Bank balance: %d\n", money, moneyInBank);

                while (1) {
                    printf("\n1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. Logout\n");
                    printf("Enter your choice: ");
                    scanf(" %c", &choice);

                    if (choice == '1') {
                        depositMoney(&money, &moneyInBank);
                        updateAccountDetails(email, password, money, moneyInBank);
                    } else if (choice == '2') {
                        withdrawMoney(&money, &moneyInBank);
                        updateAccountDetails(email, password, money, moneyInBank);
                    } else if (choice == '3') {
                        transferMoney(&moneyInBank);
                        updateAccountDetails(email, password, money, moneyInBank);
                    } else if (choice == '4') {
                        printf("Logged out successfully!\n");
                        break;
                    } else {
                        printf("Invalid choice. Please try again.\n");
                    }
                }
            } else {
                printf("Invalid email or password. Please try again.\n");
            }
        } else {
            printf("Invalid option. Please choose R or L.\n");
        }
    }

    return 0;
}
