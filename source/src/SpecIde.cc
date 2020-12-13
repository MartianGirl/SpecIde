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

enum class FileTypes {
    FILETYPE_TAP,
    FILETYPE_TZX,
    FILETYPE_PZX,
    FILETYPE_DSK,
    FILETYPE_TRD,
    FILETYPE_Z80,
    FILETYPE_SNA,
    FILETYPE_CSW,
    FILETYPE_ERR
};

using namespace std;

FileTypes guessFileType(string const& fileName);
void displayLicense();
void displayHelp();
void readOptions(map<string, string>& options);
size_t getScale(string const& scale);

int main(int argc, char* argv[]) {

    vector<string> params(argv + 1, argv + argc);
    vector<string> files;
    map<string, string> options;

    displayLicense();
    readOptions(options);

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it) {
        if (*it == "--help" || *it == "-h") {
            displayHelp();
            exit(0);
        }
    }

    // The Screen class is now actually more of a "console".
    // We create the instance, and load the given tape (if any).
    Screen screen(getScale(options["scale"]));

    for (vector<string>::iterator it = params.begin(); it != params.end(); ++it) {
        // Model selection
        if (*it == "--issue2")
            options["model"] = "issue2";
        if (*it == "--issue3" || *it == "--48")
            options["model"] = "issue3";
        if (*it == "--48sp")
            options["model"] = "48sp";
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
        if (*it == "--pentagon")
            options["model"] = "pentagon";

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
        if (*it == "--turbo")
            options["stereo"] = "turbo";
        if (*it == "--turboacb")
            options["stereo"] = "turboacb";
        if (*it == "--turboabc")
            options["stereo"] = "turboabc";
        if (*it == "--turbonext")
            options["stereo"] = "turbonext";
        if (*it == "--mono")
            options["stereo"] = "mono";

        if (*it == "--ay")
            options["psgtype"] = "ay";
        if (*it == "--ym")
            options["psgtype"] = "ym";

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
        if (*it == "--cmos")
            options["z80type"] = "cmos";

        if (it->find('.') != string::npos)
            files.push_back(*it);
    }

    // Model selection
    if (options["model"] == "issue2") {
        screen.setSoundRate(SoundRate::SOUNDRATE_48K);
        screen.spectrum.loadRoms(RomVariant::ROM_48_EN);
        screen.spectrum.ula.setUlaVersion(0);
    } else if (options["model"] == "issue3") {
        screen.setSoundRate(SoundRate::SOUNDRATE_48K);
        screen.spectrum.loadRoms(RomVariant::ROM_48_EN);
        screen.spectrum.ula.setUlaVersion(1);
    } else if (options["model"] == "48sp") {
        screen.setSoundRate(SoundRate::SOUNDRATE_48K);
        screen.spectrum.loadRoms(RomVariant::ROM_48_ES);
        screen.spectrum.ula.setUlaVersion(1);
    } else if (options["model"] == "128") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_128_EN);
        screen.spectrum.set128K();
        screen.spectrum.ula.setUlaVersion(2);
    } else if (options["model"] == "128sp") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_128_ES);
        screen.spectrum.set128K();
        screen.spectrum.ula.setUlaVersion(2);
    } else if (options["model"] == "plus2") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS2_EN);
        screen.spectrum.setPlus2();
        screen.spectrum.ula.setUlaVersion(3);
    } else if (options["model"] == "plus2sp") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS2_ES);
        screen.spectrum.setPlus2();
        screen.spectrum.ula.setUlaVersion(3);
    } else if (options["model"] == "plus2a") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS3_EN);
        screen.spectrum.setPlus2A();
        screen.spectrum.ula.setUlaVersion(4);
    } else if (options["model"] == "plus2asp") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS3_ES);
        screen.spectrum.setPlus2A();
        screen.spectrum.ula.setUlaVersion(4);
    } else if (options["model"] == "plus3") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS3_EN);
        screen.spectrum.setPlus3();
        screen.spectrum.ula.setUlaVersion(4);
    } else if (options["model"] == "plus3sp") {
        screen.setSoundRate(SoundRate::SOUNDRATE_128K);
        screen.spectrum.loadRoms(RomVariant::ROM_PLUS3_ES);
        screen.spectrum.setPlus3();
        screen.spectrum.ula.setUlaVersion(4);
    } else if (options["model"] == "pentagon") {
        screen.setSoundRate(SoundRate::SOUNDRATE_PENTAGON);
        screen.spectrum.loadRoms(RomVariant::ROM_128_EN);
        screen.spectrum.set128K();
        screen.spectrum.ula.setUlaVersion(5);
    } else {
        options["model"] = "default";
        screen.setSoundRate(SoundRate::SOUNDRATE_48K);
        screen.spectrum.loadRoms(RomVariant::ROM_48_EN);
        screen.spectrum.ula.setUlaVersion(1);
    }
    cout << "Model: " << options["model"] << endl;

    // Joystick and pad settings.
    screen.spectrum.kempston = (options["joystick"] == "kempston");
    cout << "Joystick type: " << options["joystick"] << endl;

    screen.pad = (options["pad"] == "yes");
    cout << "Map game pad extra buttons to keys: " << options["pad"] << endl;

    // Sound settings.
    screen.setTapeSound(options["tapesound"] != "no");
    cout << "Play tape sound: " << options["tapesound"] << endl;

    screen.playSound = (options["sound"] != "no");
    cout << "Play sound: " << options["sound"] << endl;

    if (options["forcepsg"] == "yes") {
        screen.psgSound = true;
        screen.spectrum.psgPresent[0] = true;
        cout << "Enable AY interface on 128K ports: " << options["forcepsg"] << endl;
    }

    if (options["stereo"] == "turbo"
            || options["stereo"] == "turboacb"
            || options["stereo"] == "turboabc") {
        screen.psgSound = true;
        screen.spectrum.psgPresent[0] = true;
        screen.spectrum.psgPresent[1] = true;
        cout << "TurboSound (2 PSGs) active." << endl;
    }

    if (options["stereo"] == "turbonext") {
        screen.psgSound = true;
        screen.spectrum.psgPresent[0] = true;
        screen.spectrum.psgPresent[1] = true;
        screen.spectrum.psgPresent[2] = true;
        screen.spectrum.psgPresent[3] = true;
        cout << "Generalized NEXT sound (4 PSGs) active." << endl;
    }

    // Set ACB stereo as default for Pentagon
    if (options["model"] == "pentagon" && options["stereo"] == "none") {
        options["stereo"] = "acb";
    }

    if (options["stereo"] == "acb") {
        screen.spectrum.stereo = StereoMode::STEREO_ACB;
    } else if (options["stereo"] == "abc") {
        screen.spectrum.stereo = StereoMode::STEREO_ABC;
    } else if (options["stereo"] == "turbo") {
        screen.spectrum.stereo = StereoMode::STEREO_TURBO_MONO;
    } else if (options["stereo"] == "turboabc") {
        screen.spectrum.stereo = StereoMode::STEREO_TURBO_ABC;
    } else if (options["stereo"] == "turboacb") {
        screen.spectrum.stereo = StereoMode::STEREO_TURBO_ACB;
    } else if (options["stereo"] == "turbonext") {
        screen.spectrum.stereo = StereoMode::STEREO_NEXT;
    } else if (options["stereo"] == "mono") {
        screen.spectrum.stereo = StereoMode::STEREO_MONO;
    } else {
        screen.spectrum.stereo = StereoMode::STEREO_MONO;
    }
    cout << "Stereo type: " << options["stereo"] << endl;

    screen.aychip = (options["psgtype"] != "ym");
    screen.spectrum.psgChip(screen.aychip);
    cout << "PSG chip: " << options["psgtype"] << endl;

    // Screen settings.
    if (options["scanmode"] == "scanlines") {
        screen.doubleScanMode = true;
        screen.spectrum.ula.scanlines = 1;
        screen.spectrum.ula.yInc = 2;
    } else if (options["scanmode"] == "average") {
        screen.doubleScanMode = false;
        screen.spectrum.ula.scanlines = 2;
        screen.spectrum.ula.yInc = 1;
    } else {
        screen.doubleScanMode = false;
        screen.spectrum.ula.scanlines = 0;
        screen.spectrum.ula.yInc = 1;
    }
    cout << "Scan mode: " << options["scanmode"] << endl;

    screen.fullscreen = (options["fullscreen"] == "yes");
    cout << "Full screen mode: " << options["fullscreen"] << endl;

    screen.flashTap = (options["flashtap"] == "yes");
    cout << "FlashTAP: " << options["flashtap"] << endl;

    if (options["sync"] == "yes") {
        screen.syncToVideo = true;
        screen.window.setVerticalSyncEnabled(true);
        cout << "Sync to video: " << options["sync"] << endl;
    }

    if (options["sd1"] == "yes") {
        screen.spectrum.idle = 0xDF;
        screen.spectrum.ula.inMask = 0x9F;
        cout << "SD1 dongle: " << options["sd1"] << endl;
    }

    screen.spectrum.z80.zeroByte = options["z80type"] == "cmos" ? 0xFF : 0x00;
    cout << "Z80 type: " << options["z80type"] << endl;

    for (vector<string>::iterator it = files.begin(); it != files.end(); ++it) {
        switch (guessFileType(*it)) {
            case FileTypes::FILETYPE_TZX:
                screen.tape.loadTzx(*it);
                break;

            case FileTypes::FILETYPE_TAP:
                screen.tape.loadTap(*it);
                break;

            case FileTypes::FILETYPE_CSW:
                screen.tape.loadCsw(*it);
                break;

            case FileTypes::FILETYPE_DSK:
                {
                    DSKFile dsk;
                    dsk.load(*it);

                    if (dsk.validFile) {
                        screen.spectrum.fdc.drive[0].images.push_back(dsk);
                        screen.spectrum.fdc.drive[0].imageNames.push_back(*it);
                        screen.spectrum.fdc.drive[0].disk = true;
                    }
                }
                break;

            default:
                cout << "Unknown file type: " << *it << endl;
                break;
        }
    }

    screen.reopenWindow(screen.fullscreen);
    screen.setFullScreen(screen.fullscreen);
    screen.spectrum.ula.playSound = screen.playSound;
    screen.spectrum.psgPlaySound(screen.psgSound && screen.playSound);
    screen.run();
}

