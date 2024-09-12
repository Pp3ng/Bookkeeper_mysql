#include "common.h"

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

bool is_valid_transaction_id(MYSQL *con, int id)
{
    std::string query =
        "SELECT COUNT(*) FROM transactions WHERE id = " + std::to_string(id);
    if (mysql_query(con, query.c_str()))
    {
        finish_with_error(con);
        return false;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr)
    {
        finish_with_error(con);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    bool exists = (std::stoi(row[0]) > 0);
    mysql_free_result(result);

    return exists;
}

bool is_valid_amount(double amount)
{
    if (amount < std::numeric_limits<double>::lowest() ||
        amount > std::numeric_limits<double>::max())
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