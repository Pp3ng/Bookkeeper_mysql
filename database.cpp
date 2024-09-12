#include "common.h"

void print_transactions(MYSQL_RES *result)
{
    MYSQL_ROW row;

    // Print table header
    std::cout << "+----+-------------+---------+------------------+--------------"
                 "-------+"
              << std::endl;
    std::cout << "| " << std::setw(2) << "id"
              << " | " << std::setw(11) << "description"
              << " | " << std::setw(7) << "amount"
              << " | " << std::setw(16) << "transaction_type"
              << " | " << std::setw(19) << "transaction_date"
              << " |" << std::endl;
    std::cout << "+----+-------------+---------+------------------+--------------"
                 "-------+"
              << std::endl;

    // Print table rows
    while ((row = mysql_fetch_row(result)))
    {
        std::cout << "| " << std::setw(2) << (row[0] ? row[0] : "NULL") << " | "
                  << std::setw(11) << (row[1] ? row[1] : "NULL") << " | "
                  << std::setw(7) << (row[2] ? row[2] : "NULL") << " | "
                  << std::setw(16) << (row[3] ? row[3] : "NULL") << " | "
                  << std::setw(19) << (row[4] ? row[4] : "NULL") << " |"
                  << std::endl;
    }

    std::cout << "+----+-------------+---------+------------------+--------------"
                 "-------+"
              << std::endl;
}

void query_transactions(MYSQL *con)
{
    if (mysql_query(con, "SELECT * FROM transactions"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    print_transactions(result);

    mysql_free_result(result);
}

void list_incomes(MYSQL *con)
{
    if (mysql_query(
            con,
            "SELECT * FROM transactions WHERE transaction_type = 'income'"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    print_transactions(result);

    mysql_free_result(result);

    back_to_menu();
}

void list_expenses(MYSQL *con)
{
    if (mysql_query(
            con,
            "SELECT * FROM transactions WHERE transaction_type = 'expense'"))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    print_transactions(result);

    mysql_free_result(result);

    back_to_menu();
}

void insert_transaction(MYSQL *con, const std::string &description,
                        double amount, const std::string &transaction_type)
{
    std::string date = get_current_date();
    std::string query = "INSERT INTO transactions (description, amount, "
                        "transaction_type, transaction_date) VALUES ('" +
                        description + "', " + std::to_string(amount) + ", '" +
                        transaction_type + "', '" + date + "')";
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
    }
    std::cout << "Transaction inserted successfully." << std::endl;
}

void delete_transaction(MYSQL *con, int id)
{
    std::string query = "DELETE FROM transactions WHERE id = ?";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        finish_with_error(con);
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()))
    {
        finish_with_error(con);
    }

    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&id;
    bind[0].buffer_length = sizeof(id);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        finish_with_error(con);
    }

    if (mysql_stmt_execute(stmt))
    {
        finish_with_error(con);
    }

    mysql_stmt_close(stmt);

    // Start transaction
    if (mysql_query(con, "START TRANSACTION;"))
    {
        finish_with_error(con);
    }

    // Update IDs
    if (mysql_query(con, "SET @count = 0;"))
    {
        finish_with_error(con);
    }

    if (mysql_query(con, "UPDATE transactions SET id = @count:= @count + 1;"))
    {
        finish_with_error(con);
    }

    if (mysql_query(con, "ALTER TABLE transactions AUTO_INCREMENT = 1;"))
    {
        finish_with_error(con);
    }

    // Commit transaction
    if (mysql_query(con, "COMMIT;"))
    {
        finish_with_error(con);
    }

    std::cout << "Transaction deleted and IDs updated successfully." << std::endl;
}

void update_transaction(MYSQL *con, int id, const std::string &description,
                        double amount)
{
    int transaction_type_choice;
    std::string transaction_type;

    // Loop until user provides a valid choice
    while (true)
    {
        std::cout << "Select new transaction type: " << std::endl;
        std::cout << "1. Income" << std::endl;
        std::cout << "2. Expense" << std::endl;
        std::cout << "Enter your choice (1 or 2): ";
        std::cin >> transaction_type_choice;

        if (transaction_type_choice == 1)
        {
            transaction_type = "income";
            break;
        }
        else if (transaction_type_choice == 2)
        {
            transaction_type = "expense";
            break;
        }
        else
        {
            std::cerr
                << "Invalid choice. Please select 1 for income or 2 for expense."
                << std::endl;
        }
    }

    // Get the current date for updating the transaction_date
    std::string date = get_current_date();

    // Construct the SQL query for updating the transaction
    std::string query = "UPDATE transactions SET description = '" + description +
                        "', amount = " + std::to_string(amount) +
                        ", transaction_type = '" + transaction_type +
                        "', transaction_date = '" + date +
                        "' WHERE id = " + std::to_string(id);

    // Execute the query and handle any errors
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
    }

    std::cout << "Transaction updated successfully." << std::endl;
    back_to_menu();
}