#include <iostream>
#include <stdio.h>
#include "file2disk.h"


int main() {

    // FILE *fp = fopen("test.txt", "ae");

    // const char* data = "write to file\n";
    // fwrite_unlocked(data,1, std::string(data).size(), fp);
    // fwrite_unlocked(data,1, std::string(data).size(), fp);

    // fclose(fp);

    WriteFile file("test.txt");
    std::string log_line = "2021-04-16 server write log";
    file.append(log_line.c_str(), log_line.size());

    return 0;
}