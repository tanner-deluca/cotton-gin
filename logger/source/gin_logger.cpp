#include <thread>
#include <chrono>
#include <iostream>
#include <ctime>


#include "gin_logger.h"


#ifdef GIN_LOGGER_DEBUG
#include <bitset>
#endif


using namespace std;


ginLogger::ginLogger()
{
  flags = 0;
  log_queue = NULL;
  log_file_name = "";
}


ginLogger::~ginLogger()
{
  stop();
  log_file.close();
}


bool ginLogger::set_log_file( string log_file_name )
{
  write_lock.lock();
  
  if( log_file.is_open() )
    log_file.close();
  
  log_file.open( log_file_name );
  if( !log_file.is_open() )
  {
    stop();
    remove_bit( flags, LOGGER_FILE_SET );
    return false;
  }
  
  this->log_file_name = log_file_name;
  set_bit( flags, LOGGER_FILE_SET );
  
  write_lock.unlock();
  
  return true;
}


string ginLogger::get_log_file()
{
  return log_file_name;
}


void ginLogger::stop()
{
  cout << "stopping before run_lock\n";
  run_lock.lock();
  
  cout << "stopping before log_queue check\n";
  if( !log_queue )
    return;

  cout << "stopping\n";
  if( check_bit( flags, LOGGER_FILE_SET ) )
    empty_queue();
  else
    delete_queue();
  
  g_async_queue_unref( log_queue );
  log_queue = NULL;
  remove_bit( flags, LOGGER_RUNNING );
  start_lock.unlock();
}


bool ginLogger::start()
{
  if( !check_bit( flags, LOGGER_FILE_SET ) )
    return false;
  
  if( start_lock.try_lock() )
  {
    log_queue = g_async_queue_new();
    if( !log_queue )
    {
      start_lock.unlock();
      return false;
    }
    
    run_lock.unlock();
    log_thread = thread( [ this ] { run(); } );
    log_thread.detach();
    set_bit( flags, LOGGER_RUNNING );
    
    return true;
  }
  else
    return false;
}


void ginLogger::run()
{
  cout << "running\n";
  
  while( run_lock.try_lock() )
  {
    this_thread::sleep_for( chrono::milliseconds( 100 ) );
    
    empty_queue();
    run_lock.unlock();
  }

  remove_bit( flags, LOGGER_RUNNING );
  cout << "run done\n";
}


void ginLogger::empty_queue()
{
  static string *msg = NULL;
  static int queue_length;
  
  if( write_lock.try_lock() )
  { 
    queue_length = g_async_queue_length( log_queue );
    
    while( queue_length > 0 )
    {
      msg = ( string * )( g_async_queue_pop( log_queue ) );
      if( !msg )
	break;
      
      log_file << *msg << "\n";
      if( check_bit( flags, LOGGER_CMD_LINE ) )
	cout << *msg << "\n";
      
      delete( msg );
      queue_length--;
    }

    write_lock.unlock();
  }
}


void ginLogger::delete_queue()
{
  string *msg = NULL;
  
  while( ( msg = ( string * )( g_async_queue_pop( log_queue ) ) ) )
  {
    delete( msg );
    msg = NULL;
  }
}


void ginLogger::log( string file, int line, string log_level, string message )
{
  if( !log_queue )
    return;
  
  time_t cur_time;
  struct tm *time_struct;
  
  cur_time = time( NULL );
  time_struct = localtime( &cur_time );
  
  string *msg;
  
  msg = new string( asctime( time_struct ) );
  msg->append( log_level );
  msg->append( "::" );
  msg->append( file );
  msg->append( "::" );
  msg->append( to_string(line ) );
  msg->append( "\n" );
  msg->append( message );
  
  g_async_queue_push( log_queue, msg );
}


flag8 ginLogger::get_flags()
{
  return flags;
}


void ginLogger::pause()
{
  write_lock.lock();
  set_bit( flags, LOGGER_PAUSED );
}


void ginLogger::resume()
{
  write_lock.unlock();
  remove_bit( flags, LOGGER_PAUSED );
}


void ginLogger::enable_cmd_line()
{
  set_bit( flags, LOGGER_CMD_LINE );
}


void ginLogger::disable_cmd_line()
{
  remove_bit( flags, LOGGER_CMD_LINE );
}


/* eof */