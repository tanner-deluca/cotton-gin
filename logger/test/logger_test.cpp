#include <iostream>
#include <glib.h>
#include <thread>
#include <bitset>
#include <queue>
#include "gin_logger.h"


using namespace std;


int main()
{
  queue<string> q;
  
  q.push( "hi" );
  cout << q.front() << "\n";
  q.pop();
  cout << q.front() << "\n";
  /*
  cout << "logger test\n";
  
  ginLogger *logger = new ginLogger();
  
  if( !logger->start() )
    cout << "won't start without setting a log file\n";
  
  logger->log( FILE_AND_LINE, LOG_INFO, "hi" );
  
  logger->set_log_file( "test_file1.txt" );
  logger->set_log_file( "test_file2.txt" );
  
  logger->enable_cmd_line();
  logger->pause();
  
  bitset<8> logger_flags1( logger->get_flags() );
  cout << "logging flags after setting cmd line and pausing::" << logger_flags1 << "\n";
  
  logger->resume();
  logger->disable_cmd_line();
  bitset<8> logger_flags2( logger->get_flags() );
  cout << "logging flags after removing cmd line and resuming::" << logger_flags2 << "\n";
  
  logger->enable_cmd_line();
  
  logger->start();
     
  this_thread::sleep_for( chrono::seconds( 1 ) );
  cout << "done sleeping\n";
  delete( logger );
  */
  return 0;
}  