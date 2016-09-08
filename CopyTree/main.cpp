/*
 * copytree - A program that recursively copies directories
 * Usage:
 *      copytree [-j[N]] [-l <LEVEL>] [-q] <src> <dst>
 *
 * args:
 *      -j[N]       Multithreaded mode. Use up to [N] threads. If [N] is not specified, use as many threads
 *                  as logical cores. Each thread works on one file at a time. If you have slow disks, you
 *                  might not want to enable this option
 *
 *      -l <LEVEL>  Sets the logging level to the specified level, one of TRACE, DEBUG, INFO, WARN, ERROR,
 *                  or FATAL. Once set, only messages logged at or above the specified level will be printed
 *                  to standard output
 *
 *      -q          Quiet Mode, disables all logging. This is equivalent to "-l OFF"
 *
 *      <src>       The source directory to copy from, not including itself. That is, the contents of this
 *                  directory are copied to the destination.
 *
 *      <dst>       The destination directory to copy into. If the last directory in the path does not exist,
 *                  it will be created.
 *
 * -----------------------------------------------------------------------------------------------------
 *
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

#include <iostream>

#include "Logger.h"
#include "opts.h"

void PrintUsage();
void InitCopy(std::string source, std::string dst, uint8_t threads);

// Global Logger for misc functions in this file
L3::Logger Log("main");

int main(int argc, char* argv[])
{
    // Set the default log level. This can be overridden by the user.
    // Set it early so we don't miss any messages
#if defined(COPYTREE_DEFAULT_LOG_TRACE)
    L3::Logger::LogLevel = L3::Level::TRACE;
#elif defined(COPYTREE_DEFAULT_LOG_DEBUG)
    L3::Logger::LogLevel = L3::Level::DEBUG;
#elif defined(COPYTREE_DEFAULT_LOG_INFO)
    L3::Logger::LogLevel = L3::Level::INFO;
#elif defined(COPYTREE_DEFAULT_LOG_WARN)
    L3::Logger::LogLevel = L3::Level::WARN;
#elif defined(COPYTREE_DEFAULT_LOG_ERROR)
    L3::Logger::LogLevel = L3::Level::ERROR;
#elif defined(COPYTREE_DEFAULT_LOG_FATAL)
    L3::Logger::LogLevel = L3::Level::FATAL
#else
    L3::Logger::LogLevel = L3::Level::INFO;
#endif

    Log.Trace("Starting Up");

    auto opts = Options(argc, argv);

    if(!opts.Errors.empty())
    {
        Log.Fatal("Failed to parse arguments:");
        Log.Fatal(opts.Errors);

        Log.Fatal("Usage: \n");
        PrintUsage();

        return -1;
    }

    if(opts.LogLevelSet)
    {
        L3::Logger::LogLevel = opts.LoggingLevel;
    }
    else if(opts.Quiet)
    {
        L3::Logger::LogLevel = L3::Level::OFF;
    }

    InitCopy(opts.SourceFolder, opts.DestinationFolder, opts.Threads);

    Log.Trace("End of Main");
    return 0;
}

void InitCopy(std::string source, std::string dst, uint8_t threads)
{
    Log.Debug("Trying to copy " + source + " to " + dst + " with " + std::to_string(threads) + " threads");
}

void PrintUsage()
{
    std::cout << "copytree - A program that recursively copies directories" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "     copytree [-j[N]] [-l <LEVEL>] [-q] <src> <dst>" << std::endl;
    std::cout << std::endl;
    std::cout << "args" << std::endl;
    std::cout << "     -j[N]       Multithreaded mode. Use up to [N] threads. If [N] is not specified, use as many threads" << std::endl;
    std::cout << "                 as logical cores. Each thread works on one file at a time. If you have slow disks, you" << std::endl;
    std::cout << "                 might not want to enable this option" << std::endl;
    std::cout << std::endl;
    std::cout << "     -l <LEVEL>  Sets the logging level to the specified level, one of TRACE, DEBUG, INFO, WARN, ERROR," << std::endl;
    std::cout << "                 or FATAL. Once set, only messages logged at or above the specified level will be printed" << std::endl;
    std::cout << "                 to standard output" << std::endl;
    std::cout << std::endl;
    std::cout << "     -q          Quiet Mode, disables all logging. This is equivalent to \"-l OFF\"" << std::endl;
    std::cout << std::endl;
    std::cout << "     <src>       The source directory to copy from, not including itself. That is, the contents of this" << std::endl;
    std::cout << "                 directory are copied to the destination." << std::endl;
    std::cout << std::endl;
    std::cout << "     <dst>       The destination directory to copy into. If the last directory in the path does not exist," << std::endl;
    std::cout << "                 it will be created." << std::endl;
}
