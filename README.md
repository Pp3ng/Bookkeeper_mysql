# Bookkeeper

Bookkeeper is a simple tool to keep track of your books. It is a command line tool that allows you to add, remove, and list books. It also allows you to mark books as read or unread.

Use mysql as the database to store the data.

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
   git clone https://github.com/ZH0UU/Bookkeeper_mysql.git
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

## Contribution

Contributions are welcome! Please fork this repository and submit a pull request.

## License

This project is licensed under the MIT License.

## Author

- [ZH0UU](https://github.com/ZH0UU)

If you have any questions or suggestions, feel free to contact me.
