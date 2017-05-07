//! @file
//! @brief DNS resolver module - header file.
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
#ifndef _RESOLVER_HEADER
#define _RESOLVER_HEADER
//============================================
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "register.hpp"
//============================================
namespace smpp { namespace resolver {
//===========================================
class Tcp : public std::enable_shared_from_this<Tcp>, public smpp::reg::Base{
private:
  //! Asynchroniczne rozwiązywanie nazw DNS.
  boost::asio::ip::tcp::resolver r;
  //! Zapytanie  DNS.
  boost::asio::ip::tcp::resolver::query q;
  //! Timer dla zapytania DNS.
  boost::asio::deadline_timer d;
public:
  Tcp(const std::string & host,const std::string & port);
  void init();
  virtual ~Tcp();
private:
  //! Rozpoczyna rozwiązywanie nazw.
  void doResolve();
protected:
  typedef std::enable_shared_from_this<Tcp> enable_shared_t;
  //! Wynik zapytania do DNS.
  boost::asio::ip::tcp::resolver::iterator ei;
  //! Funkcja wykonywana, gdy zapytanie DNS zakończy się sukcesem (funkcja obowiązkowo do nadpisania).
  virtual void afterResolve()=0;
};
//============================================
class Stream : public std::enable_shared_from_this<Stream>{
public:
  Stream(const std::string & path);
  void init();
  virtual ~Stream();
private:
  //! Rozpoczyna rozwiązywanie nazw.
  void doResolve();
protected:
  typedef std::enable_shared_from_this<Stream> enable_shared_t;
  //! Lokalny endpoint.
  boost::asio::local::stream_protocol::endpoint ep;
  //! Funkcja wykonywana, gdy zapytanie zakończy się  sukcesem (funkcja obowiązkowo do nadpisania).
  virtual void afterResolve()=0;
};
//============================================
}}
//===========================================
#endif