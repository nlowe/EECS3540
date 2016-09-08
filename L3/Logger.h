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

    enum Level {TRACE, DEBUG, INFO, WARN, ERROR, FATAL, OFF};

    class Logger {
    public:
        Logger(std::string scope) : scope(scope){}

        void Log(Level level, std::string msg);

        inline void Trace(std::string msg) { Log(Level::TRACE, msg); }
        inline void Debug(std::string msg) { Log(Level::DEBUG, msg); }
        inline void Info(std::string msg)  { Log(Level::INFO, msg);  }
        inline void Warn(std::string msg)  { Log(Level::WARN, msg);  }
        inline void Error(std::string msg) { Log(Level::ERROR, msg); }
        inline void Fatal(std::string msg) { Log(Level::FATAL, msg); }

        static L3::Level LogLevel;

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
            }

            assert(false);
            return "UNKNOWN";
        }

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
