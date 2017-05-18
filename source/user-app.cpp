//! @file
//! @brief User CLI (tester) module - Source file.
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
#include "user-app.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
#include "main-function.hpp"
//============================================
#define __WIDEN2(x) L ## x
#define __WIDEN(x) __WIDEN2(x)
//============================================
namespace smpp { namespace cli {
//===========================================
namespace params {
  namespace init {
    const static int type(8);
    const static int number(1);
#define SMPP_PARAM(name,type,value) const static type name(value);
  #include "user-app.h.in"
#undef SMPP_PARAM
  }
  std::size_t tail(0);
  std::size_t list(0);
  int type(init::type);
  int number(init::number);
#define SMPP_PARAM(name,type,value) type name(init::name);
  #include "user-app.h.in"
#undef SMPP_PARAM
}
std::map<std::wstring,int> typeDictionary={
  #define SMPP_ID(tag,number,nameSpace,className,line) {__WIDEN(#tag),line},
    #include "smpp-connection.h.in"
  #undef SMPP_ID
};
OPTIONS_CONFIG(smpp_params10,10){ 
  #define SMPP_PARAM(name,type,value) \
    const static std::wstring _##name##_(L"param-" __WIDEN2(#name));
    #include "user-app.h.in"
  #undef SMPP_PARAM
  if (config) {
    parser.registerOptNoValue(L's',L"list",params::list);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L's')<<" - List mode. If set - no SMPP command is sent, smpp-test-cli just dumps list of connections in smpp-test-client or smpp-test-server."<<std::endl;
  }
  if (config) {
    parser.registerOptNoValue(L't',L"tail",params::tail);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L't')<<" - Tail mode. If set - no SMPP command is sent, smpp-test-cli just dumps PDUs from smpp-test-client or smpp-test-server - only if list mode is not set."<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'i',L"id",params::type,typeDictionary);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'i')<<" - ID of SMPP command that should be sent (default value: 'SUBMIT_SM') - only if list or tail mode is not set."<<std::endl;
    parser.errors<<"  SMPP commands: 'SUBMIT_SM', 'QUERY_SM', 'DELIVER_SM', 'REPLACE_SM', 'CANCEL_SM', 'ENQUIRE_LINK', 'ALERT_NOTIFICATION'."<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'n',L"number",params::number);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'n')<<" - how many times SMPP PDU should be sent (default value: '"<<params::init::number<<"') - only if list or tail mode is not set."<<std::endl;
  }
  #define SMPP_PARAM(name,type,value) \
    if (config) { \
      parser.registerLongOpt(_##name##_,params::name); \
    } else { \
      parser.errors<<" "<<parser.getOptionDesc(_##name##_)<<" - value of '" #name "' SMPP param  (default value: '"<<params::init::name<<"')."<<std::endl; \
    }
    #include "user-app.h.in"
  #undef SMPP_PARAM
}
//===========================================
Connection::Connection()
  :hexOutputter(std::cout),txtOutputter(std::cout){
  REGISTER_CLI_APP.add(this);
}
Connection::~Connection(){
  REGISTER_CLI_APP.del(this);
  smpp::main::ioService().stop();
}
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
  std::cout<<p.text<<std::endl;
  readEnd(p);
}
void Connection::readPduUnknown(proto::pdu::Unknown & p){
  readBegin(p);
  readEnd(p);
}
void Connection::readPduError(anthill::smpp::SMPPPDU & p,const std::string & what){
  readEnd(p);
}
void Connection::doStart(){
  smpp::proto::pdu::Text p;
  REGISTER_CLI_APP.desc(this,"User CLI connection "+socketDesc());
  p.text=params::tail?"tail":"send";
  if (params::list) p.text="list";
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU text: "<<p.text<<std::endl;
  if (p.text=="send") for (int k=0;k<params::number;k++){
    switch (params::type){
      case 6:  sendSMPPQuerySM();break;
      case 8:  default:sendSMPPSubmitSM();break;
      case 10: sendSMPPDeliverSM();break;
      case 14: sendSMPPReplaceSM();break;
      case 16: sendSMPPCancelSM();break;
      case 21: sendSMPPEnquireLink();break;
      case 25: sendSMPPAlertNotification();break;
    }
  }
  writePdu(p);
  asyncRead();
}
void Connection::doStop(){
}
void Connection::sendSMPPAlertNotification(){
  anthill::smpp::SMPPAlertNotification p;
  //p.setPresentMSAvailabilityStatus(bool);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  p.setESMEAddrTON(params::esme_addr_ton);
  p.setESMEAddrNPI(params::esme_addr_npi);
  p.setESMEAddr(params::esme_addr);
  if (OPTIONS_PRESENT(L"param-ms_availability_status")) p.setMSAvailabilityStatus(params::ms_availability_status);
  //p.setMSAvailabilityStatusLen(const unsigned short);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
/*
SMPPBind.hpp:                   inline void setSystemId(const std::string&);
SMPPBind.hpp:                   inline void setPassword(const std::string&);
SMPPBind.hpp:                   inline void setSystemType(const std::string&);
SMPPBind.hpp:                   inline void setInterfaceVersion(const unsigned char);
SMPPBind.hpp:                   inline void setAddrTON(const unsigned char);
SMPPBind.hpp:                   inline void setAddrNPI(const unsigned char);
SMPPBind.hpp:                   inline void setAddressRange(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastAreaIdentifier(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastContentType(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastRepNum(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastFrequencyInterval(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentAlertOnMsgDelivery(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastChannelIndicator(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastContentTypeInfo(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastMessageClass(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentBroadcastServiceGroup(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentCallbackNum(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentCallbackNumAtag(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentCallbackNumPresInd(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentDestAddrSubunit(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentDestSubaddress(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentDestPort(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentDisplayTime(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentLanguageIndicator(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentMessagePayload(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentMsValidity(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentPayloadType(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentPrivacyIndicator(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentSmsSignal(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentSourceAddrSubunit(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentSourcePort(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentSourceSubaddress(const bool);
SMPPBroadcastSM.hpp:                    inline void setPresentUserMessageReference(const bool);
SMPPBroadcastSM.hpp:                    inline void setServiceType(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setSourceAddrTON(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setSourceAddrNPI(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setSourceAddr(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setMessageId(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setPriorityFlag(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setScheduleDeliveryTime(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setValidityPeriod(const std::string&);
SMPPBroadcastSM.hpp:                    inline void setReplaceIfPresentFlag(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setDataCoding(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setSMDefaultMsgId(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setBroadcastAreaIdentifier(const std::vector<BroadcastAreaIdentifier>&);
SMPPBroadcastSM.hpp:                    inline void setBroadcastContentType(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setBroadcastRepNum(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastFrequencyInterval(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setAlertOnMsgDelivery(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setBroadcastChannelIndicator(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setBroadcastContentTypeInfo(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setBroadcastMessageClass(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setBroadcastServiceGroup(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setCallbackNum(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setCallbackNumAtag(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setCallbackNumPresInd(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setDestAddrSubunit(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setDestSubaddress(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setDestPort(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setDisplayTime(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setLanguageIndicator(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setMessagePayload(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setMsValidity(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setPayloadType(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setPrivacyIndicator(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setSmsSignal(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSourceAddrSubunit(const unsigned char);
SMPPBroadcastSM.hpp:                    inline void setSourcePort(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSourceSubaddress(const std::vector<unsigned char>&);
SMPPBroadcastSM.hpp:                    inline void setUserMessageReference(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastAreaIdentifierLen(const std::vector<unsigned short>&);
SMPPBroadcastSM.hpp:                    inline void setBroadcastContentTypeLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastRepNumLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastFrequencyIntervalLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setAlertOnMsgDeliveryLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastChannelIndicatorLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastContentTypeInfoLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastMessageClassLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setBroadcastServiceGroupLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setCallbackNumLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setCallbackNumAtagLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setCallbackNumPresIndLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setDestAddrSubunitLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setDestSubaddressLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setDestPortLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setDisplayTimeLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setLanguageIndicatorLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setMessagePayloadLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setMsValidityLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setPayloadTypeLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setPrivacyIndicatorLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSmsSignalLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSourceAddrSubunitLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSourcePortLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setSourceSubaddressLen(const unsigned short);
SMPPBroadcastSM.hpp:                    inline void setUserMessageReferenceLen(const unsigned short);
SMPPCancelBroadcastSM.hpp:                      inline void setPresentBroadcastContentType(const bool);
SMPPCancelBroadcastSM.hpp:                      inline void setPresentUserMessageReference(const bool);
SMPPCancelBroadcastSM.hpp:                      inline void setServiceType(const std::string&);
SMPPCancelBroadcastSM.hpp:                      inline void setMessageId(const std::string&);
SMPPCancelBroadcastSM.hpp:                      inline void setSourceAddrTON(const unsigned char);
SMPPCancelBroadcastSM.hpp:                      inline void setSourceAddrNPI(const unsigned char);
SMPPCancelBroadcastSM.hpp:                      inline void setSourceAddr(const std::string&);
SMPPCancelBroadcastSM.hpp:                      inline void setBroadcastContentType(const std::vector<unsigned char>&);
SMPPCancelBroadcastSM.hpp:                      inline void setUserMessageReference(const unsigned short);
SMPPCancelBroadcastSM.hpp:                      inline void setBroadcastContentTypeLen(const unsigned short);
SMPPCancelBroadcastSM.hpp:                      inline void setUserMessageReferenceLen(const unsigned short);
*/
void Connection::sendSMPPCancelSM(){
  anthill::smpp::SMPPCancelSM p;
  p.setServiceType(params::service_type);
  p.setMessageId(params::message_id);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  p.setDestAddrTON(params::dest_addr_ton);
  p.setDestAddrNPI(params::dest_addr_npi);
  p.setDestAddr(params::destination_addr);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
/*
SMPPDataSM.hpp:                 inline void setPresentSourcePort(const bool);
SMPPDataSM.hpp:                 inline void setPresentSourceAddrSubunit(const bool);
SMPPDataSM.hpp:                 inline void setPresentSourceNetworkType(const bool);
SMPPDataSM.hpp:                 inline void setPresentSourceBearerType(const bool);
SMPPDataSM.hpp:                 inline void setPresentSourceTelematicsId(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestinationPort(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestAddrSubunit(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestNetworkType(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestBearerType(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestTelematicsId(const bool);
SMPPDataSM.hpp:                 inline void setPresentSarMsgRefNum(const bool);
SMPPDataSM.hpp:                 inline void setPresentSarTotalSegments(const bool);
SMPPDataSM.hpp:                 inline void setPresentSarSegmentSeqnum(const bool);
SMPPDataSM.hpp:                 inline void setPresentMoreMessagesToSend(const bool);
SMPPDataSM.hpp:                 inline void setPresentQosTimeToLive(const bool);
SMPPDataSM.hpp:                 inline void setPresentPayloadType(const bool);
SMPPDataSM.hpp:                 inline void setPresentMessagePayload(const bool);
SMPPDataSM.hpp:                 inline void setPresentSetDpf(const bool);
SMPPDataSM.hpp:                 inline void setPresentReceiptedMessageId(const bool);
SMPPDataSM.hpp:                 inline void setPresentMessageState(const bool);
SMPPDataSM.hpp:                 inline void setPresentNetworkErrorCode(const bool);
SMPPDataSM.hpp:                 inline void setPresentUserMessageReference(const bool);
SMPPDataSM.hpp:                 inline void setPresentPrivacyIndicator(const bool);
SMPPDataSM.hpp:                 inline void setPresentCallbackNum(const bool);
SMPPDataSM.hpp:                 inline void setPresentCallbackNumPresInd(const bool);
SMPPDataSM.hpp:                 inline void setPresentCallbackNumAtag(const bool);
SMPPDataSM.hpp:                 inline void setPresentSourceSubaddress(const bool);
SMPPDataSM.hpp:                 inline void setPresentDestSubaddress(const bool);
SMPPDataSM.hpp:                 inline void setPresentDisplayTime(const bool);
SMPPDataSM.hpp:                 inline void setPresentSmsSignal(const bool);
SMPPDataSM.hpp:                 inline void setPresentMsValidity(const bool);
SMPPDataSM.hpp:                 inline void setPresentMsMsgWaitFacilities(const bool);
SMPPDataSM.hpp:                 inline void setPresentNumberOfMessages(const bool);
SMPPDataSM.hpp:                 inline void setPresentAlertOnMsgDelivery(const bool);
SMPPDataSM.hpp:                 inline void setPresentLanguageIndicator(const bool);
SMPPDataSM.hpp:                 inline void setPresentItsReplyType(const bool);
SMPPDataSM.hpp:                 inline void setPresentItsSessionInfo(const bool);
SMPPDataSM.hpp:                 inline void setServiceType(const std::string&);
SMPPDataSM.hpp:                 inline void setSourceAddrTON(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourceAddrNPI(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourceAddr(const std::string&);
SMPPDataSM.hpp:                 inline void setDestAddrTON(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestAddrNPI(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestAddr(const std::string&);
SMPPDataSM.hpp:                 inline void setEsmClass(const unsigned char);
SMPPDataSM.hpp:                 inline void setRegisteredDelivery(const unsigned char);
SMPPDataSM.hpp:                 inline void setDataCoding(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourcePort(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceAddrSubunit(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourceNetworkType(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourceBearerType(const unsigned char);
SMPPDataSM.hpp:                 inline void setSourceTelematicsId(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestinationPort(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestAddrSubunit(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestNetworkType(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestBearerType(const unsigned char);
SMPPDataSM.hpp:                 inline void setDestTelematicsId(const unsigned char);
SMPPDataSM.hpp:                 inline void setSarMsgRefNum(const unsigned short);
SMPPDataSM.hpp:                 inline void setSarTotalSegments(const unsigned char);
SMPPDataSM.hpp:                 inline void setSarSegmentSeqnum(const unsigned char);
SMPPDataSM.hpp:                 inline void setMoreMessagesToSend(const unsigned char);
SMPPDataSM.hpp:                 inline void setQosTimeToLive(const unsigned);
SMPPDataSM.hpp:                 inline void setPayloadType(const unsigned char);
SMPPDataSM.hpp:                 inline void setMessagePayload(const std::vector<unsigned char>&);
SMPPDataSM.hpp:                 inline void setSetDpf(const unsigned char);
SMPPDataSM.hpp:                 inline void setReceiptedMessageId(const std::string&);
SMPPDataSM.hpp:                 inline void setReceiptedMessageId(const unsigned);
SMPPDataSM.hpp:                 inline void setMessageState(const unsigned char);
SMPPDataSM.hpp:                 inline void setNetworkErrorCode(const unsigned char nNetworkCode, const unsigned short nErrorCode);
SMPPDataSM.hpp:                 inline void setNetworkErrorCode(const NetworkErrorCode&);
SMPPDataSM.hpp:                 inline void setUserMessageReference(const unsigned short);
SMPPDataSM.hpp:                 inline void setPrivacyIndicator(const unsigned char);
SMPPDataSM.hpp:                 inline void setCallbackNum(const std::vector<unsigned char>&);
SMPPDataSM.hpp:                 inline void setCallbackNumPresInd(const unsigned char);
SMPPDataSM.hpp:                 inline void setCallbackNumAtag(const std::vector<unsigned char>&);
SMPPDataSM.hpp:                 inline void setSourceSubaddress(const std::vector<unsigned char>&);
SMPPDataSM.hpp:                 inline void setDestSubaddress(const std::vector<unsigned char>&);
SMPPDataSM.hpp:                 inline void setDisplayTime(const unsigned char);
SMPPDataSM.hpp:                 inline void setSmsSignal(const unsigned short);
SMPPDataSM.hpp:                 inline void setMsValidity(const unsigned char);
SMPPDataSM.hpp:                 inline void setMsMsgWaitFacilities(const unsigned char);
SMPPDataSM.hpp:                 inline void setNumberOfMessages(const unsigned char);
SMPPDataSM.hpp:                 inline void setAlertOnMsgDelivery(const unsigned char);
SMPPDataSM.hpp:                 inline void setLanguageIndicator(const unsigned char);
SMPPDataSM.hpp:                 inline void setItsReplyType(const unsigned char);
SMPPDataSM.hpp:                 inline void setItsSessionInfo(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourcePortLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceAddrSubunitLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceNetworkTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceBearerTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceTelematicsIdLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestinationPortLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestAddrSubunitLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestNetworkTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestBearerTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestTelematicsIdLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSarMsgRefNumLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSarTotalSegmentsLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSarSegmentSeqnumLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setMoreMessagesToSendLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setQosTimeToLiveLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setPayloadTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setMessagePayloadLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSetDpfLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setReceiptedMessageIdLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setMessageStateLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setNetworkErrorCodeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setUserMessageReferenceLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setPrivacyIndicatorLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setCallbackNumLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setCallbackNumPresIndLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setCallbackNumAtagLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSourceSubaddressLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDestSubaddressLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setDisplayTimeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setSmsSignalLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setMsValidityLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setMsMsgWaitFacilitiesLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setNumberOfMessagesLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setAlertOnMsgDeliveryLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setLanguageIndicatorLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setItsReplyTypeLen(const unsigned short);
SMPPDataSM.hpp:                 inline void setItsSessionInfoLen(const unsigned short);
*/
void Connection::sendSMPPDeliverSM(){
  anthill::smpp::SMPPDeliverSM p;
  //p.setPresentUserMessageReference(const bool);
  //p.setPresentSourcePort(const bool);
  //p.setPresentDestinationPort(const bool);
  //p.setPresentSarMsgRefNum(const bool);
  //p.setPresentSarTotalSegments(const bool);
  //p.setPresentSarSegmentSeqnum(const bool);
  //p.setPresentPrivacyIndicator(const bool);
  //p.setPresentPayloadType(const bool);
  //p.setPresentMessagePayload(const bool);
  //p.setPresentCallbackNum(const bool);
  //p.setPresentSourceSubaddress(const bool);
  //p.setPresentDestSubaddress(const bool);
  //p.setPresentLanguageIndicator(const bool);
  //p.setPresentItsSessionInfo(const bool);
  //p.setPresentNetworkErrorCode(const bool);
  //p.setPresentMessageState(const bool);
  //p.setPresentReceiptedMessageId(const bool);
  p.setServiceType(params::service_type);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  p.setDestAddrTON(params::dest_addr_ton);
  p.setDestAddrNPI(params::dest_addr_npi);
  p.setDestAddr(params::destination_addr);
  p.setEsmClass(params::esm_class);
  p.setProtocolId(params::protocol_id);
  p.setPriorityFlag(params::priority_flag);
  p.setScheduleDeliveryTime(params::schedule_delivery_time);
  p.setValidityPeriod(params::validity_period);
  p.setRegisteredDelivery(params::registered_delivery);
  p.setReplaceIfPresentFlag(params::replace_if_present_flag);
  p.setDataCoding(params::data_coding);
  p.setSMDefaultMsgId(params::sm_default_msg_id);
  p.setSMLength(params::sm_length);
  p.setShortMessage(params::short_message);
  if (OPTIONS_PRESENT(L"param-user_message_reference")) p.setUserMessageReference(params::user_message_reference);
  if (OPTIONS_PRESENT(L"param-source_port")) p.setSourcePort(params::source_port);
  if (OPTIONS_PRESENT(L"param-destination_port")) p.setDestinationPort(params::destination_port);
  if (OPTIONS_PRESENT(L"param-sar_msg_ref_num")) p.setSarMsgRefNum(params::sar_msg_ref_num);
  if (OPTIONS_PRESENT(L"param-sar_total_segments")) p.setSarTotalSegments(params::sar_total_segments);
  if (OPTIONS_PRESENT(L"param-sar_segment_seqnum")) p.setSarSegmentSeqnum(params::sar_segment_seqnum);
  if (OPTIONS_PRESENT(L"param-privacy_indicator")) p.setPrivacyIndicator(params::privacy_indicator);
  if (OPTIONS_PRESENT(L"param-payload_type")) p.setPayloadType(params::payload_type);
  if (OPTIONS_PRESENT(L"param-message_payload")) p.setMessagePayload(std::vector<unsigned char>(params::message_payload.cbegin(),params::message_payload.cend()));
  if (OPTIONS_PRESENT(L"param-callback_num")) p.setCallbackNum(std::vector<unsigned char>(params::callback_num.cbegin(),params::callback_num.cend()));
  if (OPTIONS_PRESENT(L"param-source_subaddress")) p.setSourceSubaddress(std::vector<unsigned char>(params::source_subaddress.cbegin(),params::source_subaddress.cend()));
  if (OPTIONS_PRESENT(L"param-dest_subaddress")) p.setDestSubaddress(std::vector<unsigned char>(params::dest_subaddress.cbegin(),params::dest_subaddress.cend()));
  if (OPTIONS_PRESENT(L"param-language_indicator")) p.setLanguageIndicator(params::language_indicator);
  if (OPTIONS_PRESENT(L"param-its_session_info")) p.setItsSessionInfo(params::its_session_info);
  //p.setNetworkErrorCode(const unsigned char nNetworkCode, const unsigned short nErrorCode);
  //p.setNetworkErrorCode(const NetworkErrorCode&);
  if (OPTIONS_PRESENT(L"param-message_state")) p.setMessageState(params::message_state);
  if (OPTIONS_PRESENT(L"param-receipted_message_id")) p.setReceiptedMessageId(params::receipted_message_id);
  //p.setReceiptedMessageId(const unsigned);
  //p.setUserMessageReferenceLen(const unsigned short);
  //p.setSourcePortLen(const unsigned short);
  //p.setDestinationPortLen(const unsigned short);
  //p.setSarMsgRefNumLen(const unsigned short);
  //p.setSarTotalSegmentsLen(const unsigned short);
  //p.setSarSegmentSeqnumLen(const unsigned short);
  //p.setPrivacyIndicatorLen(const unsigned short);
  //p.setPayloadTypeLen(const unsigned short);
  //p.setMessagePayloadLen(const unsigned short);
  //p.setCallbackNumLen(const unsigned short);
  //p.setSourceSubaddressLen(const unsigned short);
  //p.setDestSubaddressLen(const unsigned short);
  //p.setLanguageIndicatorLen(const unsigned short);
  //p.setItsSessionInfoLen(const unsigned short);
  //p.setNetworkErrorCodeLen(const unsigned short);
  //p.setMessageStateLen(const unsigned short);
  //p.setReceiptedMessageIdLen(const unsigned short);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
/*
SMPPOutbind.hpp:                        inline void setSystemId(const std::string&);
SMPPOutbind.hpp:                        inline void setPassword(const std::string&);
SMPPQueryBroadcastSM.hpp:                       inline void setPresentUserMessageReference(const bool);
SMPPQueryBroadcastSM.hpp:                       inline void setMessageId(const std::string&);
SMPPQueryBroadcastSM.hpp:                       inline void setSourceAddrTON(const unsigned char);
SMPPQueryBroadcastSM.hpp:                       inline void setSourceAddrNPI(const unsigned char);
SMPPQueryBroadcastSM.hpp:                       inline void setSourceAddr(const std::string&);
SMPPQueryBroadcastSM.hpp:                       inline void setUserMessageReference(const unsigned short);
SMPPQueryBroadcastSM.hpp:                       inline void setUserMessageReferenceLen(const unsigned short);
*/
void Connection::sendSMPPQuerySM(){
  anthill::smpp::SMPPQuerySM p;
  p.setMessageId(params::message_id);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
void Connection::sendSMPPReplaceSM(){
  anthill::smpp::SMPPReplaceSM p;
  //p.setPresentMessagePayload(const bool);
  p.setMessageId(params::message_id);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  p.setScheduleDeliveryTime(params::schedule_delivery_time);
  p.setValidityPeriod(params::validity_period);
  p.setRegisteredDelivery(params::registered_delivery);
  p.setSMDefaultMsgId(params::sm_default_msg_id);
  p.setSMLength(params::sm_length);
  p.setShortMessage(params::short_message);
  p.setMessagePayload(std::vector<unsigned char>(params::message_payload.cbegin(),params::message_payload.cend()));
  //p.setMessagePayloadLen(const unsigned short);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
/*
SMPPSubmitMulti.hpp:                    inline void setPresentUserMessageReference(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSourcePort(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSourceAddrSubunit(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentDestinationPort(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentDestAddrSubunit(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSarMsgRefNum(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSarTotalSegments(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSarSegmentSeqnum(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentPayloadType(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentMessagePayload(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentPrivacyIndicator(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentCallbackNum(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentCallbackNumPresInd(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentCallbackNumAtag(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSourceSubaddress(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentDestSubaddress(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentDisplayTime(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentSmsSignal(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentMsValidity(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentMsMsgWaitFacilities(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentAlertOnMsgDelivery(const bool);
SMPPSubmitMulti.hpp:                    inline void setPresentLanguageIndicator(const bool);
SMPPSubmitMulti.hpp:                    inline void setServiceType(const std::string&);
SMPPSubmitMulti.hpp:                    inline void setSourceAddrTON(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSourceAddrNPI(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSourceAddr(const std::string&);
SMPPSubmitMulti.hpp:                    inline void setNumberOfDest(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setDestAddresses(const std::vector<DestAddress>&);
SMPPSubmitMulti.hpp:                    inline void setEsmClass(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setProtocolId(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setPriorityFlag(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setScheduleDeliveryTime(const std::string&);
SMPPSubmitMulti.hpp:                    inline void setValidityPeriod(const std::string&);
SMPPSubmitMulti.hpp:                    inline void setRegisteredDelivery(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setReplaceIfPresentFlag(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setDataCoding(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSMDefaultMsgId(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSMLength(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setShortMessage(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setShortMessage(const std::string&);
SMPPSubmitMulti.hpp:                    inline void setUserMessageReference(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSourcePort(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSourceAddrSubunit(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setDestinationPort(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setDestAddrSubunit(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSarMsgRefNum(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSarTotalSegments(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSarSegmentSeqnum(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setPayloadType(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setMessagePayload(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setPrivacyIndicator(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setCallbackNum(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setCallbackNumPresInd(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setCallbackNumAtag(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setSourceSubaddress(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setDestSubaddress(const std::vector<unsigned char>&);
SMPPSubmitMulti.hpp:                    inline void setDisplayTime(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setSmsSignal(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setMsValidity(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setMsMsgWaitFacilities(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setAlertOnMsgDelivery(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setLanguageIndicator(const unsigned char);
SMPPSubmitMulti.hpp:                    inline void setUserMessageReferenceLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSourcePortLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSourceAddrSubunitLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setDestinationPortLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setDestAddrSubunitLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSarMsgRefNumLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSarTotalSegmentsLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSarSegmentSeqnumLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setPayloadTypeLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setMessagePayloadLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setPrivacyIndicatorLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setCallbackNumLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setCallbackNumPresIndLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setCallbackNumAtagLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSourceSubaddressLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setDestSubaddressLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setDisplayTimeLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setSmsSignalLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setMsValidityLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setMsMsgWaitFacilitiesLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setAlertOnMsgDeliveryLen(const unsigned short);
SMPPSubmitMulti.hpp:                    inline void setLanguageIndicatorLen(const unsigned short);
*/
void Connection::sendSMPPSubmitSM(){
  anthill::smpp::SMPPSubmitSM p;
  //p.setPresentUserMessageReference(const bool);
  //p.setPresentSourcePort(const bool);
  //p.setPresentSourceAddrSubunit(const bool);
  //p.setPresentDestinationPort(const bool);
  //p.setPresentDestAddrSubunit(const bool);
  //p.setPresentSarMsgRefNum(const bool);
  //p.setPresentSarTotalSegments(const bool);
  //p.setPresentSarSegmentSeqnum(const bool);
  //p.setPresentMoreMessagesToSend(const bool);
  //p.setPresentPayloadType(const bool);
  //p.setPresentMessagePayload(const bool);
  //p.setPresentPrivacyIndicator(const bool);
  //p.setPresentCallbackNum(const bool);
  //p.setPresentCallbackNumPresInd(const bool);
  //p.setPresentCallbackNumAtag(const bool);
  //p.setPresentSourceSubaddress(const bool);
  //p.setPresentDestSubaddress(const bool);
  //p.setPresentDisplayTime(const bool);
  //p.setPresentSmsSignal(const bool);
  //p.setPresentMsValidity(const bool);
  //p.setPresentMsMsgWaitFacilities(const bool);
  //p.setPresentNumberOfMessages(const bool);
  //p.setPresentAlertOnMsgDelivery(const bool);
  //p.setPresentLanguageIndicator(const bool);
  //p.setPresentItsReplyType(const bool);
  //p.setPresentItsSessionInfo(const bool);
  //p.setPresentUssdServiceOp(const bool);
  p.setServiceType(params::service_type);
  p.setSourceAddrTON(params::source_addr_ton);
  p.setSourceAddrNPI(params::source_addr_npi);
  p.setSourceAddr(params::source_addr);
  p.setDestAddrTON(params::dest_addr_ton);
  p.setDestAddrNPI(params::dest_addr_npi);
  p.setDestAddr(params::destination_addr);
  p.setEsmClass(params::esm_class);
  p.setProtocolId(params::protocol_id);
  p.setPriorityFlag(params::priority_flag);
  p.setScheduleDeliveryTime(params::schedule_delivery_time);
  p.setValidityPeriod(params::validity_period);
  p.setRegisteredDelivery(params::registered_delivery);
  p.setReplaceIfPresentFlag(params::replace_if_present_flag);
  p.setDataCoding(params::data_coding);
  p.setSMDefaultMsgId(params::sm_default_msg_id);
  p.setShortMessage(params::short_message);
  if (OPTIONS_PRESENT(L"param-user_message_reference")) p.setUserMessageReference(params::user_message_reference);
  if (OPTIONS_PRESENT(L"param-source_port")) p.setSourcePort(params::source_port);
  if (OPTIONS_PRESENT(L"param-source_addr_subunit")) p.setSourceAddrSubunit(params::source_addr_subunit);
  if (OPTIONS_PRESENT(L"param-destination_port")) p.setDestinationPort(params::destination_port);
  if (OPTIONS_PRESENT(L"param-dest_addr_subunit")) p.setDestAddrSubunit(params::dest_addr_subunit);
  if (OPTIONS_PRESENT(L"param-sar_msg_ref_num")) p.setSarMsgRefNum(params::sar_msg_ref_num);
  if (OPTIONS_PRESENT(L"param-sar_total_segments")) p.setSarTotalSegments(params::sar_total_segments);
  if (OPTIONS_PRESENT(L"param-sar_segment_seqnum")) p.setSarSegmentSeqnum(params::sar_segment_seqnum);
  if (OPTIONS_PRESENT(L"param-more_messages_to_send")) p.setMoreMessagesToSend(params::more_messages_to_send);
  if (OPTIONS_PRESENT(L"param-payload_type")) p.setPayloadType(params::payload_type);
  if (OPTIONS_PRESENT(L"param-message_payload")) p.setMessagePayload(std::vector<unsigned char>(params::message_payload.cbegin(),params::message_payload.cend()));
  if (OPTIONS_PRESENT(L"param-privacy_indicator")) p.setPrivacyIndicator(params::privacy_indicator);
  if (OPTIONS_PRESENT(L"param-callback_num")) p.setCallbackNum(std::vector<unsigned char>(params::callback_num.cbegin(),params::callback_num.cend()));
  if (OPTIONS_PRESENT(L"param-callback_num_pres_ind")) p.setCallbackNumPresInd(params::callback_num_pres_ind);
  if (OPTIONS_PRESENT(L"param-callback_num_atag")) p.setCallbackNumAtag(std::vector<unsigned char>(params::callback_num_atag.cbegin(),params::callback_num_atag.cend()));
  if (OPTIONS_PRESENT(L"param-source_subaddress")) p.setSourceSubaddress(std::vector<unsigned char>(params::source_subaddress.cbegin(),params::source_subaddress.cend()));
  if (OPTIONS_PRESENT(L"param-dest_subaddress")) p.setDestSubaddress(std::vector<unsigned char>(params::dest_subaddress.cbegin(),params::dest_subaddress.cend()));
  if (OPTIONS_PRESENT(L"param-display_time")) p.setDisplayTime(params::display_time);
  if (OPTIONS_PRESENT(L"param-sms_signal")) p.setSmsSignal(params::sms_signal);
  if (OPTIONS_PRESENT(L"param-ms_validity")) p.setMsValidity(params::ms_validity);
  if (OPTIONS_PRESENT(L"param-ms_msg_wait_facilities")) p.setMsMsgWaitFacilities(params::ms_msg_wait_facilities);
  if (OPTIONS_PRESENT(L"param-number_of_messages")) p.setNumberOfMessages(params::number_of_messages);
  if (OPTIONS_PRESENT(L"param-alert_on_message_delivery")) p.setAlertOnMsgDelivery(params::alert_on_message_delivery);
  if (OPTIONS_PRESENT(L"param-language_indicator")) p.setLanguageIndicator(params::language_indicator);
  if (OPTIONS_PRESENT(L"param-its_reply_type")) p.setItsReplyType(params::its_reply_type);
  if (OPTIONS_PRESENT(L"param-its_session_info")) p.setItsSessionInfo(params::its_session_info);
  if (OPTIONS_PRESENT(L"param-ussd_service_op")) p.setUssdServiceOp(params::ussd_service_op);
  //p.setUserMessageReferenceLen(const unsigned short);
  //p.setSourcePortLen(const unsigned short);
  //p.setSourceAddrSubunitLen(const unsigned short);
  //p.setDestinationPortLen(const unsigned short);
  //p.setDestAddrSubunitLen(const unsigned short);
  //p.setSarMsgRefNumLen(const unsigned short);
  //p.setSarTotalSegmentsLen(const unsigned short);
  //p.setSarSegmentSeqnumLen(const unsigned short);
  //p.setMoreMessagesToSendLen(const unsigned short);
  //p.setPayloadTypeLen(const unsigned short);
  //p.setMessagePayloadLen(const unsigned short);
  //p.setPrivacyIndicatorLen(const unsigned short);
  //p.setCallbackNumLen(const unsigned short);
  //p.setCallbackNumPresIndLen(const unsigned short);
  //p.setCallbackNumAtagLen(const unsigned short);
  //p.setSourceSubaddressLen(const unsigned short);
  //p.setDestSubaddressLen(const unsigned short);
  //p.setDisplayTimeLen(const unsigned short);
  //p.setSmsSignalLen(const unsigned short);
  //p.setMsValidityLen(const unsigned short);
  //p.setMsMsgWaitFacilitiesLen(const unsigned short);
  //p.setNumberOfMessagesLen(const unsigned short);
  //p.setAlertOnMsgDeliveryLen(const unsigned short);
  //p.setLanguageIndicatorLen(const unsigned short);
  //p.setItsReplyTypeLen(const unsigned short);
  //p.setItsSessionInfoLen(const unsigned short);
  //p.setUssdServiceOpLen(const unsigned short);
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
}
void Connection::sendSMPPEnquireLink(){
  anthill::smpp::SMPPEnquireLink p;
  writePdu(p);
  LOGGER_DEBUG<<__LOGGER__<<"PDU sent ..."<<std::endl;
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