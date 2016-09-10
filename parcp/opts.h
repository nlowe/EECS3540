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

    /** Whether or not Quiet mode was enabled */
    bool Quiet = false;

    /** Whether or not to print the usage information and exit */
    bool PrintHelp = false;

    /** Any errors encountered while parsing arguments. If this is an empty string, then no errors were encountered */
    std::string Errors = "";

    Options(int argc, char* argv[])
    {
        Log.Debug("Parsing args");

        if(argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))
        {
            PrintHelp = true;
            return;
        }

        if(argc <= 2)
        {
            Errors = "Incorrect Syntax: Not Enough Arguments";
            return;
        }

        for(int i=1; i < argc; i++)
        {
            auto arg = std::string(argv[i]);
            Log.Trace("Processing " + arg);

            if(arg == "-h" || arg == "--help")
            {
                Log.Trace("Printing Help");
                PrintHelp = true;
                return;
            }
            else if (arg == "-f")
            {
                if(i < argc - 1)
                {
                    SourceFolder = std::string(argv[++i]);
                    Log.Trace("Source Folder set to: " + SourceFolder);
                }
                else
                {
                    Errors += "-f: Not enough arguments remaining for argument\n";
                }
            }
            else if(arg == "-t")
            {
                if(i < argc - 1)
                {
                    DestinationFolder = std::string(argv[++i]);
                    Log.Trace("Destination Folder set to: " + DestinationFolder);
                }
                else
                {
                    Errors += "-t: Not enough arguments remaining for argument\n";
                }
            }
            else if(arg == "-l")
            {
                if (i < argc - 1)
                {
                    auto rawLevel = std::string(argv[++i]);
                    if (!L3::Logger::LevelForName(rawLevel, LoggingLevel)) {
                        Errors += " * -l: Unknown log level " + rawLevel + "\n";
                    }
                    else
                    {
                        LogLevelSet = true;
                    }

                    Log.Trace("LogLevel: Set to " + L3::Logger::NameOfLevel(LoggingLevel));
                } else {
                    Errors += " * -l: Not enough arguments remaining for argument\n";
                }
            }
            else if(arg == "-q")
            {
                Quiet = true;
                Log.Trace("Quiet Mode Enabled");
            }
        }
    }
};

#endif //EECS3540_COPYTREE_OPTS_H
