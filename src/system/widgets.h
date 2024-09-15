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

void updateControlWidget(Widget *w, int c);
Widget *getWidget(const char *name, const char *groupName);
void showWidgets(const char *groupName, int visible);
void calculateWidgetFrame(const char *groupName);
void drawWidgetFrame(void);
void drawWidgets(const char *groupName);
void doWidgets(const char *groupName);
void initWidgets(void);
