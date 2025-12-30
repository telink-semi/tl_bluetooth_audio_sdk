extern int btp_browsing_sendGeneralReject(uint16_t aclHandle, uint8_t errCode);
extern int btp_browsing_setChangePathCmd(uint16_t aclHandle, uint16_t uidCounter, uint8_t direct, uint8_t *pUID);
extern int btp_browsing_setBrowsedPlayerCmd(uint16_t aclHandle, uint16_t playerID);
extern int btp_browsing_setBrowsedPlayerRsp(uint16_t aclHandle, uint8_t status);
extern int btp_browsing_sendGetFolderItemsCmd(uint16_t aclHandle, uint8_t scope, 
	uint32_t startItem, uint32_t endItem, uint8_t attCount, uint32_t *pAttList);
extern int btp_browsing_sendGetItemAttrCmd(uint16_t aclHandle, uint8_t scope, 
	uint8_t *pUids, uint8_t uidCounter, uint8_t attCount, uint32_t *pAttList);
extern int btp_browsing_sendGetTotalNumbOfItemsCmd(uint16_t aclHandle, uint8_t scope);
extern int btp_browsing_sendGetTotalNumbOfItemsRsp(uint16_t aclHandle, uint8_t status,
	uint16_t uidCounter, uint32_t number);

extern void btp_browsing_regItemsReportCB(BtpBrowsingItemsReportCB cb);
extern void btp_browsing_regInsCompleteCB(BtpBrowsingInsCompleteCB cb);


1. 获取当前的播放器列表
发送：
	btp_browsing_sendGetFolderItemsCmd(, BTP_BROWSING_SCOPE_MEDIA_PLAYER_LIST, 0, 100, 0xFF, NULL);
返回：
	BtpBrowsingItemsReportCB: btp_browsing_playerItem_t, 返回playerID, IsPlayable
	BtpBrowsingInsCompleteCB：返回指令完成；
2. 设置当前播放器，参数使用第1步中的playerID
发送：
	btp_browsing_setBrowsedPlayerCmd(,playerID)
返回：
	BtpBrowsingInsCompleteCB：返回播放器playerID的Item个数，以及播放器名字
3. 获取正在播放的播放器的音乐信息
发送：
	btp_browsing_sendGetItemAttrCmd：
		scope=BTP_BROWSING_SCOPE_NOW_PLAYGING，
		pUids=FF FF FF FF FF FF FF FF,   （该字段为：BTP_AVRCP_EVTID_TRACK_CHANGED事件的回调中返回的TrackID）
		uidCounter=0,
		pAttList=BTP_BROWSING_ATTID_TITLE+BTP_BROWSING_ATTID_ARTIST_NAME+BTP_BROWSING_ATTID_ALBUM_NAME+BTP_BROWSING_ATTID_TRACK_NUMBER+BTP_BROWSING_ATTID_GENRE+BTP_BROWSING_ATTID_PLAYING_TIME
返回：
	BtpBrowsingItemsReportCB：btp_browsing_attrItem_t （如果有歌曲正在播放，则会将歌曲信息列举返回）
	BtpBrowsingInsCompleteCB：返回指令完成状态
4. 获取播放器的虚拟文件系统
发送：
	btp_browsing_sendGetFolderItemsCmd
		scope=BTP_BROWSING_SCOPE_MEDIA_PLAYER_VIRTUAL_FILESYSTEM，
		startItem=0，
		endItem=1,
		attCount=7，和pAttList相关
		pAttList=BTP_BROWSING_ATTID_TITLE+BTP_BROWSING_ATTID_ARTIST_NAME+BTP_BROWSING_ATTID_ALBUM_NAME+BTP_BROWSING_ATTID_TRACK_NUMBER+BTP_BROWSING_ATTID_TOTAL_TRACKS+BTP_BROWSING_ATTID_GENRE+BTP_BROWSING_ATTID_PLAYING_TIME
返回：
	BtpBrowsingItemsReportCB：btp_browsing_folderItem_t
	BtpBrowsingInsCompleteCB：返回指令完成状态





