/*
 * parcp - A program that recursively copies directories
 * Usage:
 *      parcp [-l <LEVEL>] [-q] -f <src> -t <dst>
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
 *      -f <src>    The source directory to copy from, not including itself. That is, the contents of this
 *                  directory are copied to the destination.
 *
 *      -t <dst>    The destination directory to copy into. If the last directory in the path does not exist,
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

// Forward declare these so main can be first
void PrintUsage();
int InitCopy(std::string source, std::string dst);

// Global Logger for misc functions in this file
L3::Logger Log("main");

/** Whether or not the process was forked. This disables logging until the copy process resumes */
bool isForkedProcess = false;

int main(int argc, char* argv[])
{
    Log.Trace("Starting Up (L3::GlobalLogLevel is " + L3::Logger::NameOfLevel(L3::GlobalLogLevel) + ")");

    // Parse command line options
    Options::CommandLineArgs.parse(argc, argv);

    // Are we forked?
    isForkedProcess = Options::CommandLineArgs.IsForked;

    // Arg errors?
    if(!Options::CommandLineArgs.Errors.empty())
    {
        Log.Fatal("Failed to parse arguments:");
        Log.Fatal(Options::CommandLineArgs.Errors);

        Log.Fatal("Usage: \n");
        PrintUsage();

        return -1;
    }

    // Was the help flag provided?
    if(Options::CommandLineArgs.PrintHelp)
    {
        PrintUsage();
        return 0;
    }

    // Set the log level appropriately
    if(Options::CommandLineArgs.LogLevelSet)
    {
        L3::GlobalLogLevel = Options::CommandLineArgs.LoggingLevel;
    }
    else if(Options::CommandLineArgs.Quiet)
    {
        L3::GlobalLogLevel = L3::Level::OFF;
    }

    // Copy all the things
    auto result = InitCopy(Options::CommandLineArgs.SourceFolder, Options::CommandLineArgs.DestinationFolder);

    if(!isForkedProcess) Log.Trace("End of Main, exiting with " + std::to_string(result));
    return result;
}

/**
 * Perform some validation and begin the copy process to copy the specified source directory to the specified
 * destination recursively
 *
 * @param source the directory to copy
 * @param dst the directory to copy to
 * @return 0 iff the operation was successfull
 */
int InitCopy(std::string source, std::string dst)
{
    // Make sure the paths end with a '/'
    if(!util::StringEndsWith(source, '/')) source = source + '/';
    if(!util::StringEndsWith(dst, '/')) dst = dst + '/';

    if(!isForkedProcess) Log.Debug("Trying to copy " + source + " to " + dst);

    // Make sure the source directory exists
    if(!isForkedProcess) Log.Trace("Validating source location (" + source + ")");
    if(!util::DirectoryExists(source))
    {
        Log.Fatal(source + " does not exist or is not a directory");
        return -1;
    }

    return Copy::BeginCopy(source, dst);
}

/**
 * Prints the usage information for the program
 */
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
