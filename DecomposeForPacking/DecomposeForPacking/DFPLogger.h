#pragma once

#include <memory>
#include <iostream>
#include <string>

using std::string;
using std::shared_ptr;

/** This logger is used by the Decompose for Packing program to report error and info messages */
class DFPLogger {
    
public:
    
    enum LogLevel: int {
		ERROR = 3,
		WARNING = 2,
		INFO = 1,
		DEBUG_MODE = 0
    };
    
    static DFPLogger& getInstance();
    virtual ~DFPLogger();
    
    /** Log the given message. The message may be filtered depending on the current settings of the logger. */
    void log(string errorMessage, LogLevel severity);
    
    /** Getters & Setters */
    LogLevel loggerSeverity() const;
    void loggerSeverity(LogLevel newLevel);
    
private:
    
    /** Singleton ctors & operator overloading */
    DFPLogger() {};
    DFPLogger(DFPLogger const&);      // No implementation (enforce a single instance)
    void operator=(DFPLogger const&); // No implementation (enforce a single instance)
    
    /** This state determines which log messages are filtered */
    LogLevel _loggerSeverity;
};