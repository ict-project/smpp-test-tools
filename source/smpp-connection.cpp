//! @file
//! @brief SMPP connection module - Source file.
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
#include "smpp-connection.hpp"
#include "main-function.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
#include <iostream>
#include <sstream>
#include <map>
//============================================
namespace smpp { namespace proto {
//===========================================
const static std::map<unsigned long,int> pduTypeMap={
  #define SMPP_ID(tag,number,nameSpace,className,line) {number,line},
    #include "smpp-connection.h.in"
  #undef SMPP_ID
  {0,0}
};
//===========================================
namespace pdu {
  Unknown::Unknown():anthill::smpp::SMPPPDU(0x0ffffffful){}
  Unknown::~Unknown(){}
  void Unknown::encodePacket(anthill::smpp::SMPPEncoder* poEncoder){
    poEncoder->encodeHeader(m_oHeader);
  }
  void Unknown::decodePacket(anthill::smpp::SMPPDecoder* poDecoder){
    poDecoder->decodeHeader(m_oHeader);
  }
  void Unknown::outputPacket(anthill::smpp::ISMPPOutputter* poOutputter){
    poOutputter->outputHeader(m_oHeader);
  }
  void Unknown::validatePacket(anthill::smpp::SMPPValidator* poValidator){
    poValidator->validateHeader(m_oHeader);
  }
  anthill::smpp::SMPPPDU* Unknown::cloneSMPPPDU(){
    return(new Unknown);
  }
  //===========================================
  Text::Text():anthill::smpp::SMPPPDU(0x0f000000ul){}
  Text::~Text(){}
  void Text::encodePacket(anthill::smpp::SMPPEncoder* poEncoder){
    poEncoder->encodeHeader(m_oHeader);
    poEncoder->encodeBroadcastEndTime(text,text.size()+1);
  }
  void Text::decodePacket(anthill::smpp::SMPPDecoder* poDecoder){
    unsigned short nLen;
    poDecoder->decodeHeader(m_oHeader);
    poDecoder->decodeBroadcastEndTime(text,nLen,getCommandLength());
  }
  void Text::outputPacket(anthill::smpp::ISMPPOutputter* poOutputter){
  }
  void Text::validatePacket(anthill::smpp::SMPPValidator* poValidator){
  }
  anthill::smpp::SMPPPDU* Text::cloneSMPPPDU(){
    return(new Text);
  }
}
//===========================================
std::size_t Decoder::pushData(unsigned char * input,std::size_t size){
  m_aBuffer.insert(m_aBuffer.end(),input,input+size);
  return(size);
}
//===========================================
std::size_t Encoder::pullData(unsigned char * ouput,std::size_t size){
  if (size>m_aBuffer.size()) size=m_aBuffer.size();
  memcpy(ouput,m_aBuffer.data(),size);
  m_aBuffer.erase(m_aBuffer.begin(),m_aBuffer.begin()+size);
  if (m_nOffset>=size) {
    m_nOffset-=size;
  } else {
    m_nOffset=0;
  }
  return(size);
}
void Encoder::encodePduSize(std::size_t begin,uint32_t size){
  const unsigned int nSizeof = 4;
  int nTemp = dohtonl(size);
  std::vector<unsigned char> aData;
  for (unsigned j = 0; j < nSizeof; ++j)
    aData.push_back(((char*)&nTemp)[j]);
  const std::size_t nShift = begin+anthill::smpp::SMPPOffset::COMMAND_LENGTH_POS;
  unsigned char* aStart = &m_aBuffer[nShift];
  memcpy(aStart,&aData[0],aData.size());
}
//===========================================
Connection::Connection(){
}
Connection::~Connection(){
}
void Connection::doRead(){
  bool again=true;//Czy powtarzać odczyt.
  if (readSize){//Przenoszę dane z bufora lokalnego do bufora dekodera.
    decoder.pushData(readData,readSize);
  }
  while (decoder.hasPdu()){//Jeśli PDU jest już cały.
    uint32_t pduType=decoder.getPduType();
    uint32_t pduSize=decoder.getPduSize();
    again=false;
    LOGGER_DEBUG<<__LOGGER__<<"decoder.getPduType()=0x"<<std::hex<<pduType<<", decoder.getPduSize()=0x"<<decoder.getPduSize()<<std::dec<<std::endl;
    switch (pduTypeMap.count(pduType)?pduTypeMap.at(pduType):0){
      #define SMPP_ID(tag,number,nameSpace,className,line) \
        case line: { \
          nameSpace::className p; \
          try { \
            p.decodePacket(&decoder); \
            decoder.validateOffset(pduSize); \
            readPdu(p); \
          } catch (std::exception& e) { \
            readPduError(p,e.what()); \
            LOGGER_WARN<<__LOGGER__<<"Decoder error: "<<e.what()<<std::endl; \
          } \
        } break;
        #include "smpp-connection.h.in"
      #undef SMPP_ID
      default:{
        pdu::Unknown p;
        try { 
          p.decodePacket(&decoder);
          decoder.validateOffset(pduSize);
          readPduUnknown(p);
        } catch (std::exception& e) {
          readPduError(p,e.what());
          LOGGER_WARN<<__LOGGER__<<"Decoder error: "<<e.what()<<std::endl;
        }
      } break;
    }
    decoder.erasePdu();//Dane obsłuzonego pakietu są kasowane w buforze dekodera.
  } 
  if (again){//Jeśli PDU nie jest jeszcze cały.
    asyncRead();
  }
}
void Connection::doWrite(){
  {//Przenoszę dane z bufora dekodera do bufora lokalnego.
    writeSize=encoder.pullData(writeData,bufferSize);
    LOGGER_DEBUG<<__LOGGER__<<"writeSize="<<writeSize<<", bufferSize="<<bufferSize<<std::endl;
  }
  if (encoder.getBufferSize()||writeSize){//Jeśli coś jeszcze do wysłania.
    asyncWrite();
  } else {
  }
}
#define SMPP_ID(tag,number,nameSpace,className,line) \
  void Connection::writePdu(nameSpace::className & p){ \
    try { \
      std::size_t begin=encoder.getOffset(); \
      p.encodePacket(&encoder); \
      encoder.encodePduSize(begin,encoder.getOffset()-begin); \
      LOGGER_DEBUG<<__LOGGER__<<"offset="<<encoder.getOffset()<<", begin="<<begin<<std::endl; \
    } catch (std::exception& e) { \
      LOGGER_WARN<<__LOGGER__<<"Encoder error: "<<e.what()<<std::endl; \
    } \
    asyncWrite(); \
  }
  #include "smpp-connection.h.in"
#undef SMPP_ID
//============================================
}}
//============================================