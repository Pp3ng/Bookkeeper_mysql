#include "common.h"

int main(int argc, const char *argv[], const char *envp[])
{
    MYSQL *con = mysql_init(nullptr);
    if (con == nullptr)
    {
        std::cerr << "mysql_init() failed" << std::endl;
        return EXIT_FAILURE;
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
            std::cout << "Invalid input. Please enter a integer between 1 and 5."
                      << std::endl;
            back_to_menu();
            continue;
        }
        switch (choice)
        {
        case 1:
            handle_query_transactions(con);
            break;
        case 2:
            handle_insert_transaction(con);
            break;
        case 3:
            handle_delete_transaction(con);
            break;
        case 4:
            handle_update_transaction(con);
            break;
        case 5:
            std::cout << "Bye" << std::endl;
            mysql_close(con);
            return EXIT_SUCCESS;
        }
    }

    mysql_close(con);
    return EXIT_SUCCESS;
}