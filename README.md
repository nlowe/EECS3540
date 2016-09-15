# EECS 3540
Projects, Code, and Samples for EECS3540 - Operating Systems and Systems Programming
for the Fall 2016 Semester at the University of Toledo

## Projects
* [`L3`](L3/README.md) - A lightweight Logging Library for general logging in all projects
* [`parcp`](parcp/README.md) - Recursive, parallel directory copy - [Project Spec](Assignments/01_Parallel_File_Copy.pdf)

## Building
All projects are a part of the `EECS3540` cmake project. You can build all of
them like any other `cmake` project:

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
```

> Note that these projects are designed to be run on a `UNIX`-like operating system,
  meaning you'll need access to standard UNIX headers and libraries such as `pthread`

## License
All projects are licensed under the `MIT` license unless otherwise stated

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
