//! @file
//! @brief Client module - header file.
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
#ifndef _CLIENT_HEADER
#define _CLIENT_HEADER
//============================================
#include "resolver.hpp"
#include "connection.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
//============================================
namespace smpp { namespace client {
//===========================================
class Tcp : public resolver::Tcp {
private:
  //! Czy klient jest zatrzymany.
  bool stopped=false;
  //! Gniazdo do obsługi połączenia.
  boost::asio::ip::tcp::socket s;
  //! Fabryka połączeń.
  smpp::connection::factory_ip_t f;
  //! Timer dla połączenia.
  boost::asio::deadline_timer d;
public:
  Tcp(const std::string & host,const std::string & port,smpp::connection::factory_ip_t factory);
  Tcp(const std::string & host,const std::string & port,smpp::connection::factory_ip_t factory,resolver::error_handler_t onError);
  virtual ~Tcp();
  //! Zamyka połączenie.
  void doStop();
  void destroyThis(){doStop();}
private:
  //! Funkcja wykonywana, gdy zapytanie DNS zakończy się sukcesem
  void afterResolve();
  //! Otwiera połączenie.
  void doConnect();
};
void factory(const std::string & host,const std::string & port,smpp::connection::factory_ip_t factory);
void factory(const std::string & host,const std::string & port,smpp::connection::factory_ip_t factory,resolver::error_handler_t onError);
//============================================
class Stream : public resolver::Stream {
private:
  //! Czy klient jest zatrzymany.
  bool stopped=false;
  //! Gniazdo do obsługi połączenia.
  boost::asio::local::stream_protocol::socket s;
  //! Fabryka połączeń.
  smpp::connection::factory_local_t f;
public:
  Stream(const std::string & path,smpp::connection::factory_local_t factory);
  Stream(const std::string & path,smpp::connection::factory_local_t factory,resolver::error_handler_t onError);
  virtual ~Stream();
  //! Zamyka połączenie.
  void doStop();
  void destroyThis(){doStop();}
private:
  //! Funkcja wykonywana, gdy zapytanie zakończy się sukcesem
  void afterResolve();
  //! Otwiera połączenie.
  void doConnect();
};
void factory(const std::string & path,smpp::connection::factory_local_t factory);
void factory(const std::string & path,smpp::connection::factory_local_t factory,resolver::error_handler_t onError);
//============================================
}}
//===========================================
#endif