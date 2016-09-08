# parcp - Recursive Parallel Directory Copy
A forking file copy program that recursively copies the contents of one directory to another

## Usage
```
parcp - A program that recursively copies directories
 Usage:
      parcp [-l <LEVEL>] [-q] <src> <dst>

 args:
      -l <LEVEL>  Sets the logging level to the specified level, one of TRACE, DEBUG, INFO, WARN, ERROR,
                  or FATAL. Once set, only messages logged at or above the specified level will be printed
                  to standard output

      -q          Quiet Mode, disables all logging. This is equivalent to "-l OFF"

      -f <src>    The source directory to copy from, not including itself. That is, the contents of this
                  directory are copied to the destination.

      -t <dst>    The destination directory to copy into. If the last directory in the path does not exist,
                  it will be created.
```

## License

### The MIT License
Copyright © `2016` `Nathan Lowe`

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the “Software”), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.