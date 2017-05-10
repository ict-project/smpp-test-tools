//! @file
//! @brief Main module - Source file.
//! @author Mariusz Ornowski (mariusz.ornowski@ict-project.pl)
//! @version 1.0
//! @date 2016-2017
//! @copyright ICT-Project Mariusz Ornowski (ict-project.pl)
/* **************************************************************
Copyright (c) 2016-2017, ICT-Project Mariusz Ornowski (ict-project.pl)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the ICT-Project Mariusz Ornowski nor the names
of its contributors may be used to endorse or promote products
derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************/
//============================================
#include "main-function.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
#include <iostream>
#include <map>
#include <boost/bind.hpp>
//============================================
namespace smpp { namespace main {
//===========================================
std::string syslogInfoString;
static std::size_t print_help=0;
OPTIONS_CONFIG(main1,1){
  if (config) {
    parser.registerOptNoValue(L'h',L"help",print_help);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'h')<<" - print help."<<std::endl;
  }
}
static std::size_t print_log=0;
OPTIONS_CONFIG(main2,2){
  if (config) {
    parser.registerOptNoValue(L'l',L"logger",print_log);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'l')<<" - print logger on stderr."<<std::endl;
  }
}
static const std::map<std::wstring,int> logger_dict={
  {L"critical",ict::logger::critical},
  {L"error",ict::logger::error},
  {L"warning",ict::logger::warning},
  {L"notice",ict::logger::notice},
  {L"info",ict::logger::info},
  {L"debug",ict::logger::debug},
  {L"errors",ict::logger::errors},
  {L"warnings",ict::logger::warnings},
  {L"notices",ict::logger::notices},
  {L"infos",ict::logger::infos},
  {L"all",ict::logger::all},
  {L"nocritical",ict::logger::nocritical},
  {L"noerrors",ict::logger::noerrors},
  {L"nowarnings",ict::logger::nowarnings},
  {L"nonotices",ict::logger::nonotices},
  {L"none",ict::logger::none},
  {L"nodebug",ict::logger::nodebug}
};
static int logger_direct=ict::logger::notices;
OPTIONS_CONFIG(main3,201){
  if (config) {
    parser.registerLongOpt(L"logger-direct",logger_direct,logger_dict);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L"logger-direct")<<" - severity of logger messages that printed directly (default - 'notices')."<<std::endl;
  }
}
static int logger_buffered=ict::logger::nonotices;
OPTIONS_CONFIG(main4,202){
  if (config) {
    parser.registerLongOpt(L"logger-buffered",logger_buffered,logger_dict);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L"logger-buffered")<<" - severity of logger messages that are buffered and printed only on errors (default - 'nonotices')."<<std::endl;
  }
}
static int logger_dump=ict::logger::errors;
OPTIONS_CONFIG(main5,203){
  if (config) {
    parser.registerLongOpt(L"logger-dump",logger_dump,logger_dict);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L"logger-dump")<<" - severity of logger messages that trigger printout of buffered messages (default - 'errors')."<<std::endl;
  }
}
OPTIONS_CONFIG(main6,204){
  if (config) {
  } else {
    parser.errors<<" "<<"Available logger severity values: 'critical', 'error', 'warning', 'notice', 'info', 'debug', 'errors', 'warnings', 'notices', 'infos', 'all', 'nocritical', 'noerrors', 'nowarnings', 'nonotices', 'none', 'nodebug'."<<std::endl;
  }
}
static std::size_t daemon_mode=0;
OPTIONS_CONFIG(main7,205){
  if (config) {
    parser.registerOptNoValue(L'd',L"daemon",daemon_mode);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'd')<<" - run as daemon."<<std::endl;
  }
}
//===========================================
boost::asio::io_service & ioService(){
  static boost::asio::io_service io_service;
  return(io_service);
}
//===========================================
int mainFunction(int argc,const char* argv[],Application & app){
  int out=0;
  std::string locale(setlocale(LC_ALL,std::getenv("LANG")));
  try {// Parsowanie parametrów.
    out=OPTIONS_PARSE(argc,argv,std::cout);
    if (out) return(out);
    if (print_help){
      OPTIONS_HELP(std::cout);
      return(0);
    }
  } catch (std::exception& e) {
    std::cout<<"Exception: "<<e.what()<<std::endl<<std::endl;
    OPTIONS_HELP(std::cout);
    return(-100);
  }
  {// Ustawianie loggera.
    if (print_log) {
      if (daemon_mode){
        LOGGER_SET(syslogInfoString);
      } else{
        LOGGER_SET(std::cerr);
      }
    }
    LOGGER_DEFAULT(
      (print_log?(ict::logger::all&logger_direct):(ict::logger::none)),
      (print_log?(ict::logger::all&logger_buffered):(ict::logger::none)),
      (print_log?(ict::logger::all&logger_dump):(ict::logger::none))
    );
    LOGGER_BASEDIR;
  }
  try {// Konfigurowanie asychronicznej aplikacji
    boost::asio::signal_set signals(ioService(),SIGINT,SIGTERM);
    //signals.async_wait(boost::bind(&boost::asio::io_service::stop,&(ioService())));
    signals.async_wait([&app](const boost::system::error_code& ec,int signal_number){
      ioService().stop();// Zatrzymanie aplikacji.
    });
    if (daemon_mode){
      const int write_flags=O_WRONLY|O_CREAT|O_APPEND;
      const mode_t write_mode=S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
      ioService().notify_fork(boost::asio::io_service::fork_prepare);
      // First fork
      if (pid_t pid=fork()) {
        if (pid>0) {//Parent
          exit(0);
        } else {//Error
          return(-201);
        }
      }
      setsid();
      out=chdir("/");
      umask(0);
      // Second fork.
      if (pid_t pid = fork()) {
        if (pid>0){//Parent
          exit(0);
        } else {//Error
          return(-202);
        }
      }
      // Close the standard streams.
      close(0);
      close(1);
      close(2);
      if (open("/dev/null",O_RDONLY)<0){//STDIN from /dev/null
        return(-203);//Error
      }
      if (open("/dev/null",write_flags,write_mode)<0) {//STDOUT to /dev/null
        return(-204);//Error
      }
      if (dup(1)<0) {//STDERR to /dev/null
        return(-205);//Error
      }
      ioService().notify_fork(boost::asio::io_service::fork_child);
    }
    {
      LOGGER_LAYER;
      LOGGER_DEBUG<<__LOGGER__<<"Application is about to start ..."<<std::endl;
      app.doStart();
      LOGGER_INFO<<__LOGGER__<<"IO service is about to start ..."<<std::endl;
      ioService().run();// Uruchomienie aplikacji.
      LOGGER_INFO<<__LOGGER__<<"Application is about to stop ..."<<std::endl;
      app.doClean();
      ioService().reset();//Zamykanie.
      ioService().poll();
      LOGGER_INFO<<__LOGGER__<<"IO service has stopped ..."<<std::endl;
      app.doStop();
      LOGGER_DEBUG<<__LOGGER__<<"Application has stopped ..."<<std::endl;
    }
  } catch (std::exception& e) {
    std::cout<<"Exception: "<<e.what()<<std::endl<<std::endl;
    OPTIONS_HELP(std::cout);
    return(-102);
  }
  return(0);
}
void memoryDump(std::ostream & out,void * ptr,std::size_t size){
  static std::map<uint8_t,char> map={
    {0x0,'0'},
    {0x1,'1'},
    {0x2,'2'},
    {0x3,'3'},
    {0x4,'4'},
    {0x5,'5'},
    {0x6,'6'},
    {0x7,'7'},
    {0x8,'8'},
    {0x9,'9'},
    {0xA,'A'},
    {0xB,'B'},
    {0xC,'C'},
    {0xD,'D'},
    {0xE,'E'},
    {0xF,'F'}
  };
  out<<'|';
  for (std::size_t k=0;k<size;k++){
    uint8_t * p=(uint8_t *)ptr+k;
    out<<map[((*p)>>4)];
    out<<map[((*p)&&0xF)];
    out<<' ';
  }
  out<<'|';
  for (std::size_t k=0;k<size;k++){
    uint8_t * p=(uint8_t *)ptr+k;
    if (isprint(*p)) {
      out<<(*p);
    } else {
      out<<'_';
    }
  }
  out<<'|'<<size<<'|';
}
//============================================
}}
//============================================