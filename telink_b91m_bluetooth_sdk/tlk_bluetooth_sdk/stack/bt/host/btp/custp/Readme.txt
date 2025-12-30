1. custp(Customized Profile).Customized profiles designed to meet customer requirements for the private transmission of non-standard
   profile data or the configuration of test environments.
2. The custp PSM is 'BTP_PSMID_PROP_SIGNAL(0x1003)'.
3. Related to SDP are service class(BTP_SDP_SRVCLASS_ID_CUSTP), Service Record Handle(BTP_SDP_CUSTP_HANDLE), Protocol UUID(BTP_SDP_PROTOCOL_CUSTP_UUID)
   and gcBthSdpCustpItem.


/********************* running process *********************/
1. enable "TLKBTP_CFG_CUSTP_ENABLE"
2. custp initialization(no need to care). It has been initialized in "btp_init()".
3. Link to the server. Connect to the server using interface "btp_custp_connect()".
   If L2CAP channel create success "tlkmdi" layer will receive 'BTP_PTYPE_CUSTP' profile connect event. 
4. Use "btp_custp_regRecvDataCB()" deal receive data from server device.
5. Use "btp_custp_sendData()" send data to server device.

