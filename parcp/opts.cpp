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

#include "opts.h"

Options Options::CommandLineArgs;

void Options::parse(int argc, char* argv[])
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

    ProgramPath = std::string(argv[0]);

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
        else if(arg == "-__forked")
        {
            IsForked = true;
        }
    }
}