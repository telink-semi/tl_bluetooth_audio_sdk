1. voice-processing algorithms worked in dsp core, currently supported ENC, AEC, ANS;
2. download addr: 0x00200000;
3. TLK_MW_DSP_COMM_ENABLE = 1: voice pcm data will be transported to DSP and be processed by algorithms in voice state, 
   DSP clock will be disable after left voice state, and the power consumption will increase ~5mA;

20250416
1. The additional power consumption is caused by action entering voice state, not by dsp.
2. Power down DSP instead of disable DSP clock after left voice state;

20250521
1. Only nn_ns running on dsp, other algorithm running on d25f.
2. DSP needed by the following audio paths:
	1) BT voice
	2) Dongle voice
	3) Interphone in BTBLE headset project for fengyu