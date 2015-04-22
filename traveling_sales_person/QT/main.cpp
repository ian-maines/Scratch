//************************************************************
// Copyright 2015 Ian Maines
//
// Description:     main.cpp for traveling sales person.
// Date:            04/21/2015
//
//************************************************************

// C++ headers
#include <iostream>
#include <chrono>
#include <thread>

// My headers.
#include "thread_base.h"

using namespace std;

namespace
{
void preamble ()
    {
    cout << "***************************************" << endl
         << "* Copyright 2015 Ian Maines           *" << endl
         << "* Traveling sales person C++ solution *" << endl
         << "***************************************" << endl;
    return;
    }
}

int main(int argc, char* argv[])
    {
    // Print out our preamble
    preamble ();


    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    return 0;
    }

