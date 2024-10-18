# Bookkeeper

Bookkeeper is a simple financial management system that allows users to add, remove, and update transaction records. This project uses a MySQL database to store transaction records and user information, and provides a command-line interface for user interaction.

It's my software engineering course project.

## Features

- User registration and login
- Query transaction records
- Insert new transactions
- Delete transactions
- Update transactions
- Calculate balance
- Sort transactions by amount
- Search transactions by date

## Dependencies

- MySQL
- C++ compiler (supporting C++11 or later)
- OpenSSL library

## Installation

1. Clone this repository:

   ```sh
   git clone https://github.com/Pp3ng/Bookkeeper_mysql.git
   cd Bookkeeper_mysql
   ```

2. Run the setup script to install dependencies and set up the database:
   ```sh
   chmod +x setup.sh
   ./setup.sh
   ```

## Usage

1. Start the MySQL server if it is not already running:

   ```sh
   sudo service mysql start
   ```

2. Run the application:

   ```sh
   ./bookkeeper
   ```

3. Follow the on-screen prompts to register a new user or log in.
4. Once logged in, use the main menu to interact with the system.

## File Structure

- `bookkeeper.cpp`: Main program file, contains the main function and main menu logic.
- `database.cpp`: Contains functions for database interactions.
- `menus.cpp`: Contains functions for displaying menus.
- `utilities.cpp`: Contains utility functions.
- `user_management.cpp`: Contains functions for user registration, login, and password hashing.
- `common.h`: Contains declarations for all functions and necessary headers.

## Security

- User passwords are hashed using SHA-256 before being stored in the database.
- Prepared statements are used to prevent SQL injection attacks.
- Each user can only access their own transaction records.
