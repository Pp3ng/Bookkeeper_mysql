# Bookkeeper

Bookkeeper is a simple fiancial management system that allows users to add, remove, and update transaction records. This project uses a MYSQL database to store transaction records and C++ API to interract with database,provides a command-line interface for user interaction.

It's my software engineering course project.

## Features

- Query transaction records
- Insert new transactions
- Delete transactions
- Update transactions
- Calculate balance

## Dependencies

- MySQL
- C++ compiler

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

3. Follow the on-screen prompts to interact with the system.

## File Structure

- `bookkeeper.cpp`: Main program file, contains the main function and main menu logic.
- `database.cpp`: Contains functions for database interactions.
- `menus.cpp`: Contains functions for displaying menus.
- `utilities.cpp`: Contains utility functions.
- `common.h`: Contains declarations for all functions and necessary headers.