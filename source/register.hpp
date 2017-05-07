//! @file
//! @brief Object register template - header file.
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
#ifndef _OBJECT_REGISTER_HEADER
#define _OBJECT_REGISTER_HEADER
//============================================
#include <map>
//============================================
namespace smpp { namespace reg {
//===========================================
class Base {
public:
  Base(){}
  virtual ~Base(){}
  virtual void destroyThis()=0;
};
//===========================================
template <typename I=int> I randomInt(I from,I to){
  static std::random_device rd;
  static std::mt19937 generator(rd());
  std::uniform_int_distribution<I> distribution(from,to);
  return(distribution(generator));
}
template<class T> class Register {
public:
  typedef std::map<T*,std::string> register_t;
  typedef typename register_t::iterator iterator;
private:
  register_t r;
public:
  bool add(T* ptr,const std::string & desc){
    if (r.count(ptr)) return(false);
    r[ptr]=desc;
    return(true);
  }
  bool del(T* ptr){
    if (!r.count(ptr)) return(false);
    r.erase(ptr);
    return(true);
  }
  bool test(T* ptr){
    return(r.count(ptr));
  }
  T* get(void *ptr){
    if (r.count((T*)ptr)){
      return((T*)ptr);
    }
    return(nullptr);
  }
  T* rand(){
    if (r.size()){
      iterator it=r.begin();
      std::advance(it,randomInt<std::size_t>(0,r.size()-1));
      return(it->first);
    }
    return(nullptr);
  }
  std::size_t size(){
    return(r.size());
  }
  void destroy(){
    for (iterator it=r.begin();it!=r.end();++it){
      if(it->first)  it->first->destroyThis();
    }
  }
  iterator begin(){
    return(r.begin());
  }
  iterator end(){
    return(r.end());
  }
};
template<class T> Register<T> & get(){
  static Register<T> r;
  return(r);
}
#define REG_IT_SMPP_APP ::smpp::reg::Register<::smpp::app::Connection>::iterator
#define REGISTER_SMPP_APP ::smpp::reg::get<::smpp::app::Connection>()
#define REG_IT_CLI_APP ::smpp::reg::Register<::smpp::cli::Connection>::iterator
#define REGISTER_CLI_APP ::smpp::reg::get<::smpp::cli::Connection>()
#define REG_IT_CLIENT_TCP ::smpp::reg::Register<::smpp::client::Tcp>::iterator
#define REGISTER_CLIENT_TCP ::smpp::reg::get<::smpp::client::Tcp>()
#define REG_IT_CLIENT_STREAM ::smpp::reg::Register<::smpp::client::Stream>::iterator
#define REGISTER_CLIENT_STREAM ::smpp::reg::get<::smpp::client::Stream>()
#define REG_IT_SERVER_TCP ::smpp::reg::Register<::smpp::server::Tcp>::iterator
#define REGISTER_SERVER_TCP ::smpp::reg::get<::smpp::server::Tcp>()
#define REG_IT_SERVER_STREAM ::smpp::reg::Register<::smpp::server::Stream>::iterator
#define REGISTER_SERVER_STREAM ::smpp::reg::get<::smpp::server::Stream>()
//============================================
}}
//===========================================
#endif