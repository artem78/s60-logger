# Logging library for Symbian

## Usage

```
CLogger *logger = CLogger::NewL(file, CLogger::ELevelAll & ~CLogger::ELevelDebug); // Prints all messages except Debug
CleanupStack::PushL(logger);
LoggerStatic::SetLogger(logger);

// You may use both 16 or 8-bit descriptors
LOG(_L16("Simple message in 16bit descriptor (uncategorized)"));
// or
LOG(_L8("Simple message in 8bit descriptor (uncategorized)"));

DEBUG(_L("This is debug message")); // This kind of message disabled in logger settings
INFO(_L("This is info message"));
WARNING(_L("This is warning message"));
_LIT8(KErrTxt, "Not Found");
ERROR(_L("This is error message with code: %d (%S)"), 404, &KErrTxt);

CleanupStack::PopAndDestroy(logger);

LOG(_L("Logger has been destroyed and this message didn`t be included to the log file :("));
```

## Limitations
* Max strings length (with formatting) - 4 Kb