//! @file
//! @brief SMPP application (tester) module - header file.
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
#ifndef _SMPP_APP_HEADER
#define _SMPP_APP_HEADER
//============================================
#include "smpp-connection.hpp"
#include "register.hpp"
#include "daemon-app.hpp"
//============================================
namespace smpp { namespace app {
//===========================================
enum BindType{
  Bind_SMSC=0,
  Bind_TX,
  Bind_RX,
  Bind_TRX
};
enum BindState{
  Bind_SMSC_OPEN=0,
  Bind_SMSC_TX,
  Bind_SMSC_RX,
  Bind_SMSC_TRX,
  Bind_ESME_OPEN,
  Bind_ESME_TX,
  Bind_ESME_RX,
  Bind_ESME_TRX,
  Bind_CLOSE
};
//===========================================
namespace params {
  extern int bindType;
  extern std::string system_id;
  extern std::string password;
  extern std::string system_type;
  extern int interface_version;
  extern int addr_ton;
  extern int addr_npi;
  extern std::string addresss_range;
}
//===========================================
class Connection : public proto::Connection {
private:
  int type;
  BindState state;
  anthill::smpp::SMPPValidator validator;
  typedef uint32_t seqence_t;
  typedef uint32_t error_t;
  seqence_t seqence=0x0;
  seqence_t getSeqence(){
    seqence++;
    if (seqence>0x7fffffff) seqence=0x1;
    return(seqence);
  }
  void sendTextIn(const std::string & text,seqence_t s);
  void sendTextOut(const std::string & text,seqence_t s);
  #define SMPP_ID(tag,number,nameSpace,className,line) void sendPduIn(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  #define SMPP_ID(tag,number,nameSpace,className,line) void sendPduOut(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
public:
  Connection();
  virtual ~Connection();
  template<class T> std::string readBegin(T & p){
    std::string what("OK");
    seqence_t s=p.getSequenceNumber();
    try{
      p.validatePacket(&validator);
    } catch (std::exception& e) {
      what=e.what();
    }
    sendTextIn(">>> Incoming packet on "+socketDesc()+" >>>",s);
    sendPduIn(p);
    sendTextIn(">>> Validator: "+what+" >>>",s);
    return(what);
  }
  template<class T> void readEnd(T & p){
    asyncRead();
  }
  template<class T> void writeReq(T & req){
    req.setSequenceNumber(getSeqence());
    writePdu(req);
    {
      std::string what("OK");
      try{
        req.validatePacket(&validator);
      } catch (std::exception& e) {
        what=e.what();
      }
      seqence_t s=req.getSequenceNumber();
      sendTextOut("<<< Outgoing packet on "+socketDesc()+" <<<",s);
      sendPduOut(req);
      sendTextOut("<<< Validator: "+what+" <<<",s);
    }
  }
  template<class T> void writeResp(T & resp,seqence_t s){
    resp.setSequenceNumber(s);
    writePdu(resp);
    {
      std::string what("OK");
      try{
        resp.validatePacket(&validator);
      } catch (std::exception& e) {
        what=e.what();
      }
      seqence_t s=resp.getSequenceNumber();
      sendTextOut("<<< Outgoing packet on "+socketDesc()+" <<<",s);
      sendPduOut(resp);
      sendTextOut("<<< Validator: "+what+" <<<",s);
    }
  }
  void writeGenericNack(seqence_t s,error_t e){
    anthill::smpp::SMPPGenericNack resp;
    resp.setCommandStatus(e);
    writeResp(resp,s);
  }
  #define SMPP_ID(tag,number,nameSpace,className,line) virtual void readPdu(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  virtual void readPduUnknown(proto::pdu::Unknown & p);
  virtual void readPduError(anthill::smpp::SMPPPDU & p,const std::string & what);
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