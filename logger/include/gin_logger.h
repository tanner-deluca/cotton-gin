#pragma once


#include <thread>
#include <fstream>
#include <string>
#include <glib.h>

#include "flag.h"


//#define GIN_LOGGER_DEBUG


/**
 * the class that handles the logging
 */
class ginLogger
{
  /* variables */
  public:
  
  private:
    flag		flags;		/**< holds all of the flags that are used to control the logger */
    GAsyncQueue		*log_queue;	/**< holds all the pending messages to log */
    std::string		log_file_name;	/**< the name of the file to log to */
    std::ofstream	log_file;	/**< the file to write to */
    std::thread		log_thread;	/**< the thread the logger is using */
    
  /* functions */
  public:
    /**
     * @brief the constructor
     * 
     * @param log_file_name the file name and path to log to
     */
    ginLogger( std::string log_file_name );
    
    /**
     * @brief the destructor
     * 
     * @note will call the stop()
     */
    ~ginLogger();
    
    /**
     * @brief checks if the logger is initialized
     * 
     * @return true if it is, false if else
     */
    bool initialized();
    
    /**
     * @brief starts the logging process
     *
     * @return false if the logger is already running or the log_queue couldn't be allocated, true if else
     */
    bool start();
    
    /**
     * @brief stops the logging process
     */
    void stop();
    
    /**
     * @brief pauses the logging process
     */
    void pause();
    
    /**
     * @brief resumes the logging process
     */
    void resume();
    
    /**
     * @brief enables the logger to write to the command line
     */
    void enable_cmd_line();
    
    /**
     * @brief stops the logger from writing to the command line
     */
    void disable_cmd_line();
    
    /**
     * @brief gets the file name and path that the logger is writing to
     * 
     * @return the file name and path the logger is writing to
     */
    std::string get_log_file();
    
    /**
     * @brief puts a message onto the logging queue to be log
     * 
     * @param file the file name of the line that is calling the log_message function
     * @param line the line number of the line that is calling the log_message function
     * @param log_level the log level to write to
     * @param message the message to write
     * 
     * @return true on success, false if else
     */
    bool log_message( std::string file, int line,  std::string log_level, std::string message );
    
  private:
    /**
     * @brief writes all messages in the log_queue to the log_file
     */
    void empty_queue();
    
    /**
     * @brief checks to see if anything needs to be logged
     */
    void run();
};


/* standard logging levels */
/**
 * log level for fatal errors
 */
#define LOG_FATAL	"FATAL LOG"
/**
 * log level for non-fatal errors
 */
#define LOG_ERROR	"ERROR LOG"
/**
 * log level for warning messages
 */
#define LOG_WARN	"WARNING LOG"
/**
 * log level for tracing messages
 */
#define LOG_TRACE	"TRACE LOG"
/**
 * log level for miscellaneous information
 */
#define LOG_INFO	"INFO LOG"


/**
 * @brief an ease of use macro that automatically puts in the file and line into a log message
 */
#define log( level, message )	( log_message( __FILE__, __LINE__, level, message ) )


/* eof */