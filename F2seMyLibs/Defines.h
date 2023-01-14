/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details. */

#ifndef DEFINES_H
#define DEFINES_H

#ifdef _WIN32
#define DIR_SEPARATOR "\\"
#else
#define DIR_SEPARATOR "/"
#endif

#define SFALLGVSAV "sfallgv.sav"

#define LANGUAGE_DIR "Language"
#define LANGUAGE_FILE_MASK "*.lng"

#define INI_FILENAME "F12se.ini"
#define INI_SECTION "Options"
#define INI_FALLOUT_DIR "falloutDir"
#define INI_FALLOUT_NUM "FalloutNum"

#define STATS_CNT 35
#define SKILLS_CNT 18
#define ITEMS_TYPES_CNT 7
#define TRAITS_CNT 16
#define MONTHS_CNT 12

#define NULL_CHARACTER_SIZE 1
#define MAP_FILENAME_MAX_LENGTH 0x10

#define INVEN_ITEM_FID    0x24
#define INVEN_ITEM_PID    0x30
#define INVEN_ITEM_AMMO_Q 0x5C
#define INVEN_ITEM_AMMO_T 0x60

#define PRO_PID    0x00
#define PRO_DID    0x04
#define PRO_FID    0x08
#define PRO_AMMO_T 0x71
#define PRO_AMMO_Q 0x75

#define PRO_CRITTER_FLAGS_OFFSET      0x2C
#define PRO_CRITTER_BASE_STAT_OFFSET  0x30
#define PRO_CRITTER_BONUS_STAT_OFFSET 0xBC
#define PRO_CRITTER_SKILL_OFFSET      0x148

#define F1_CFG "fallout.cfg"
#define F1_EXE "falloutw.exe"
#define F1_KILL_TYPES_CNT 15
#define F1_KILL_TYPES_MSG_START 450
#define F1_PERKS_CNT 64
#define F1_START_YEAR 2161
//функция написана так что первый месяц нулевой, поэтому месяц на 1 меньше
#define F1_START_MONTH 11
#define F1_START_DAY 5
#define F1_FALLOUT_NUM 1
#define F1_REG_KEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\falloutw.exe"

#define F2_CFG "fallout2.cfg"
#define F2_EXE "fallout2.exe"
#define F2_KILL_TYPES_CNT 19
#define F2_KILL_TYPES_MSG_START 1450
#define F2_PERKS_CNT 119
#define F2_START_YEAR 2241
#define F2_START_MONTH 6
#define F2_START_DAY 25
#define F2_FALLOUT_NUM 2
#define F2_REG_KEY "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\fallout2.exe"

// Header
#define PLAYER_NAME_OFFSET     0x1D
#define PLAYER_NAME_SIZE       0x20
#define SAVEGAME_NAME_OFFSET   0x3D
#define SAVEGAME_NAME_SIZE     0x1E
#define TICKS_OFFSET           0x6B
#define CURRENT_MAP_OFFSET     0x6F
#define CURRENT_MAPNAME_OFFSET 0x73
#define CURRENT_MAPNAME_SIZE   0x10
#define BITMAP_OFFSET          0x83
#define BITMAP_WIDTH           224
#define BITMAP_HEIGHT          133
#define BITMAP_SIZE            (BITMAP_WIDTH*BITMAP_HEIGHT)
#define PALETTE_DEPTH          4
#define PALETTE_SIZE           (256*PALETTE_DEPTH)

// Function 3
#define NUMBER_OF_OPEN_MAPS_OFFSET 0

// Function 5
#define LOCAL_COORD_OFFSET     0x04
#define LOCAL_MAP_LEVEL_OFFSET 0x28
#define ITEMS_COUNT_OFFSET     0x48
#define HITPOINTS_OFFSET       0x74
#define RADIATION_LEVEL_OFFSET 0x78
#define POISON_LEVEL_OFFSET    0x7C
#define FIRST_ITEM_OFFSET      0x80

// Function 6
#define BASE_STAT_OFFSET  0x08
#define BONUS_STAT_OFFSET 0x94
#define STAT_SIZE         0x8C
#define SKILLS_OFFSET     0x120
#define SKILLS_SIZE     (0x04 * 0x12)

// Function 7
#define KILLS_COUNT_OFFSET  0x00

// Function 9
#define PERKS_OFFSET  0x00

// Function 13
#define SKILL_POINTS_OFFSET     0x00
#define PLAYER_LEVEL_F13_OFFSET 0x04
#define EXPERIENCE_OFFSET       0x08

// Function 19
#define WORLD_MAP_X_OFFSET          0x08
#define WORLD_MAP_Y_OFFSET          0x0C
#define WORLD_MAP_CAR_MAP_OFFSET    0x24
#define WORLD_MAP_FUEL_OFFSET       0x28
#define WORLD_MAP_CITIES_NUM_OFFSET 0x2C

#define UNUSED(x) ((void)(x))

// Sav files
#define SAV_SVARNUM_OFFSET 0x20
#define SAV_ELEVATION_FLAGS_OFFSET 0x28
#define SAV_MVARNUM_OFFSET 0x30
#define SAV_MVAR_OFFSET 0xEC

#endif // DEFINES_H
