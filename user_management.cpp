#include "common.h"

/**
 * Hash a password using SHA-256 algorithm.
 *
 * @param password The password to be hashed.
 * @return A string representation of the hashed password.
 */
std::string hash_password(const std::string &password)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    // Create a new EVP_MD_CTX for the hashing operation
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr)
    {
        std::cerr << "Error creating EVP_MD_CTX" << std::endl;
        return "";
    }

    // Initialize the digest operation
    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1)
    {
        std::cerr << "Error initializing digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Update the digest with the password
    if (EVP_DigestUpdate(mdctx, password.c_str(), password.length()) != 1)
    {
        std::cerr << "Error updating digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Finalize the digest operation
    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1)
    {
        std::cerr << "Error finalizing digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    // Free the EVP_MD_CTX
    EVP_MD_CTX_free(mdctx);

    // Convert the hash to a hexadecimal string
    std::stringstream ss;
    for (unsigned int i = 0; i < hashLen; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

/**
 * Register a new user in the database.
 *
 * @param con Pointer to the MySQL connection.
 * @param username The username of the new user.
 * @param password The password of the new user.
 * @return True if registration is successful, false otherwise.
 */
bool register_user(MYSQL *con, const std::string &username, const std::string &password)
{
    std::string hashed_password = hash_password(password);

    // Prepare SQL statement
    const char *query = "INSERT INTO users (username, password_hash) VALUES (?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        std::cerr << "mysql_stmt_init() failed" << std::endl;
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        std::cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Bind parameters
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)username.c_str();
    bind[0].buffer_length = username.length();

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void *)hashed_password.c_str();
    bind[1].buffer_length = hashed_password.length();

    if (mysql_stmt_bind_param(stmt, bind))
    {
        std::cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt))
    {
        std::cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    mysql_stmt_close(stmt);
    std::cout << "User registered successfully!" << std::endl;
    return true;
}

/**
 * Authenticate a user login.
 *
 * @param con Pointer to the MySQL connection.
 * @param username The username to authenticate.
 * @param password The password to authenticate.
 * @return True if login is successful, false otherwise.
 */
bool login_user(MYSQL *con, const std::string &username, const std::string &password)
{
    std::string hashed_password = hash_password(password);

    // Prepare SQL statement
    const char *query = "SELECT * FROM users WHERE username = ? AND password_hash = ?";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        std::cerr << "mysql_stmt_init() failed" << std::endl;
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        std::cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Bind parameters
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)username.c_str();
    bind[0].buffer_length = username.length();

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void *)hashed_password.c_str();
    bind[1].buffer_length = hashed_password.length();

    if (mysql_stmt_bind_param(stmt, bind))
    {
        std::cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt))
    {
        std::cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Store the result
    if (mysql_stmt_store_result(stmt))
    {
        std::cerr << "mysql_stmt_store_result() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Check if a matching user was found
    bool login_success = (mysql_stmt_num_rows(stmt) == 1);

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);

    if (login_success)
    {
        std::cout << "Login successful!" << std::endl;
    }
    else
    {
        std::cout << "Invalid username or password." << std::endl;
    }

    return login_success;
}

/**
 * Get the user ID for a given username.
 *
 * @param con Pointer to the MySQL connection.
 * @param username The username to look up.
 * @return The user ID if found, -1 otherwise.
 */
int get_user_id(MYSQL *con, const std::string &username)
{
    // Prepare SQL statement
    const char *query = "SELECT id FROM users WHERE username = ?";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        std::cerr << "mysql_stmt_init() failed" << std::endl;
        return -1;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        std::cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    // Bind parameter
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)username.c_str();
    bind[0].buffer_length = username.length();

    if (mysql_stmt_bind_param(stmt, bind))
    {
        std::cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt))
    {
        std::cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    // Prepare result binding
    MYSQL_BIND result_bind[1];
    int user_id = -1;
    bool is_null;
    memset(result_bind, 0, sizeof(result_bind));

    result_bind[0].buffer_type = MYSQL_TYPE_LONG;
    result_bind[0].buffer = (void *)&user_id;
    result_bind[0].is_null = &is_null;

    if (mysql_stmt_bind_result(stmt, result_bind))
    {
        std::cerr << "mysql_stmt_bind_result() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    // Fetch the result
    if (mysql_stmt_fetch(stmt) != 0)
    {
        std::cerr << "mysql_stmt_fetch() failed or no user found: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return -1;
    }

    mysql_stmt_close(stmt);

    return user_id;
}

bool reset_password(MYSQL *con, const std::string &username, const std::string &new_password)
{
    std::string hashed_password = hash_password(new_password);

    // Prepare SQL statement
    const char *query = "UPDATE users SET password_hash = ? WHERE username = ?";
    MYSQL_STMT *stmt = mysql_stmt_init(con);
    if (!stmt)
    {
        std::cerr << "mysql_stmt_init() failed" << std::endl;
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        std::cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Bind parameters
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)hashed_password.c_str();
    bind[0].buffer_length = hashed_password.length();

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (void *)username.c_str();
    bind[1].buffer_length = username.length();

    if (mysql_stmt_bind_param(stmt, bind))
    {
        std::cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt))
    {
        std::cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    mysql_stmt_close(stmt);
    std::cout << "Password reset successful!" << std::endl;
    return true;
}
