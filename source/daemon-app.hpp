//! @file
//! @brief Daemon CLI (tester) module - header file.
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
#ifndef _SMPP_D_CLI_HEADER
#define _SMPP_D_CLI_HEADER
//============================================
#include "smpp-connection.hpp"
#include "smpp-app.hpp"
#include "register.hpp"
//============================================
namespace smpp { namespace cli {
//===========================================
namespace params {
}
//===========================================
class Connection : public proto::Connection {
private:
  typedef uint32_t seqence_t;
  typedef uint32_t type_t;
  typedef std::map<seqence_t,int> sent_t;
  bool ready=false;
  bool done=false;
  void * linked=nullptr;
  sent_t sent;
  bool writePduDec (void * src,type_t type,seqence_t seqence,bool outgoing);
  void writePduEnd(seqence_t seqence);
  #define SMPP_ID(tag,number,nameSpace,className,line) void sendReq(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
public:
  Connection();
  virtual ~Connection();
  template<class T> void readBegin(T & p){
    sendReq(p);
    if (linked) if (!(p.getCommandId()&0x80000000ul)) sent[p.getSequenceNumber()]=0;
  }
  template<class T> void readEnd(T & p){
    if (!done) asyncRead();
  }
  #define SMPP_ID(tag,number,nameSpace,className,line) virtual void readPdu(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  virtual void readPduUnknown(proto::pdu::Unknown & p);
  virtual void readPduError(anthill::smpp::SMPPPDU & p,const std::string & what);
  template<class T> void writePduIn(T & p,void * src){
    if (writePduDec(src,p.getCommandId(),p.getSequenceNumber(),false)) {
      writePdu(p);
      writePduEnd(p.getSequenceNumber());
    }
  }
  template<class T> void writePduOut(T & p,void * src){
    if (writePduDec(src,p.getCommandId(),p.getSequenceNumber(),true)) {
      writePdu(p);
      writePduEnd(p.getSequenceNumber());
    }
  }
  virtual void doStart();
  virtual void doStop();
};
//============================================
void factory(boost::asio::ip::tcp::socket & socket);
void factory(boost::asio::local::stream_protocol::socket & socket);
//============================================
}}
//===========================================
#endif