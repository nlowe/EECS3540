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

#ifndef EECS3540_COPYTREE_OPTS_H
#define EECS3540_COPYTREE_OPTS_H

#include <algorithm>
#include <string>
#include <thread>
#include "Logger.h"

class Options
{
private:
    L3::Logger Log{"Options"};
public:
    /** The Logging Level to set */
    L3::Level LoggingLevel = L3::Level::INFO;
    /** Whether or not the log level was changed */
    bool LogLevelSet = false;

    /** The Source Folder to copy from */
    std::string SourceFolder;
    /** The Destination Folder to copy to */
    std::string DestinationFolder;

    /** The max number of threads to create. If set to zero, use the number of logical processors of the system */
    uint8_t Threads = 1;

    /** Whether or not Quiet mode was enabled */
    bool Quiet = false;

    std::string Errors = "";

    Options(int argc, char* argv[])
    {
        Log.Debug("Parsing args");

        if(argc <= 2)
        {
            Errors = "Incorrect Syntax: Not Enough Arguments";
            return;
        }

        for(int i=1; i < argc; i++)
        {
            auto arg = std::string(argv[i]);
            Log.Trace("Processing " + arg);

            if (i == argc-2)
            {
                SourceFolder = arg;
            }
            else if(i == argc-1)
            {
                DestinationFolder = arg;
            }
            else if(arg.compare(0, 2, "-j") == 0)
            {
                Threads = arg.length() == 2 ? std::thread::hardware_concurrency() : (uint8_t)stoul(arg.substr(2));
                Log.Trace("Threads: Set to " + std::to_string(Threads));
            }
            else if(arg == "-l")
            {
                if (i < argc - 1)
                {
                    auto rawLevel = std::string(argv[++i]);
                    if (!L3::Logger::LevelForName(rawLevel, LoggingLevel)) {
                        Errors += " * -l: Unknown log level " + rawLevel + "\n";
                    }

                    Log.Trace("LogLevel: Set to " + L3::Logger::NameOfLevel(LoggingLevel));
                } else {
                    Errors += " * -l: Not enough arguments remaining for argument\n";
                }
            }
        }
    }
};

#endif //EECS3540_COPYTREE_OPTS_H
