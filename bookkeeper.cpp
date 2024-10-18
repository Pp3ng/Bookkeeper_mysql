#include "common.h"

// Global variable to store the current user's ID
int current_user_id = -1;

int main(int argc, const char *argv[], const char *envp[])
{
    // Connect to the database
    MYSQL *con = connect_to_database();

    int option;
    bool logged_in = false;
    std::string username;
    std::string password;

    while (true)
    {
        if (!logged_in)
        {
            // Display login/register menu
            std::cout << "1. Login" << std::endl;
            std::cout << "2. Register" << std::endl;
            std::cout << "3. Exit" << std::endl;
            std::cout << "Choose an option: ";

            // Input validation for menu choice
            while (!(std::cin >> option) || option < 1 || option > 3)
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter 1, 2, or 3: ";
            }

            switch (option)
            {
            case 1:
                // Handle login
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;
                logged_in = login_user(con, username, password);
                if (logged_in)
                {
                    current_user_id = get_user_id(con, username);
                }
                break;
            case 2:
                // Handle registration
                std::cout << "Enter new username: ";
                std::cin >> username;
                std::cout << "Enter new password: ";
                std::cin >> password;
                register_user(con, username, password);
                break;
            case 3:
                // Exit the program
                std::cout << "Goodbye!" << std::endl;
                mysql_close(con);
                return EXIT_SUCCESS;
            }
        }
        else
        {
            // Display main menu for logged-in users
            showMainMenu();
            std::cin >> option;
            if (std::cin.fail() || option < 1 || option > 6)
            {
                std::cin.clear();
                std::cout << "Invalid input. Please enter an integer between 1 and 6." << std::endl;
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
                // Log out
                logged_in = false;
                current_user_id = -1;
                std::cout << "Logged out successfully." << std::endl;
                break;
            case 6:
                // Exit the program
                std::cout << "Goodbye!" << std::endl;
                mysql_close(con);
                return EXIT_SUCCESS;
            }
        }
    }

    // Close the database connection and exit
    mysql_close(con);
    return EXIT_SUCCESS;
}
