//! @file
//! @brief smpp-test-client - SMPP Tool Client - Source file.
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
#include "main-function.hpp"
#include "client.hpp"
#include "server.hpp"
#include "daemon-app.hpp"
#include "smpp-app.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
#include <map>
//============================================
namespace params {
  namespace init {
    const static std::string path("");
    const static std::string host("localhost");
    const static std::string port("2775");
  }
  std::string path(init::path);
  std::string host(init::host);
  std::string port(init::port);
}
//============================================
std::map<std::wstring,int> bindDictionary={
  {L"SMSC",smpp::app::Bind_SMSC},
  {L"RX",smpp::app::Bind_RX},
  {L"TX",smpp::app::Bind_TX},
  {L"TRX",smpp::app::Bind_TRX}
};
//============================================
OPTIONS_CONFIG(client0,0){
  if (config) {
  } else {
    parser.errors<<"Usage: "<<std::endl;
    parser.errors<<" smpp-test-client --ctr-id=smpp-test-client-0"<<std::endl;
    parser.errors<<" smpp-test-client -h"<<std::endl;
    parser.errors<<std::endl;
    parser.errors<<"Options: "<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'c',L"ctr-id",params::path);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'c')<<" - Instance ID for smpp-test-cli tool."<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'n',L"host-name",params::host);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'n')<<" - Host name that SMPP server should connect to (default: "<<params::init::host<<")."<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'p',L"port",params::port);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'h')<<" - Port that SMPP server should connect to (default: "<<params::init::port<<")."<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'b',L"bind",smpp::app::params::bindType,bindDictionary);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'b')<<" - Bind type (default: 'SMSC')."<<std::endl;
    parser.errors<<" "<<" Bind type values: 'SMSC' (no bind),'TX','RX','TRX'."<<std::endl;
  }
}
//============================================
class App: public smpp::main::Application {
public:
  void doStart(){
    if (!params::path.size()) throw std::invalid_argument("Instance ID for smpp-test-cli tool must be provided!");
    smpp::server::factory("/tmp/"+params::path,smpp::cli::factory);
    smpp::client::factory(params::host,params::port,smpp::app::factory);
  }
  void doClean(){
    REGISTER_CLIENT_TCP.destroy();
    REGISTER_SERVER_STREAM.destroy();
    REGISTER_CLI_APP.destroy();
    REGISTER_SMPP_APP.destroy();
  }
  void doStop(){
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_CLIENT_TCP.size(): "<<REGISTER_CLIENT_TCP.size()<<std::endl;
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_SERVER_STREAM.size(): "<<REGISTER_SERVER_STREAM.size()<<std::endl;
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_CLI_APP.size(): "<<REGISTER_CLI_APP.size()<<std::endl;
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_SMPP_APP.size(): "<<REGISTER_SMPP_APP.size()<<std::endl;
  }
};
//============================================
int main(int argc, const char* argv[]){
  App app;
  smpp::main::syslogInfoString="smpp-test-client";
  return(smpp::main::mainFunction(argc,argv,app));
}
//====================================================================