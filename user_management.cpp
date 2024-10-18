#include "common.h"

std::string hash_password(const std::string &password) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hashLen;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == nullptr) {
        std::cerr << "Error creating EVP_MD_CTX" << std::endl;
        return "";
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
        std::cerr << "Error initializing digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    if (EVP_DigestUpdate(mdctx, password.c_str(), password.length()) != 1) {
        std::cerr << "Error updating digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    if (EVP_DigestFinal_ex(mdctx, hash, &hashLen) != 1) {
        std::cerr << "Error finalizing digest" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hashLen; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

bool register_user(MYSQL *con, const std::string &username, const std::string &password) {
    std::string hashed_password = hash_password(password);
    
    std::string query = "INSERT INTO users (username, password_hash) VALUES ('" + 
                        username + "', '" + hashed_password + "')";

    if (mysql_query(con, query.c_str())) {
        std::cerr << "Registration failed: " << mysql_error(con) << std::endl;
        return false;
    }
    
    std::cout << "User registered successfully!" << std::endl;
    return true;
}

bool login_user(MYSQL *con, const std::string &username, const std::string &password) {
    std::string hashed_password = hash_password(password);
    
    std::string query = "SELECT * FROM users WHERE username = '" + username + 
                        "' AND password_hash = '" + hashed_password + "'";

    if (mysql_query(con, query.c_str())) {
        std::cerr << "Login query failed: " << mysql_error(con) << std::endl;
        return false;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
        std::cerr << "Failed to get result: " << mysql_error(con) << std::endl;
        return false;
    }

    bool login_success = (mysql_num_rows(result) == 1);
    mysql_free_result(result);

    if (login_success) {
        std::cout << "Login successful!" << std::endl;
    } else {
        std::cout << "Invalid username or password." << std::endl;
    }

    return login_success;
}

int get_user_id(MYSQL *con, const std::string &username) {
    std::string query = "SELECT id FROM users WHERE username = '" + username + "'";
    
    if (mysql_query(con, query.c_str())) {
        std::cerr << "Query failed: " << mysql_error(con) << std::endl;
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == nullptr) {
        std::cerr << "Failed to get result: " << mysql_error(con) << std::endl;
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int user_id = row ? std::stoi(row[0]) : -1;
    mysql_free_result(result);

    return user_id;
}
