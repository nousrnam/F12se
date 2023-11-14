1. Usage

If Fallout is installed in "Program Files", you may have problems(editor doesn't see savegames, doesn't save changes...).

After you finished editing, click "Save" button under the list of savgames to apply the changes.

Path in the settings should be game main path. It looks like "C:\Fallout2", not like "C:\Fallout2\data\savegame" or "C:\Fallout2\data\savegame\slot01".

2. Miscellaneous information

Editing of inventory is dangerous, because information about format is incomplete!(Backup savegames)
- Changing quantity is almost 100% safe. You can set quantity to zero(very bad idea), but why would you?
- The transformation from one item to another - a bit dangerous.
- Creating items - very dangerous. Also created items don't have attached script. (e.g. Fallout 2 Hintbook won't work)

If you delete map from "List of *.sav files"(Maps tab), it will return map to its original state next time when you visit it. (When you delete maps from list, you must be in some other location or savegame will be damaged). May be useful to respawn accidentally killed NPC or something like that.

If you change character stats, better change "Bonus Stats" than "Base Stats". In game stats = base + bonus, but some of base stats are recalculated by game engine at each load. Also some stats have ranges can be seen there: https://www.nma-fallout.com/threads/engine-reversing-exploration.188326/#post-3634316

3. Misc tab. "Bugfixes"

"Ghost mode" box must always be uncheked. If it is checked, then I bet you are "drawn under objects and scenery and cannot be seen by other critters"
Some information: https://www.nma-fallout.com/threads/ghost-mode-fixer.188353/

"Clear combat info" button: If you save in combat, it can corrupt your savegame and you will receive message that game unable to load savegame. Button clears combat info and turns combat mode off.

4. Some Settings

4.1 Msg directory
Specifies falloutdir\data\text subdirectory from which msg files are loaded. (english, french, spanish, etc)

4.2 Auto correction

4.2.1. Item's FID
Each item in inventory stores it's on-ground image. With this option enabled, when you change one item to another, image will be changed automatically.

4.2.2. Ammo type
Each weapon in inventory stores it's ammo type. With this option enabled, when you change one weapon to another, ammo type will be changed automatically.

4.2.3. Ammo quantity
If number of loaded ammo greater then weapon can hold the game will crash. With this option enabled, when you change one weapon to another, ammo quantity will be reduced if necessary.

4.2.4. Perks
Some perks have an effect only when they are taken, so changing values in editor may not do anything, if this option disabled. If option enabled editor tries to find perk's data in fallout2.exe and automatically changes affected stat when you change some perk value.
