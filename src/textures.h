#ifndef TEXTURES_H
#define TEXTURES_H

#include "textures/floor_48x48.c"
#include "textures/wall_48x48.c"
#include "textures/door_closed_48x48.c"
#include "textures/door_open_48x48.c"
#include "textures/ladder_48x48.c"
#include "textures/fire_48x48.c"
#include "textures/acid_48x48.c"
#include "textures/spikes_48x48.c"
#include "textures/chest_48x48.c"
#include "textures/altar_48x48.c"
#include "textures/cauldron_48x48.c"
#include "textures/bookshelf_48x48.c"
#include "textures/coffin_48x48.c"
#include "textures/statue_48x48.c"
#include "textures/table_48x48.c"

static const guint8 *tile_textures[] = {
    // Standard Navigation
	[FLOOR]       = floor_tex.pixel_data,
	[WALL]		  = wall_tex.pixel_data,
	[DOOR_CLOSED] = door_closed_tex.pixel_data,
	[DOOR_OPEN]   = door_open_tex.pixel_data,
	[LADDER]	  = ladder_tex.pixel_data,

	// Damage Tiles
	[FIRE]        = fire_tex.pixel_data,
	[ACID]        = acid_tex.pixel_data,
	[SPIKES]	  = spikes_tex.pixel_data,

	// Interactive Objects
	[CHEST]       = chest_tex.pixel_data,
	[ALTAR]       = altar_tex.pixel_data,
	[CAULDRON]    = cauldron_tex.pixel_data,

	// Decorative Objects
	[BOOKSHELF]   = bookshelf_tex.pixel_data,
	[COFFIN]	  = coffin_tex.pixel_data,
	[STATUE]	  = statue_tex.pixel_data,
	[TABLE]       = table_tex.pixel_data,
};

#endif
