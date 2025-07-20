#pragma once

// Each game is defined as YYYY_MM_DD_MA_MI_PA, where
// - YYYY = release year
// - MM   = release month
// - DD   = release day
// - MA   = major version
// - MI   = minor version
// - PA   = patch version

#define GameCode(Year, Month, Day, Major, Minor, Patch) \
    ((Year * 100 * 100 * 100 * 100 * 100)  \
  +       (Month * 100 * 100 * 100 * 100)  \
  +               (Day * 100 * 100 * 100)  \
  +                   (Major * 100 * 100)  \
  +                         (Minor * 100)  \
  +                               (Patch)) 

// Spyro's Adventure
// Patch number corresponds to revision number
#define SKYSA_01_00_01     GameCode(2011, 10, 13,  1,  0,  1)
#define SKYSA_01_00_02     GameCode(2011, 10, 13,  1,  0,  2)
#define SKYSA_01_00_03     GameCode(2011, 10, 13,  1,  0,  3)
#define SKYSA_BEGIN        SKYSA_01_00_01
#define SKYSA_END          SKYSA_01_00_03

// Skylanders Trap Team
#define SKYTT_01_00_00     GameCode(2014, 10,  2,  1,  0,  0)
#define SKYTT_01_01_00     GameCode(2014, 10,  2,  1,  1,  0)
#define SKYTT_BEGIN        SKYTT_01_00_00
#define SKYTT_END          SKYTT_01_01_00

// Skylanders SuperChargers
#define SKYSC_01_00_00     GameCode(2015,  9, 20,  1,  0,  0)
#define SKYSC_01_02_02     GameCode(2015,  9, 20,  1,  2,  2)
#define SKYSC_01_06_00     GameCode(2015,  9, 20,  1,  6,  0)
#define SKYSC_01_06_06     GameCode(2015,  9, 20,  1,  6,  6)
#define SKYSC_BEGIN        SKYSC_01_00_00
#define SKYSC_END          SKYSC_01_06_06

// Skylanders Imaginators, these dates are wrong
#define SKYIM_01_00_00     GameCode(2016, 10, 14,  1,  0,  0)
#define SKYIM_01_01_00     GameCode(2016, 10, 14,  1,  1,  0)
#define SKYIM_BEGIN        SKYIM_01_00_00
#define SKYIM_END          SKYIM_01_01_00


#ifndef TARGET_GAME
#error No game was specified
#endif // TARGET_GAME
