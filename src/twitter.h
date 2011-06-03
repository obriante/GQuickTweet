//      twitter.h
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

#ifndef _TWITTER_H_
#define _TWITTER_H_

typedef struct
{
	char id[10];
	char screen_name[140];
	char token[50];
	char secret[41];
}user_t;

void Tweet(const char *msg,user_t *user);
void ValidatePIN(const char *pin,const char *token,user_t *user);
char *ObtainToken(void);

#endif
