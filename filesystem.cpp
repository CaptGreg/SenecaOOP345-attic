// https://en.cppreference.com/w/cpp/filesystem/is_directory

#include <iostream>
#include <fstream>
// #include <cstdio>
#include <cstring>              // strcpy
#include <filesystem>
#include <unistd.h>             // close
#include <sys/socket.h>         // AF_UNIX
#include <sys/un.h>             // struct sockaddr_un addr;
#include <sys/stat.h>           // mkfifo
 
namespace fs = std::filesystem;
 
void demo_status(const fs::path& p, fs::file_status s)
{
    std::cout << p;
    // alternative: switch(s.type()) { case fs::file_type::regular: ...}
    if(fs::is_regular_file(s)) std::cout << " is a regular file\n";
    if(fs::is_directory(s)) std::cout << " is a directory\n";
    if(fs::is_block_file(s)) std::cout << " is a block device\n";
    if(fs::is_character_file(s)) std::cout << " is a character device\n";
    if(fs::is_fifo(s)) std::cout << " is a named IPC pipe\n";
    if(fs::is_socket(s)) std::cout << " is a named IPC socket\n";
    if(fs::is_symlink(s)) std::cout << " is a symlink\n";
    if(!fs::exists(s)) std::cout << " does not exist\n";
}
int main()
{
    // create files of different kinds
    fs::create_directory("sandbox");
    std::ofstream("sandbox/file"); // create regular file
    fs::create_directory("sandbox/dir");
    mkfifo("sandbox/pipe", 0644);
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    std::strcpy(addr.sun_path, "sandbox/sock");
    int fd = socket(PF_UNIX, SOCK_STREAM, 0);
    bind(fd, (struct sockaddr*)&addr, sizeof addr);
    fs::create_symlink("file", "sandbox/symlink");
 
    // demo different status accessors
    for(auto it = fs::directory_iterator("sandbox"); it != fs::directory_iterator(); ++it)
        demo_status(*it, it->symlink_status()); // use cached status from directory entry
    demo_status("dev/null", fs::status("/dev/null")); // direct calls to status
    demo_status("dev/sda", fs::status("/dev/sda"));
    demo_status("sandbox/no", fs::status("/sandbox/no"));
 
    // cleanup
    close(fd);
    fs::remove_all("sandbox");
}
