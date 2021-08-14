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

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::ref;
using std::string;
#ifdef USE_BOOST_THREADS
using boost::lock_guard;
using boost::mutex;
#else
using std::lock_guard;
using std::mutex;
#endif

ConsoleThread::ConsoleThread() :
    consoleThread(ref(console)) {
}

ConsoleThread::~ConsoleThread() {

    cout << "Console shutting down..." << endl;
    consoleThread.detach();
}

void Console::operator()() {

    string cmd;

    for (;;) {
        getline(cin, cmd);
        if (!cmd.empty()) {
            lock_guard<mutex> lock(consoleMutex);
            cmdList.push_back(cmd);
        }
    }
}

string Console::get() {

    string cmd;

    lock_guard<mutex> lock(consoleMutex);
    if (!cmdList.empty()) {
        cmd = cmdList.front();
        cmdList.pop_front();
    }
    return cmd;
}

// vim: et:sw=4:ts=4
