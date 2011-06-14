//      core.h
//      
//      Copyright 2011 Alfredo Liguoro <freddy@haxaddicts.net>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#ifndef _CORE_H_
#define _CORE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "libui_gtk/libui_gtk.h"

#define CONSUMER_KEY "dufOWZyhvWBwGO0osvfBQ"
#define CONSUMER_SECRET "91Sw4yExaTbqN0bq8cbnyA20B8V6FlIctFk9xj71M0"

#define WIDTH 200
#define HEIGHT 250

typedef struct
{
	char id[10];
	char screen_name[140];
	char token[50];
	char secret[41];
}Twitter_user;

typedef struct
{
	Twitter_user *user;
	GtkWidget *window;
	GtkWidget *button_close;
	GtkWidget *button_move;
	GtkWidget *button_tweet;
	GtkWidget *icon;
	GtkWidget *fixed;
	GtkWidget *tw_view;
	GtkWidget *frame;
	GtkWidget *pinentry;
	int x_window;
	int y_window;
	gboolean presence;
	FILE *userconf;
	char *token;
}GQT_Core;

#endif
