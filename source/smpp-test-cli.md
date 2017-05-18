# smpp-test-cli

## Info

smpp-test-cli connects (using local unix stream socket) to smpp-test-server or smpp-test-client and enables to control them.

There are three main modes for this tool:
* Sending SMPP command (and waiting for its response);
* Listing all SMPP connections on given smpp-test-server or smpp-test-client;
* Dumping all SMPP commands that are sent or received on given smpp-test-server or smpp-test-client.

## Usage
```
Usage: 
 smpp-test-cli --ctr-id=smpp-test-client-0 --id=SUBMIT_SM
 smpp-test-cli --ctr-id=smpp-test-client-0 --list
 smpp-test-cli --ctr-id=smpp-test-client-0 --tail
 smpp-test-cli -h

Options: 
 -c, --ctr-id - Instance ID (smpp-test-server or smpp-test-client) that smpp-test-cli should connect to. In order to get ID list use 'ls /tmp/*' .
 -h, --help - print help.
 -l, --logger - print logger on stderr.
 -s, --list - List mode. If set - no SMPP command is sent, smpp-test-cli just dumps list of connections in smpp-test-client or smpp-test-server.
 -t, --tail - Tail mode. If set - no SMPP command is sent, smpp-test-cli just dumps PDUs from smpp-test-client or smpp-test-server - only if list mode is not set.
 -i, --id - ID of SMPP command that should be sent (default value: 'SUBMIT_SM') - only if list or tail mode is not set.
  SMPP commands: 'SUBMIT_SM', 'QUERY_SM', 'DELIVER_SM', 'REPLACE_SM', 'CANCEL_SM', 'ENQUIRE_LINK', 'ALERT_NOTIFICATION'.
 -n, --number - how many times SMPP PDU should be sent (default value: '1') - only if list or tail mode is not set.
 --param-system_id - value of 'system_id' SMPP param  (default value: 'smpp-test').
 --param-password - value of 'password' SMPP param  (default value: '').
 --param-system_type - value of 'system_type' SMPP param  (default value: '').
 --param-interface_version - value of 'interface_version' SMPP param  (default value: '').
 --param-addr_ton - value of 'addr_ton' SMPP param  (default value: '').
 --param-source_addr_ton - value of 'source_addr_ton' SMPP param  (default value: '0').
 --param-dest_addr_ton - value of 'dest_addr_ton' SMPP param  (default value: '0').
 --param-esme_addr_ton - value of 'esme_addr_ton' SMPP param  (default value: '0').
 --param-addr_npi - value of 'addr_npi' SMPP param  (default value: '').
 --param-source_addr_npi - value of 'source_addr_npi' SMPP param  (default value: '0').
 --param-dest_addr_npi - value of 'dest_addr_npi' SMPP param  (default value: '0').
 --param-esme_addr_npi - value of 'esme_addr_npi' SMPP param  (default value: '0').
 --param-address_range - value of 'address_range' SMPP param  (default value: '').
 --param-source_addr - value of 'source_addr' SMPP param  (default value: '').
 --param-destination_addr - value of 'destination_addr' SMPP param  (default value: '').
 --param-esme_addr - value of 'esme_addr' SMPP param  (default value: '').
 --param-service_type - value of 'service_type' SMPP param  (default value: '').
 --param-esm_class - value of 'esm_class' SMPP param  (default value: '0').
 --param-protocol_id - value of 'protocol_id' SMPP param  (default value: '0').
 --param-priority_flag - value of 'priority_flag' SMPP param  (default value: '0').
 --param-schedule_delivery_time - value of 'schedule_delivery_time' SMPP param  (default value: '').
 --param-validity_period - value of 'validity_period' SMPP param  (default value: '').
 --param-registered_delivery - value of 'registered_delivery' SMPP param  (default value: '0').
 --param-replace_if_present_flag - value of 'replace_if_present_flag' SMPP param  (default value: '0').
 --param-data_coding - value of 'data_coding' SMPP param  (default value: '0').
 --param-sm_default_msg_id - value of 'sm_default_msg_id' SMPP param  (default value: '0').
 --param-sm_length - value of 'sm_length' SMPP param  (default value: '0').
 --param-short_message - value of 'short_message' SMPP param  (default value: '').
 --param-message_id - value of 'message_id' SMPP param  (default value: '').
 --param-number_of_dests - value of 'number_of_dests' SMPP param  (default value: '').
 --param-dest_flag - value of 'dest_flag' SMPP param  (default value: '').
 --param-no_unsuccess - value of 'no_unsuccess' SMPP param  (default value: '').
 --param-dl_name - value of 'dl_name' SMPP param  (default value: '').
 --param-message_state - value of 'message_state' SMPP param  (default value: '0').
 --param-dest_addr_subunit - value of 'dest_addr_subunit' SMPP param  (default value: '0').
 --param-source_addr_subunit - value of 'source_addr_subunit' SMPP param  (default value: '0').
 --param-dest_network_type - value of 'dest_network_type' SMPP param  (default value: '').
 --param-source_network_type - value of 'source_network_type' SMPP param  (default value: '').
 --param-dest_bearer_type - value of 'dest_bearer_type' SMPP param  (default value: '').
 --param-source_bearer_type - value of 'source_bearer_type' SMPP param  (default value: '').
 --param-dest_telematics_id - value of 'dest_telematics_id' SMPP param  (default value: '').
 --param-source_telematics_id - value of 'source_telematics_id' SMPP param  (default value: '').
 --param-qos_time_to_live - value of 'qos_time_to_live' SMPP param  (default value: '').
 --param-payload_type - value of 'payload_type' SMPP param  (default value: '0').
 --param-additional_status_info_text - value of 'additional_status_info_text' SMPP param  (default value: '').
 --param-receipted_message_id - value of 'receipted_message_id' SMPP param  (default value: '').
 --param-ms_msg_wait_facilities - value of 'ms_msg_wait_facilities' SMPP param  (default value: '0').
 --param-privacy_indicator - value of 'privacy_indicator' SMPP param  (default value: '0').
 --param-source_subaddress - value of 'source_subaddress' SMPP param  (default value: '').
 --param-dest_subaddress - value of 'dest_subaddress' SMPP param  (default value: '').
 --param-user_message_reference - value of 'user_message_reference' SMPP param  (default value: '0').
 --param-user_response_code - value of 'user_response_code' SMPP param  (default value: '').
 --param-language_indicator - value of 'language_indicator' SMPP param  (default value: '0').
 --param-source_port - value of 'source_port' SMPP param  (default value: '0').
 --param-destination_port - value of 'destination_port' SMPP param  (default value: '0').
 --param-sar_msg_ref_num - value of 'sar_msg_ref_num' SMPP param  (default value: '0').
 --param-sar_total_segments - value of 'sar_total_segments' SMPP param  (default value: '0').
 --param-sar_segment_seqnum - value of 'sar_segment_seqnum' SMPP param  (default value: '0').
 --param-sc_interface_version - value of 'sc_interface_version' SMPP param  (default value: '').
 --param-display_time - value of 'display_time' SMPP param  (default value: '0').
 --param-ms_validity - value of 'ms_validity' SMPP param  (default value: '0').
 --param-dpf_result - value of 'dpf_result' SMPP param  (default value: '').
 --param-set_dpf - value of 'set_dpf' SMPP param  (default value: '').
 --param-ms_availability_status - value of 'ms_availability_status' SMPP param  (default value: '0').
 --param-network_error_code - value of 'network_error_code' SMPP param  (default value: '').
 --param-message_payload - value of 'message_payload' SMPP param  (default value: '').
 --param-delivery_failure_reason - value of 'delivery_failure_reason' SMPP param  (default value: '').
 --param-more_messages_to_send - value of 'more_messages_to_send' SMPP param  (default value: '0').
 --param-callback_num - value of 'callback_num' SMPP param  (default value: '').
 --param-callback_num_pres_ind - value of 'callback_num_pres_ind' SMPP param  (default value: '0').
 --param-callback_num_atag - value of 'callback_num_atag' SMPP param  (default value: '').
 --param-number_of_messages - value of 'number_of_messages' SMPP param  (default value: '0').
 --param-sms_signal - value of 'sms_signal' SMPP param  (default value: '0').
 --param-alert_on_message_delivery - value of 'alert_on_message_delivery' SMPP param  (default value: '0').
 --param-its_reply_type - value of 'its_reply_type' SMPP param  (default value: '0').
 --param-its_session_info - value of 'its_session_info' SMPP param  (default value: '0').
 --param-ussd_service_op - value of 'ussd_service_op' SMPP param  (default value: '0').
 --logger-direct - severity of logger messages that printed directly (default - 'notices').
 --logger-buffered - severity of logger messages that are buffered and printed only on errors (default - 'nonotices').
 --logger-dump - severity of logger messages that trigger printout of buffered messages (default - 'errors').
 Available logger severity values: 'critical', 'error', 'warning', 'notice', 'info', 'debug', 'errors', 'warnings', 'notices', 'infos', 'all', 'nocritical', 'noerrors', 'nowarnings', 'nonotices', 'none', 'nodebug'.
 -d, --daemon - run as daemon.
```