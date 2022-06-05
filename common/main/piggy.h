/*
 * Portions of this file are copyright Rebirth contributors and licensed as
 * described in COPYING.txt.
 * Portions of this file are copyright Parallax Software and licensed
 * according to the Parallax license below.
 * See COPYING.txt for license details.

THE COMPUTER CODE CONTAINED HEREIN IS THE SOLE PROPERTY OF PARALLAX
SOFTWARE CORPORATION ("PARALLAX").  PARALLAX, IN DISTRIBUTING THE CODE TO
END-USERS, AND SUBJECT TO ALL OF THE TERMS AND CONDITIONS HEREIN, GRANTS A
ROYALTY-FREE, PERPETUAL LICENSE TO SUCH END-USERS FOR USE BY SUCH END-USERS
IN USING, DISPLAYING,  AND CREATING DERIVATIVE WORKS THEREOF, SO LONG AS
SUCH USE, DISPLAY OR CREATION IS FOR NON-COMMERCIAL, ROYALTY OR REVENUE
FREE PURPOSES.  IN NO EVENT SHALL THE END-USER USE THE COMPUTER CODE
CONTAINED HEREIN FOR REVENUE-BEARING PURPOSES.  THE END-USER UNDERSTANDS
AND AGREES TO THE TERMS HEREIN AND ACCEPTS THE SAME BY USE OF THIS FILE.
COPYRIGHT 1993-1999 PARALLAX SOFTWARE CORPORATION.  ALL RIGHTS RESERVED.
*/

/*
 *
 * Interface to piggy functions.
 *
 */

#pragma once

#include <physfs.h>
#include "sounds.h"
#include "inferno.h"
#include "gr.h"
#include "fwd-partial_range.h"
#include "dxxsconf.h"
#include "dsx-ns.h"

#ifdef dsx
namespace dsx {
struct digi_sound;
}
#endif

#define D1_SHARE_BIG_PIGSIZE    5092871 // v1.0 - 1.4 before RLE compression
#define D1_SHARE_10_PIGSIZE     2529454 // v1.0 - 1.2
#define D1_SHARE_PIGSIZE        2509799 // v1.4
#define D1_10_BIG_PIGSIZE       7640220 // v1.0 before RLE compression
#define D1_10_PIGSIZE           4520145 // v1.0
#define D1_PIGSIZE              4920305 // v1.4 - 1.5 (Incl. OEM v1.4a)
#define D1_OEM_PIGSIZE          5039735 // v1.0
#define D1_MAC_PIGSIZE          3975533
#define D1_MAC_SHARE_PIGSIZE    2714487

#if defined(DXX_BUILD_DESCENT_II)
#define D1_PIGFILE              "descent.pig"
#define MAX_ALIASES 20

struct alias
{
	char alias_name[FILENAME_LEN];
	char file_name[FILENAME_LEN];
};

extern std::array<alias, MAX_ALIASES> alias_list;
extern unsigned Num_aliases;

extern int Piggy_hamfile_version;
extern int Pigfile_initialized;
#endif

// an index into the bitmap collection of the piggy file
struct bitmap_index
{
	ushort index;
};
#define DEFINE_BITMAP_SERIAL_UDT() DEFINE_SERIAL_UDT_TO_MESSAGE(bitmap_index, b, (b.index))

struct BitmapFile
{
	std::array<char, 13> name;
};

#if defined(DXX_BUILD_DESCENT_I)
extern int MacPig;
extern int PCSharePig;

extern grs_bitmap bogus_bitmap;
#endif
extern std::array<uint8_t, 64 * 64> bogus_data;

#ifdef dsx
namespace dsx {
int properties_init();
void piggy_close();
bitmap_index piggy_register_bitmap(grs_bitmap &bmp, const char * name, int in_file);
int piggy_register_sound( digi_sound * snd, const char * name, int in_file );
bitmap_index piggy_find_bitmap(const char *name);
}
int piggy_find_sound(const char *name);

void piggy_read_bitmap_data(grs_bitmap * bmp);
namespace dsx {
void piggy_read_sound_data(digi_sound *snd);
void piggy_load_level_data();

#if defined(DXX_BUILD_DESCENT_I)
constexpr std::integral_constant<unsigned, 1800> MAX_BITMAP_FILES{};
#define PIGGY_PC_SHAREWARE 2
#elif defined(DXX_BUILD_DESCENT_II)
// Upped for CD Enhanced
constexpr std::integral_constant<unsigned, 2620> MAX_BITMAP_FILES{};
#endif
#define MAX_SOUND_FILES     MAX_SOUNDS

using GameBitmaps_array = std::array<grs_bitmap, MAX_BITMAP_FILES>;
extern std::array<digi_sound, MAX_SOUND_FILES> GameSounds;
extern GameBitmaps_array GameBitmaps;
void piggy_bitmap_page_in(GameBitmaps_array &, bitmap_index bmp);
#  define  PIGGY_PAGE_IN(bmp) _piggy_page_in(GameBitmaps, bmp)
static inline void _piggy_page_in(GameBitmaps_array &GameBitmaps, bitmap_index bmp)
{
	if (GameBitmaps[bmp.index].get_flag_mask(BM_FLAG_PAGED_OUT))
        piggy_bitmap_page_in(GameBitmaps, bmp);
}

#if defined(DXX_BUILD_DESCENT_I)
void piggy_read_sounds(int pc_shareware);
#elif defined(DXX_BUILD_DESCENT_II)
void piggy_init_pigfile(const char *filename);
void piggy_read_sounds(void);

//reads in a new pigfile (for new palette)
//returns the size of all the bitmap data
void piggy_new_pigfile(char *pigname);

//loads custom bitmaps for current level
void load_bitmap_replacements(const char *level_name);
//if descent.pig exists, loads descent 1 texture bitmaps
void load_d1_bitmap_replacements();
/*
 * Find and load the named bitmap from descent.pig
 */
grs_bitmap *read_extra_bitmap_d1_pig(const char *name, grs_bitmap &out);
int read_hamfile();
void read_sndfile(int required);
#endif
}

namespace dcx {
/*
 * reads a bitmap_index structure from a PHYSFS_File
 */
void bitmap_index_read(PHYSFS_File *fp, bitmap_index &bi);
void bitmap_index_read_n(PHYSFS_File *fp, partial_range_t<bitmap_index *> r);
}

extern void remove_char( char * s, char c );	// in piggy.c
#define REMOVE_EOL(s)		remove_char((s),'\n')
#define REMOVE_COMMENTS(s)	remove_char((s),';')
#define REMOVE_DOTS(s)  	remove_char((s),'.')

extern unsigned Num_bitmap_files;
extern int Num_sound_files;
extern ubyte bogus_bitmap_initialized;
namespace dsx {
extern digi_sound bogus_sound;
}
#endif
#define space_tab " \t"
#define equal_space " \t="
#if defined(DXX_BUILD_DESCENT_I)
#include "hash.h"
extern hashtable AllBitmapsNames;
extern hashtable AllDigiSndNames;
#elif defined(DXX_BUILD_DESCENT_II)
extern std::array<BitmapFile, MAX_BITMAP_FILES> AllBitmaps;
#endif
void swap_0_255(grs_bitmap &bmp);

enum class pig_bitmap_offset : unsigned
{
	None = 0
};
