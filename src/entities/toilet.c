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

#include "toilet.h"

static void erupt(void);
static void idle(void);
static void escape(void);
static void touch(Entity *other);
static void load(cJSON *root);
static void save(cJSON *root);

static AtlasImage *idleTexture;
static AtlasImage *eruptFrames[2];
static AtlasImage *escapeFrames[5];
static AtlasImage *stinkFrames[2];
static AtlasImage *plungingFrames[2];

void initToilet(Entity *e)
{
	Toilet *t;
	char filename[MAX_FILENAME_LENGTH];
	int i;
	
	t = malloc(sizeof(Toilet));
	memset(t, 0, sizeof(Toilet));
	
	for (i = 0 ; i < 5 ; i++)
	{
		sprintf(filename, "gfx/entities/toiletEscape%d.png", i + 1);
		
		escapeFrames[i] = getAtlasImage(filename, 1);
	}
	
	eruptFrames[0] = getAtlasImage("gfx/entities/toiletErupt1.png", 1);
	eruptFrames[1] = getAtlasImage("gfx/entities/toiletErupt2.png", 1);
	
	stinkFrames[0] = getAtlasImage("gfx/entities/toiletStink1.png", 1);
	stinkFrames[1] = getAtlasImage("gfx/entities/toiletStink2.png", 1);
	
	plungingFrames[0] = getAtlasImage("gfx/entities/toiletPlunging1.png", 1);
	plungingFrames[1] = getAtlasImage("gfx/entities/toiletPlunging2.png", 1);
	
	idleTexture = getAtlasImage("gfx/entities/toilet.png", 1);
	
	e->typeName = "toilet";
	e->type = ET_TOILET;
	e->data = t;
	e->atlasImage = idleTexture;
	e->w = e->atlasImage->rect.w;
	e->h = e->atlasImage->rect.h;
	e->tick = idle;
	e->touch = touch;
	
	e->load = load;
	e->save = save;
}

static void idle(void)
{
	if (stage.time / 60 == 0)
	{
		self->atlasImage = eruptFrames[0];
		
		self->tick = erupt;
		
		self->touch = NULL;
	}
}

static void stink(void)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	t->animTimer++;
	
	if (t->animTimer % 30 == 0)
	{
		if (++t->frameNum > 1)
		{
			t->frameNum = 0;
		}
	}
	
	self->atlasImage = stinkFrames[t->frameNum];
}

static void plunging(void)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	t->animTimer++;
	
	if (t->animTimer % 16 == 0)
	{
		if (++t->frameNum > 1)
		{
			t->frameNum = 0;
		}
		
		playSound(SND_PLUNGE, -1);
	}
	
	self->atlasImage = plungingFrames[t->frameNum];
	
	if (--t->requiresPlunger <= 0)
	{
		self->tick = idle;
		
		self->atlasImage = idleTexture;
		
		self->touch = touch;
	}
}

static void erupt(void)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	t->animTimer++;
	
	if (t->animTimer % 30 == 0)
	{
		if (++t->frameNum > 1)
		{
			t->frameNum = 0;
		}
		
		self->atlasImage = eruptFrames[t->frameNum];
	}
}

static void escape(void)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	if (--t->animTimer == 0)
	{
		t->animTimer = 8;
		
		t->frameNum++;
		
		if (t->frameNum < 5)
		{
			self->atlasImage = escapeFrames[t->frameNum];
		}
		else
		{
			self->atlasImage = idleTexture;
		}
	}
}

static void touch(Entity *other)
{
	Toilet *t;
	
	if (other != NULL)
	{
		t = (Toilet*)self->data;
		
		if (!t->requiresPlunger && other->type == ET_PLAYER)
		{
			self->tick = escape;
			
			self->atlasImage = escapeFrames[0];
			
			t->animTimer = FPS;
			
			other->health = 0;
			
			/* just remove player */
			other->die = NULL;
			
			stage.status = SS_COMPLETE;
			
			stage.nextStageTimer = FPS * 3;
			
			playSound(SND_FLUSH, CH_PLAYER);
		}
		else if (other->flags & EF_PLUNGING)
		{
			other->flags &= ~EF_PLUNGING;
			
			self->tick = plunging;
			
			self->touch = NULL;
		}
	}
}

static void load(cJSON *root)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	self->facing = strcmp(cJSON_GetObjectItem(root, "facing")->valuestring, "left") == 0 ? 0 : 1;
	
	if (cJSON_GetObjectItem(root, "requiresPlunger"))
	{
		t->requiresPlunger = FPS * 2;
		
		self->atlasImage = stinkFrames[0];
		
		self->tick = stink;
	}
}

static void save(cJSON *root)
{
	Toilet *t;
	
	t = (Toilet*)self->data;
	
	cJSON_AddStringToObject(root, "type", self->typeName);
	cJSON_AddStringToObject(root, "facing", self->facing == 0 ? "left" : "right");
	
	if (t->requiresPlunger)
	{
		cJSON_AddNumberToObject(root, "requiresPlunger", 1);
	}
}
