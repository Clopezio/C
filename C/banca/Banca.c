#include <stdio.h>
#include <string.h>

char choice;
char email[50];
char password[50];
char email1[50];
char password1[50];
int money = 100;
int moneyinbank = 500;

void saveAccount(const char *email, const char *password, int money, int moneyinbank) {
    FILE *file = fopen("accounts.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s %s %d %d\n", email, password, money, moneyinbank);
    fclose(file);
}

int loadAccount(const char *email, const char *password) {
    FILE *file = fopen("accounts.txt", "r");
    if (file == NULL) {
        printf("No accounts found. Please register a new account.\n");
        return 0;
    }
    while (fscanf(file, "%s %s %d %d", email1, password1, &money, &moneyinbank) != EOF) {
        if (strcmp(email, email1) == 0 && strcmp(password, password1) == 0) {
            fclose(file);
            return 1; // Account found
        }
    }
    fclose(file);
    return 0; // Account not found
}

void updateAccounts() {
    FILE *file = fopen("accounts.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    char tempEmail[50], tempPassword[50];
    int tempMoney, tempMoneyInBank;

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%s %s %d %d", tempEmail, tempPassword, &tempMoney, &tempMoneyInBank) != EOF) {
        if (strcmp(tempEmail, email1) == 0) {
            fprintf(tempFile, "%s %s %d %d\n", email1, password1, money, moneyinbank);
        } else {
            fprintf(tempFile, "%s %s %d %d\n", tempEmail, tempPassword, tempMoney, tempMoneyInBank);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");
}

int main() {
    while (1) {
        printf("Welcome to the bank\n");
        printf("this code was made by ClopeziohH\n");
        printf("Would you like to open an account or login? (R/L): ");
        scanf(" %c", &choice);

        if (choice == 'R' || choice == 'r') {
            printf("Enter email: \n");
            scanf("%s", email1);
            printf("Enter password: \n");
            scanf("%s", password1);
            saveAccount(email1, password1, money, moneyinbank);
            printf("You have successfully opened an account\n");
        } else if (choice == 'L' || choice == 'l') {
            printf("Enter email: \n");
            scanf("%s", email);
            printf("Enter password: \n");
            scanf("%s", password);
            if (loadAccount(email, password)) {
                printf("You have successfully logged in\n");
                while (1) {
                    printf("Welcome back %s\n", email);
                    printf("You have %d money in your account\n", money);
                    printf("You have %d money in your bank\n", moneyinbank);
                    printf("What would you like to do?\n");
                    printf("1. Deposit money\n");
                    printf("2. Withdraw money\n");
                    printf("3. Logout\n");
                    printf("4. Money transfer\n");
                    scanf(" %c", &choice);
                    if (choice == '1') {
                        int deposit;
                        printf("How much money would you like to deposit?\n");
                        scanf("%d", &deposit);
                        if (deposit > money) {
                            printf("You cannot deposit more than you have\n");
                        } else {
                            money -= deposit;
                            moneyinbank += deposit;
                            printf("You have successfully deposited %d money\n", deposit);
                            printf("You have %d money in your account\n", money);
                            printf("You have %d money in your bank\n", moneyinbank);
                            updateAccounts();
                        }
                    } else if (choice == '2') {
                        int withdraw;
                        printf("How much money would you like to withdraw?\n");
                        scanf("%d", &withdraw);
                        if (withdraw > moneyinbank) {
                            printf("You cannot withdraw more than you have in the bank\n");
                        } else {
                            money += withdraw;
                            moneyinbank -= withdraw;
                            printf("You have successfully withdrawn %d money\n", withdraw);
                            printf("You have %d money on you\n", money);
                            printf("You have %d money in your bank\n", moneyinbank);
                            updateAccounts();
                        }
                    } else if (choice == '3') {
                        printf("You have successfully logged out\n");
                        break;
                    } else if (choice == '4') {
                        char transferEmail[50];
                        int transferAmount;
                        printf("Enter the email of the user you want to transfer to: \n");
                        scanf("%s", transferEmail);
                        printf("Enter the amount to transfer: \n");
                        scanf("%d", &transferAmount);
                        if (transferAmount > moneyinbank) {
                            printf("You cannot transfer more than you have in the bank\n");
                        } else {
                            FILE *file = fopen("accounts.txt", "r");
                            FILE *tempFile = fopen("temp.txt", "w");
                            char tempEmail[50], tempPassword[50];
                            int tempMoney, tempMoneyInBank;
                            int found = 0;

                            if (file == NULL || tempFile == NULL) {
                                printf("Error opening file!\n");
                                return 1;
                            }

                            while (fscanf(file, "%s %s %d %d", tempEmail, tempPassword, &tempMoney, &tempMoneyInBank) != EOF) {
                                if (strcmp(tempEmail, email1) == 0) {
                                    fprintf(tempFile, "%s %s %d %d\n", email1, password1, money, moneyinbank - transferAmount);
                                } else if (strcmp(tempEmail, transferEmail) == 0) {
                                    fprintf(tempFile, "%s %s %d %d\n", tempEmail, tempPassword, tempMoney, tempMoneyInBank + transferAmount);
                                    found = 1;
                                } else {
                                    fprintf(tempFile, "%s %s %d %d\n", tempEmail, tempPassword, tempMoney, tempMoneyInBank);
                                }
                            }

                            fclose(file);
                            fclose(tempFile);
                            remove("accounts.txt");
                            rename("temp.txt", "accounts.txt");

                            if (found) {
                                moneyinbank -= transferAmount;
                                printf("You have successfully transferred %d money to %s\n", transferAmount, transferEmail);
                                printf("You have %d money in your bank\n", moneyinbank);
                            } else {
                                printf("Account with email %s not found\n", transferEmail);
                            }
                        }
                    } else {
                        printf("Invalid choice\n");
                    }
                }
            } else {
                printf("Invalid email or password\n");
            }
        } else {
            printf("Invalid choice\n");
        }
    }
    return 0;
}