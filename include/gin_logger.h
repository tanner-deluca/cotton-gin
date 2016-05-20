#pragma once


#include <thread>
#include <mutex>
#include <fstream>
#include <string>


#include "gin_std_types.h"
#include "gin_std_macros.h"
#include "gin_async_queue.h"


/**
 * @file gin_logger.h
 * 
 * @brief A file that contains classes, macros, and defines for logging messages to files in a multi-threaded environment.
 */

#define GIN_LOGGER_DEBUG


/**
 * @brief an enumeration of all the logger exceptions
 */
enum logger_exceptions
{
  LOGGER_CRASH_EXCEPTION = 1,
  LOGGER_FILE_EXCEPTION,
  LOGGER_START_EXCEPTION,
  LOGGER_LOG_EXCEPTION
};


/**
 * an enumeration of all the logger flags
 */
enum logger_flags
{
  LOGGER_SET = 1,
  LOGGER_RUNNING = 2,
  LOGGER_PAUSED = 4,
  LOGGER_CMD_LINE = 8
};


/**
 * The gin_logger is a class for logging messages to files and the command line in a multi-threaded environment.
 */
class gin_logger
{
  /* variables */
public:
  
private:
  flag8					flags;			/**< the flags the show the status of the logger */
  
  uint32				log_rate;		/**< how often the logger loops ( in ms ) */
  std::string				log_file_name;		/**< the name of the file to log to */
  std::ofstream				*log_file_stream;	/**< the file stream to write to */
  gin_async_queue< std::string >	log_queue;		/**< the queue to hold pending messages */
  std::thread				log_thread;		/**< the thread that the logger runs on */
  
  std::mutex				start_lock;		/**< prevents the logger from starting */
  std::mutex				run_lock;		/**< prevents the logger from running */
  std::mutex				empty_lock;		/**< prevents the logger from writing */
 
  
  /* functions */
public:
  /**
   * @brief initializes the logger
   * 
   * @param log_file the file to log to
   * @param rate the log rate
   * 
   * @exception LOGGER_FILE_EXCEPTION integer exception thrown when the log_file fails to open 
   */
  gin_logger( std::string log_file, uint32 rate );
  
  /**
   * @brief destroys the logger
   * 
   * @note finishes logging the remaining messages in the queue before deleting
   */
  ~gin_logger();
  
  /**
   * @brief changes the file to log to
   * 
   * @param new_file the new file to log to
   * 
   * @note can be used while the logger is running
   * 
   * @exception LOGGER_FILE_EXCEPTION thrown when the new_file fails to open and will stop the logger
   */
  void change_log_file( std::string new_file );
  
  /**
   * @brief starts the logger
   * 
   * @exception LOGGER_FILE_EXCEPTION thrown when trying to start the logger without a log file set
   * @exception LOGGER_START_EXCEPTION thrown when trying to start the logger when it is already running
   */
  void start();
  
  /**
   * @brief stops the logger 
   * 
   * @note finishes logging the remaining messages
   */
  void stop();
  
  /**
   * @brief pauses the logger
   * 
   * @note messages can still be added while paused
   */
  void pause();
  
  /**
   * @brief resumes the logger
   */
  void resume();
  
  /**
   * @brief enables the logger to write the log messages to the command line as well as the log file
   * 
   * @note disabled by default
   */
  void enable_cmd_line();
  
  /**
   * @brief prevents the logger from writing to the command line
   */
  void disable_cmd_line();
  
  /**
   * @brief returns the current logging flags
   */
  flag8 get_status();
  
  /**
   * @brief returns the name of the log file
   */
  std::string get_log_file();
  
  /**
   * @brief adds a message to the log queue
   * 
   * @param file the file name of the line that is calling the log_message function
   * @param line the line number of the line that is calling the log_message function
   * @param log_type the type of log to write ( i.e. FATAL, ERROR, WARNING, TRACE, INFO )
   * @param message the message to write
   * 
   * @note messages will have a timestamp of when they were logged
   * @note messages will appear in the following format:\ntimestamp::log_type::file::line\nmessage
   * 
   * @exception LOGGER_START_EXCEPTION thrown when trying to write a message before the logger has started
   */
  void log( std::string file, int line, std::string log_type, std::string message );
  
private:
  /**
   * @brief the thread function that continously loops and write messages
   */
  void run();
  
  /**
   * @brief writes all the messages in the log queue to the log file
   */
  void empty_queue();
};


/* eof */