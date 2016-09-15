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

/**
 * A model for all possible arguments passed on the command line
 */
class Options
{
private:
    L3::Logger Log{"Options"};
public:
    /** The options passed on the command line to the program */
    static Options CommandLineArgs;

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

    /** Whether or not the process was "forked", which just disables logging until the copy function */
    bool IsForked = false;

    /** The path to the program, used to fork new copies */
    std::string ProgramPath;

    /**
     * Parses the specified arguments
     * @param argc the number of arguments passed
     * @param argv an array of c-strings containing the arguments
     */
    void parse(int argc, char* argv[]);
};

#endif //EECS3540_COPYTREE_OPTS_H
