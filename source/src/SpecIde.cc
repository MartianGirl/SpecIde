/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2021.
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

#include <algorithm>
#include <fstream>
#include <memory>

#include "SpecIde.h"
#include "SpeccyScreen.h"
#include "CpcScreen.h"

#include "config.h"

using namespace std;

/** Map of arguments to option name-value pairs. */
map<string, Option> arguments = {
    // Model selection
    {"--issue2",        {"model", "issue2"}},
    {"--issue3",        {"model", "issue3"}},
    {"--48",            {"model", "issue3"}},
    {"--48sp",          {"model", "48sp"}},
    {"--128",           {"model", "128"}},
    {"--128sp",         {"model", "128sp"}},
    {"--plus2",         {"model", "plus2"}},
    {"--plus2sp",       {"model", "plus2sp"}},
    {"--plus2a",        {"model", "plus2a"}},
    {"--plus2asp",      {"model", "plus2asp"}},
    {"--plus3",         {"model", "plus3"}},
    {"--plus3sp",       {"model", "plus3sp"}},
    {"--pentagon",      {"model", "pentagon"}},
    {"--cpc464",        {"model", "cpc464"}},
    {"--cpc464sp",      {"model", "cpc464sp"}},
    {"--cpc664",        {"model", "cpc664"}},
    {"--cpc664sp",      {"model", "cpc664sp"}},
    {"--cpc6128",       {"model", "cpc6128"}},
    {"--cpc6128sp",     {"model", "cpc6128sp"}},

    // Joystick options
    {"--kempston",      {"joystick", "kempston"}},
    {"--sinclair",      {"joystick", "sinclair"}},
    {"--pad",           {"pad", "yes"}},
    {"--nopad",         {"pad", "no"}},

    // Sound options
    {"--tapesound",     {"tapesound", "yes"}},
    {"--notapesound",   {"tapesound", "no"}},
    {"--sound",         {"sound", "yes"}},
    {"--nosound",       {"sound", "no"}},
    {"--psg",           {"forcepsg", "yes"}},
    {"--nopsg",         {"forcepsg", "no"}},
    {"--abc",           {"stereo", "abc"}},
    {"--acb",           {"stereo", "acb"}},
    {"--turbo",         {"stereo", "turbo"}},
    {"--turboabc",      {"stereo", "turboabc"}},
    {"--turboacb",      {"stereo", "turboacb"}},
    {"--turbonext",     {"stereo", "turbonext"}},
    {"--mono",          {"stereo", "mono"}},
    {"--ay",            {"psgtype", "ay"}},
    {"--ym",            {"psgtype", "ym"}},

    // Screen options
    {"--average",       {"scanmode", "average"}},
    {"--scanlines",     {"scanmode", "scanlines"}},
    {"--nodoublescan",  {"scanmode", "normal"}},
    {"--window",        {"fullscreen", "no"}},
    {"--fullscreen",    {"fullscreen", "yes"}},
    {"--antialias",     {"antialias", "yes"}},
    {"--sync",          {"sync", "yes"}},
    {"--nosync",        {"sync", "no"}},
    {"--cmos",          {"z80type", "cmos"}},
    {"--nmos",          {"z80type", "nmos"}},

    // Switches
    {"--flashtap",      {"flashtap", "yes"}},
    {"--noflashtap",    {"flashtap", "no"}},

    // SD1 was a protection device used in Camelot Warriors.
    {"--sd1",           {"sd1", "yes"}},
    {"--nosd1",         {"sd1", "no"}}
};

int main(int argc, char* argv[]) {

    displayLicense();

    // Read options from the config file.
    map<string, string> options;
    readOptions(options);

    // Read options from the command line. These options modify the ones from
    // the config file.
    vector<string> params(argv + 1, argv + argc);
    vector<string> files;
    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it) {
        if (*it == "--help" || *it == "-h") {
            displayHelp();
            exit(0);
        }
    }

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it) {
        map<string, Option>::iterator argument = arguments.find(*it);
        if (argument != arguments.end()) {
            options[argument->second.name] = argument->second.value;
        } else if (it->find('.') != string::npos) {
            files.push_back(*it);
        }
    }

    // Create the correct Screen class, depending on the selected computer.
    string model = options["model"];
    if (isSpectrum(model)) {
        SpeccyScreen speccy(options, files);
        speccy.setup();
        speccy.run();
    } else if (isCpc(model)) {
        CpcScreen cpc(options, files);
        cpc.setup();
        cpc.run();
    }
}

