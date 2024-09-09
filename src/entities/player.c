/*
Copyright (C) 2019,2022 Parallel Realities

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
#include "player.h"
#include "../json/cJSON.h"
#include "../system/atlas.h"
#include "../world/particles.h"
#include "../system/sound.h"
#include "../world/entityFactory.h"
#include "../system/controls.h"

extern Entity *self;
extern Game game;
extern Stage stage;

static void tick(void);
static void die(void);
static void load(cJSON *root);
static void save(cJSON *root);

static AtlasImage *normalTexture;
static AtlasImage *shieldTexture;
static AtlasImage *plungerTexture;
static AtlasImage *waterPistolTexture;
static AtlasImage *bulletTexture;
static float px;
static float py;

void initPlayer(Entity *e)
{
	Walter *p;

	stage.player = e;

	p = malloc(sizeof(Walter));
	memset(p, 0, sizeof(Walter));

	e->typeName = "player";
	e->data = p;
	e->type = ET_PLAYER;
	e->atlasImage = getAtlasImage("gfx/entities/guy.png", 1);
	e->flags = EF_PUSH+EF_PUSHABLE+EF_SLOW_PUSH;
	e->tick = tick;
	e->die = die;
	e->load = load;
	e->save = save;

	e->w = e->atlasImage->rect.w;
	e->h = e->atlasImage->rect.h;

	normalTexture = e->atlasImage;

	shieldTexture = getAtlasImage("gfx/entities/guyShield.png", 1);

	plungerTexture = getAtlasImage("gfx/entities/guyPlunger.png", 1);

	waterPistolTexture = getAtlasImage("gfx/entities/guyPistol.png", 1);

	bulletTexture = getAtlasImage("gfx/entities/waterBullet.png", 1);

	px = e->x;
	py = e->y;
}

static void tick(void)
{
	Walter *p;

	p = (Walter*)self->data;

	px = self->x;
	py = self->y;

	self->dx = 0;
	p->action = 0;

	self->w = self->atlasImage->rect.w;
	self->h = self->atlasImage->rect.h;

	if (self->health > 0)
	{
		if (isControl(CONTROL_LEFT))
		{
			self->dx = -PLAYER_MOVE_SPEED;

			self->facing = FACING_LEFT;
		}

		if (isControl(CONTROL_RIGHT))
		{
			self->dx = PLAYER_MOVE_SPEED;

			self->facing = FACING_RIGHT;
		}

		if (isControl(CONTROL_JUMP) && self->isOnGround)
		{
			self->dy = -20;

			playSound(SND_JUMP, CH_PLAYER);
		}
	}
}

static void die(void)
{
	addDeathParticles(self->x, self->y);

	playSound(SND_DEATH, CH_PLAYER);
}

static void load(cJSON *root)
{
	self->facing = strcmp(cJSON_GetObjectItem(root, "facing")->valuestring, "left") == 0 ? 0 : 1;
}

static void save(cJSON *root)
{
	cJSON_AddStringToObject(root, "facing", self->facing == 0 ? "left" : "right");
}

