#include <iostream>
#include <glib.h>
#include <thread>
#include "gin_logger.h"


using namespace std;


int main()
{
  ginLogger *logger = new ginLogger( "hey.txt" );
  
  logger->initialized();
  
  logger->get_log_file(); 
 
  logger->start();
 
  logger->enable_cmd_line();
  logger->log_message( __FILE__, __LINE__, LOG_FATAL, "hi" );
  delete( logger ); 
  return 0;
}  