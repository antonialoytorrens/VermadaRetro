/*
Copyright (C) 2019,2022 Parallel Realities
Copyright (C) 2024 Antoni Aloy Torrens

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#define PI 3.14159265358979323846
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CAROLINE(a,b) (((a)<(b))?(a):(b))
#define STRNCPY(dest, src, n) strncpy(dest, src, n); dest[n - 1] = '\0'

#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720

#define CONFIG_FILENAME    "config.json"
#define SAVE_FILENAME      "game.json"

#define FPS   60

#define MAX_TILES    255

#define TILE_SIZE    48

#define MAP_WIDTH    108
#define MAP_HEIGHT   15

#define MAP_RENDER_WIDTH    27
#define MAP_RENDER_HEIGHT   15

#define PLAYER_MOVE_SPEED 6

#define MAX_TIPS    12

#define MAX_QT_CANDIDATES   128

#define MAX_NAME_LENGTH           32
#define MAX_DESCRIPTION_LENGTH    256
#define MAX_LINE_LENGTH           1024
#define MAX_FILENAME_LENGTH       256
#define MAX_PATH_LENGTH           4096

#define MAX_KEYBOARD_KEYS   350
#define MAX_MOUSE_BUTTONS   6

#define NUM_ATLAS_BUCKETS 2

#define EF_NONE            0
#define EF_WEIGHTLESS      (2 << 0)
#define EF_SOLID           (2 << 1)
#define EF_PUSH            (2 << 2)
#define EF_NO_WORLD_CLIP   (2 << 3)
#define EF_NO_MAP_BOUNDS   (2 << 4)
#define EF_NO_ENT_CLIP     (2 << 5)
#define EF_INVISIBLE       (2 << 6)
#define EF_STATIC          (2 << 7)

enum
{
	ET_PLAYER,
	ET_CHURCH,
	ET_ITEM,
	ET_STRUCTURE,
	ET_TRAP,
	ET_DECORATION
};

enum
{
	WIPE_FADE,
	WIPE_IN,
	WIPE_OUT
};

enum
{
	WT_BUTTON,
	WT_SELECT,
	WT_INPUT
};

enum
{
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT
};

enum
{
	FACING_LEFT,
	FACING_RIGHT
};

enum
{
	SND_JUMP,
	SND_COIN,
	SND_CLOCK,
	SND_EXPIRED,
	SND_NUDGE,
	SND_FANFARE,
	SND_FINISH,
	SND_WIPE,
	SND_NEGATIVE,
	SND_ITEM,
	SND_DEATH,
	SND_TIP,
	SND_MAX
};

enum
{
	CH_PLAYER,
	CH_COIN,
	CH_ITEM,
	CH_CLOCK,
	CH_STRUCTURE,
	CH_WIDGET,
	CH_MAX
};

enum
{
	SS_INCOMPLETE,
	SS_COMPLETE,
	SS_FAILED,
	SS_GAME_COMPLETE
};

enum
{
	JOYPAD_AXIS_X,
	JOYPAD_AXIS_Y,
	JOYPAD_AXIS_MAX
};

enum
{
	CONTROL_LEFT,
	CONTROL_RIGHT,
	CONTROL_UP,
	CONTROL_DOWN,
	CONTROL_JUMP,
	CONTROL_RESTART,
	CONTROL_PAUSE,
	CONTROL_MAX
};
