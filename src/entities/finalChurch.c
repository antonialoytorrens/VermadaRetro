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

#include "../common.h"
#include "finalChurch.h"
#include "../json/cJSON.h"
#include "../system/atlas.h"

extern Stage stage;

static void touch(Entity *other);

void initFinalChurch(Entity *e)
{
	Church *t;

	t = malloc(sizeof(Church));
	memset(t, 0, sizeof(Church));

	e->typeName = "finalChurch";
	e->facing = 0;
	e->type = ET_CHURCH;
	e->data = t;
	e->atlasImage = getAtlasImage("gfx/entities/church.png", 1);
	e->w = e->atlasImage->rect.w;
	e->h = e->atlasImage->rect.h;
	e->flags = EF_NO_ENT_CLIP+EF_STATIC;
	e->touch = touch;
}

static void touch(Entity *other)
{
	if (other != NULL && other->type == ET_PLAYER)
	{
		stage.status = SS_GAME_COMPLETE;
	}
}

