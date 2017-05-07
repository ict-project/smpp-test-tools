//! @file
//! @brief smpp-test-cli - SMPP Tool Command line interface - Source file.
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
#include "user-app.hpp"
#include "../libict/source/logger.hpp"
#include "../libict/source/options.hpp"
//====================================================================
namespace params {
  namespace init {
    const static std::string path("");
  }
  std::string path(init::path);
}
//====================================================================
OPTIONS_CONFIG(cli0,0){
  if (config) {
  } else {
    parser.errors<<"Usage: "<<std::endl;
    parser.errors<<" smpp-test-client --ctr-id=smpp-test-client-0"<<std::endl;
    parser.errors<<" smpp-test-cli -h"<<std::endl;
    parser.errors<<std::endl;
    parser.errors<<"Options: "<<std::endl;
  }
  if (config) {
    parser.registerOpt(L'c',L"ctr-id",params::path);
  } else {
    parser.errors<<" "<<parser.getOptionDesc(L'c')<<" - Instance ID (smpp-test-server or smpp-test-client) that smpp-test-cli should connect to. In order to get ID list use smpp-test-list."<<std::endl;
  }
}
//====================================================================
class App: public smpp::main::Application {
public:
  void doStart(){
    if (!params::path.size()) throw std::invalid_argument("Instance ID of smpp-test-server or smpp-test-client must be provided!");
    smpp::client::factory("/tmp/"+params::path,smpp::cli::factory);
  }
  void doClean(){
    REGISTER_CLIENT_STREAM.destroy();
    REGISTER_CLI_APP.destroy();
  }
  void doStop(){
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_CLIENT_STREAM.size(): "<<REGISTER_CLIENT_STREAM.size()<<std::endl;
    LOGGER_DEBUG<<__LOGGER__<<"REGISTER_CLI_APP.size(): "<<REGISTER_CLI_APP.size()<<std::endl;
  }
};
//============================================
int main(int argc, const char* argv[]){
  App app;
  return(smpp::main::mainFunction(argc,argv,app));
}
//============================================