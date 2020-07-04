/*
 * Logger.cpp
 *
 *  Created on: 01.09.2019
 *      Author: user
 */

#include "Logger.h"

#if LOGGING_ENABLED

_LIT8(KLineBreak, "\r\n");
_LIT8(KTab, "\t");


/*// Set initial values for static properties
TBool Logger::iIsConfigured = EFalse;
RFile Logger::iFile = RFile();*/


CLogger::CLogger(/*const*/ RFile &aFile) //:
		/*iFile(aFile)*//*,
		iIsConfigured(ETrue)*/
	{
	//iFileBuf = RFileBuf(); // Default buffer size is 4Kb
	iFileBuf.Attach(aFile, 0);
	}

CLogger::~CLogger()
	{
	iFileBuf.Synch/*L*/();
	iFileBuf.Close();
	
	if (this == LoggerStatic::iLogger)
		LoggerStatic::SetLogger(NULL);
	}

CLogger* CLogger::NewLC(/*const*/ RFile &aFile)
	{
	CLogger* self = new (ELeave) CLogger(aFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CLogger* CLogger::NewL(/*const*/ RFile &aFile)
	{
	CLogger* self = CLogger::NewLC(aFile);
	CleanupStack::Pop(); // self;
	return self;
	}

void CLogger::ConstructL()
	{
	}

/*void CLogger::Configure(const RFile &aFile)
	{
	iFile = aFile;
	iIsConfigured = ETrue;
	}*/

void CLogger::Write(const TDesC8 &aModule, const TDesC8 &aDes)
	{
	/*// Deny write to not configured logger
	if (!iIsConfigured)
		return;*/
	
	// Strings
	_LIT(KTimeFormat, "%H:%T:%S.%*C3");
	_LIT8(KOpeningSquareBracket, "[");
	_LIT8(KClosingSquareBracket, "]");
	_LIT8(KThreeSpaces, "   ");	
	
	// Print current time
	TBuf<20> timeBuff;
	TTime time;
	time.HomeTime();
	time.FormatL(timeBuff, KTimeFormat);
	
	TBuf8<20> timeBuff8;
	timeBuff8.Copy(timeBuff);
	WriteToFile(timeBuff8);

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
    
    // VA_END(list); // вроде не нужно
	}

void CLogger::WriteFormatList(const TDesC8 &aModule, /*TRefByValue<const TDesC8> aFmt*/ const TDesC8 &aFmt, VA_LIST aList)
	{
	// ToDo: Add format for date and time
	
	/*// Deny write to not configured logger
	if (!iIsConfigured)
		return;*/

	
	TBuf8<256> buff;
	buff.Zero();
	buff.FormatList(aFmt, aList);
	
	Write(aModule, buff);
	}

//void CLogger::WriteEmptyLine()
//	{
//	iFile.Write(KLineBreak);
//	}

void CLogger::WriteToFile/*L*/(const TDesC8 &aDes)
	{
	/*// Deny write to not configured logger
	if (!iIsConfigured)
		return;*/
	
	TRequestStatus stat;
	iFileBuf.Write/*L*/(aDes, stat);
	User::WaitForRequest(stat);
	//iFileBuff.Synch/*L*/();
	}


CLogger* LoggerStatic::iLogger = NULL;

/*static*/ void LoggerStatic::SetLogger(CLogger* aLogger)
	{
	iLogger = aLogger;
	}

//static LoggerStatic::Logger* Instance()
//	{
//    if (!iInstance)           
//    	iInstance = Logger::NewL();
//    return iInstance;
//	}

//static void LoggerStatic::Configure(const RFile &aFile)
//	{
//	}

/*static*/ void LoggerStatic::WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...)
	{
	// Deny write to not configured logger
	if (!iLogger)
		return;
	
	VA_LIST list;
	VA_START(list, aFmt);
	iLogger->WriteFormatList(aModule, aFmt, list);
	VA_END(list);
	}


#endif
