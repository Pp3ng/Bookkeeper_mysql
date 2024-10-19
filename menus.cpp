#include "common.h"

void showMainMenu()
{
    system("clear");
    printf(" \n");
    printf("  -----------------------------------\n");
    printf(" <  Ready to manage your finances? >\n");
    printf("  -----------------------------------\n");
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
    std::cout << "          5.Set Budget            " << std::endl;
    std::cout << "          6.Check Budget          " << std::endl;
    std::cout << "          7.Export Transactions   " << std::endl;
    std::cout << "          8.Reset Password        " << std::endl;
    std::cout << "          9.Logout                " << std::endl;
    std::cout << "          10.Exit                 " << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Choose an option(1~10)" << std::endl;
}

void showAccountingMenu()
{
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "|========Please choose bookkeeping type=========|" << std::endl;
    std::cout << "           1.Income                               " << std::endl;
    std::cout << "           2.Expense                              " << std::endl;
    std::cout << "           3.Back to Menu                         " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Please choose:(1~3)" << std::endl;
}

void showQueryMenu()
{
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "|===========Please choose Query type============|" << std::endl;
    std::cout << "           1.Counting of all accounts             " << std::endl;
    std::cout << "           2.Counting incomes                     " << std::endl;
    std::cout << "           3.Counting expenses                    " << std::endl;
    std::cout << "           4.Sorting by amount                    " << std::endl;
    std::cout << "           5.Search by date                       " << std::endl;
    std::cout << "           6.Calculate balance                      " << std::endl;
    std::cout << "           7.Back to Menu                         " << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;
    std::cout << "Please choose:(1~6)" << std::endl;
}

void back_to_menu()
{
    std::cout << "Press Enter to return to main menu....." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
