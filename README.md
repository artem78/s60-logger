# Logging library for Symbian

## Usage example

```c++
RFs fs;
User::LeaveIfError(fs.Connect());
CleanupClosePushL(fs);

_LIT(KLogFile, "c:\\log.txt");
RFile file;
file.Replace(fs, KLogFile, EFileWrite);
CleanupClosePushL(file);

CLogger *logger = CLogger::NewL(file, CLogger::ELevelAll & ~CLogger::ELevelDebug); // Prints all messages except Debug
CleanupStack::PushL(logger);
LoggerStatic::SetLogger(logger); // Set default logger

// You may use either 16 or 8-bit descriptors
LOG(_L16("Simple message in 16bit descriptor (uncategorized)"));
// or
LOG(_L8("Simple message in 8bit descriptor (uncategorized)"));

DEBUG(_L("This is debug message")); // This kind of message currently disabled in logger settings
INFO(_L("This is info message"));
WARNING(_L("This is warning message"));
_LIT(KErrTxt, "Not Found");
ERROR(_L("This is formatted error message with code: %d (%S)"), 404, &KErrTxt);

CleanupStack::PopAndDestroy(3, &fs); // logger + file + fs

LOG(_L("Logger already has been destroyed and this message won`t be included to the log file :("));
```

c:\log.txt file:
```
20:12:50.176	[LoggerDemo()]   Simple message in 16bit descriptor (uncategorized)
20:12:50.176	[LoggerDemo()]   Simple message in 8bit descriptor (uncategorized)
20:12:50.176	(Info)	[LoggerDemo()]   This is info message
20:12:50.176	(Warning)	[LoggerDemo()]   This is warning message
20:12:50.176	(Error)	[LoggerDemo()]   This is formatted error message with code: 404 (Not Found)
```

## Limitations
* Max strings length (with formatting) - 4 Kb
