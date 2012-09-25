#ifndef __CZBDB__HPP__
#define __CZBDB__HPP__

// std includes
#include <string>
#include "common.h"

// mysql includes
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"
#include <string>
#include <fstream>
#include <iostream>
#include "common.h"

class CZBDataBase{
public:    
    CZBDataBase();
    ~CZBDataBase();
    bool connect(void);
    bool disconnect(void);
    void log_data(string_ptr _variable_name, string_ptr _value);
    void log_info(string_ptr _event, string_ptr _msg);
    //set
    void set_hostname(string_ptr _hostname);
    void set_username(string_ptr _username);
    void set_password(string_ptr _password);
    void set_database(string_ptr _database);
    void set_hostname(std::string _hostname);
    void set_username(std::string _username);
    void set_password(std::string _password);
    void set_database(std::string _database);
    // get
    std::string get_hostname() { return hostname; } 
    std::string get_username() { return username; }
    std::string get_password() { return password; }
    std::string get_database() { return database; }
    // callbacks
    
private:
    bool isConnected;
    std::string hostname;
    std::string username;
    std::string password; //TODO: encrypt this
    std::string database;
    sql::Driver *sql_driver;
    sql::Connection *sql_connection;
    sql::Statement *sql_statement;
    sql::ResultSet *sql_resultset;
    sql::PreparedStatement *sql_preparedstatement;
    sql::Savepoint *sql_savepoint;
    std::string log_file_name;
    std::fstream log_file;

    void local_log(char *);
};  

extern CZBDataBase *DB;

#endif
