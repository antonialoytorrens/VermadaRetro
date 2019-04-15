/*
Copyright (C) 2019 Parallel Realities

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

#include "manholeCover.h"

static void tick(void);
static void touch(Entity *other);
static void die(void);
static void save(cJSON *root);

void initManholeCover(Entity *e)
{
	ManholeCover *m;
	
	m = malloc(sizeof(ManholeCover));
	memset(m, 0, sizeof(ManholeCover));
	
	e->type = ET_ITEM;
	e->data = m;
	e->atlasImage = getAtlasImage("gfx/entities/manholeCover.png", 1);
	e->w = e->atlasImage->rect.w;
	e->h = e->atlasImage->rect.h;
	e->flags = EF_WEIGHTLESS;
	e->tick = tick;
	e->touch = touch;
	e->die = die;
	
	e->save = save;
}

static void tick(void)
{
	ManholeCover *m;
	
	m = (ManholeCover*)self->data;
	
	m->bobValue += 0.1;
	
	self->y += sin(m->bobValue) * 0.25;
}

static void touch(Entity *other)
{
	if (self->health > 0 && other != NULL && (other->type == ET_PLAYER || other->type == ET_CLONE) && (!(other->flags & EF_SHIELDED)))
	{
		self->health = 0;
		
		other->flags |= EF_SHIELDED;
		
		addPowerupParticles(self->x + self->w / 2, self->y + self->h / 2);
		
		playSound(SND_MANHOLE_COVER, CH_ITEM);
	}
}

static void die(void)
{
	addPowerupParticles(self->x + self->w / 2, self->y + self->h / 2);
}

static void save(cJSON *root)
{
	cJSON_AddStringToObject(root, "type", "manholeCover");
}
