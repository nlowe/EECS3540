# copytree - Recursive Directory Copy
Still waiting for an oficial project spec, but we believe the first project will be to
recursively copy a directory.

## Usage
```
copytree - A program that recursively copies directories
 Usage:
      copytree [-j[N]] [-l <LEVEL>] [-q] <src> <dst>

 args:
      -j[N]       Multithreaded mode. Use up to [N] threads. If [N] is not specified, use as many threads
                  as logical cores. Each thread works on one file at a time. If you have slow disks, you
                  might not want to enable this option

      -l <LEVEL>  Sets the logging level to the specified level, one of TRACE, DEBUG, INFO, WARN, ERROR,
                  or FATAL. Once set, only messages logged at or above the specified level will be printed
                  to standard output

      -q          Quiet Mode, disables all logging. This is equivalent to "-l OFF"

      <src>       The source directory to copy from, not including itself. That is, the contents of this
                  directory are copied to the destination.

      <dst>       The destination directory to copy into. If the last directory in the path does not exist,
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