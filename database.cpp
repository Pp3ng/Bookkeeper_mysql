#include "common.h"

// Function to establish a connection to the MySQL database
MYSQL *connect_to_database()
{
    // Initialize a MySQL object
    MYSQL *con = mysql_init(nullptr);
    if (con == nullptr)
    {
        std::cerr << "mysql_init() failed" << std::endl;
        return nullptr;
    }

    // Attempt to connect to the database
    if (mysql_real_connect(con, "localhost", "root", "your_new_password",
                           "bookkeeper", 0, nullptr, 0) == nullptr)
    {
        finish_with_error(con);
    }

    return con;
}

// Function to handle querying transactions
void handle_query_transactions(MYSQL *con)
{
    showQueryMenu();
    while (true)
    {
        int queryChoice;

        // Get user input for query choice
        std::cin >> queryChoice;
        if (std::cin.fail() || queryChoice < 1 || queryChoice > 7)
        {
            // Handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a integer between 1 and 7." << std::endl;
            continue;
        }
        // Process user's query choice
        switch (queryChoice)
        {
        case 1:
            query_transactions(con);
            back_to_menu();
            return;
        case 2:
            list_incomes(con);
            return;
        case 3:
            list_expenses(con);
            return;
        case 4:
            sort_transactions_by_amount(con);
            back_to_menu();
            return;
        case 5:
            search_transactions_by_date(con);
            back_to_menu();
            return;
        case 6:
            calculate_balance(con);
            back_to_menu();
            return;
        case 7:
            return;
        }
    }
}

// Function to handle inserting a new transaction
void handle_insert_transaction(MYSQL *con)
{
    showAccountingMenu();
    while (true)
    {
        int accountingChoice;
        std::cin >> accountingChoice;
        if (std::cin.fail() || (accountingChoice != 1 && accountingChoice != 2 && accountingChoice != 3))
        {
            // Handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1 for income or 2 for expense." << std::endl;
            continue;
        }
        switch (accountingChoice)
        {
        case 1:
        case 2:
        {
            // Get transaction details from user
            std::string description, transaction_type;
            double amount;
            std::cout << "Enter description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, description);
            if (description.empty())
            {
                std::cout << "Invalid description. It cannot be empty." << std::endl;
                continue;
            }
            std::cout << "Enter amount: ";
            std::cin >> amount;
            if (!is_valid_amount(amount))
            {
                std::cout << "Invalid amount. It must be a positive number." << std::endl;
                continue;
            }
            transaction_type = (accountingChoice == 1) ? "income" : "expense";
            insert_transaction(con, description, amount, transaction_type);
            back_to_menu();
            break;
        }
        case 3:
            return;
        default:
            std::cout << "Invalid choice" << std::endl;
            continue;
        }
        break;
    }
}

// Function to handle deleting a transaction
void handle_delete_transaction(MYSQL *con)
{
    int id;
    query_transactions(con);
    while (true)
    {
        std::cout << "Enter transaction ID to delete: ";
        std::cin >> id;
        if (std::cin.fail() || id <= 0)
        {
            // Handle invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid ID. It must be a positive integer." << std::endl;
            continue;
        }
        if (!is_valid_transaction_id(con, id))
        {
            std::cout << "Error: Transaction ID does not exist. Please enter a valid ID." << std::endl;
            continue;
        }

        break;
    }
    delete_transaction(con, id);
    back_to_menu();
}

// Function to handle updating a transaction
void handle_update_transaction(MYSQL *con)
{
    int id;
    std::string description;
    double amount;

    query_transactions(con);

    // Get transaction ID to update
    while (true)
    {
        std::cout << "Enter transaction ID to update: ";
        std::cin >> id;
        if (std::cin.fail() || id <= 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid ID. It must be a positive integer." << std::endl;
            continue;
        }

        if (!is_valid_transaction_id(con, id))
        {
            std::cout << "Error: Transaction ID does not exist. Please enter a valid ID." << std::endl;
            continue;
        }

        break;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Get new description
    std::cout << "Enter new description: ";
    std::getline(std::cin, description);

    // Get new amount
    while (true)
    {
        std::cout << "Enter new amount: ";
        std::cin >> amount;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid amount. Please enter a valid number." << std::endl;
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!is_valid_amount(amount))
        {
            std::cout << "Invalid amount." << std::endl;
            continue;
        }

        break;
    }

    update_transaction(con, id, description, amount);
    back_to_menu();
}

