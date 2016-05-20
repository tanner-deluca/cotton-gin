#include <thread>
#include <chrono>
#include <iostream>
#include <ctime>


#include "gin_logger.h"


using namespace std;


gin_logger::gin_logger( std::string log_file, uint32 rate )
{
  run_lock.lock();
  
  log_rate = rate;
  
  log_file_stream = new ofstream( log_file );
  if( !log_file_stream->is_open() )
  {
    delete( log_file_stream );
    log_file_stream = nullptr;
    
    throw LOGGER_FILE_EXCEPTION;
  }
  
  log_file_name = log_file;
  set_bit( flags, LOGGER_SET );
}


gin_logger::~gin_logger()
{
  stop();
  
  log_file_stream->close();
  delete( log_file_stream );
  
  if( log_thread.joinable() )
    log_thread.join();
}


void gin_logger::change_log_file( std::string new_file )
{
  ofstream *new_stream = new ofstream( new_file );
  if( !new_stream->is_open() )
  {
    remove_bit( flags, LOGGER_SET );
    delete( new_stream );
    throw LOGGER_FILE_EXCEPTION;
  }
  
  empty_lock.lock();
  
  if( log_file_stream )
  {
    log_file_stream->close();
    delete( log_file_stream );
  }
  
  log_file_stream = new_stream;
  log_file_name = new_file;
  set_bit( flags, LOGGER_SET );
  
  empty_lock.unlock();
}


void gin_logger::start()
{
  if( !log_file_stream )
    throw LOGGER_FILE_EXCEPTION;
  
  if( start_lock.try_lock() )
  {
    run_lock.unlock();
    empty_lock.unlock();
    set_bit( flags, LOGGER_RUNNING );
    log_thread = thread( [ this ] { run(); } );
  }
  else
    throw LOGGER_START_EXCEPTION;
}


void gin_logger::stop()
{
  remove_bit( flags, LOGGER_RUNNING );
  
  run_lock.lock();
  
  if( log_thread.joinable() )
    log_thread.join();
  
  log_queue.clear();
  start_lock.unlock();
}


void gin_logger::run()
{
  while( run_lock.try_lock() )
  {
    empty_queue();
    run_lock.unlock();
    
    this_thread::sleep_for( chrono::milliseconds( log_rate ) );
  }
  
  empty_queue();
}


void gin_logger::empty_queue()
{
  static string *message = nullptr;
  
  if( empty_lock.try_lock() )
  {
    while( !log_queue.is_empty() )
    {
      message = log_queue.pop();
      if( !message )
	break;
      
      *log_file_stream << *message << endl;
      if( check_bit( flags, LOGGER_CMD_LINE ) )
	cout << *message << endl;
      
      delete( message );
    }
    
    empty_lock.unlock();
  }
}


void gin_logger::log( string file, int line, string log_type, string message )
{
  return_if_fail( check_bit( flags, LOGGER_RUNNING ) );
  
  time_t current_time = time( NULL );
  struct tm *time_struct = localtime( &current_time );
  
  string *msg = new string( log_type );
  msg->append( "::" );
  msg->append( file );
  msg->append( "::" );
  msg->append( to_string(line ) );
  msg->append( "\n" );
  msg->append( asctime( time_struct ) );
  msg->append( message );
 
  log_queue.push( msg );
}


void gin_logger::pause()
{
  empty_lock.lock();
  set_bit( flags, LOGGER_RUNNING );
}


void gin_logger::resume()
{
  empty_lock.unlock();
  remove_bit( flags, LOGGER_RUNNING );
}


void gin_logger::enable_cmd_line()
{
  set_bit( flags, LOGGER_CMD_LINE );
}


void gin_logger::disable_cmd_line()
{
  remove_bit( flags, LOGGER_CMD_LINE );
}


flag8 gin_logger::get_status()
{
  return flags;
}


string gin_logger::get_log_file()
{
  return log_file_name;
}


/* eof */