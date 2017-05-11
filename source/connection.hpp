//! @file
//! @brief Connection module - header file.
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
#ifndef _CONNECTION_HEADER
#define _CONNECTION_HEADER
//============================================
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../libict/source/logger.hpp"
#include "register.hpp"
#include "main-function.hpp"
//============================================
namespace smpp { namespace connection {
//===========================================
class Iface : public std::enable_shared_from_this<Iface>, public smpp::reg::Base {
public:
  typedef std::enable_shared_from_this<Iface> enable_shared_t;
  //! Opis połączenia.
  std::string sDesc;
  //! Rozmiar lokalnego bufora (zapisu i odczytu).
  enum {bufferSize=1024};
  //! Lokalny bufor odczytu.
  unsigned char readData[bufferSize];
  //! Rozmiar odczytanych danych.
  std::size_t readSize=0;
  //! Lokalny bufor zapisu.
  unsigned char writeData[bufferSize];
  //! Rozmiar danych do zapisu.
  std::size_t writeSize=0;
  //! Minimalna liczba bajtów na minutę przy odczycie (jeśli nie jest zachowana to połączene jest zamykane) - jeśli 0, to brak ograniczenia.
  std::size_t readMinFlow=0;
  //! Minimalna liczba bajtów na minutę przy zapisie (jeśli nie jest zachowana to połączene jest zamykane) - jeśli 0, to brak ograniczenia.
  std::size_t writeMinFlow=0;
  //! Ustawienie asychronicznego odczytu (funkcja obowiązkowo do nadpisania).
  virtual void asyncRead()=0;
  //! Ustawienie asychronicznego zapisu (funkcja obowiązkowo do nadpisania).
  virtual void asyncWrite()=0;
  //! Zamyka połącznie (funkcja obowiązkowo do nadpisania).
  virtual void doClose()=0;
  virtual void shutdownRead()=0;
  virtual void shutdownWrite()=0;
  //!
  //! @brief Wykonuje odczyt (funkcja obowiązkowo do nadpisania).
  //!  Powinna ustawić przy zakończeniu nowy asynchroniczny odczyt 
  //!  (lub ewentualnie zapis) - funkcje asyncRead() i/lub asyncWrite().
  //!  Jeśli tego nie zrobi połączenie może być automatycznie zamknięte.
  //!
  virtual void doRead()=0;
  //!
  //! @brief Wykonuje zapis (funkcja obowiązkowo do nadpisania).
  //!  Powinna ustawić przy zakończeniu nowy asynchroniczny zapis 
  //!  (lub ewentualnie odczyt) - funkcje asyncWrite() i/lub asyncRead().
  //!  Jeśli tego nie zrobi połączenie może być automatycznie zamknięte.
  //!
  virtual void doWrite()=0;
  //!
  //! @brief Obsługuje błąd odczytu (funkcja ewentualnie do nadpisania).
  //!  Może ustawić przy zakończeniu nowy asynchroniczny odczyt 
  //!  (lub ewentualnie zapis) - funkcje asyncRead() i/lub asyncWrite().
  //!  Jeśli tego nie zrobi połączenie może być automatycznie zamknięte.
  //! @param ec Kod błędu.
  //!
  virtual void readError(boost::system::error_code ec){}
  //!
  //! @brief Obsługuje błąd zapisu (funkcja ewentualnie do nadpisania).
  //!  Może ustawić przy zakończeniu nowy asynchroniczny zapis 
  //!  (lub ewentualnie odczyt) - funkcje asyncWrite() i/lub asyncRead().
  //!  Jeśli tego nie zrobi połączenie może być automatycznie zamknięte.
  //! @param ec Kod błędu.
  //!
  virtual void writeError(boost::system::error_code ec){}
  //! Standardowe funkcje (bez kodu) - do ewentualnego nadpisania.
  virtual void doStart(){};
  //! Standardowe funkcje (bez kodu) - do ewentualnego nadpisania.
  virtual void doStop(){};
public:
  Iface();
  virtual ~Iface();
  //! Zwraca opis połączenia.
  std::string socketDesc() const;
};
//============================================
template<class Socket,class Stack>class Connection : public Stack{
private:
  //! Informuje, czy połączenie jest zamknięte.
  bool stopped=false;
  //! Timer do obliczania liczby bajtów na minutę.
  boost::asio::deadline_timer d;
  //! Rozmiar odczytanych danych w ostatniej minucie.
  std::size_t readSizeMin=0;
  //! Rozmiar zapisanych danych w ostatniej minucie.
  std::size_t writeSizeMin=0;
  //! Funkcja ustawiająca timer do obliczania liczby bajtów na minutę.
  void scheduleMinFlow();
  //! Funkkcja sprawdzająca liczbę bajtów na minutę i zamukająca połączenie, gdy nie są spełnione określone minima.
  void checkMinFlow();
protected:
  //! Socket.
  Socket s;
  //! Ustawienie asychronicznego odczytu.
  void asyncRead();
  //! Ustawienie asychronicznego zapisu.
  void asyncWrite();
public:
  Connection(Socket & socket);
  void init();
  virtual ~Connection();
  //! Zamyka połącznie.
  void doClose();
  void destroyThis();
  void shutdownRead();
  void shutdownWrite();
};
template<class Socket,class Stack>void Connection<Socket,Stack>::scheduleMinFlow() {
  auto self(Stack::shared_from_this());
  if (stopped) return;
  d.expires_from_now(boost::posix_time::seconds(60));
  d.async_wait(
    [this,self](const boost::system::error_code & ec){
      LOGGER_LAYER;
      if(!ec){
        if (stopped) return;
        LOGGER_INFO<<__LOGGER__<<"Read flow "<<Stack::readMinFlow<<" bytes/minute on connection "<<Stack::socketDesc()<<std::endl;
        LOGGER_INFO<<__LOGGER__<<"Write flow "<<Stack::writeMinFlow<<" bytes/minute on connection "<<Stack::socketDesc()<<std::endl;
        if ((Stack::readMinFlow&&(readSizeMin<Stack::readMinFlow))||(Stack::writeMinFlow&&(writeSizeMin<Stack::writeMinFlow))){
          doClose();
          if (readSizeMin<Stack::readMinFlow) LOGGER_WARN<<__LOGGER__<<"Read flow to low ("<<readSizeMin<<"<"<<Stack::readMinFlow<<") on connection "<<Stack::socketDesc()<<std::endl;
          if (writeSizeMin<Stack::writeMinFlow) LOGGER_WARN<<__LOGGER__<<"Write flow to low ("<<writeSizeMin<<"<"<<Stack::writeMinFlow<<") on connection "<<Stack::socketDesc()<<std::endl;
        }
        readSizeMin=0;
        writeSizeMin=0;
        scheduleMinFlow();
      }
    }
  );
  LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" use count: "<<self.use_count()<<std::endl;
}
template<class Socket,class Stack>void Connection<Socket,Stack>::asyncRead(){
  auto self(Stack::shared_from_this());
  if (stopped) return;
  //boost::asio::async_read(
    //s,
  s.async_read_some(
    boost::asio::buffer(Stack::readData,Stack::bufferSize),
    [this,self](const boost::system::error_code & ec, std::size_t length){
      LOGGER_LAYER;
      try {
        if (ec) {
          Stack::readError(ec);
        } else {
          Stack::readSize=length;
          readSizeMin+=length;
            LOGGER_DEBUG<<__LOGGER__;
            smpp::main::memoryDump(LOGGER_DEBUG,Stack::readData,length);
            LOGGER_DEBUG<<std::endl;
          Stack::doRead();
          LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" read("<<ec<<") count: "<<length<<std::endl;
        }
      } catch (std::exception& e) {
        LOGGER_ERR<<__LOGGER__<<"Exception: "<<e.what()<<std::endl;
      }
    }
  );
  LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" use count: "<<self.use_count()<<std::endl;
}
template<class Socket,class Stack>void Connection<Socket,Stack>::asyncWrite(){
  auto self(Stack::shared_from_this());
  if (stopped) return;
  boost::asio::async_write(
    s,
    boost::asio::buffer(Stack::writeData,Stack::writeSize),
    [this,self](const boost::system::error_code & ec, std::size_t length){
      LOGGER_LAYER;
      try {
        if (ec){
          Stack::readError(ec);
        } else {
            LOGGER_DEBUG<<__LOGGER__;
            smpp::main::memoryDump(LOGGER_DEBUG,Stack::writeData,length);
            LOGGER_DEBUG<<std::endl;
          Stack::writeSize=0;
          writeSizeMin+=length;
          Stack::doWrite();
          LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" write("<<ec<<") count: "<<length<<std::endl;
        }
      } catch (std::exception& e) {
        LOGGER_ERR<<__LOGGER__<<"Exception: "<<e.what()<<std::endl;
      }
    }
  );
  LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" use count: "<<self.use_count()<<std::endl;
}
template<class Socket,class Stack>Connection<Socket,Stack>::Connection(Socket & socket):s(std::move(socket)),d(smpp::main::ioService()){
  std::ostringstream out;
  LOGGER_INFO<<__LOGGER__<<"smpp::connection::Connection has been created ..."<<std::endl;
  out<<"<local:"<<s.local_endpoint()<<", remote:"<<s.remote_endpoint()<<", ptr:"<<this<<">";
  Stack::sDesc=out.str();
}
template<class Socket,class Stack> void Connection<Socket,Stack>::init() {
  scheduleMinFlow();
  Stack::doStart();
}
template<class Socket,class Stack>Connection<Socket,Stack>::~Connection() {
  LOGGER_INFO<<__LOGGER__<<"smpp::connection::Connection has been destroyed ..."<<std::endl;
}
template<class Socket,class Stack>void Connection<Socket,Stack>::doClose(){
  auto self(Stack::shared_from_this());
  boost::system::error_code ec;
  if (stopped) return;
  stopped=true;
  Stack::doStop();
  s.close();
  d.cancel();
  LOGGER_DEBUG<<__LOGGER__<<"Connection "<<Stack::socketDesc()<<" use count: "<<self.use_count()<<std::endl;
}
template<class Socket,class Stack>void Connection<Socket,Stack>::destroyThis(){
  doClose();
}
template<class Socket,class Stack>void Connection<Socket,Stack>::shutdownRead() {
  auto self(Stack::shared_from_this());
  if (stopped) return;
  s.shutdown(Socket::shutdown_receive);
}
template<class Socket,class Stack>void Connection<Socket,Stack>::shutdownWrite() {
  auto self(Stack::shared_from_this());
  if (stopped) return;
  s.shutdown(Socket::shutdown_send);
}
//============================================
typedef void (*factory_ip_t)(boost::asio::ip::tcp::socket & socket);
typedef void (*factory_local_t)(boost::asio::local::stream_protocol::socket & socket);
//============================================
}}
//===========================================
#endif