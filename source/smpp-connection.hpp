//! @file
//! @brief SMPP connection module - header file.
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
#ifndef _SMPP_CONNECTION_HEADER
#define _SMPP_CONNECTION_HEADER
#define ANTHILLSMPP_DLLPRFX
//============================================
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "connection.hpp"
#include "../libsmpp/source/AntHill.hpp"
#include "../libsmpp/source/AntHillSMPP.hpp"
#include "../libsmpp/source/SMPPConstants.hpp"
#include "../libsmpp/source/SMPPStructures.hpp"
#include "../libsmpp/source/SMPPDatatypes.hpp"
#include "../libsmpp/source/SMPPSubmitSM.hpp"
#include "../libsmpp/source/SMPPSubmitSMResp.hpp"
#include "../libsmpp/source/SMPPQuerySM.hpp"
#include "../libsmpp/source/SMPPQuerySMResp.hpp"
#include "../libsmpp/source/SMPPCancelSM.hpp"
#include "../libsmpp/source/SMPPCancelSMResp.hpp"
#include "../libsmpp/source/SMPPReplaceSM.hpp"
#include "../libsmpp/source/SMPPReplaceSMResp.hpp"
#include "../libsmpp/source/SMPPDeliverSM.hpp"
#include "../libsmpp/source/SMPPDeliverSMResp.hpp"
#include "../libsmpp/source/SMPPDataSM.hpp"
#include "../libsmpp/source/SMPPDataSMResp.hpp"
#include "../libsmpp/source/SMPPBroadcastSM.hpp"
#include "../libsmpp/source/SMPPBroadcastSMResp.hpp"
#include "../libsmpp/source/SMPPQueryBroadcastSM.hpp"
#include "../libsmpp/source/SMPPQueryBroadcastSMResp.hpp"
#include "../libsmpp/source/SMPPCancelBroadcastSM.hpp"
#include "../libsmpp/source/SMPPCancelBroadcastSMResp.hpp"
#include "../libsmpp/source/SMPPBindReceiver.hpp"
#include "../libsmpp/source/SMPPBindReceiverResp.hpp"
#include "../libsmpp/source/SMPPBindTransmitter.hpp"
#include "../libsmpp/source/SMPPBindTransmitterResp.hpp"
#include "../libsmpp/source/SMPPBindTransceiver.hpp"
#include "../libsmpp/source/SMPPBindTransceiverResp.hpp"
#include "../libsmpp/source/SMPPUnbind.hpp"
#include "../libsmpp/source/SMPPUnbindResp.hpp"
#include "../libsmpp/source/SMPPAlertNotification.hpp"
#include "../libsmpp/source/SMPPEnquireLink.hpp"
#include "../libsmpp/source/SMPPEnquireLinkResp.hpp"
#include "../libsmpp/source/SMPPGenericNack.hpp"
#include "../libsmpp/source/SMPPOutbind.hpp"
#include "../libsmpp/source/SMPPSubmitMulti.hpp"
#include "../libsmpp/source/SMPPSubmitMultiResp.hpp"
#include "../libsmpp/source/SMPPDecoder.hpp"
#include "../libsmpp/source/SMPPEncoder.hpp"
#include "../libsmpp/source/SMPPValidator.hpp"
#include "../libsmpp/source/SMPPHexOutputter.hpp"
#include "../libsmpp/source/SMPPTxtOutputter.hpp"
//============================================
namespace smpp { namespace proto {
//===========================================
namespace pdu {
  class Unknown : public anthill::smpp::SMPPPDU,public boost::enable_shared_from_this<Unknown>{
    public:
      Unknown();
      virtual ~Unknown();
    public:
      virtual void encodePacket(anthill::smpp::SMPPEncoder* poEncoder);
      virtual void decodePacket(anthill::smpp::SMPPDecoder* poDecoder);
      virtual void outputPacket(anthill::smpp::ISMPPOutputter* poOutputter);
      virtual void validatePacket(anthill::smpp::SMPPValidator* poValidator);
    protected:
      virtual anthill::smpp::SMPPPDU* cloneSMPPPDU();
      virtual void fireOnReceived(anthill::smpp::SMPPSession*){}
   };
  class Text : public anthill::smpp::SMPPPDU,public boost::enable_shared_from_this<Text>{
    public:
      Text();
      virtual ~Text();
    public:
      virtual void encodePacket(anthill::smpp::SMPPEncoder* poEncoder);
      virtual void decodePacket(anthill::smpp::SMPPDecoder* poDecoder);
      virtual void outputPacket(anthill::smpp::ISMPPOutputter* poOutputter);
      virtual void validatePacket(anthill::smpp::SMPPValidator* poValidator);
    protected:
      virtual anthill::smpp::SMPPPDU* cloneSMPPPDU();
      virtual void fireOnReceived(anthill::smpp::SMPPSession*){}
    public:
      std::string text;
   };
}
//===========================================
template<class T> class Coder: public T{
public:
  bool hasPdu();
  uint32_t getPduType();
  uint32_t getPduSize();
  void erasePdu();
  unsigned getOffset(){return(T::m_nOffset);}
  unsigned getBufferSize(){return(T::m_aBuffer.size());}
};
template<class T> bool Coder<T>::hasPdu(){
  if (T::m_aBuffer.size()<anthill::smpp::SMPPOffset::SMPP_HEADER_SIZE) return(false);
  if (T::m_aBuffer.size()<getPduSize()) return(false);
  return(true);
}
template<class T> uint32_t Coder<T>::getPduType(){
  uint32_t nValue=0;
  const uint32_t nSizeof = 4;
  if(anthill::smpp::SMPPOffset::COMMAND_ID_POS+nSizeof<=T::m_aBuffer.size()){
    const uint32_t nShift=anthill::smpp::SMPPOffset::COMMAND_ID_POS;
    unsigned char* aStart=&T::m_aBuffer[nShift];
    memcpy(((char*)&nValue),aStart,nSizeof);
    nValue=T::dontohl(nValue);
  }
  return(nValue);
}
template<class T> uint32_t Coder<T>::getPduSize(){
  uint32_t nValue=0;
  const uint32_t nSizeof = 4;
  if(anthill::smpp::SMPPOffset::COMMAND_LENGTH_POS+nSizeof<=T::m_aBuffer.size()){
    const uint32_t nShift=anthill::smpp::SMPPOffset::COMMAND_LENGTH_POS;
    unsigned char* aStart=&T::m_aBuffer[nShift];
    memcpy(((char*)&nValue),aStart,nSizeof);
    nValue=T::dontohl(nValue);
  }
  return(nValue);
}
template<class T> void Coder<T>::erasePdu(){
  uint32_t pduSize=getPduSize();
  T::m_aBuffer.erase(T::m_aBuffer.begin(),T::m_aBuffer.begin()+pduSize);
  if (T::m_nOffset>pduSize){
    T::m_nOffset-=pduSize;
  } else {
    T::m_nOffset=0;
  }
}
//===========================================
class Decoder: public Coder<anthill::smpp::SMPPDecoder>{
public:
  std::size_t pushData(unsigned char * input,std::size_t size);
};
//===========================================
class Encoder: public Coder<anthill::smpp::SMPPEncoder>{
public:
  std::size_t pullData(unsigned char * ouput,std::size_t size);
  void encodePduSize(std::size_t begin,uint32_t size);
};
//===========================================
class Connection : public connection::Iface {
private:
  Decoder decoder;
  Encoder encoder;
protected:
  //!
  //! @brief Wykonuje odczyt.
  //!
  void doRead();
  //!
  //! @brief Wykonuje zapis.
  //!
  void doWrite();
public:
  Connection();
  virtual ~Connection();
  #define SMPP_ID(tag,number,nameSpace,className,line) virtual void readPdu(nameSpace::className & p)=0;
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  virtual void readPduUnknown(pdu::Unknown & p)=0;
  virtual void readPduError(anthill::smpp::SMPPPDU & p,const std::string & what)=0;
  #define SMPP_ID(tag,number,nameSpace,className,line) void writePdu(nameSpace::className & p);
    #include "smpp-connection.h.in"
  #undef SMPP_ID
};
//============================================
}}
//===========================================
#endif