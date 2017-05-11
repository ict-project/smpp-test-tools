//! @file
//! @brief SMPP application (tester) module - Source file.
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
#include "smpp-app.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
//============================================
namespace smpp { namespace app {
//===========================================
namespace params {
  namespace init {
    const static int bindType=Bind_SMSC;
    const static std::string system_id("smpp-test");
    const static std::string password;
    const static std::string system_type;
    const static int interface_version=0x34;
    const static int addr_ton=0;
    const static int addr_npi=0;
    const static std::string addresss_range(".*");
  }
  int bindType(init::bindType);
  std::string system_id(init::system_id);
  std::string password(init::password);
  std::string system_type(init::system_type);
  int interface_version(init::interface_version);
  int addr_ton(init::addr_ton);
  int addr_npi(init::addr_npi);
  std::string addresss_range(init::addresss_range);
}
OPTIONS_CONFIG(smpp_app10,10){
  const static std::wstring _system_id_(L"param-" L"system_id");
  const static std::wstring _password_(L"param-" L"password");
  const static std::wstring _system_type_(L"param-" L"system_type");
  const static std::wstring _interface_version_(L"param-" L"interface_version");
  const static std::wstring _addr_ton_(L"param-" L"addr_ton");
  const static std::wstring _addr_npi_(L"param-" L"addr_npi");
  const static std::wstring _addresss_range_(L"param-" L"addresss_range");
  if (config) {
    parser.registerLongOpt(_system_id_,params::system_id);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_system_id_)<<" - value of 'system_id' param for binding procedure (default value: '"<<params::init::system_id<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_password_,params::password);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_password_)<<" - value of 'password' param for binding procedure (default value: '"<<params::init::password<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_system_type_,params::system_type);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_system_type_)<<" - value of 'system_type' param for binding procedure (default value: '"<<params::init::system_type<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_interface_version_,params::interface_version);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_interface_version_)<<" - value of 'interface_version' param for binding procedure (default value: '"<<params::init::interface_version<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_addr_ton_,params::addr_ton);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_addr_ton_)<<" - value of 'addr_ton' param for binding procedure (default value: '"<<params::init::addr_ton<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_addr_npi_,params::addr_npi);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_addr_npi_)<<" - value of 'addr_npi' param for binding procedure (default value: '"<<params::init::addr_npi<<"')."<<std::endl;
  }
  if (config) {
    parser.registerLongOpt(_addresss_range_,params::addresss_range);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(_addresss_range_)<<" - value of 'addresss_range' param for binding procedure (default value: '"<<params::init::addresss_range<<"')."<<std::endl;
  }
}
//===========================================
std::string randomString(std::size_t len){
  std::string out;
  static const char alphanum[]=
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
  for (std::size_t i=0;i<len;++i) {
    out.append(1,alphanum[::smpp::reg::randomInt<std::size_t>(0,(sizeof(alphanum)-2))]);
  }
  return(out);
}
//===========================================
Connection::Connection()
  :type(params::bindType),state((params::bindType==Bind_SMSC)?Bind_SMSC_OPEN:Bind_ESME_OPEN){
  REGISTER_SMPP_APP.add(this);
}
Connection::~Connection(){
  REGISTER_SMPP_APP.del(this);
}
void Connection::sendTextIn(const std::string & text,seqence_t s,void * origin){
  ::smpp::proto::pdu::Text p;
  p.text=text;
  p.setSequenceNumber(s);
  for (REG_IT_CLI_APP it=REGISTER_CLI_APP.begin();it!=REGISTER_CLI_APP.end();++it){
    it->first->writePduIn(p,origin?origin:this);
  }
}
void Connection::sendTextOut(const std::string & text,seqence_t s,void * origin){
  ::smpp::proto::pdu::Text p;
  p.text=text;
  p.setSequenceNumber(s);
  for (REG_IT_CLI_APP it=REGISTER_CLI_APP.begin();it!=REGISTER_CLI_APP.end();++it){
    it->first->writePduOut(p,origin?origin:this);
  }
}
#define SMPP_ID(tag,number,nameSpace,className,line) \
  void Connection::sendPduIn(nameSpace::className & p,void * origin){ \
    for (REG_IT_CLI_APP it=REGISTER_CLI_APP.begin();it!=REGISTER_CLI_APP.end();++it){ \
      it->first->writePduIn(p,origin?origin:this); \
    } \
  }
  #include "smpp-connection.h.in"
