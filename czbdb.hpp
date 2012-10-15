#ifndef __CZBDB__HPP__
#define __CZBDB__HPP__

// std includes
#include <string>
#include <fstream>
#include <iostream>
#include "common.h"

/* MySQL Connector/C++ specific headers */
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>
#include <cppconn/warning.h>

// mysql includes
#include "mysql_driver.h"
#include "mysql_connection.h"
#include "cppconn/resultset.h"

class CZBDataBase
{
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

public:    
    CZBDataBase();
    ~CZBDataBase();
    bool connect(void);
    bool disconnect(void);
    bool is_connected(void);
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

    sql::ResultSet *getSQLQuery(std::string _str){
	return sql_statement->executeQuery(_str);
    }
    
    /*std::list<string_ptr> get_end_devices_address(void)
    {
	std::list<string_ptr> ret;
	sql::ResultSet *rs = NULL;
	try
	{
	    rs = sql_statement->executeQuery("SELECT address FROM end_devices");
	    while(rs->next()){
		std::cout << rs->getString("address") << std::endl;
	    }
	}catch(...)
	{
	    if(rs!=NULL) delete rs;
	}
	return ret;
	}*/
    
};  

extern CZBDataBase *DB;

#endif
