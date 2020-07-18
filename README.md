# Logging library for Symbian

## Usage

```
CLogger *logger = CLogger::NewL(file, CLogger::ELevelAll & ~CLogger::ELevelDebug); // Prints all messages except Debug
CleanupStack::PushL(logger);
LoggerStatic::SetLogger(logger);

LOG(_L8("Simple message (uncategorized)"));
DEBUG(_L8("This is debug message")); // This kind of message disabled in logger settings
INFO(_L8("This is info message"));
WARNING(_L8("This is warning message"));
_LIT8(KErrTxt, "Not Found");
ERROR(_L8("This is error message with code: %d (%S)"), 404, &KErrTxt);

CleanupStack::PopAndDestroy(logger);

LOG(_L8("Logger has been destroyed and this message didn`t be included to the log file :("));
```

## Limitations
* Only 8-bit descriptors allowed
* Max strings length (with formatting) - 4 Kb