void displayLicense() {

    cout << "SpecIde Version ";
    cout << SPECIDE_VERSION_MAJOR << ".";
    cout << SPECIDE_VERSION_MINOR << ".";
    cout << SPECIDE_VERSION_TWEAK << endl << endl;
    cout << "(c) 2016-2019 Marta Sevillano Mancilla." << endl << endl;
    cout << "This program is free software: you can redistribute it and/or modify" << endl;
    cout << "it under the terms of the GNU General Public License as published by" << endl;
    cout << "the Free Software Foundation, version 3." << endl;
    cout << endl;
    cout << "This program is distributed in the hope that it will be useful," << endl;
    cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
    cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl;
    cout << "GNU General Public License for more details." << endl;
    cout << endl;
    cout << "You should have received a copy of the GNU General Public License" << endl;
    cout << "along with this program.  If not, see <https://www.gnu.org/licenses/>." << endl;
    cout << endl;
    cout << "ZX Spectrum ROMs are (c) Amstrad PLC. Amstrad PLC has kindly given" << endl;
    cout << "permission for the redistribution of the ZX Spectrum ROMs for their" << endl;
    cout << "use with emulators, but retains the copyright for them." << endl;
    cout << endl;
}

void displayHelp() {

    cout << "Usage: SpecIde [options] [tapefiles]" << endl;
    cout << endl;
    cout << "Supported formats are TAP and TZX." << endl;
    cout << endl;
    cout << "Options:" << endl;
    cout << endl;
    cout << "Model selection options:" << endl;
    cout << "--issue2               Spectrum 48K, issue 2." << endl;
    cout << "--issue3 | --48        Spectrum 48K, issue 3. (Default)" << endl;
    cout << "--48sp                 Spectrum + 48K. (Spanish ROM)" << endl;
    cout << "--128                  Spectrum 128K." << endl;
    cout << "--128sp                Spectrum 128K. (Spanish ROM)" << endl;
    cout << "--plus2                Spectrum +2." << endl;
    cout << "--plus2sp              Spectrum +2. (Spanish ROM)" << endl;
    cout << "--plus2a               Spectrum +2A." << endl;
    cout << "--plus2asp             Spectrum +2A. (Spanish ROM)" << endl;
    cout << "--plus3                Spectrum +3." << endl;
    cout << "--plus3sp              Spectrum +3. (Spanish ROM)" << endl;
    cout << endl;
    cout << "Hardware options:" << endl;
    cout << "--kempston             Map joystick to Kempston interface." << endl;
    cout << "--sinclair             Map joystick to Sinclair interface. (Default)" << endl;
    cout << "--pad|--nopad          Map pad extra buttons to keys." << endl;
    cout << "--psg|--nopsg          Emulate AY chip in 48K Spectrum." << endl;
    cout << "--abc|--acb|--mono     Select stereo mode." << endl;
    cout << "--ay|--ym              Select PSG: AY-3-8912/YM-2149." << endl;
    cout << "--sd1                  Emulate Dinamic SD1 dongle." << endl;
    cout << "--cmos                 Z80 is CMOS - OUT(C),0 outputs FFh." << endl;
    cout << endl;
    cout << "Video options:" << endl;
    cout << "--fullscreen           Start SpecIde in full screen mode." << endl;
    cout << "--window               Start SpecIde in windowed mode." << endl;
    cout << "--scanlines            Render PAL double scan mode." << endl;
    cout << "--average              Render PAL double scan mode, averaging scanlines." << endl;
    cout << "--nodoublescan         Single scan mode. (Default)" << endl;
    cout << "--sync                 Sync emulation to PC video refresh rate." << endl;
    cout << "--antialias            Set antialiasing." << endl;
    cout << endl;
    cout << "Sound options (add prefix 'no' to disable. Eg. --nosound):" << endl;
    cout << "--sound                Enable beeper/PSG sound. (Default)" << endl;
    cout << "--tapesound            Enable tape sound." << endl;
    cout << endl;
    cout << "Emulation options (add prefix 'no' to disable. Eg. --noflashtap):" << endl;
    cout << "--flashtap         Enable ROM traps for LOAD and SAVE." << endl;
    cout << endl;
}

