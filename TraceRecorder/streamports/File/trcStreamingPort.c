/*
 * Trace Recorder for Tracealyzer v4.5.3
 * Copyright 2021 Percepio AB
 * www.percepio.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Supporting functions for trace streaming, used by the "stream ports" 
 * for reading and writing data to the interface.
 * Existing ports can easily be modified to fit another setup, e.g., a 
 * different TCP/IP stack, or to define your own stream port.
 */

#include "trcRecorder.h"
#include "arch/clib.h"

#if (TRC_CFG_RECORDER_MODE == TRC_RECORDER_MODE_STREAMING)  
#if (TRC_USE_TRACEALYZER_RECORDER == 1)

VP_FILE traceFile = -1;

void openFile(char* fileName)
{
	if (traceFile < 0)
	{
		printf("Trace file %s opened.\n", fileName);
		traceFile = vp_fopen(fileName, 0, 0);
	}
}

int32_t writeToFile(void* data, uint32_t size, int32_t *ptrBytesWritten)
{
	int32_t written = 0;
	if (traceFile > -1)
	{
		written = vp_fwrite(data, 1, size, traceFile);
	}
	else
	{
		written = 0;
	}

	if (ptrBytesWritten != 0)
		*ptrBytesWritten = written;

	if ((int32_t)size == written)
		return 0;
	else
		return -1;
}

void closeFile(void)
{
	if (traceFile > -1)
	{
		vp_fclose(traceFile);
		traceFile = -1;
		printf("Trace file closed.\n");
	}
}

#endif /*(TRC_USE_TRACEALYZER_RECORDER == 1)*/
#endif /*(TRC_CFG_RECORDER_MODE == TRC_RECORDER_MODE_STREAMING)*/
