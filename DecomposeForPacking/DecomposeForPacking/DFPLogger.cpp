#include "DFPLogger.h"

#pragma region - Lifecycle methods

DFPLogger::~DFPLogger() {
}

/** Singleton instance of the logger */
DFPLogger& DFPLogger::getInstance() {
    
    /** Singleton instance of the logger - created on first use and guranteed to be destroyred */
    static DFPLogger _instance;
    return _instance;
}

#pragma region - Logic methods

/** Log the given message. The message may be filtered depending on the current settings of the logger. */
void DFPLogger::log(string errorMessage, DFPLogger::LogLevel severity) {
    
    if (severity >= _loggerSeverity) {
        
        string msgTitle;
        
        switch (severity) {
            case (ERROR): msgTitle = "Error: "; break;
            case (WARNING): msgTitle = "Warning: "; break;
            case (INFO): msgTitle = "Info: "; break;
            case (DEBUG_MODE): msgTitle = "Debug: "; break;
        }
        
        std::cout << msgTitle << errorMessage << std::endl;
    }
}

#pragma region - Getters & Setters

/** This state determines which log messages are filtered */
DFPLogger::LogLevel DFPLogger::loggerSeverity() const {
    return _loggerSeverity;
}

/** This state determines which log messages are filtered */
void DFPLogger::loggerSeverity(DFPLogger::LogLevel newLevel) {
    _loggerSeverity = newLevel;
}