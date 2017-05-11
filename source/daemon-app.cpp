//! @file
//! @brief Daemon CLI (tester) module - Source file.
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
#include "daemon-app.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
//============================================
namespace smpp { namespace cli {
//===========================================
namespace params {
}
//===========================================
Connection::Connection(){
  REGISTER_CLI_APP.add(this,"Daemon CLI connection "+socketDesc());
}
Connection::~Connection(){
  REGISTER_CLI_APP.del(this);
}
bool Connection::writePduDec (void * src,type_t type,seqence_t seqence,bool outgoing){
  if (!ready) return(false);
  if (linked){//Jeśl powiązany tylko z jednym połączeniem.
    if (linked==src){//Jeśli to jest właśnie to połączenie.
      if (outgoing){//Jeśli jest to PDU wychodzace.
        if (!(type&0x80000000ul)) return(true);//Jeśli jest request.
        if (type==0x0f000000ul) return(true);//Jeśli jest to text.
      } else {//Jeśli jest to PDU wychodzace.
        if (sent.count(seqence)){//Jeśli numer sekwencji się zgadza.
          if (type&0x80000000ul) return(true);//Jeśli jest to response.
          if (type==0x0f000000ul) return(true);//Jeśli jest to text.
        }
      }
    }
    return(false);
  }
  return(true);
}
void Connection::writePduEnd(seqence_t seqence){
  if (linked){//Jeśl powiązany tylko z jednym połączeniem.
    done=true;
    if (sent.count(seqence)) sent[seqence]++;
    for(sent_t::const_iterator it=sent.cbegin();it!=sent.cend();++it){
      if (it->second<2) done=false;
    }
  }
  if (done) shutdownRead();
}
#define SMPP_ID(tag,number,nameSpace,className,line) \
  void Connection::sendReq(nameSpace::className & p){ \
    if (linked){ \
      smpp::app::Connection * c=REGISTER_SMPP_APP.get(linked); \
      if (c) c->writeReq(p); \
    } \
  }
  #include "smpp-connection.h.in"
#undef SMPP_ID
void Connection::readPdu(anthill::smpp::SMPPGenericNack & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindReceiver & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindReceiverResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransmitter & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransmitterResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransceiver & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransceiverResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPUnbind & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPUnbindResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPOutbind & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPEnquireLink & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPEnquireLinkResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPDeliverSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPDeliverSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPQuerySM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPQuerySMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPReplaceSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPReplaceSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPCancelSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPCancelSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitMulti & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitMultiResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPDataSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPDataSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBroadcastSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBroadcastSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPQueryBroadcastSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPQueryBroadcastSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPCancelBroadcastSM & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPCancelBroadcastSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPAlertNotification & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(::smpp::proto::pdu::Text & p){
  LOGGER_DEBUG<<__LOGGER__<<"::smpp::proto::pdu::Text.text="<<p.text<<std::endl;
  if (p.text=="list") {
    smpp::proto::pdu::Text text;
    text.text="+++ Connection list +++";
    writePdu(text);
    for (REG_IT_SMPP_APP it=REGISTER_SMPP_APP.begin();it!=REGISTER_SMPP_APP.end();++it){
      text.text=it->second;
      writePdu(text);
    }
    text.text="+++ +++";
    writePdu(text);
  } else if (p.text=="send") {
    linked=REGISTER_SMPP_APP.rand();
    LOGGER_DEBUG<<__LOGGER__<<"linked="<<linked<<std::endl;
    ready=true;
    readEnd(p);
  } else {
    ready=true;
    readEnd(p);
  }
}
void Connection::readPduUnknown(proto::pdu::Unknown & p){
  readEnd(p);
}
void Connection::readPduError(anthill::smpp::SMPPPDU & p,const std::string & what){
  readEnd(p);
}
void Connection::doStart(){
  asyncRead();
}
void Connection::doStop(){
}
//============================================
void factory(boost::asio::ip::tcp::socket & socket){
  auto ptr=std::make_shared<smpp::connection::Connection<boost::asio::ip::tcp::socket,Connection>>(socket);
  if (ptr) ptr->init();
}
void factory(boost::asio::local::stream_protocol::socket & socket){
  auto ptr=std::make_shared<smpp::connection::Connection<boost::asio::local::stream_protocol::socket,Connection>>(socket);
  if (ptr) ptr->init();
}
//============================================
}}
//============================================