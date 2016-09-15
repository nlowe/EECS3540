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

#include <iostream>
#include "Logger.h"

std::mutex L3::Logger::output_lock;
L3::Level L3::GlobalLogLevel(L3::Level::INFO);

/**
 * Write the specified message to standard out at the specified level. If the logger is not configured to log
 * at or below this level, or the logger is disabled, the message will not be emitted and the call returns
 * immediately. Note that fatal messages will always be emitted.
 *
 * @param level The level of the message
 * @param msg The message to log
 */
void L3::Logger::Log(L3::Level level, std::string msg) {
    // Log if the level is FATAL, or if the logger isn't disabled and the specified level is acceptable
    if (level == L3::Level::FATAL || (level != L3::Level::OFF && L3::GlobalLogLevel <= level))
    {
        output_lock.lock();
        std::cout << "[" << NameOfLevel(level) << "] [" << scope << "] " << msg << std::endl;
        output_lock.unlock();
    }
}
