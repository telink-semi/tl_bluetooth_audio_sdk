Name Definition:

    RC: Stands for “Recording Card”.
    Stream in: Indicates that this stream endpoint supports write operations.
    Stream out: Indicates that this stream endpoint supports read operations.

UI Logic:
    Dsp alg default close.
    Wav save dafault close.
    BLE defualt enable;

    Key1:
        Short click: Initiate or terminate the recording process.
        Double click: Enable or disable BLE.
        Three click: Enable dsp alg.
        Four click: Enable wav write function. 

    Key2:
        Three click：Disable dsp alg.
        Four click: Disable wav write function. 

    Auto power off: The system will shut down automatically after 30 seconds if it is idle. The “system idle” status means there is no activity such as recording, file transfer, or BLE connection.

    
