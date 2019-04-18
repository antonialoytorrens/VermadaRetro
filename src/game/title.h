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

#include "../common.h"

extern void blitAtlasImage(AtlasImage *atlasImage, int x, int y, int center, SDL_RendererFlip flip);
extern void doWidgets(const char *groupName);
extern void drawWidgets(const char *groupName);
extern AtlasImage *getAtlasImage(char *filename, int required);
extern Widget *getWidget(const char *name, const char *groupName);
extern void initStage(void);
extern void loadRandomStageMusic(void);
extern void loadStage(int randomTiles);
extern void drawText(int x, int y, int size, int align, SDL_Color color, const char *format, ...);
extern void drawEntities(int background);
extern void drawRect(int x, int y, int w, int h, int r, int g, int b, int a);
extern void drawMap(void);
extern void randomizeTiles(void);

extern App app;
extern Stage stage;
