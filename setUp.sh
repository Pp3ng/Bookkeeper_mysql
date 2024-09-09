#!/bin/bash

# 更新并安装必要的依赖
sudo apt-get update
sudo apt-get upgrade -y

# 安装 g++ 编译器和 MySQL 服务器与客户端库
sudo apt-get install -y g++ libmysqlclient-dev mysql-server

# 启动 MySQL 服务
sudo service mysql start

# 设置 MySQL 根用户密码 (请使用与代码中的密码一致的密码: your_new_password)
sudo mysql -u root <<EOF
ALTER USER 'root'@'localhost' IDENTIFIED WITH 'mysql_native_password' BY 'your_new_password';
FLUSH PRIVILEGES;
EOF

# 创建数据库和表
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

# 编译 C++ 代码
g++ -o bookkeeping_app bookkeeper.cpp -lmysqlclient

# 提示用户
echo "Setup complete! You can now run the application with ./bookkeeping_app"

