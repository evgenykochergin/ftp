
#ifndef FTPSERVER_H
#define FTPSERVER_H

#include "../common/common.h"




/**
 * Send list of files in current directory
 * over data connection
 * Return -1 on error, 0 on success
 */
int ftpserver_list(int sock_data, int sock_control);




/**
 * Open data connection to client 
 * Returns: socket for data connection
 * or -1 on error
 */
int ftpserver_start_data_conn(int sock_control);



/**
 * Authenticate a user's credentials
 * Return 1 if authenticated, 0 if not
 */
int ftpserver_check_user(char*user, char*pass);



/** 
 * Log in connected client
 */
int ftpserver_login(int sock_control);


/**
 * Wait for command from client and send response
 * Returns response code
 */
int ftpserver_recv_cmd(int sock_control, char*cmd, char*arg);



/** 
 * Child process handles connection to client
 */
void ftpserver_process(int sock_control);


#endif
