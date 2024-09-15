#include "common.h"

int main(int argc, const char *argv[], const char *envp[])
{
    MYSQL *con = connect_to_database();

    int option;
    
    while (true)
    {
        showMainMenu();
        std::cin >> option;
        if (std::cin.fail() || option < 1 || option > 5)
        {
            std::cin.clear();
            std::cout << "Invalid input. Please enter a integer between 1 and 5."
                      << std::endl;
            back_to_menu();
            continue;
        }
        switch (option)
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