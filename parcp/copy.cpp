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

#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <queue>
#include <fcntl.h>
#include "copy.h"
#include "Logger.h"
#include "util.h"

#define COPY_BUFFER_SIZE 8192

namespace Copy
{
    L3::Logger Log("Copy");

    std::string ModeName(mode_t mode)
    {
        if((mode & S_IFMT) == S_IFREG) return "RegularFile";
        if((mode & S_IFMT) == S_IFDIR) return "Directory";
        if((mode & S_IFMT) == S_IFCHR) return "CharacterSpecialDevice";
        if((mode & S_IFMT) == S_IFBLK) return "BlockDevice";
        if((mode & S_IFMT) == S_IFLNK) return "SymbolicLink";
        if((mode & S_IFMT) == S_IFIFO) return "FIFO";
        if((mode & S_IFMT) == S_IFSOCK) return "UnixDomainSocket";

        return "UNKNOWN";
    }

    bool IsDirectory(dirent* details) { return details->d_type == DT_DIR; }

    bool CopyFile(std::string source, std::string dest, struct stat info)
    {
        auto myPid = getpid();

        Log.Info("[" + std::to_string(myPid) + "] '" + source + "' --> '" + dest + "'");

        int readerFD = open(source.c_str(), O_RDONLY);
        if(readerFD < 0)
        {
            Log.Fatal("[" + std::to_string(myPid) + "] Could not open file for read " + source);
            return false;
        }

#ifndef NO_POSIX_ADVISE
        // Hint to the kernel that we're only sequentially reading
        posix_fadvise(readerFD, 0, 0, POSIX_FADV_SEQUENTIAL);
#endif

        int writerFD = open(dest.c_str(), O_CREAT | O_WRONLY, info.st_mode);
        if(writerFD < 0)
        {
            Log.Fatal("[" + std::to_string(myPid) + "] Could not open file for write " + dest);
            return false;
        }

        bool error = false;
        char buf[COPY_BUFFER_SIZE];
        ssize_t bytesRead;

        while((bytesRead = read(readerFD, &buf[0], sizeof(buf))) != 0)
        {
            ssize_t bytesWritten = write(writerFD, &buf[0], (size_t) bytesRead);

            if(bytesRead != bytesWritten)
            {
                Log.Error("[" + std::to_string(myPid) + "] Falure in copying " + std::to_string(bytesRead) + " to destination. Actually wrote " + std::to_string(bytesWritten));
                error = true;
            }
        }

        close(readerFD);
        close(writerFD);

        return !error;
    }

    bool TryCreateDirectory(std::string dir, mode_t mode)
    {
        auto normailizedPath = dir;
        if(util::StringEndsWith(normailizedPath, '/')) normailizedPath = normailizedPath.substr(0, normailizedPath.length()-1);

        Log.Trace("[" + std::to_string(getpid()) + "] Trying to create directory " + normailizedPath);
        auto result = mkdir(normailizedPath.c_str(), mode);

        if(result != 0)
        {
            if(errno != EEXIST)
            {
                Log.Fatal("[" + std::to_string(getpid()) + "] Unable to create directory: Error code " + std::to_string(errno));
                return false;
            }
            else
            {
                Log.Trace(normailizedPath + " already exists");
            }
        }

        return true;
    }

    int BeginCopy(std::string parcpPath, std::string source, std::string dest)
    {
        auto myPid = getpid();

        struct stat rootStat;
        lstat(source.c_str(), &rootStat);

        // Try to create the directory if it doesn't exist, with the same mode as the source
        if (!TryCreateDirectory(dest, rootStat.st_mode))
        {
            return -1;
        }

        Log.Trace("[" + std::to_string(myPid) + "] Begin Copy To '" + dest + "' - Scanning " + source);

        DIR* root = nullptr;

        root = opendir(source.c_str());

        if(root == nullptr)
        {
            Log.Fatal("[" + std::to_string(myPid) + "] Unable to open directory " + source);
            return -1;
        }

        struct dirent* details;
        struct stat file;
        std::string path;

        bool error = false;

        while((details = readdir(root)) != nullptr)
        {
            path = source + details->d_name;
            lstat(path.c_str(), &file);

            Log.Trace("[" + std::to_string(myPid) + "] INODE: " + std::to_string(details->d_ino) + ", A " + ModeName(file.st_mode) + ": " + path);

            auto newDest = dest + details->d_name;

            if(IsDirectory(details))
            {
                if(strcmp(details->d_name, ".") == 0 || strcmp(details->d_name, "..") == 0)
                {
                    Log.Trace("[" + std::to_string(myPid) + "] Skipping special entry " + std::string(details->d_name));
                    continue;
                }

                // Fork and spawn new process and remember child pid
                auto pid = fork();
                if(pid == 0)
                {
                    char* args[] = {
                            const_cast<char*>(parcpPath.c_str()),
                            (char*)"-__forked", // Signal that this is a forked process. This disables early logging
                            (char*)"-l", const_cast<char*>(L3::Logger::NameOfLevel(L3::GlobalLogLevel).c_str()),
                            (char*)"-f", const_cast<char*>(path.c_str()),
                            (char*)"-t", const_cast<char*>(newDest.c_str()),
                            NULL
                    };

                    execv(parcpPath.c_str(), args);
                    Log.Fatal("Unable to start child process");
                }
                else
                {
                    Log.Debug("[" + std::to_string(myPid) + "] Spawned child process " + std::to_string(pid) + " for " + path + " (copying to " + newDest + ")");
                }
            }
            else
            {
                if(!CopyFile(path, newDest, file)) error = true;
            }
        }

        //Wait all child pids
        Log.Debug("[" + std::to_string(myPid) + "] Waiting for child processes to finish");

        __pid_t child;
        int status;
        while ((child = waitpid(-1, &status, 0)))
        {
            if(errno == ECHILD) break;
            if(status != 0)
            {
                Log.Error("[" + std::to_string(myPid) + "] Child process " + std::to_string(child) + " exited with code " + std::to_string(status));
            }
        }

        closedir(root);

        return error ? -1 : 0;
    }
}