// Function to sort transactions by amount
void sort_transactions_by_amount(MYSQL *con)
{
    int sortOrder;
    while (true)
    {
        std::cout << "Enter 1 for ascending order or 2 for descending order: ";
        std::cin >> sortOrder;
        if (std::cin.fail() || (sortOrder != 1 && sortOrder != 2))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1 or 2." << std::endl;
            continue;
        }
        break;
    }

    std::string order = (sortOrder == 1) ? "ASC" : "DESC";
    std::string query = "SELECT * FROM transactions WHERE user_id = " + std::to_string(current_user_id) +
                        " ORDER BY amount " + order;

    if (mysql_query(con, query.c_str()))
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

// Function to search transactions by date range
void search_transactions_by_date(MYSQL *con)
{
    std::string startDate, endDate;
    std::cout << "Enter start date (YYYY-MM-DD): ";
    std::cin >> startDate;
    while (!is_valid_date(startDate))
    {
        std::cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << std::endl;
        std::cout << "Enter start date (YYYY-MM-DD): ";
        std::cin >> startDate;
    }

    // Prompt for end date
    std::cout << "Enter end date (YYYY-MM-DD): ";
    std::cin >> endDate;
    while (!is_valid_date(endDate))
    {
        std::cout << "Invalid date format. Please enter a valid date (YYYY-MM-DD)." << std::endl;
        std::cout << "Enter end date (YYYY-MM-DD): ";
        std::cin >> endDate;
    }

    // Construct the query
    std::string query = "SELECT * FROM transactions WHERE user_id = " + std::to_string(current_user_id) +
                        " AND transaction_date BETWEEN '" + startDate + "' AND '" + endDate + "'";

    // Execute the query
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
    }

    // Store the result
    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    // Print the transactions
    print_transactions(result);

    // Free the result
    mysql_free_result(result);
}

// Function to print transactions in a formatted table
void print_transactions(MYSQL_RES *result)
{
    MYSQL_ROW row;
    // Print table header
    std::cout << "+-----+----------------------+-------------+----------+---------------------+" << std::endl;
    std::cout << "| " << std::setw(3) << "id"
              << " | " << std::setw(20) << "description"
              << " | " << std::setw(11) << "amount"
              << " | " << std::setw(8) << "type"
              << " | " << std::setw(19) << "transaction_date"
              << " |" << std::endl;
    std::cout << "+-----+----------------------+-------------+----------+---------------------+" << std::endl;

    // Print table rows
    while ((row = mysql_fetch_row(result)))
    {
        std::cout << "| " << std::setw(3) << (row[0] ? row[0] : "NULL") << " | "
                  << std::setw(20) << (row[2] ? row[2] : "NULL") << " | "
                  << std::setw(11) << (row[3] ? row[3] : "NULL") << " | "
                  << std::setw(8) << (row[4] ? row[4] : "NULL") << " | "
                  << std::setw(19) << (row[5] ? row[5] : "NULL") << " |"
                  << std::endl;
    }

    std::cout << "+-----+----------------------+-------------+----------+---------------------+" << std::endl;
    std::cout << mysql_num_rows(result) << " records found." << std::endl;
}