void readOptions(map<string, string>& options) {

    // Set default values first
    options["model"] = "default";
    options["joystick"] = "sinclair";
    options["pad"] = "no";
    options["tapesound"] = "yes";
    options["sound"] = "yes";
    options["forcepsg"] = "no";
    options["stereo"] = "none";
    options["psgtype"] = "ay";
    options["scanmode"] = "normal";
    options["fullscreen"] = "no";
    options["antialias"] = "no";
    options["flashtap"] = "no";
    options["sync"] = "no";
    options["sd1"] = "no";
    options["scale"] = "1";
    options["z80type"] = "nmos";

    vector<string> cfgPaths;
    string cfgName("SpecIde.cfg");
    char* pHome = getenv(SPECIDE_HOME_ENV);

    cfgPaths.push_back("");
    if (pHome != nullptr) {
        string cfgPath(pHome);
#if (SPECIDE_ON_UNIX==1)
        cfgPath += string("/") + string(SPECIDE_CONF_DIR) + string("/");
#else
        cfgPath += string("\\") + string(SPECIDE_CONF_DIR) + string("\\");
#endif
        cfgPaths.push_back(cfgPath);
    }

    ifstream ifs;

    size_t j = 0;
    bool fail = true;
    do {
        string cfg = cfgPaths[j] + cfgName;
        cout << "Trying config file: " << cfg << "... ";
        ifs.open(cfg);
        fail = ifs.fail();
        cout << string(fail ? "FAIL" : "OK") << endl;
        ++j;
    } while (fail && j < cfgPaths.size());

    if (fail) {
        cout << "Config file SpecIde.cfg not found." << endl;
        return;
    }

    string entry;
    string key, val;
    size_t pos;
    while (ifs.good()) {
        getline(ifs, entry);

        // Remove comments with #
        pos = entry.find("#");
        if (pos != string::npos) {
            entry = entry.substr(0, pos);
        }

        // Find if there is a key and value. Otherwise, skip.
        pos = entry.find("=");
        if (pos != string::npos) {
            key = entry.substr(0, pos);
            val = entry.substr(pos + 1);

            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());

            options[key] = val;
        }
    }
}

bool isSpectrum(string const& model) {

    set<string> models = {"issue2", "issue3", "128", "plus2", "plus2a", "plus3",
        "48sp", "128sp", "plus2sp", "plus2asp", "plus3sp", "pentagon"};

    return (models.find(model) != models.end());
}

bool isCpc(string const& model) {

    set<string> models = {"cpc464", "cpc664", "cpc6128", "cpc464es", "cpc664es", "cpc6128es"};

    return (models.find(model) != models.end());
}

vector<string> getRomDirs() {

    vector<string> romDirs;
    romDirs.push_back("");

    char* pHome = getenv(SPECIDE_HOME_ENV);
    if (pHome != nullptr) {
#if (SPECIDE_ON_UNIX==1)
        string home(pHome);
        home += string("/") + string(SPECIDE_CONF_DIR) + string("/roms/");
        romDirs.push_back(home);
#else
        string home(pHome);
        home += string("\\") + string(SPECIDE_CONF_DIR) + string("\\roms\\");
        romDirs.push_back(home);
#endif
    }
#if (SPECIDE_ON_UNIX==1)
    romDirs.push_back("/usr/local/share/spectrum-roms/");
    romDirs.push_back("/usr/share/spectrum-roms/");
#endif

    return romDirs;
}
// vim: et:sw=4:ts=4
