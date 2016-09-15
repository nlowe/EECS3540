/*
 * Copyright (c) 2016 Nathan Lowe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef EECS3540_L3_LOGGER_H
#define EECS3540_L3_LOGGER_H

#include <string>
#include <assert.h>
#include <mutex>
#include <algorithm>

namespace L3 {

    /** Represents a logging level */
    enum Level {
        /** The most verbose logging level */
        TRACE,
        /** A level inbetween info and trace, used for debugging */
        DEBUG,
        /** A standard logging level for general messages*/
        INFO,
        /** A logging level used for warnings */
        WARN,
        /** A logging level used for recoverable errors */
        ERROR,
        /** A logging level used for non-recoverable errors */
        FATAL,
        /** Specifies that the logger should be disabled */
        OFF
    };

    /** The level all loggers will log at */
    extern Level GlobalLogLevel;

    /**
     * A general purpose class for writing log messages to standard output. The output is thread safe but will
     * block on logging calls until standard out becomes available.
     */
    class Logger {
    public:
        /**
         * Construct a logger with the specified scope
         * @param scope the scope of the logger
         * @return a new Logger Object
         */
        Logger(std::string scope) : scope(scope){}

        /**
         * Write the specified message to standard out at the specified level. If the logger is not configured to log
         * at or below this level, or the logger is disabled, the message will not be emitted and the call returns
         * immediately. Note that fatal messages will always be emitted.
         *
         * @param level The level of the message
         * @param msg The message to log
         */
        void Log(Level level, std::string msg);

        /**
         * Write the specified message at the TRACE level to standard output
         *
         * @param msg the message to write
         */
        inline void Trace(std::string msg) { Log(Level::TRACE, msg); }
        /**
         * Write the specified message at the DEBUG level to standard output
         *
         * @param msg the message to write
         */
        inline void Debug(std::string msg) { Log(Level::DEBUG, msg); }
        /**
         * Write the specified message at the INFO level to standard output
         *
         * @param msg the message to write
         */
        inline void Info(std::string msg)  { Log(Level::INFO, msg);  }
        /**
         * Write the specified message at the WARN level to standard output
         *
         * @param msg the message to write
         */
        inline void Warn(std::string msg)  { Log(Level::WARN, msg);  }
        /**
         * Write the specified message at the ERROR level to standard output
         *
         * @param msg the message to write
         */
        inline void Error(std::string msg) { Log(Level::ERROR, msg); }
        /**
         * Write the specified message at the TRACE level to standard output
         *
         * @param msg the message to write
         */
        inline void Fatal(std::string msg) { Log(Level::FATAL, msg); }

        /**
         * Gets the name of the specified level as a string
         *
         * @param l the level to get
         * @return the name of the level, as a string
         */
        static std::string NameOfLevel(Level l)
        {
            switch(l)
            {
                case Level::TRACE: return "TRACE";
                case Level::DEBUG: return "DEBUG";
                case Level::INFO:  return "INFO";
                case Level::WARN:  return "WARN";
                case Level::ERROR: return "ERROR";
                case Level::FATAL: return "FATAL";
                case Level::OFF:   return "OFF";
            }

            assert(false);
            return "UNKNOWN";
        }

        /**
         * Gets the specified level by name (case insensitive)
         *
         * @param rawLevel the level to try to get
         * @param level the result of the operation
         * @return true iff the specified level was found
         */
        static bool LevelForName(std::string rawLevel, L3::Level& level)
        {
            std::transform(rawLevel.begin(), rawLevel.end(), rawLevel.begin(), ::tolower);

            if(rawLevel == "trace")
            {
                level = L3::Level::TRACE;
            }
            else if(rawLevel == "debug")
            {
                level = L3::Level::DEBUG;
            }
            else if(rawLevel == "info")
            {
                level = L3::Level::INFO;
            }
            else if(rawLevel == "warn")
            {
                level = L3::Level::WARN;
            }
            else if(rawLevel == "error")
            {
                level = L3::Level::ERROR;
            }
            else if(rawLevel == "fatal")
            {
                level = L3::Level::FATAL;
            }
            else if(rawLevel == "off")
            {
                level = L3::Level::ERROR;
            }
            else
            {
                return false;
            }

            return true;
        }
    private:
        std::string scope;
        static std::mutex output_lock;
    };
}

#endif //EECS3540_L3_LOGGER_H
