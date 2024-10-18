#!/bin/bash

# Update and upgrade system packages
sudo apt-get update
sudo apt-get upgrade -y

# Install necessary packages
sudo apt-get install -y g++ libmysqlclient-dev mysql-server libssl-dev

# Start MySQL service
sudo service mysql start

# Set MySQL root password
sudo mysql -u root <<EOF
ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'your_new_password';
FLUSH PRIVILEGES;
EOF

# Create database and tables
mysql -u root -pyour_new_password <<EOF
CREATE DATABASE IF NOT EXISTS bookkeeper;
USE bookkeeper;

CREATE TABLE IF NOT EXISTS users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS transactions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    description VARCHAR(255) NOT NULL,
    amount DOUBLE NOT NULL,
    transaction_type ENUM('income', 'expense') NOT NULL,
    transaction_date DATETIME NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
);
EOF

# Compile the project
g++ -std=c++11 -Wall -I/usr/include/mysql -I/usr/include/openssl \
    bookkeeper.cpp database.cpp menus.cpp utilities.cpp user_management.cpp \
    -o bookkeeper -L/usr/lib/mysql -lmysqlclient -lcrypto -lssl

echo "Setup complete! You can now run the application with ./bookkeeper"
