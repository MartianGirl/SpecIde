/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2019.
 *
 * SpecIde is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * SpecIde is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SpecIde.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Console.h"

#include <iostream>

#ifdef USE_BOOST_THREADS
boost::mutex theMutex;
#else
std::mutex theMutex;
#endif

using namespace std;

ConsoleThread::ConsoleThread() :
    consoleThread(std::ref(console)) {
}

ConsoleThread::~ConsoleThread() {

    cout << "Console shutting down..." << endl;
    consoleThread.detach();
}

Console::Console() :
    consoleMutex(theMutex) {
}

void Console::operator()() {

    string cmd;

    for (;;) {
        getline(cin, cmd);
        if (!cmd.empty()) {
            consoleMutex.lock();
            cmdList.push_back(cmd);
            consoleMutex.unlock();
        }
    }
}

string Console::get() {

    string cmd;

    consoleMutex.lock();
    if (!cmdList.empty()) {
        cmd = cmdList.front();
        cmdList.pop_front();
    }
    consoleMutex.unlock();
    return cmd;
}

// vim: et:sw=4:ts=4
