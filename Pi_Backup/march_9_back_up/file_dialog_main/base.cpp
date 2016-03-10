#include <iostream>
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <stdio.h>
#include <stdlib.h>
#include "examplewindow.h"
#include <gtkmm/main.h>

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    char buffer[4096];
    long size;
    long size_remainder;
    size_t result = 0;
    string dest_name = "/home/jess/MusicLib/";
    string filename = "";

    Gtk::Main kit(argc, argv);

    ExampleWindow window;
    //Shows the window and returns when it is closed.
    Gtk::Main::run(window);

    filename = window.GetFilename();

    fp1 = fopen(filename.c_str(), "rb");
    
    size_t found = filename.find_last_of("/");
    dest_name += filename.substr(found + 1);
    
    fp2 = fopen(dest_name.c_str(), "w+b");

    
    if(fp1 == NULL)
    {
        cout << "File error " << stderr << endl;
        exit(1);
    }

    fseek (fp1, 0, SEEK_END);
    size = (ftell (fp1)) / 4096;
    size_remainder = (ftell (fp1)) % 4096;
    rewind(fp1);
    
    if(buffer == NULL)
    {
        cout << "Memory error " << stderr << endl;
        exit(2);
    }

    for (int i = 0; i < size; i++)
    {
        result += fread(buffer, 1, 4096, fp1);
        fwrite(buffer, sizeof(char), 4096, fp2);
    }

    result += fread(buffer, 1, size_remainder, fp1);
    fwrite(buffer, sizeof(char), size_remainder, fp2);
    
    if(result != (ftell(fp1)))
    {
        cout << "Reading error " << stderr << endl;
        exit(3);
    }

    fclose(fp1);
    fclose(fp2);
    
    return 0;
}