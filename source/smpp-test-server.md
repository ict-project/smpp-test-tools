# smpp-test-server

## Info

smpp-test-server waits for SMPP clients.

## Usage
```
Usage: 
 smpp-test-server --ctr-id=smpp-test-server-0
 smpp-test-server -h

Options: 
 -c, --ctr-id - Instance ID for smpp-test-cli tool.
 -n, --host-name - Host name that SMPP server should bind to (default: localhost). If empty string - <any> IP is used.
 -h, --help - Port that SMPP server should bind to (default: 2775).
 -h, --help - print help.
 -l, --logger - print logger on stderr.
 --param-system_id - value of 'system_id' param for binding procedure (default value: 'smpp-test').
 --param-password - value of 'password' param for binding procedure (default value: '').
 --param-system_type - value of 'system_type' param for binding procedure (default value: '').
 --param-interface_version - value of 'interface_version' param for binding procedure (default value: '52').
 --param-addr_ton - value of 'addr_ton' param for binding procedure (default value: '0').
 --param-addr_npi - value of 'addr_npi' param for binding procedure (default value: '0').
 --param-addresss_range - value of 'addresss_range' param for binding procedure (default value: '.*').
 --logger-direct - severity of logger messages that printed directly (default - 'notices').
 --logger-buffered - severity of logger messages that are buffered and printed only on errors (default - 'nonotices').
 --logger-dump - severity of logger messages that trigger printout of buffered messages (default - 'errors').
 Available logger severity values: 'critical', 'error', 'warning', 'notice', 'info', 'debug', 'errors', 'warnings', 'notices', 'infos', 'all', 'nocritical', 'noerrors', 'nowarnings', 'nonotices', 'none', 'nodebug'.
 -d, --daemon - run as daemon.
```