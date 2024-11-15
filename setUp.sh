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
mysql -u root -pyour_new_password < init.sql

# Compile the project
make

echo "Setup complete! You can now run the application with ./bookkeeper"
