#ifndef COMMON_H
#define COMMON_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mysql/mysql.h>
#include <regex>
#include <sstream>

// Menu functions
void showMainMenu();
void showAccountingMenu();
void showQueryMenu();
void back_to_menu();

// Utility functions
void finish_with_error(MYSQL *con);
std::string get_current_date();
bool is_valid_transaction_id(MYSQL *con, int id);
bool is_valid_amount(double amount);
bool is_valid_date(const std::string &date);

// Handle functions
void handle_query_transactions(MYSQL *con);
void handle_insert_transaction(MYSQL *con);
void handle_delete_transaction(MYSQL *con);
void handle_update_transaction(MYSQL *con);

// Database functions
void print_transactions(MYSQL_RES *result);
void query_transactions(MYSQL *con);
void list_incomes(MYSQL *con);
void list_expenses(MYSQL *con);
void insert_transaction(MYSQL *con, const std::string &description, double amount, const std::string &transaction_type);
void delete_transaction(MYSQL *con, int id);
void update_transaction(MYSQL *con, int id, const std::string &description, double amount);
void sort_transactions_by_amount(MYSQL *con);
void search_transactions_by_date(MYSQL *con);
void calculate_balance(MYSQL *con);
#endif // COMMON_H