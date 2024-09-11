#include <iostream>
#include <mysql/mysql.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <regex>
#include <limits>

void showMainMenu()
{
    printf(" \n");
    printf("  --------------\n");
    printf(" <  Hello! >\n");
    printf("  --------------\n");
    printf("         \\   ^__^\n");
    printf("          \\  (oo)\\_______\n");
    printf("             (__)\\       )\\/\\\n");
    printf("                 ||----w |\n");
    printf("                 ||     ||\n");
    std::cout << "----------------------------------" << std::endl;
    std::cout << "|============Welcome!============|" << std::endl;
    std::cout << "          1.Query Transactions    " << std::endl;
    std::cout << "          2.Insert Transaction    " << std::endl;
    std::cout << "          3.Delete Transaction    " << std::endl;
    std::cout << "          4.Update Transaction    " << std::endl;
    std::cout << "          5.Exit                  " << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Choose an option" << std::endl;
}

void showAccountingMenu()
{
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "|========Please choose bookkeeping type=========|" << std::endl;
    std::cout << "          1.Income                               " << std::endl;
    std::cout << "          2.Expense                              " << std::endl;
    std::cout << "          3.Back to Menu                         " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Please choose:(1~3)" << std::endl;
}

void showQueryMenu()
{
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "|===========Please choose Query type============|" << std::endl;
    std::cout << "          1.Counting of all accounts             " << std::endl;
    std::cout << "          2.Counting incomes                     " << std::endl;
    std::cout << "          3.Counting expenses                    " << std::endl;
    std::cout << "          4.Back to Menu                         " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Please choose:(1~4)" << std::endl;
}

void finish_with_error(MYSQL *con)
{
    std::cerr << "Error: " << mysql_error(con) << std::endl;
    mysql_close(con);
    exit(1);
}

std::string get_current_date()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);

    std::tm tm = *std::localtime(&now_time_t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS
    return oss.str();
}

bool is_valid_amount(double amount)
{
    if (amount < std::numeric_limits<double>::lowest() || amount > std::numeric_limits<double>::max())
    {
        return false;
    }
    return true;
}

bool is_valid_date(const std::string &date)
{
    std::regex date_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})");
    return std::regex_match(date, date_regex);
}

void print_transactions(MYSQL_RES *result)
{
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;

    // Print table header
    std::cout << "+----+-------------+---------+------------------+---------------------+" << std::endl;
    std::cout << "| " << std::setw(2) << "id" << " | "
              << std::setw(11) << "description" << " | "
              << std::setw(7) << "amount" << " | "
              << std::setw(16) << "transaction_type" << " | "
              << std::setw(19) << "transaction_date" << " |" << std::endl;
    std::cout << "+----+-------------+---------+------------------+---------------------+" << std::endl;

    // Print table rows
    while ((row = mysql_fetch_row(result)))
    {
        std::cout << "| " << std::setw(2) << (row[0] ? row[0] : "NULL") << " | "
                  << std::setw(11) << (row[1] ? row[1] : "NULL") << " | "
                  << std::setw(7) << (row[2] ? row[2] : "NULL") << " | "
                  << std::setw(16) << (row[3] ? row[3] : "NULL") << " | "
                  << std::setw(19) << (row[4] ? row[4] : "NULL") << " |" << std::endl;
    }

    std::cout << "+----+-------------+---------+------------------+---------------------+" << std::endl;
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
    if (mysql_query(con, "SELECT * FROM transactions WHERE transaction_type = 'income'"))
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

void list_expenses(MYSQL *con)
{
    if (mysql_query(con, "SELECT * FROM transactions WHERE transaction_type = 'expense'"))
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

void insert_transaction(MYSQL *con, const std::string &description, double amount, const std::string &transaction_type)
{
    std::string date = get_current_date();
    std::string query = "INSERT INTO transactions (description, amount, transaction_type, transaction_date) VALUES ('" + description + "', " + std::to_string(amount) + ", '" + transaction_type + "', '" + date + "')";
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

void update_transaction(MYSQL *con, int id, const std::string &description, double amount, const std::string &transaction_type)
{
    std::string date = get_current_date();
    std::string query = "UPDATE transactions SET description = '" + description + "', amount = " + std::to_string(amount) + ", transaction_type = '" + transaction_type + "', transaction_date = '" + date + "' WHERE id = " + std::to_string(id);
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
    }
    std::cout << "Transaction updated successfully." << std::endl;
}

int main()
{
    MYSQL *con = mysql_init(nullptr);
    if (con == nullptr)
    {
        std::cerr << "mysql_init() failed" << std::endl;
        return 1;
    }

    if (mysql_real_connect(con, "localhost", "root", "your_new_password", "bookkeeper", 0, nullptr, 0) == nullptr)
    {
        finish_with_error(con);
    }

    int choice;
    while (true)
    {
        showMainMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            showQueryMenu();
            int queryChoice;
            std::cin >> queryChoice;
            switch (queryChoice)
            {
            case 1:
                query_transactions(con);
                break;
            case 2:
                list_incomes(con);
                break;
            case 3:
                list_expenses(con);
                break;
            case 4:
                continue;
            default:
                std::cerr << "Invalid choice" << std::endl;
                break;
            }
            break;
        case 2:
            while (true) // 添加循环以处理无效输入
            {
                showAccountingMenu();
                int accountingChoice;
                std::cin >> accountingChoice;
                if (std::cin.fail())
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid input. Please enter a number." << std::endl;
                    continue;
                }
                switch (accountingChoice)
                {
                case 1:
                case 2:
                {
                    std::string description, transaction_type;
                    double amount;
                    std::cout << "Enter description: ";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::getline(std::cin, description);
                    if (description.empty())
                    {
                        std::cerr << "Invalid description. It cannot be empty." << std::endl;
                        continue;
                    }
                    std::cout << "Enter amount: ";
                    std::cin >> amount;
                    if (!is_valid_amount(amount))
                    {
                        std::cerr << "Invalid amount. It must be a positive number." << std::endl;
                        continue;
                    }
                    transaction_type = (accountingChoice == 1) ? "income" : "expense";
                    insert_transaction(con, description, amount, transaction_type);
                    break;
                }
                case 3:
                    break;
                default:
                    std::cerr << "Invalid choice" << std::endl;
                    continue;
                }
                break;
            }
            break;
        case 3:
        {
            int id;
            std::cout << "Enter transaction ID to delete: ";
            std::cin >> id;
            if (id <= 0)
            {
                std::cerr << "Invalid ID. It must be a positive integer." << std::endl;
                break;
            }
            delete_transaction(con, id);
            break;
        }
        case 4:
        {
            int id;
            std::string description, transaction_type;
            double amount;
            std::cout << "Enter transaction ID to update: ";
            std::cin >> id;
            if (id <= 0)
            {
                std::cerr << "Invalid ID. It must be a positive integer." << std::endl;
                break;
            }
            std::cout << "Enter new description: ";
            std::cin.ignore(); // Ignore newline character from previous input
            std::getline(std::cin, description);
            std::cout << "Enter new amount: ";
            std::cin >> amount;
            if (!is_valid_amount(amount))
            {
                std::cerr << "Invalid amount. It must be a positive number." << std::endl;
                break;
            }
            std::cout << "Enter new transaction type: ";
            std::cin.ignore(); // Ignore newline character from previous input
            std::getline(std::cin, transaction_type);
            update_transaction(con, id, description, amount, transaction_type);
            break;
        }
        case 5:
            mysql_close(con);
            return 0;
        default:
            std::cerr << "Invalid choice" << std::endl;
            break;
        }
    }

    mysql_close(con);
    return 0;
}