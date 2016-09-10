/*
 * parcp - A program that recursively copies directories
 * Usage:
 *      parcp [-j[N]] [-l <LEVEL>] [-q] -f <src> -t <dst>
 *
 * args:
 *      -h,--help   Prints this help message
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
#include "util.h"
#include "copy.h"

void PrintUsage();
int InitCopy(std::string parcpPath, std::string source, std::string dst);

// Global Logger for misc functions in this file
L3::Logger Log("main");

int main(int argc, char* argv[])
{
    Log.Trace("Starting Up (L3::GlobalLogLevel is " + L3::Logger::NameOfLevel(L3::GlobalLogLevel) + ")");

    auto opts = Options(argc, argv);

    if(!opts.Errors.empty())
    {
        Log.Fatal("Failed to parse arguments:");
        Log.Fatal(opts.Errors);

        Log.Fatal("Usage: \n");
        PrintUsage();

        return -1;
    }

    if(opts.PrintHelp)
    {
        PrintUsage();
        return 0;
    }

    if(opts.LogLevelSet)
    {
        L3::GlobalLogLevel = opts.LoggingLevel;
    }
    else if(opts.Quiet)
    {
        L3::GlobalLogLevel = L3::Level::OFF;
    }

    auto result = InitCopy(std::string(argv[0]), opts.SourceFolder, opts.DestinationFolder);

    Log.Trace("End of Main, exiting with " + std::to_string(result));
    return result;
}

int InitCopy(std::string parcpPath, std::string source, std::string dst)
{
    Log.Debug("Trying to copy " + source + " to " + dst);

    Log.Trace("Validating source location (" + source + ")");
    if(!util::DirectoryExists(source))
    {
        Log.Fatal(source + " does not exist or is not a directory");
        return -1;
    }

    // TODO: Validate Destination Location

    return Copy::BeginCopy(parcpPath, source, dst);
}

void PrintUsage()
{
    std::cout << "parcp - A program that recursively copies directories" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "     parcp [-l <LEVEL>] [-q] <src> <dst>" << std::endl;
    std::cout << std::endl;
    std::cout << "args" << std::endl;
    std::cout << "     -h,--help   Prints this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "     -l <LEVEL>  Sets the logging level to the specified level, one of TRACE, DEBUG, INFO, WARN, ERROR," << std::endl;
    std::cout << "                 or FATAL. Once set, only messages logged at or above the specified level will be printed" << std::endl;
    std::cout << "                 to standard output" << std::endl;
    std::cout << std::endl;
    std::cout << "     -q          Quiet Mode, disables all logging. This is equivalent to \"-l OFF\"" << std::endl;
    std::cout << std::endl;
    std::cout << "     -f <src>    The source directory to copy from, not including itself. That is, the contents of this" << std::endl;
    std::cout << "                 directory are copied to the destination." << std::endl;
    std::cout << std::endl;
    std::cout << "     -t <dst>    The destination directory to copy into. If the last directory in the path does not exist," << std::endl;
    std::cout << "                 it will be created." << std::endl;
}
