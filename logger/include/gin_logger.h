#pragma once


#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <glib.h>


#include "flag.h"

/**
 * @file gin_logger.h
 * 
 * @brief A file that contains classes, macros, and defines for logging messages to files in a multi-threaded environment.
 */

//#define GIN_LOGGER_DEBUG


/**
 * The ginLogger is the class that is used for controlling the logging process.
 */
class ginLogger
{
  /* variables */
  public:
  
  private:
    volatile flag8	flags;		/**< holds all of the flags that are used to control the logger */
    GAsyncQueue		*log_queue;	/**< holds all the pending messages to log */
    std::string		log_file_name;	/**< the name of the file to log to */
    std::ofstream	log_file;	/**< the file to write to */
    std::thread		log_thread;	/**< the thread the logger is using */
    std::mutex		run_lock;	/**< the lock that prevents a user from starting a running logger */

  
  /* functions */
  public:
    /**
     * @brief The ginLogger destructor.
     * 
     * @note It will stop the logger.
     * @note It will close the log file.
     */
    ~ginLogger();
    
    /**
     * @brief The function to set the file to log to
     * 
     * @param log_file_name the name of the file to log to
     * 
     * @note This function can be used during runtime.
     * @note If the log file fails to open during runtime, the logger will be stopped.
     */
    bool set_log_file( std::string log_file_name );
    
    /**
     * @brief The function to start the logging process.
     *
     * @return true on success, false if else
     * 
     * @note False is returned when called by a logger that is already running.
     */
    bool start();
    
    /**
     * @brief The function to check if a logger is running.
     * 
     * @return true is the logger is running, false if else
     */
    bool is_running();
    
    /**
     * @brief The function to stop the logging process.
     */
    void stop();
    
    /**
     * @brief The function to pause the logging process.
     */
    void pause();
    
    /**
     * @brief The function to resume the logging process.
     */
    void resume();
    
    /**
     * @brief The function to enable the logger to write to the command line.
     */
    void enable_cmd_line();
    
    /**
     * @brief The function to stop the logger from writing to the command line.
     */
    void disable_cmd_line();
    
    /**
     * @brief The function to get the name of the file being logged to.
     * 
     * @return the name of the file being logged to
     */
    std::string get_log_file();
    
    /**
     * @brief The function to add a message onto the log queue.
     * 
     * @param file the file name of the line that is calling the log_message function
     * @param line the line number of the line that is calling the log_message function
     * @param log_level the log level to write to
     * @param message the message to write
     * 
     * @return true on success, false if trying to write to non-running logger
     * 
     * @note messages will appear in the following format,\nlog_level::file::line\nmessage
     */
    bool log_message( std::string file, int line,  std::string log_level, std::string message );
    
  private:
    /**
     * @brief The function to write all messages in the log_queue to the log_file.
     */
    void empty_queue();
    
    /**
     * @brief The function that handles the changing of log files during runtime.
     * 
     * @param log_file_name the name of the new file to log to
     * 
     * @return true on success, false if else
     */
    bool runtime_file_change( std::string log_file_name );
    
    /**
     * @brief The logging thread function.
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