// Function to query all transactions for the current user
void query_transactions(MYSQL *con)
{
    std::string query = "SELECT * FROM transactions WHERE user_id = " + std::to_string(current_user_id);
    if (mysql_query(con, query.c_str()))
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

// Function to list all income transactions for the current user
void list_incomes(MYSQL *con)
{
    std::string query = "SELECT * FROM transactions WHERE user_id = " + std::to_string(current_user_id) +
                        " AND transaction_type = 'income'";
    if (mysql_query(con, query.c_str()))
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

// Function to list all expense transactions for the current user
void list_expenses(MYSQL *con)
{
    std::string query = "SELECT * FROM transactions WHERE user_id = " + std::to_string(current_user_id) +
                        " AND transaction_type = 'expense'";
    if (mysql_query(con, query.c_str()))
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

// Function to insert a new transaction into the database
void insert_transaction(MYSQL *con, const std::string &description,
                        double amount, const std::string &transaction_type)
{
    std::string date = get_current_date();
    if (transaction_type == "expense")
    {
        amount = -amount;
    }
    std::string query = "INSERT INTO transactions (description, amount, "
                        "transaction_type, transaction_date, user_id) VALUES ('" +
                        description + "', " + std::to_string(amount) + ", '" +
                        transaction_type + "', '" + date + "', " +
                        std::to_string(current_user_id) + ")";
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
    }
    std::cout << "Transaction inserted successfully." << std::endl;
}

// Function to delete a transaction from the database
void delete_transaction(MYSQL *con, int id)
{
    std::string query = "DELETE FROM transactions WHERE id = ? AND user_id = ?";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        finish_with_error(con);
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()))
    {
        finish_with_error(con);
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char *)&id;
    bind[0].buffer_length = sizeof(id);

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = (char *)&current_user_id;
    bind[1].buffer_length = sizeof(current_user_id);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        finish_with_error(con);
    }

    if (mysql_stmt_execute(stmt))
    {
        mysql_stmt_close(stmt);
        finish_with_error(con);
    }

    my_ulonglong affected_rows = mysql_stmt_affected_rows(stmt);
    mysql_stmt_close(stmt);

    if (affected_rows == 0)
    {
        std::cout << "Error: No transaction found with the specified ID or it doesn't belong to you. No deletion performed." << std::endl;
        return;
    }

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

// Function to update an existing transaction in the database
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

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1 for income or 2 for expense." << std::endl;
            continue;
        }

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
            std::cout << "Invalid choice. Please select 1 for income or 2 for expense." << std::endl;
        }
    }

    std::string date = get_current_date();

    if (transaction_type == "expense")
    {
        amount = -amount;
    }

    // Prepare the SQL query for updating the transaction
    std::string query = "UPDATE transactions SET description = ?, amount = ?, "
                        "transaction_type = ?, transaction_date = ? "
                        "WHERE id = ? AND user_id = ?";

    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        finish_with_error(con);
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length()))
    {
        mysql_stmt_close(stmt);
        finish_with_error(con);
    }

    MYSQL_BIND bind[6];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)description.c_str();
    bind[0].buffer_length = description.length();

    bind[1].buffer_type = MYSQL_TYPE_DOUBLE;
    bind[1].buffer = (void *)&amount;

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (void *)transaction_type.c_str();
    bind[2].buffer_length = transaction_type.length();

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = (void *)date.c_str();
    bind[3].buffer_length = date.length();

    bind[4].buffer_type = MYSQL_TYPE_LONG;
    bind[4].buffer = (void *)&id;

    bind[5].buffer_type = MYSQL_TYPE_LONG;
    bind[5].buffer = (void *)&current_user_id;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        mysql_stmt_close(stmt);
        finish_with_error(con);
    }

    if (mysql_stmt_execute(stmt))
    {
        mysql_stmt_close(stmt);
        finish_with_error(con);
    }

    my_ulonglong affected_rows = mysql_stmt_affected_rows(stmt);
    mysql_stmt_close(stmt);

    if (affected_rows == 0)
    {
        std::cout << "Error: No transaction found with the specified ID or it doesn't belong to you. No update performed." << std::endl;
        return;
    }

    std::cout << "Transaction updated successfully." << std::endl;
}

// Function to calculate and display the balance
void calculate_balance(MYSQL *con)
{
    // Query to get total income
    std::string income_query = "SELECT SUM(amount) FROM transactions WHERE user_id = " +
                               std::to_string(current_user_id) + " AND transaction_type = 'income'";
    if (mysql_query(con, income_query.c_str()))
    {
        finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    double totalIncome = row[0] ? std::stod(row[0]) : 0.0;
    mysql_free_result(result);

    // Query to get total expense
    std::string expense_query = "SELECT SUM(amount) FROM transactions WHERE user_id = " +
                                std::to_string(current_user_id) + " AND transaction_type = 'expense'";
    if (mysql_query(con, expense_query.c_str()))
    {
        finish_with_error(con);
    }

    result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
    }

    row = mysql_fetch_row(result);
    double totalExpense = row[0] ? std::stod(row[0]) : 0.0;
    mysql_free_result(result);

    // Calculate net balance
    double Net = totalIncome - (-totalExpense);

    // Display results
    std::cout << "Total Income: " << totalIncome << std::endl;
    std::cout << "Total Expense: " << totalExpense << std::endl;
    std::cout << "Net (Income - Expense): " << Net << std::endl;
}