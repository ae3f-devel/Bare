#ifndef Svr_Main_h
#define Svr_Main_h

#include <ae2f/Cast.h>
#include <ae2f/Call.h>

/**
 * @brief
 * Starts a server
 * 
 * @details
 * It works as singleton. \n
 * Count of its call does not matter, it operates only once.
 * 
 * @param port
 * @returns 0 when succeed.
 */
ae2f_extern ae2f_SHAREDEXPORT 
int SvrMain(unsigned short port);

/**
 * @brief
 * Stops the servers started by `SvrMain`.
 */
ae2f_extern ae2f_SHAREDEXPORT
void SvrExit();

#endif