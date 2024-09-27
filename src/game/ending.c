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
#include "ending.h"
#include "../game/credits.h"
#include "../game/title.h"
#include "../world/map.h"
#include "../world/camera.h"
#include "../system/wipe.h"
#include "../world/stage.h"
#include "../system/text.h"
#include "../world/entities.h"
#include "../system/atlas.h"

extern App app;
extern Stage stage;

static void logic(void);
static void draw(void);
static void returnFromCredits(void);

static int timeout;

void initEnding(void)
{
	stage.entityTail = &stage.entityHead;
	stage.particleTail = &stage.particleHead;

	stage.num = 0;

	loadStage(0);

	/* prevent player control */
	stage.player->tick = NULL;

	initWipe(WIPE_FADE);

	timeout = FPS * 5;

	app.delegate.logic = logic;
	app.delegate.draw = draw;
}

static void logic(void)
{
	/* don't let time run out */
	stage.time = FPS * 60 * 60;

	doWipe();

	doEntities();

	if (--timeout <= 0)
	{
		initCredits(returnFromCredits);
	}
}

static void draw(void)
{
	app.dev.drawing = 0;

	// TODO: Force background draw
	// drawBackground();

	drawEntities(1);

	drawMap();

	drawEntities(0);

	// Fix camera to the player in order to prevent random map location focus
	doCamera();

	if (timeout > 0)
	{
		drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 50, TEXT_CENTER, app.colors.white, "Enhorabona, ho has aconseguit!");
		drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 15, 50, TEXT_CENTER, app.colors.white, "Visca Binissalem, i visca Sa Vermada!");
	}

	drawWipe();
}

static void returnFromCredits(void)
{
	destroyStage();

	initTitle();
}

