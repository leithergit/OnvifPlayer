#ifndef _TraceMessage_H_
#define _TraceMessage_H_
#pragma once
#include <Windows.h>
#include <stdio.h>

#define __countof(array) (sizeof(array)/sizeof(array[0]))
#pragma warning (disable:4996)

void TraceMsgA(LPCSTR pFormat, ...);

#endif