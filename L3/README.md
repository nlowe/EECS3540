# L3 - Lightweight Logging Library
This is a really, really simple logging library for quick, organized, and
tiered output. I wouldn't use it for anything serious

## Usage
Everything writes to `std::cout`.

To change the global log level simply set `L3::Logger::LogLevel` to the appropriate value.

Example:

```c++
#include "L3/Logger.h"

L3::Logger Log("MyComponentName")

Log.Trace("A Trace Message"); // [TRACE] [MyComponentName] A Trace Message
Log.Debug("A Debug Message"); // [DEBUG] [MyComponentName] A Debug Message
Log.Info("An Info Message"); // [INFO] [MyComponentName] An Info Message
Log.Warn("A Warn Message"); // [WARN] [MyComponentName] A Warn Message
Log.Error("An Error Message"); // [ERROR] [MyComponentName] An Error Message
Log.Fatal("A Fatal Message"); // [FATAL] [MyComponentName] A Fatal Message
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