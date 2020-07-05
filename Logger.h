/*
 * Logger.h
 *
 *  Created on: 01.09.2019
 *      Author: artem78
 *      Description: Simple module for writing log to file.
 *      
 *      Usage:
 *      First you need to create instance of CLogger. After that set this logger
 *      as default calling LoggerStatic::SetLogger() static method. Use LOG
 *      macros from any place for write message to the log. If no logger set up,
 *      nothing was doing. Prefer to use LOG macros instead of directly call
 *      LoggerStatic::WriteFormat() method. Preprocessor constant LOGGING_ENABLED
 *      globally enabled or disabled logging functionality at compile time.
 *      For enabling logging add:
 *      	#define LOGGING_ENABLED 1
 *
 */

#ifndef LOGGER_H_
#define LOGGER_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <s32file.h>
#include "LoggingDefs.h"


// Check that ENABLE_LOGGING is defined
#ifndef LOGGING_ENABLED
#warning LOGGING_ENABLED not defined! Set it to 0.
#define LOGGING_ENABLED 0
#endif


// ToDo: Write to log additinal info: __FILE__, __LINE__, etc...
// ToDo: Add levels (debug, error, info, warning, etc...)
// ToDo: Write 16bit descriptors
// ToDo: Allow to create logger by filename and file server
// ToDo: Add formatting of date and time in message (ex.: %date, %time, %datetime)

#if LOGGING_ENABLED
#define LOG(aMsg, anArgs...) LoggerStatic::WriteFormat(_L8(__PRETTY_FUNCTION__), aMsg, ##anArgs);
// https://ru.stackoverflow.com/questions/1026888/%d0%a1%d1%83%d1%89%d0%b5%d1%81%d1%82%d0%b2%d1%83%d0%b5%d1%82-%d0%bb%d0%b8-%d0%b2-c-%d0%bc%d0%b0%d0%ba%d1%80%d0%be%d1%81-%d1%81-%d0%b8%d0%bc%d0%b5%d0%bd%d0%b5%d0%bc-%d0%ba%d0%bb%d0%b0%d1%81%d1%81%d0%b0?noredirect=1#comment1747251_1026888


/*
 * Main class for logging
 */
class CLogger : public CBase
	{
public:
	~CLogger();
	static CLogger* NewL(RFile &aFile);
	static CLogger* NewLC(RFile &aFile);

//public:
private:
	void Write(const TDesC8 &aModule, const TDesC8 &aDes);
	void WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...);
	void WriteFormatList(const TDesC8 &aModule, const TDesC8 &aFmt, VA_LIST aList);
	//static void WriteEmptyLine();
	void WriteToFile/*L*/(const TDesC8 &aDes);
	
private:
	CLogger(RFile &aFile);
	void ConstructL();
	
	RFileBuf iFileBuf;
	
	friend class LoggerStatic;
	};


/*
 * Static class for providing access to logger from everywhere. Set current logger
 * with LoggerStatic::SetLogger() method and than every call to LOG macros will be
 * send to this logger.
 */
class LoggerStatic
	{
private:
	static CLogger* iLogger;
public:
	static void SetLogger(CLogger* aLogger);
	static void WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...);
	
	friend class CLogger;
	};


#else
#define LOG(aMsg, anArgs...)
#endif

#endif /* LOGGER_H_ */
