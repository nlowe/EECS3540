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
#include "copy.h"
#include "Logger.h"
#include "util.h"

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

    bool CopyFile(std::string source, std::string dest)
    {
        Log.Info("[" + std::to_string(getpid()) + "] " + source);

        return true;
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

        std::queue<int> childPids;

        bool error = false;

        while((details = readdir(root)) != nullptr)
        {
            path = source + details->d_name;
            lstat(path.c_str(), &file);

            Log.Trace("[" + std::to_string(myPid) + "] INODE: " + std::to_string(details->d_ino) + ", A " + ModeName(file.st_mode) + ": " + path);

            if(IsDirectory(details))
            {
                if(strcmp(details->d_name, ".") == 0 || strcmp(details->d_name, "..") == 0)
                {
                    Log.Trace("[" + std::to_string(myPid) + "] Skipping special entry " + std::string(details->d_name));
                    continue;
                }

                auto newDest = dest + details->d_name;

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
                    childPids.push(pid);
                    Log.Debug("[" + std::to_string(myPid) + "] Spawned child process " + std::to_string(pid) + " for " + path + " (copying to " + newDest + ")");
                }
            }
            else
            {
                if(!CopyFile(path, dest)) error = true;
            }
        }

        //Wait all child pids
        while(!childPids.empty())
        {
            auto pid = childPids.front();

            Log.Debug("[" + std::to_string(myPid) + "] Waiting for child process " + std::to_string(pid) + " to finish");

            int status;
            if(waitpid(pid, &status, 0) == pid)
            {
                if(status != 0) error = true;
            }
            else
            {
                Log.Fatal("[" + std::to_string(myPid) + "] Couldn't wait on pid " + std::to_string(pid));
                error = true;
            }

            childPids.pop();
        }

        closedir(root);

        return error ? -1 : 0;
    }
}

