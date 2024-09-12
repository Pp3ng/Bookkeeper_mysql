#!/bin/bash

sudo apt-get update
sudo apt-get upgrade -y

sudo apt-get install -y g++ libmysqlclient-dev mysql-server

sudo service mysql start

sudo mysql -u root <<EOF
ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'your_new_password';
FLUSH PRIVILEGES;
EOF

//create database and table
mysql -u root -pyour_new_password <<EOF
CREATE DATABASE IF NOT EXISTS bookkeeper;
USE bookkeeper;
CREATE TABLE IF NOT EXISTS transactions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    description VARCHAR(255) NOT NULL,
    amount DOUBLE NOT NULL,
    transaction_type ENUM('income', 'expense') NOT NULL,
    transaction_date DATETIME NOT NULL
);
EOF

//compile 
make

echo "Setup complete! You can now run the application with ./bookkeeper"

