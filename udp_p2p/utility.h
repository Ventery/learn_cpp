/**********************
project name : p2p_shell
license : MIT License
Author : Ventery
Email :1297054069@qq.com
***********************/
#pragma once
#include "global.h"

pid_t get_tid();
void error_handling(char *message);
void praseIP_PORT(char *message,int IP ,int PORT );
void genetate_vec();
void make_message(const char * );
std::string decode(const std::string );
void handle_alarm(int );
void sig_handling(int );
AnalysisState analysis(const std::string);
