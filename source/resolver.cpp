//! @file
//! @brief DNS resolver module - Source file.
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
#include "resolver.hpp"
#include "main-function.hpp"
#include "../libict/source/logger.hpp"
//============================================
namespace smpp { namespace resolver {
//============================================
Tcp::Tcp(const std::string & host,const std::string & port)
  :r(smpp::main::ioService()),q(host,port),d(smpp::main::ioService()){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Tcp has been created ..."<<std::endl;
}
Tcp::Tcp(const std::string & host,const std::string & port,error_handler_t onError)
  :r(smpp::main::ioService()),q(host,port),d(smpp::main::ioService()),e(onError){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Tcp has been created ..."<<std::endl;
}
void Tcp::init(){
  doResolve();
}
Tcp::~Tcp(){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Tcp has been destroyed ..."<<std::endl;
}
void Tcp::doResolve(){
  auto self(enable_shared_t::shared_from_this());
  LOGGER_DEBUG<<__LOGGER__<<"Trying to resolve "<<q.host_name()<<":"<<q.service_name()<<" ..."<<std::endl;
  if ((q.host_name()=="")||(q.host_name()=="0.0.0.0")||(q.host_name()=="[::]")){
    any=true;
    try{
      boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v6(),std::stol(q.service_name()));
      ep=endpoint;
      LOGGER_DEBUG<<__LOGGER__<<"Resolving "<<"<any>"<<":"<<q.service_name()<<" has succeeded ..."<<std::endl;
      afterResolve();
    }catch(...){
      LOGGER_INFO<<__LOGGER__<<"Resolving "<<"<any>"<<":"<<q.service_name()<<" has failed ..."<<std::endl;
    }
    return;
  }
  d.expires_from_now(boost::posix_time::seconds(60));
  d.async_wait(
    [this,self](const boost::system::error_code& ec){
      LOGGER_LAYER;
      if (ec){
        if (e) e(ec);
      } else {
        LOGGER_INFO<<__LOGGER__<<"Resolving timer "<<q.host_name()<<":"<<q.service_name()<<" has expired ..."<<std::endl;
        r.cancel();
        if (e) e(ec);
      }
    }
  );
  r.async_resolve(
    q,
    [this,self](const boost::system::error_code& ec,boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
      LOGGER_LAYER;
      if (ec){
        if (e) e(ec);
      } else {
        d.cancel();
        ei=endpoint_iterator;
        LOGGER_DEBUG<<__LOGGER__<<"Resolving "<<q.host_name()<<":"<<q.service_name()<<" has succeeded ..."<<std::endl;
        afterResolve();
      }
    }
  );
}
//============================================
Stream::Stream(const std::string & path):ep(path){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Stream has been created ..."<<std::endl;
}
Stream::Stream(const std::string & path,error_handler_t onError):ep(path),e(onError){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Stream has been created ..."<<std::endl;
}
Stream::~Stream(){
  LOGGER_INFO<<__LOGGER__<<"smpp::resolver::Stream has been destroyed ..."<<std::endl;
}
void Stream::init(){
  doResolve();
}
void Stream::doResolve(){
  auto self(enable_shared_t::shared_from_this());
  afterResolve();
}
//============================================
} }
//============================================