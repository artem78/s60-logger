/*
 * Logger.cpp
 *
 *  Created on: 01.09.2019
 *      Author: artem78
 */

#include "Logger.h"

#if LOGGING_ENABLED

_LIT8(KLineBreak, "\r\n");
_LIT8(KTab, "\t");


/* CLogger */

CLogger::CLogger(RFile &aFile, TUint aLoggingLevels)
		: iLoggingLevels(aLoggingLevels)
	{
	// Leave iFileBuf buffer size as default (4Kb)
	iFileBuf.Attach(aFile, 0);
	}

CLogger::~CLogger()
	{
	iFileBuf.Synch/*L*/();
	iFileBuf.Close();
	
	if (this == LoggerStatic::iLogger)
		LoggerStatic::SetLogger(NULL);
	}

CLogger* CLogger::NewLC(RFile &aFile, TUint aLoggingLevels)
	{
	CLogger* self = new (ELeave) CLogger(aFile, aLoggingLevels);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLogger* CLogger::NewL(RFile &aFile, TUint aLoggingLevels)
	{
	CLogger* self = CLogger::NewLC(aFile, aLoggingLevels);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLogger::ConstructL()
	{
	}

void CLogger::Write(const TDesC8 &aModule, const TDesC8 &aDes, TLoggingLevel aLoggingLevel)
	{
	// Check reporting level
	if (aLoggingLevel != ELevelUnknown && !(aLoggingLevel & iLoggingLevels))
		return;
	
	// Strings
	_LIT(KTimeFormat, "%H:%T:%S.%*C3");
	_LIT8(KOpeningBracket, "(");
	_LIT8(KClosingBracket, ")");
	_LIT8(KOpeningSquareBracket, "[");
	_LIT8(KClosingSquareBracket, "]");
	_LIT8(KThreeSpaces, "   ");
	
	_LIT8(KDebugText,	"Debug");
	_LIT8(KInfoText,	"Info");
	_LIT8(KWarningText,	"Warning");
	_LIT8(KErrorText,	"Error");
	
	// Print current time
	TBuf<20> timeBuff;
	TTime time;
	time.HomeTime();
	time.FormatL(timeBuff, KTimeFormat);
	
	TBuf8<20> timeBuff8;
	timeBuff8.Copy(timeBuff);
	WriteToFile(timeBuff8);
	
	// Print message level
	if (aLoggingLevel != ELevelUnknown)
		{
		WriteToFile(KTab);
		WriteToFile(KOpeningBracket);
		
		if (aLoggingLevel & ELevelDebug)
			WriteToFile(KDebugText);
		else if (aLoggingLevel & ELevelInfo)
			WriteToFile(KInfoText);
		else if (aLoggingLevel & ELevelWarning)
			WriteToFile(KWarningText);
		else if (aLoggingLevel & ELevelError)
			WriteToFile(KErrorText);
		
		WriteToFile(KClosingBracket);
		}

	// Print module/class/function name
	WriteToFile(KTab);
	WriteToFile(KOpeningSquareBracket);
	WriteToFile(aModule);
	WriteToFile(KClosingSquareBracket);
	WriteToFile(KThreeSpaces);
	
	// Print message
	WriteToFile(aDes);
	WriteToFile(KLineBreak);
	}

void CLogger::WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...)
	{
	VA_LIST list;
	VA_START(list, aFmt);
	WriteFormatList(aModule, aFmt, list);
	// VA_END(list); // As I understand it, this is not necessary
	}

void CLogger::WriteFormatList(const TDesC8 &aModule, const TDesC8 &aFmt, VA_LIST aList, TLoggingLevel aLoggingLevel)
	{
	const TInt KDefaultBufferSize = 4 * KKilo; // 4Kb
	
	RBuf8 buff;
	buff.Create(KDefaultBufferSize);
	buff.FormatList(aFmt, aList);
	
	Write(aModule, buff, aLoggingLevel);
	
	buff.Close();
	}

//void CLogger::WriteEmptyLine()
//	{
//	iFile.Write(KLineBreak);
//	}

void CLogger::WriteToFile/*L*/(const TDesC8 &aDes)
	{
	TRequestStatus stat;
	iFileBuf.Write/*L*/(aDes, stat);
	User::WaitForRequest(stat);
	//iFileBuff.Synch/*L*/();
	}


/* LoggerStatic */

CLogger* LoggerStatic::iLogger = NULL;

void LoggerStatic::SetLogger(CLogger* aLogger)
	{
	iLogger = aLogger;
	}

void LoggerStatic::WriteFormat(CLogger::TLoggingLevel aLoggingLevel,
		const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...)
	{
	// Deny write to not configured logger
	if (!iLogger)
		return;
	
	VA_LIST list;
	VA_START(list, aFmt);
	iLogger->WriteFormatList(aModule, aFmt, list, aLoggingLevel);
	VA_END(list);
	}

#endif
