#include "common.h"

int main(int argc, const char *argv[], const char *envp[])
{
    MYSQL *con = mysql_init(nullptr);
    if (con == nullptr)
    {
        std::cerr << "mysql_init() failed" << std::endl;
    }

    if (mysql_real_connect(con, "localhost", "root", "your_new_password",
                           "bookkeeper", 0, nullptr, 0) == nullptr)
    {
        finish_with_error(con);
    }

    int choice;
    while (true)
    {
        showMainMenu();
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 5)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter an integer between 1 and 5." << std::endl;
            back_to_menu();
            continue;
        }
        switch (choice)
        {
        case 1:
            showQueryMenu();
            int queryChoice;
            std::cin >> queryChoice;
            if (std::cin.fail() || queryChoice < 1 || queryChoice > 4)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid choice" << std::endl;
                back_to_menu();
                continue;
            }
            switch (queryChoice)
            {
            case 1:
                query_transactions(con);
                back_to_menu();
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
                back_to_menu();
                break;
            }
            break;
        case 2:
            while (true)
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
                    if (std::cin.fail() || !is_valid_amount(amount))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cerr << "Invalid amount. It must be a positive number." << std::endl;
                        continue;
                    }
                    transaction_type = (accountingChoice == 1) ? "income" : "expense";
                    insert_transaction(con, description, amount, transaction_type);
                    back_to_menu();
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
            query_transactions(con);
            while (true)
            {
                std::cout << "Enter transaction ID to delete: ";
                std::cin >> id;
                if (std::cin.fail() || id <= 0)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid ID. It must be a positive integer." << std::endl;
                    continue;
                }

                if (!is_valid_transaction_id(con, id))
                {
                    std::cerr << "Error: Transaction ID does not exist. Please enter a valid ID." << std::endl;
                    continue;
                }

                break;
            }
            delete_transaction(con, id);
            back_to_menu();
            break;
        }
        case 4:
        {
            int id;
            std::string description;
            double amount;

            query_transactions(con);

            while (true)
            {
                std::cout << "Enter transaction ID to update: ";
                std::cin >> id;
                if (std::cin.fail() || id <= 0)
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Invalid ID. It must be a positive integer." << std::endl;
                    continue;
                }

                if (!is_valid_transaction_id(con, id))
                {
                    std::cerr << "Error: Transaction ID does not exist. Please enter a valid ID." << std::endl;
                    continue;
                }

                break;
            }

            std::cout << "Enter new description: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, description);
            std::cout << "Enter new amount: ";
            std::cin >> amount;
            if (std::cin.fail() || !is_valid_amount(amount))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid amount. It must be a positive number." << std::endl;
                break;
            }

            update_transaction(con, id, description, amount);
            back_to_menu();
            break;
        }

        case 5:
            std::cout << "Bye" << std::endl;
            mysql_close(con);
            return EXIT_SUCCESS;
        default:
            break;
        }
    }

    mysql_close(con);
    return EXIT_SUCCESS;
}