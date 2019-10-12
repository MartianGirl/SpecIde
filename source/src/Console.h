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

#pragma once

/** Console
 *
 */

#include <list>
#include <string>

#ifdef USE_BOOST_THREADS
#include <boost/thread.hpp>
#include <boost/mutex.hpp>
#else
#include <thread>
#include <mutex>
#endif

class Console {

    public:
#ifdef USE_BOOST_THREADS
        boost::mutex &consoleMutex;
#else
        std::mutex &consoleMutex;
#endif
        std::list<std::string> cmdList;

        Console();
        void operator()();
        std::string get();
};

class ConsoleThread {

    public:
        Console console;
#ifdef USE_BOOST_THREADS
        boost::thread consoleThread;
#else
        std::thread consoleThread;
#endif

        ConsoleThread();
        ~ConsoleThread();
};

// vim: et:sw=4:ts=4