#undef SMPP_ID
#define SMPP_ID(tag,number,nameSpace,className,line) \
  void Connection::sendPduOut(nameSpace::className & p,void * origin){ \
    for (REG_IT_CLI_APP it=REGISTER_CLI_APP.begin();it!=REGISTER_CLI_APP.end();++it){ \
      it->first->writePduOut(p,origin?origin:this); \
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
  {
    anthill::smpp::SMPPBindReceiverResp resp;
    switch (state) {
      case Bind_SMSC_OPEN:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        state=Bind_SMSC_RX;
        break;
      case Bind_SMSC_RX:  case Bind_SMSC_TX: case Bind_SMSC_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RALYBND);
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    resp.setSystemId(p.getSystemId());
    writeResp(resp,p.getSequenceNumber());
  }
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindReceiverResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransmitter & p){
  readBegin(p);
  {
    anthill::smpp::SMPPBindTransmitterResp resp;
    switch (state) {
      case Bind_SMSC_OPEN:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        state=Bind_SMSC_TX;
        break;
      case Bind_SMSC_RX:  case Bind_SMSC_TX: case Bind_SMSC_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RALYBND);
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    resp.setSystemId(p.getSystemId());
    writeResp(resp,p.getSequenceNumber());
  }
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransmitterResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransceiver & p){
  readBegin(p);
  {
    anthill::smpp::SMPPBindTransceiverResp resp;
    switch (state) {
      case Bind_SMSC_OPEN:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        state=Bind_SMSC_TRX;
        break;
      case Bind_SMSC_RX:  case Bind_SMSC_TX: case Bind_SMSC_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RALYBND);
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    resp.setSystemId(p.getSystemId());
    writeResp(resp,p.getSequenceNumber());
  }
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPBindTransceiverResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPUnbind & p){
  readBegin(p);
  {
    anthill::smpp::SMPPUnbindResp resp;
    switch (state) {
      case Bind_SMSC_RX:  case Bind_SMSC_TX: case Bind_SMSC_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        state=Bind_CLOSE;
        doClose();
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    writeResp(resp,p.getSequenceNumber());
  }
}
void Connection::readPdu(anthill::smpp::SMPPUnbindResp & p){
  readBegin(p);
  {
    switch (state) {
      case Bind_ESME_RX:  case Bind_ESME_TX: case Bind_ESME_TRX:
        state=Bind_CLOSE;
        doClose();
        break;
      default:
        break;
    }
  }
}
void Connection::readPdu(anthill::smpp::SMPPOutbind & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPEnquireLink & p){
  readBegin(p);
  {
    anthill::smpp::SMPPEnquireLinkResp resp;
    switch (state) {
      case Bind_SMSC_RX:  case Bind_SMSC_TX: case Bind_SMSC_TRX:
      case Bind_ESME_RX:  case Bind_ESME_TX: case Bind_ESME_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    writeResp(resp,p.getSequenceNumber());
  }
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPEnquireLinkResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitSM & p){
  readBegin(p);
  {
    anthill::smpp::SMPPSubmitSMResp resp;
    switch (state) {
      case Bind_SMSC_TX: case Bind_SMSC_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        resp.setMessageId("smpp-test-"+randomString(50));
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    writeResp(resp,p.getSequenceNumber());
  }
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPSubmitSMResp & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPdu(anthill::smpp::SMPPDeliverSM & p){
  readBegin(p);
  {
    anthill::smpp::SMPPDeliverSMResp resp;
    switch (state) {
      case Bind_ESME_RX: case Bind_ESME_TRX:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_ROK);
        break;
      default:
        resp.setCommandStatus(anthill::smpp::ErrorCode::ESME_RINVBNDSTS);
        break;
    }
    writeResp(resp,p.getSequenceNumber());
  }
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
  readBegin(p);
  {
    writeGenericNack(p.getSequenceNumber(),anthill::smpp::ErrorCode::ESME_RINVCMDID);
  }
  readEnd(p);
}
void Connection::readPduUnknown(proto::pdu::Unknown & p){
  {
    writeGenericNack(p.getSequenceNumber(),anthill::smpp::ErrorCode::ESME_RINVCMDID);
  }
  readEnd(p);
}
void Connection::readPduError(anthill::smpp::SMPPPDU & p,const std::string & what){
  {
    writeGenericNack(p.getSequenceNumber(),anthill::smpp::ErrorCode::ESME_RSYSERR);
  }
  readEnd(p);
}
void Connection::doStart(){
  REGISTER_SMPP_APP.desc(this,"SMPP connection "+socketDesc());
  if (state==Bind_ESME_OPEN) switch (type){
    case Bind_TX:{
      anthill::smpp::SMPPBindTransceiver req;
      req.setSystemId(params::system_id);
      req.setPassword(params::password);
      req.setSystemType(params::system_type);
      req.setInterfaceVersion(params::interface_version);
      req.setAddrTON(params::addr_ton);
      req.setAddrNPI(params::addr_npi);
      req.setAddressRange(params::addresss_range);
      writeReq(req);
    } break;
    case Bind_RX:{
      anthill::smpp::SMPPBindReceiver req;
      req.setSystemId(params::system_id);
      req.setPassword(params::password);
      req.setSystemType(params::system_type);
      req.setInterfaceVersion(params::interface_version);
      req.setAddrTON(params::addr_ton);
      req.setAddrNPI(params::addr_npi);
      req.setAddressRange(params::addresss_range);
      writeReq(req);
    } break;
    case Bind_TRX:{
      anthill::smpp::SMPPBindTransceiver req;
      req.setSystemId(params::system_id);
      req.setPassword(params::password);
      req.setSystemType(params::system_type);
      req.setInterfaceVersion(params::interface_version);
      req.setAddrTON(params::addr_ton);
      req.setAddrNPI(params::addr_npi);
      req.setAddressRange(params::addresss_range);
      writeReq(req);
    } break;
    default:break;
  }
  asyncRead();
  sendTextOut("+++ SMPP connection "+socketDesc()+" has started +++",0,nullptr);
}
void Connection::doStop(){
  switch (state){
    case Bind_ESME_TX:case Bind_ESME_RX:case Bind_ESME_TRX:{
      anthill::smpp::SMPPUnbind req;
      writeReq(req);
    } break;
    default:break;
  }
  sendTextOut("+++ SMPP connection "+socketDesc()+" has stoped +++",0,nullptr);
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