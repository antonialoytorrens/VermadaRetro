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
#include "game.h"
#include "../json/cJSON.h"
#include "../system/lookup.h"
#include "../game/meta.h"
#include "../system/util.h"
#include "../system/io.h"

extern App app;
extern Game game;

void loadConfig(const char *filename)
{
	cJSON *root, *controls;
	char *json;

	json = readFile(filename);

	root = cJSON_Parse(json);

	app.config.soundVolume = cJSON_GetObjectItem(root, "soundVolume")->valueint;
	app.config.musicVolume = cJSON_GetObjectItem(root, "musicVolume")->valueint;
	app.config.winWidth = cJSON_GetObjectItem(root, "winWidth")->valueint;
	app.config.winHeight = cJSON_GetObjectItem(root, "winHeight")->valueint;
	app.config.fullscreen = cJSON_GetObjectItem(root, "fullscreen")->valueint;
	app.config.tips = cJSON_GetObjectItem(root, "tips")->valueint;
	app.config.deadzone = getJSONIntVal(root, "deadzone", 64) * 256;

	controls = cJSON_GetObjectItem(root, "keyControls");

	app.config.keyControls[CONTROL_LEFT] = cJSON_GetObjectItem(controls, "left")->valueint;
	app.config.keyControls[CONTROL_RIGHT] = cJSON_GetObjectItem(controls, "right")->valueint;
	app.config.keyControls[CONTROL_JUMP] = cJSON_GetObjectItem(controls, "jump")->valueint;
	app.config.keyControls[CONTROL_RESTART] = cJSON_GetObjectItem(controls, "restart")->valueint;
	app.config.keyControls[CONTROL_PAUSE] = cJSON_GetObjectItem(controls, "pause")->valueint;

	controls = cJSON_GetObjectItem(root, "joypadControls");

	app.config.joypadControls[CONTROL_LEFT] = cJSON_GetObjectItem(controls, "left")->valueint;
	app.config.joypadControls[CONTROL_RIGHT] = cJSON_GetObjectItem(controls, "right")->valueint;
	app.config.joypadControls[CONTROL_JUMP] = cJSON_GetObjectItem(controls, "jump")->valueint;
	app.config.joypadControls[CONTROL_RESTART] = cJSON_GetObjectItem(controls, "restart")->valueint;
	app.config.joypadControls[CONTROL_PAUSE] = cJSON_GetObjectItem(controls, "pause")->valueint;

	cJSON_Delete(root);

	free(json);
}
