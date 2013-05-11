THE MANA PLUS CLIENT
===============

 Version: 1.3.5.12        Date: 2013-05-12

 Development team:
  - See AUTHORS file for a list

 Powered by:

  - SDL, SDL_image, SDL_mixer, SDL_ttf, SDL_net (Media framework), SDL_gfx
  - libxml2 (XML parsing and writing)
  - PhysFS (Data files)
  - libcurl (HTTP downloads)
  - zlib (Archives)


0. Index
--------

1. Account
2. Commands
3. Skills
4. Support

1. Account
----------

To create an account you can usually press the "Register" button after
choosing your server. When this doesn't work, visit the website of the server
you'd like to register on, since they may be using an online registration
form instead.

2. Commands
-----------

KEYBOARD:

Use arrow keys to move around. Other keys:

- Ctrl               attack
- F1                 toggle the online help
- F2                 toggle profile window
- F3                 toggle inventory window
- F4                 toggle equipment window
- F5                 toggle skills window
- F6                 toggle minimap
- F7                 toggle chat window
- F8                 toggle shortcut window
- F9                 show setup window
- F10                toggle debug window
- Alt + 0-9,-, etc   show emotions.
- S                  sit down / stand up.
- F                  toggle debug pathfinding feature (change map mode)
- P                  take screenshot
- R                  turns on anti-trade function.
- A                  target nearest monster
- H                  hide all non-sticky windows
- Z                  pick up item
- Enter              focus chat window / send message

MOUSE:

Left click to execute default action: walk, pick up an item, attack a monster
and talk to NPCs (be sure to click on their feet). Right click to show up a
context menu. Holding [Left Shift] prevents from walking when attacking.

/Commands:

Whispers:
- /closeall          close all whispers.
- /ignoreall         add all whispers to ignore list.
- /msg NICK text
- /whisper NICK text
- /w NICK text       send whisper message to nick.
- /query NICK
- /q NICK            open new whisper tab for nick.

Actions:
- /help              show small help about chat commands. /target NICK - select nick as target. Can be monster or player nick.
- /outfit N          wear outfit number N.
- /outfit next       wear next outfit.
- /outfit prev       wear previous outfit.
- /emote N           use emotion number N.
- /away
- /away MSG          set away mode.
- /follow NICK       start follow mode.
- /imitation NICK    start imitation mode.
- /heal NICK         heal nick.
- /move X Y          move to X,Y position in short distance.
- /navigate x y      move to position x,y in current map in any distance.
- /mail NICK MSG     send offline message to NICK. Working only in tmw server.
- /disconnect        quick disconnect from server.
- /attack            attack target.
- /undress NICK      remove all clothes from nick. Local effect only.

Trade:
- /trade NICK        start trade with nick.
- /priceload         load shop price from disc.
- /pricesave         save shop price to disc.

Player relations:
- /ignore NICK       add nick to ignore list.
- /unignore NICK     Remove nick from ignore list.
- /friend NICK
- /befriend NICK     add nick to friends list.
- /disregard NICK    add nick to disregarded list.
- /neutral NICK      add nick to neutral relation list.
- /erase NICK        add nick to erased list.
- /clear             clear current chat tab.
- /createparty NAME  create party with selected name.
- /me text           send text to chat as /me command in irc.

Debug:
- /who               print online players number to chat.
- /all               show visible beings list in debug tab.
- /where             print current player position to chat.
- /cacheinfo         show text cache info.
- /dirs              show client directories in debug window.

Other:
- /help              Displays the list of commands
- /announce          broadcasts a global msg(Gm Cammand only)
- /who               shows how many players are online
- /where             displays the map name your currently on

4. Support
----------

If you're having issues with this client, feel free to report them to us.
You can report on forum http://forums.themanaworld.org/viewforum.php?f=12
or IRC on irc.freenode.net in the #manaplus channel.

If you have feedback about a specific game that uses the Mana client, be sure
to contact the developers of the game instead.
