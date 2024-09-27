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
#include "toilet.h"
#include "../json/cJSON.h"
#include "../system/atlas.h"
#include "../world/particles.h"
#include "../system/sound.h"

extern Entity *self;
extern Stage stage;

static void touch(Entity *other);

void initToilet(Entity *e)
{
	Toilet *t;

	t = malloc(sizeof(Toilet));
	memset(t, 0, sizeof(Toilet));

	e->typeName = "toilet";
	e->type = ET_TOILET;
	e->data = t;
	e->atlasImage = getAtlasImage("gfx/entities/church.png", 1);
	e->w = e->atlasImage->rect.w;
	e->h = e->atlasImage->rect.h;
	e->flags = EF_NO_ENT_CLIP+EF_STATIC;
	e->touch = touch;
}

static void touch(Entity *other)
{
	Toilet *t;

	if (other != NULL)
	{
		t = (Toilet *)self->data;

		if (other->type == ET_PLAYER)
		{
			t->animTimer = FPS;

			other->health = 0;

			/* just remove player */
			other->die = NULL;

			stage.status = SS_COMPLETE;

			stage.nextStageTimer = FPS * 3;

			addToiletSplashParticles(self->x + self->atlasImage->rect.w / 2, self->y + self->atlasImage->rect.h / 2);

			playPositionalSound(SND_FINISH, CH_PLAYER, self->x, self->y, stage.player->x, stage.player->y);

			stage.status = SS_COMPLETE;
		}
	}
}
