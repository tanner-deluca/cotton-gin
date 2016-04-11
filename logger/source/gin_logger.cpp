#include <thread>
#include <chrono>
#include <iostream>


#include "gin_logger.h"


#ifdef GIN_LOGGER_DEBUG
#include <bitset>
#endif


using namespace std;


/* logging flags */
#define LOGGER_RUNNING		1
#define LOGGER_STOPPING		2
#define LOGGER_WRITING		4
#define LOGGER_PAUSED		8
#define LOGGER_CMD_LINE		16
#define LOGGER_FILE_SET		32
#define LOGGER_FAILURE		64


ginLogger::~ginLogger()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger destructor called\n";
#endif 
  
  stop();
  log_file.close();
}

bool ginLogger::set_log_file( string log_file_name )
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger set_log_file called\n";
#endif 
  
  /* check if a file is already opened */
  if( log_file.is_open() )
  {
    if( check_bit( flags, LOGGER_RUNNING ) )
      return runtime_file_change( log_file_name );
    else
      log_file.close();
  }
  
  log_file.open( log_file_name );
  if( !log_file.is_open() )
    return false;
  
  this->log_file_name = log_file_name;
  
  return true;
}


bool ginLogger::runtime_file_change( string log_file_name )
{
  /* temporarily pause the logger */
  set_bit( flags, LOGGER_PAUSED );
   
  /* the log_file can be changed once the logger is finished writing */
  while( check_bit( flags, LOGGER_WRITING ) )
  {
    this_thread::sleep_for( chrono::milliseconds( 3 ) );
  }
  
  log_file.close();
  log_file.open( log_file_name );
  if( !log_file.is_open() )
  {
    /* stop the logger if the log_file is broken */
    set_bit( flags, LOGGER_FAILURE );
    stop();
    remove_bit( flags, LOGGER_PAUSED );
    remove_bit( flags, LOGGER_FAILURE );
    
    return false;
  }
  
  this->log_file_name = log_file_name;
  remove_bit( flags, LOGGER_PAUSED );
  
  return true;
}


string ginLogger::get_log_file()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger get_log_file called\n";
#endif 
  
  return log_file_name;
}


void ginLogger::enable_cmd_line()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger enable_cmd_line called\n";
#endif 
  
  set_bit( flags, LOGGER_CMD_LINE );
}


void ginLogger::disable_cmd_line()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger disable_cmd_line called\n";
#endif 
  
  remove_bit( flags, LOGGER_CMD_LINE );
}


void ginLogger::pause()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger pause called\n";
#endif 
  
  set_bit( flags, LOGGER_PAUSED );
}


void ginLogger::resume()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger resume called\n";
#endif 
  
  remove_bit( flags, LOGGER_PAUSED );
}


bool ginLogger::start()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger start called\n";
#endif 
  
  /* don't start an already started logger */
  if( run_lock.try_lock() )
  {
    log_queue = g_async_queue_new();
    if( !log_queue )
    {
      run_lock.unlock();
      return false;
    }
    
    set_bit( flags, LOGGER_RUNNING );
    log_thread = thread( [ this ] { run(); } );
    log_thread.detach();
    
    return true;
  }
  else
    return false;
}


void ginLogger::stop()
{ 
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger stop called\n";
#endif 
  
  /* don't stop what hasn't started or is already stopping */
  if( !check_bit( flags, LOGGER_RUNNING ) || check_bit( flags, LOGGER_STOPPING ) )
    return;
  
  set_bit( flags, LOGGER_STOPPING );
  
  /* wait for the logger to finish up */
  while( check_bit( flags, LOGGER_RUNNING ) )
  {
    this_thread::sleep_for( chrono::milliseconds( 3 ) );
  }
  
  g_async_queue_unref( log_queue );
  log_queue = NULL;
  remove_bit( flags, LOGGER_STOPPING );
}


void ginLogger::run()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger run called\n";
#endif  
  
  while( !check_bit( flags, LOGGER_STOPPING ) )
  {
    if( !check_bit( flags, LOGGER_PAUSED ) )
      empty_queue();
  }
  
  /* make sure everything is logged */
  if( !check_bit( flags, LOGGER_FAILURE ) )
    empty_queue();
  remove_bit( flags, LOGGER_RUNNING );
  run_lock.unlock();
}


bool ginLogger::is_running()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger is_running called\n";
#endif
  
  if( check_bit( flags, LOGGER_RUNNING ) && !check_bit( flags, LOGGER_STOPPING ) )
    return true;
  
  return false;
}


void ginLogger::empty_queue()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger empty_queue called\n";
#endif  
  
  set_bit( flags, LOGGER_WRITING );
  
  string *msg;
  
  while( g_async_queue_length( log_queue ) > 0 )
  {
    msg = ( string * )( g_async_queue_pop( log_queue ) );
    if( !msg )
      break;
    
    log_file << *msg << "\n";
    if( check_bit( flags, LOGGER_CMD_LINE ) )
      cout << *msg << "\n";
    
    delete( msg );
    msg = NULL;
  }
  
  remove_bit( flags, LOGGER_WRITING );
}


bool ginLogger::log_message( string file, int line, string log_level, string message )
{
#ifdef GIN_LOGGER_DEBUG
  cout << "ginLogger log_message called\n";
#endif 
  
  if( !check_bit( flags, LOGGER_RUNNING ) && check_bit( flags, LOGGER_STOPPING ) )
    return false;
  
  string *msg;
 
  msg = new string( log_level );
  msg->append( "::" );
  msg->append( file );
  msg->append( "::" );
  msg->append( to_string(line ) );
  msg->append( "\n" );
  msg->append( message );
  
  g_async_queue_push( log_queue, msg );
  
  return true;
}


/* eof */