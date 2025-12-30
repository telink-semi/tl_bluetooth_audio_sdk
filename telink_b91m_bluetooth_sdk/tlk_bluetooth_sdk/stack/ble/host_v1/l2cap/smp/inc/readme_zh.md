# BLE SMP 协议层

## BLE store 存储分布

| Key      	| structural                                           	| size  	| description                                                                  	|
|----------	|------------------------------------------------------	|-------	|------------------------------------------------------------------------------	|
| version  	| uint8_t c_num;<br>uint8_t p_num;<br>uint16_t version 	| 4     	| central max bound number<br>peripheral max bound number<br>smp store version 	|
| index    	| uint32_t index;                                      	| 4     	| current pairing index.peer new pairing that bonding connect increase 1.      	|
| p_queue  	| uint8_t p_index[p_num];                              	| p_num 	| 对于peripheral情况下，配对/回连后的顺序排序                                  	|
| c_queue  	|                                                      	|       	|                                                                              	|
| p_infoXX 	| struct ble_host_smp_store_key key;                   	| N     	| XX是p_index格式化后的数据                                                    	|