FileTypes guessFileType(string const& fileName) {

    // Parse the file name, find the extension. We'll decide what to do
    // based on this.
    size_t dot = fileName.find_last_of('.');
    string extension;
    if (dot != string::npos) {
        // Get the extension in lowercase characters.
        extension = fileName.substr(dot);
        for (size_t ii = 0; ii < extension.size(); ++ii) {
            if (extension[ii] >= 'A' && extension[ii] <= 'Z') {
                extension[ii] += ('a' - 'A');
            }
        }
    }

    if (extension == ".tzx") {
        return FileTypes::FILETYPE_TZX;
    } else if (extension == ".cdt") {
        return FileTypes::FILETYPE_TZX;
    } else if (extension == ".tap") {
        return FileTypes::FILETYPE_TAP;
    } else if (extension == ".dsk") {
        return FileTypes::FILETYPE_DSK;
    } else if (extension == ".csw") {
        return FileTypes::FILETYPE_CSW;
    } else {
        return FileTypes::FILETYPE_ERR;
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
        cout << "Trying config file: " << cfg << endl;
        ifs.open(cfg);
        fail = ifs.fail();
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
        if (pos != string::npos)
            entry = entry.substr(0, pos);

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

size_t getScale(string const& scale) {
    size_t s = 1;
    try {
        s = stoi(scale);
    } catch (invalid_argument &ia) {
        cout << "Invalid scale value: '" << scale << "' - " << ia.what() << endl;
    }

    if (s < 1)
        s = 1;
    else if (s > 10)
        s = 10;

    cout << "Selected Scale Factor: " << s << endl;
    return s;
}
// vim: et:sw=4:ts=4
