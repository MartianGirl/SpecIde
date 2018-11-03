/* This file is part of SpecIde, (c) Marta Sevillano Mancilla, 2016-2018.
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
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Screen.h"

#include "config.h"

enum class FileTypes
{
    FILETYPE_TAP,
    FILETYPE_TZX,
    FILETYPE_PZX,
    FILETYPE_DSK,
    FILETYPE_TRD,
    FILETYPE_Z80,
    FILETYPE_SNA,
    FILETYPE_ERR

};

using namespace std;

FileTypes guessFileType(string const& fileName);
void displayLicense();
void readOptions(map<string, string>& options);

int main(int argc, char* argv[])
{
    vector<string> params(argv + 1, argv + argc);
    vector<string> files;
    map<string, string> options;

    displayLicense();
    readOptions(options);

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        if (*it == "--help" || *it == "-h")
        {
            cout << "Usage: SpecIde [options] [tapefiles]" << endl;
            cout << endl;
            cout << "Supported formats are TAP and TZX." << endl;
            cout << endl;
            cout << "Options:" << endl;
            cout << endl;
            cout << "Model selection options:" << endl;
            cout << "--issue2:              Spectrum 48K, issue 2." << endl;
            cout << "--issue3 | --48:       Spectrum 48K, issue 3. (Default)" << endl;
            cout << "--128:                 Spectrum 128K." << endl;
            cout << "--128sp:               Spectrum 128K. (Spanish ROM)" << endl;
            cout << "--plus2                Spectrum +2." << endl;
            cout << "--plus2sp              Spectrum +2. (Spanish ROM)" << endl;
            cout << "--plus2a               Spectrum +2A." << endl;
            cout << "--plus2asp             Spectrum +2A. (Spanish ROM)" << endl;
            // cout << "--plus3               Spectrum +3." << endl;
            // cout << "--plus3sp             Spectrum +3. (Spanish ROM)" << endl;
            cout << endl;
            cout << "Hardware options:" << endl;
            cout << "--kempston:            Map joystick to Kempston interface." << endl;
            cout << "--sinclair:            Map joystick to Sinclair interface. (Default)" << endl;
            cout << "--pad|--nopad:         Map pad extra buttons to keys." << endl;
            cout << "--psg|--nopsg:         Emulate AY chip in 48K Spectrum." << endl;
            cout << "--abc|--acb|--mono:    Select stereo mode." << endl;
            cout << "--sd1:                 Emulate Dinamic SD1 dongle." << endl;
            cout << endl;
            cout << "Video options:" << endl;
            cout << "--fullscreen           Start SpecIde in full screen mode." << endl;
            cout << "--window               Start SpecIde in windowed mode." << endl;
            cout << "--scanlines            Render PAL double scan mode." << endl;
            cout << "--average              Render PAL double scan mode, averaging scanlines." << endl;
            cout << "--nodoublescan         Single scan mode. (Default)" << endl;
            cout << "--sync                 Sync emulation to PC video refresh rate." << endl;
            cout << endl;
            cout << "Sound options (add prefix 'no' to disable. Eg. --nosound):" << endl;
            cout << "--sound                Enable buzzer/PSG sound. (Default)" << endl;
            cout << "--tapesound            Enable tape sound." << endl;
            cout << endl;
            cout << "Emulation options (add prefix 'no' to disable. Eg. --noflashtap):" << endl;
            cout << "--flashtap         Enable ROM traps for LOAD and SAVE." << endl;
            cout << endl;
            exit(0);
        }
    }

    // The Screen class is now actually more of a "console".
    // We create the instance, and load the given tape (if any).
    Screen screen(1);

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        // Model selection
        if (*it == "--issue2")
            options["model"] = "issue2";
        if (*it == "--issue3" || *it == "--48")
            options["model"] = "issue3";
        if (*it == "--128")
            options["model"] = "128";
        if (*it == "--128sp")
            options["model"] = "128sp";
        if (*it == "--plus2")
            options["model"] = "plus2";
        if (*it == "--plus2sp")
            options["model"] = "plus2sp";
        if (*it == "--plus2a")
            options["model"] = "plus2a";
        if (*it == "--plus2asp")
            options["model"] = "plus2asp";
        if (*it == "--plus3")
            options["model"] = "plus3";
        if (*it == "--plus3sp")
            options["model"] = "plus3sp";

        // Joystick options
        if (*it == "--kempston")
            options["joystick"] = "kempston";
        if (*it == "--sinclair")
            options["joystick"] = "sinclair";

        if (*it == "--pad")
            options["pad"] = "yes";
        if (*it == "--nopad")
            options["pad"] = "no";

        // Sound options
        if (*it == "--notapesound")
            options["tapesound"] = "no";
        if (*it == "--tapesound")
            options["tapesound"] = "yes";

        if (*it == "--nosound")
            options["sound"] = "no";
        if (*it == "--sound")
            options["sound"] = "yes";

        // PSG options
        if (*it == "--nopsg")
            options["forcepsg"] = "no";
        if (*it == "--psg")
            options["forcepsg"] = "yes";

        if (*it == "--abc")
            options["stereo"] = "abc";
        if (*it == "--acb")
            options["stereo"] = "acb";
        if (*it == "--mono")
            options["stereo"] = "none";

        if (*it == "--psglinear")
            options["psgtype"] = "linear";
        if (*it == "--psglog")
            options["psgtype"] = "logarithmic";

        // FlashTAP
        if (*it == "--noflashtap")
            options["flashtap"] = "no";
        if (*it == "--flashtap")
            options["flashtap"] = "yes";

        // Screen options
        if (*it == "--average")
            options["scanmode"] = "average";
        if (*it == "--scanlines")
            options["scanmode"] = "scanlines";
        if (*it == "--nodoublescan")
            options["scanmode"] = "normal";

        if (*it == "--window")
            options["fullscreen"] = "no";
        if (*it == "--fullscreen")
            options["fullscreen"] = "yes";

        if (*it == "--sync")
            options["sync"] = "yes";
        if (*it == "--nosync")
            options["sync"] = "no";

        // SD1 was a protection device used in Camelot Warriors. It simply
        // forced bit 5 low for any port read, if the device didn't force
        // this bit high.
        if (*it == "--sd1")
            options["sd1"] = "yes";
        if (*it == "--nosd1")
            options["sd1"] = "no";

        if (it->find('.') != string::npos)
            files.push_back(*it);
    }

    // Model selection
    if (options["model"] == "issue2")
    {
        screen.set128K(false);
        screen.spectrum.loadRoms(0);
        screen.spectrum.ula.setUlaVersion(0);
    }
    else if (options["model"] == "issue3")
    {
        screen.set128K(false);
        screen.spectrum.loadRoms(0);
        screen.spectrum.ula.setUlaVersion(1);
    }
    else if (options["model"] == "128")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(1);
        screen.spectrum.set128K();
        screen.spectrum.ula.setUlaVersion(2);
    }
    else if (options["model"] == "128sp")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(4);
        screen.spectrum.set128K();
        screen.spectrum.ula.setUlaVersion(2);
    }
    else if (options["model"] == "plus2")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(2);
        screen.spectrum.setPlus2();
        screen.spectrum.ula.setUlaVersion(2);
    }
    else if (options["model"] == "plus2sp")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(5);
        screen.spectrum.setPlus2();
        screen.spectrum.ula.setUlaVersion(2);
    }
    else if (options["model"] == "plus2a")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(3);
        screen.spectrum.setPlus2A();
        screen.spectrum.ula.setUlaVersion(3);
    }
    else if (options["model"] == "plus2asp")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(6);
        screen.spectrum.setPlus2A();
        screen.spectrum.ula.setUlaVersion(3);
    }
    else if (options["model"] == "plus3")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(3);
        screen.spectrum.setPlus3();
        screen.spectrum.ula.setUlaVersion(3);
    }
    else if (options["model"] == "plus3sp")
    {
        screen.set128K(true);
        screen.spectrum.loadRoms(6);
        screen.spectrum.setPlus3();
        screen.spectrum.ula.setUlaVersion(3);
    }
    else
    {
        options["model"] = "default";
        screen.set128K(false);
        screen.spectrum.loadRoms(0);
        screen.spectrum.ula.setUlaVersion(1);
    }
    cout << "Model: " << options["model"] << endl;

    // Joystick and pad settings.
    screen.spectrum.kempston = (options["joystick"] == "kempston");
    cout << "Joystick type: " << options["joystick"] << endl;

    screen.pad = (options["pad"] == "yes");
    cout << "Map game pad extra buttons to keys: " << options["pad"] << endl;

    // Sound settings.
    screen.spectrum.buzzer.tapeSound = (options["tapesound"] != "no");
    cout << "Play tape sound: " << options["tapesound"] << endl;

    screen.spectrum.buzzer.playSound = (options["sound"] != "no");
    screen.spectrum.psg.playSound = (options["sound"] != "no");
    cout << "Play sound: " << options["sound"] << endl;

    if (options["forcepsg"] == "yes")
    {
        screen.spectrum.psg.playSound = true;
        screen.spectrum.hasPsg = true;
        cout << "Enable AY interface on 128K ports: " << options["forcepsg"] << endl;
    }

    if (options["stereo"] == "acb")
        screen.stereo = 1;
    else if (options["stereo"] == "abc")
        screen.stereo = 2;
    else
        screen.stereo = 0;
    cout << "Stereo type: " << options["stereo"] << endl;

    screen.spectrum.psg.setVolumeLevels(options["psgtype"] != "linear");
    cout << "PSG DAC type: " << options["psgtype"] << endl;

    // Screen settings.
    if (options["scanmode"] == "scanlines")
    {
        screen.doubleScanMode = true;
        screen.spectrum.ula.scanlines = 1;
        screen.spectrum.ula.yInc = 2;
    }
    else if (options["scanmode"] == "average")
    {
        screen.doubleScanMode = false;
        screen.spectrum.ula.scanlines = 2;
        screen.spectrum.ula.yInc = 1;
    }
    else
    {
        screen.doubleScanMode = false;
        screen.spectrum.ula.scanlines = 0;
        screen.spectrum.ula.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    screen.fullscreen = (options["fullscreen"] == "yes");
    cout << "Full screen mode: " << options["fullscreen"] << endl;

    screen.flashTap = (options["flashtap"] == "yes");
    cout << "FlashTAP: " << options["flashtap"] << endl;

    if (options["sync"] == "yes")
    {
        screen.syncToVideo = true;
        screen.window.setVerticalSyncEnabled(true);
        cout << "Sync to video: " << options["sync"] << endl;
    }

    if (options["sd1"] == "yes")
    {
        screen.spectrum.idle = 0xDF;
        screen.spectrum.ula.inMask = 0x9F;
        cout << "SD1 dongle: " << options["sd1"] << endl;
    }

    for (vector<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        switch (guessFileType(*it))
        {
            case FileTypes::FILETYPE_TZX:
                screen.tape.loadTzx(*it);
                break;

            case FileTypes::FILETYPE_TAP:
                screen.tape.loadTap(*it);
                break;

            case FileTypes::FILETYPE_DSK:
                {
                    DSKFile dsk;
                    dsk.load(*it);

                    screen.spectrum.fdc.drive[0].images.push_back(dsk);
                    screen.spectrum.fdc.drive[0].imagenames.push_back(*it);
                    screen.spectrum.fdc.drive[0].disk = true;
                    cout << "Adding DSK file: " << *it << endl;
                }
                break;

            default:
                cout << "Unknown file type: " << *it << endl;
                break;
        }
    }

    screen.reopenWindow(screen.fullscreen);
    screen.setFullScreen(screen.fullscreen);
    screen.run();
}

FileTypes guessFileType(string const& fileName)
{
    // Parse the file name, find the extension. We'll decide what to do
    // based on this.
    size_t dot = fileName.find_last_of('.');
    string extension;
    if (dot != string::npos)
    {
        // Get the extension in lowercase characters.
        extension = fileName.substr(dot);
        for (size_t ii = 0; ii < extension.size(); ++ii)
        {
            if (extension[ii] >= 'A' && extension[ii] <= 'Z')
                extension[ii] += ('a' - 'A');
        }
    }

    if (extension == ".tzx")
        return FileTypes::FILETYPE_TZX;
    else if (extension == ".tap")
        return FileTypes::FILETYPE_TAP;
    else if (extension == ".dsk")
        return FileTypes::FILETYPE_DSK;

    return FileTypes::FILETYPE_ERR;
}

void displayLicense()
{
    cout << "SpecIde Version ";
    cout << SPECIDE_VERSION_MAJOR << ".";
    cout << SPECIDE_VERSION_MINOR << ".";
    cout << SPECIDE_VERSION_TWEAK << endl << endl;
    cout << "(c) 2016-2018 Marta Sevillano Mancilla." << endl << endl;
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
}

void readOptions(map<string, string>& options)
{
    // Set default values first
    options["model"] = "default";
    options["joystick"] = "sinclair";
    options["pad"] = "no";
    options["tapesound"] = "yes";
    options["sound"] = "yes";
    options["forcepsg"] = "no";
    options["stereo"] = "none";
    options["psgtype"] = "logarithmic";
    options["scanmode"] = "normal";
    options["fullscreen"] = "no";
    options["flashtap"] = "no";
    options["sync"] = "no";
    options["sd1"] = "no";

    vector<string> cfgPaths;
    string cfgName("SpecIde.cfg");
    char* pHome = getenv(SPECIDE_HOME_ENV);

    cfgPaths.push_back("");
    if (pHome != nullptr)
    {
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
    do
    {
        string cfg = cfgPaths[j] + cfgName;
        cout << "Trying config file: " << cfg << endl;
        ifs.open(cfg);
        fail = ifs.fail();
        ++j;
    } while (fail && j < cfgPaths.size());

    if (fail)
    {
        cout << "Config file SpecIde.cfg not found." << endl;
        return;
    }

    string entry;
    string key, val;
    size_t pos;
    while (ifs.good())
    {
        getline(ifs, entry);

        // Remove comments with #
        pos = entry.find("#");
        if (pos != string::npos)
            entry = entry.substr(0, pos);

        // Find if there is a key and value. Otherwise, skip.
        pos = entry.find("=");
        if (pos != string::npos)
        {
            key = entry.substr(0, pos);
            val = entry.substr(pos + 1);

            key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
            val.erase(remove_if(val.begin(), val.end(), ::isspace), val.end());

            options[key] = val;
        }
    }
    ifs.close();
}
// vim: et:sw=4:ts=4
