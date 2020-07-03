/*
 * Logger.h
 *
 *  Created on: 01.09.2019
 *      Author: user
 *      Description: Simple module for writing log to file. Do not call class methods
 *      directly, use macroses instead. Before using it`s need to set output file by
 *      LOG_CONFIGURE macros. For enable logging you need to define constant:
 *      	#define LOGGING_ENABLED 1
 *		
 */

#ifndef LOGGER_H_
#define LOGGER_H_

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include "LoggingDefs.h"


// Check that ENABLE_LOGGING is defined
#ifndef LOGGING_ENABLED
#warning LOGGING_ENABLED not defined! Set it to 0.
#define LOGGING_ENABLED 0
#endif


// ToDo: Write to log additinal info: __FILE__, __LINE__, etc...
// ToDo: Add levels (debug, error, info, warning, etc...)
// ToDo: Write 16bit descriptors

#if LOGGING_ENABLED
//#define LOG_CONFIGURE(aFile) LoggerStatic::Configure(aFile)
//#define LOG_CONFIGURE(aFileName)
//#define LOG(aMsg, ...) Logger::WriteFormat(aMsg, ## __VA_ARGS__);
#define LOG(aMsg, anArgs...) LoggerStatic::WriteFormat(_L8(__PRETTY_FUNCTION__), aMsg, ##anArgs);
// https://ru.stackoverflow.com/questions/1026888/%d0%a1%d1%83%d1%89%d0%b5%d1%81%d1%82%d0%b2%d1%83%d0%b5%d1%82-%d0%bb%d0%b8-%d0%b2-c-%d0%bc%d0%b0%d0%ba%d1%80%d0%be%d1%81-%d1%81-%d0%b8%d0%bc%d0%b5%d0%bd%d0%b5%d0%bc-%d0%ba%d0%bb%d0%b0%d1%81%d1%81%d0%b0?noredirect=1#comment1747251_1026888


class CLogger : public CBase
	{
public:
	~CLogger();
	static CLogger* NewL(const RFile &aFile);
	static CLogger* NewLC(const RFile &aFile);
	
	//static void Configure(const RFile &aFile);
	//static void Configure(const TDesC aFileName);
	/*static*/ void Write(const TDesC8 &aModule, const TDesC8 &aDes);
	/*static*/ void WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...);
	/*static*/ void WriteFormatList(const TDesC8 &aModule, /*TRefByValue<const TDesC8> aFmt*/ const TDesC8 &aFmt, VA_LIST aList);
	//static void WriteEmptyLine();
	
private:
	CLogger(const RFile &aFile);
	void ConstructL();
	
	//TBool iIsConfigured;
	RFile iFile;
	};


class LoggerStatic
	{
private:
	static CLogger* iLogger;
	//static Logger* Instance();
public:
	//static void Configure(const RFile &aFile);
	static void SetLogger(CLogger* aLogger);
	//static CLogger* Logger();
	static void WriteFormat(const TDesC8 &aModule, TRefByValue<const TDesC8> aFmt, ...);
	friend CLogger;
	};


#else
//#define LOG_CONFIGURE(aFile)
#define LOG(aMsg, anArgs...)
#endif

#endif /* LOGGER_H_ */
