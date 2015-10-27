# Castnow-controller, a Qt5-based castnow daemon with a simple, basic, QML-based controller for Android

This project is based on [Simon Kusterer's Castnow](https://github.com/xat/castnow),
a Chromecast local streaming solution build on Node.js. **Castnow** it's CLI-oriented and
lacks of GUI, so I've developed a pair of programs...

## Castnow daemon (cnDaemon)

The Castnow daemon searchs a directory list, looking for movies and video files
(currently only MP4, MKV and AVI) and subtitles (only SRT), and then waits for a controller command:

* *LIST*: Send a list of files and subtitles
* *PLAY*: Play a file, with or without subtitles.

## Castnow controller (cnController)

The Castnow controller send a broadcast request, looking for an active cnDaemon, and retrieves
the file's list. The user can select a file and a subtitle, and start castnow with them.

### Castnow daemon configuration.

The Castnow daemon uses a *config.json* file as a configuration file. There's a example:
```json
{
    "discovery_port": 5050,
    "interface": "all",
    "port": 5000,
    "server_name": "CNDaemon Server",
    "myip": "eth0",
    "directory": [
	 "/my/Home/Movies"
	]
}
```
* **discovery_port**: UDP auto-discovery port. The castnow controller sends a UDP broadcast request, looking for this port.
* **interface**: Listening interface. Use *"all"* by now, because there's a nasty bug :D
* **port**: TCP port. The commands are in JSON format, and uses TCP as transmission protocol.
* **server_name**: CNDaemon's instance name.
* **myip**: Same as castnow's *--myip* option; use a interface's name, and the daemon retrieves the current interface's IP. Really cool if you're using a DHCP server.
* **directory**: List of directory's to look for files and subtitles.

> NOTE: The subtitles MUST have the same filename as the movie they belongs.

### Icons

All the icons belongs to [Numix Circle icon theme](https://github.com/numixproject/numix-icon-theme-circle).

### Some last words

CNDaemon and CNController are very basic programs; I've been developing them for a time now (a weekend project), but
I know there's a lot of room for upgrades.

### License: GPL v3

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
