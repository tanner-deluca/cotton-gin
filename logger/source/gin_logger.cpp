#include <thread>
#include <chrono>
#include <iostream>

#include "gin_logger.h"


#ifdef GIN_LOGGER_DEBUG
#include <bitset>
#endif

using namespace std;


/* logging flags */
#define LOGGER_STOPPED		0
#define LOGGER_RUNNING		1
#define LOGGER_WRITING		2
#define LOGGER_STOPPING		4
#define LOGGER_PAUSED		8
#define LOGGER_CMD_LINE		16
#define LOGGER_INITIALIZED	32


ginLogger::ginLogger( string log_file_name )
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger constructor start debug\n" << log_file_name << "\ndone\n\n";
#endif
  
  log_file.open( log_file_name );
  if( !log_file.is_open() )
    return;
  
  this->log_file_name = log_file_name;
  flags = 0;
  
  set_bit( flags, LOGGER_INITIALIZED );
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger constructor end debug\n" << this->log_file_name << "\n" << "flags::" << bits << "\ndone\n\n";
#endif
}

ginLogger::~ginLogger()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger destructor start debug\n\n";
#endif
    
  stop();
  log_file.close();
  
#ifdef GIN_LOGGER_DEBUG
  cout << "logger destructor end debug\n\n";
#endif
}


bool ginLogger::initialized()
{
  if( check_bit( flags, LOGGER_INITIALIZED ) )
  {

#ifdef GIN_LOGGER_DEBUG
    cout << "logger initialized debug\n" << "true\ndone\n\n";
#endif
  
    return true;
  }
  
#ifdef GIN_LOGGER_DEBUG
  cout << "logger initialized debug\n" << "false\ndone\n\n";
#endif
 
  return false;
}


void ginLogger::enable_cmd_line()
{
  set_bit( flags, LOGGER_CMD_LINE );
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger enable_cmd_line debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
}


void ginLogger::disable_cmd_line()
{
  remove_bit( flags, LOGGER_CMD_LINE );
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger disable_cmd_line debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
}


string ginLogger::get_log_file()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger get_log_file debug\n" << log_file_name << "\ndone\n\n";
#endif

  return log_file_name;
}


bool ginLogger::start()
{
  if( check_bit( flags, LOGGER_RUNNING ) )
    return false;
  else
    set_bit( flags, LOGGER_RUNNING );
  
  log_queue = g_async_queue_new();
  if( !log_queue )
    return false;
  
  log_thread = thread( [ this ] { run(); } );
  log_thread.detach();
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger start debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
  
  return true;
}


void ginLogger::stop()
{ 
  set_bit( flags, LOGGER_STOPPING );
  
  while( check_bit( flags, LOGGER_RUNNING ) )
  {
    this_thread::sleep_for( chrono::milliseconds( 10 ) );
  }
  
  empty_queue();
  g_async_queue_unref( log_queue );
  log_queue = NULL;
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger stop debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
}


void ginLogger::pause()
{
  set_bit( flags, LOGGER_PAUSED );
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger paused debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
}


void ginLogger::resume()
{
  remove_bit( flags, LOGGER_PAUSED );
  
#ifdef GIN_LOGGER_DEBUG
  bitset<32> bits( flags );
  cout << "logger resume debug\n" << "flags::" << bits << "\ndone\n\n";
#endif
}


bool ginLogger::log_message( string file, int line, string log_level, string message )
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger log_message start debug\n" << file << "\n" << to_string( line ) << "\n" << \
  log_level << "\n" << message << "\ndone\n\n";
#endif
  
  if( !check_bit( flags, LOGGER_RUNNING ) )
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

#ifdef GIN_LOGGER_DEBUG
  cout << "logger log_message end debug\n" << *msg << "\ndone\n\n";
#endif
  
  return true;
}


void ginLogger::empty_queue()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger empty_queue start debug\n\n";
#endif  
  
  string *msg;
  
  while( g_async_queue_length( log_queue ) > 0 )
  {
    msg = ( string * )( g_async_queue_pop( log_queue ) );
    if( !msg )
      break;
    
    set_bit( flags, LOGGER_WRITING );
    log_file << *msg << "\n";
    if( check_bit( flags, LOGGER_CMD_LINE ) )
      cout << *msg << "\n";
    
    delete( msg );
    msg = NULL;
    remove_bit( flags, LOGGER_WRITING );
  }
  
#ifdef GIN_LOGGER_DEBUG
  cout << "logger empty_queue end debug\n\n";
#endif 
}


void ginLogger::run()
{
#ifdef GIN_LOGGER_DEBUG
  cout << "logger running\n\n";
#endif  
  
  while( !check_bit( flags, LOGGER_STOPPING ) )
  {
    if( !check_bit( flags, LOGGER_PAUSED ) )
      empty_queue();
  }
  
  remove_bit( flags, LOGGER_STOPPING );
  remove_bit( flags, LOGGER_RUNNING );
}


/* eof */