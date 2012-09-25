#include "czbdb.hpp"
#include "cppconn/exception.h"

CZBDataBase *DB;

CZBDataBase::CZBDataBase():log_file_name("log_db.txt"),isConnected(false)
{
    log_file.open(log_file_name.c_str(),std::fstream::in | std::fstream::out | std::fstream::app);
    if(!log_file.is_open())
    {
	std::cerr << "can't open log file " << log_file_name << std::endl;
    }
}

CZBDataBase::~CZBDataBase()
{
    log_file.close();
}

bool CZBDataBase::connect()
{
    try
    {
	sql_driver = get_driver_instance();
	sql_connection = sql_driver->connect(hostname.c_str(),username.c_str(),password.c_str());
	sql_connection->setAutoCommit(0);
	sql_connection->setSchema(database.c_str());
	sql_statement = sql_connection->createStatement();
    }catch(sql::SQLException &e)
    {
	std::cout << "ERROR: SQLException in " << __FILE__;
	std::cout << " (" << __func__<< ") on line " << __LINE__ << std::endl;
	std::cout << "ERROR: " << e.what();
	std::cout << " (MySQL error code: " << e.getErrorCode();
	std::cout << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	
	if (e.getErrorCode() == 1047)
	{
	    /*
	      Error: 1047 SQLSTATE: 08S01 (ER_UNKNOWN_COM_ERROR)
	      Message: Unknown command
	    */
	    std::cout << "\nYour server does not seem to support Prepared Statements at all. ";
	    std::cout << "Perhaps MYSQL < 4.1?" << std::endl;
	}
	
	return false;
	
    }catch(std::runtime_error &e)
    {
	std::cout << "ERROR: runtime_error in " << __FILE__;
	std::cout << " (" << __func__ << ") on line " << __LINE__ << std::endl;
	std::cout << "ERROR: " << e.what() << std::endl;
	
	return false;
    }
    
    return true;
    
}

bool CZBDataBase::disconnect(void)
{
    
}

void CZBDataBase::log_data(string_ptr _variable_name, string_ptr _value)
{

}

void CZBDataBase::log_info(string_ptr _event, string_ptr _msg)
{

}

void CZBDataBase::set_hostname(string_ptr _hostname)
{
    hostname = *_hostname;
}

void CZBDataBase::set_username(string_ptr _username)
{
    username = *_username;
}

void CZBDataBase::set_password(string_ptr _password)
{
    password = *_password;
}

void CZBDataBase::set_database(string_ptr _database)
{
    database = *_database;
}

void CZBDataBase::set_hostname(std::string _hostname)
{
    hostname = _hostname;
}

void CZBDataBase::set_username(std::string _username)
{
    username = _username;
}

void CZBDataBase::set_password(std::string _password)
{
    password = _password;
}

void CZBDataBase::set_database(std::string _database)
{
    database = _database;
}

void CZBDataBase::local_log(char *_msg)
{
    if(!log_file.is_open())
    {
	return;
    }

    log_file << _msg << std::endl; //TODO: put timestamp
}
