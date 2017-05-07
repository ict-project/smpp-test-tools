//! @file
//! @brief User CLI (tester) module - header file.
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
#ifndef _SMPP_U_CLI_HEADER
#define _SMPP_U_CLI_HEADER
//============================================
#include "smpp-connection.hpp"
#include "register.hpp"
//============================================
namespace smpp { namespace cli {
//===========================================
namespace params {
  extern std::size_t tail;
  extern std::size_t list;
  extern int type;
  extern int number;
#define SMPP_PARAM(name,type,value) extern type name;
  #include "user-app.h.in"
#undef SMPP_PARAM
}
//===========================================
class Connection : public proto::Connection {
private:
  anthill::smpp::SMPPHexOutputter hexOutputter;
  anthill::smpp::SMPPTxtOutputter txtOutputter;
public:
  Connection();
  virtual ~Connection();
  template<class T> void readBegin(T & p){
    ict::time::TimeInfo ti;
    std::cout<<"HEX DUMP:"<<std::endl;
    std::cout<<std::setfill(' ')<< std::setw(0)<<std::left;
    p.outputPacket(&hexOutputter);
    std::cout<<std::setfill(' ')<< std::setw(0)<<std::left<<std::endl;
    std::cout<<"DECODED DUMP:"<<std::endl;
    p.outputPacket(&txtOutputter);
    std::cout<<std::setfill(' ')<< std::setw(0)<<std::left;
    std::cout<<"TIME STAMP: "<<ti<<std::endl;
  }
  template<class T> void readEnd(T & p){
    asyncRead();
  }
  #define SMPP_ID(tag,number,nameSpace,className,line) virtual void readPdu(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  virtual void readPduUnknown(proto::pdu::Unknown & p);
  virtual void readPduError(anthill::smpp::SMPPPDU & p,const std::string & what);
  virtual void doStart();
  virtual void doStop();
  void sendSMPPAlertNotification();
  void sendSMPPCancelSM();
  void sendSMPPDeliverSM();
  void sendSMPPQuerySM();
  void sendSMPPReplaceSM();
  void sendSMPPSubmitSM();
  void sendSMPPEnquireLink();
};
//============================================
void factory(boost::asio::ip::tcp::socket & socket);
void factory(boost::asio::local::stream_protocol::socket & socket);
//============================================
}}
//===========================================
#endif