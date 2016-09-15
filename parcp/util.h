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

#ifndef EECS3540_UTIL_H
#define EECS3540_UTIL_H

#include <string>

namespace util
{
    /**
     * Checks to see if the specified path exists and is a directory
     *
     * @param dir the path to check
     * @return true iff the path exists and is a directory
     */
    bool DirectoryExists(std::string dir);

    /**
     * Checks to see if the specified input string ends with the specified character
     * @param input the string to check
     * @param c the character to check for
     * @return true iff the input string ends with the specified character
     */
    inline bool StringEndsWith(std::string input, const char c)
    {
        return input.back() == c;
    }
}
#endif //EECS3540_UTIL_H
