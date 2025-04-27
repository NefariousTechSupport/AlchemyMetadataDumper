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

// Skylanders Trap Team
#define SKYTT_01_00_00     GameCode(2014, 10,  2,  1,  0,  0)
#define SKYTT_01_01_00     GameCode(2014, 10,  2,  1,  1,  0)

// Skylanders SuperChargers
#define SKYSC_01_00_00     GameCode(2015,  9, 20,  1,  0,  0)
#define SKYSC_01_02_02     GameCode(2015,  9, 20,  1,  2,  2)
#define SKYSC_01_06_00     GameCode(2015,  9, 20,  1,  6,  0)
#define SKYSC_01_06_06     GameCode(2015,  9, 20,  1,  6,  6)

// Skylanders Imaginators, these dates are wrong
#define SKYIM_01_00_00     GameCode(2016, 10, 14,  1,  0,  0)
#define SKYIM_01_01_00     GameCode(2016, 10, 14,  1,  1,  0)