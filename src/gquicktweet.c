//      gquicktweet.c
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

#include <stdio.h>
#include <string.h>

#include <gtk/gtk.h>

#include "twitter.h"

user_t g_user;
char *g_token = NULL;
FILE *g_user_file = NULL;

void GtkSendTweet(GtkWidget *parent,gpointer data)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
    GtkTextIter start,end;
    gtk_text_buffer_get_start_iter(buf,&start);
    gtk_text_buffer_get_end_iter(buf,&end);
    char *t_tweet = gtk_text_buffer_get_text(buf,&start,&end,TRUE);
    if(strlen(t_tweet) < 140)
    {
        Tweet(t_tweet,&g_user);
    }
}

void GtkObtainToken(GtkWidget *parent,gpointer data)
{
    g_token = ObtainToken();
}

void GtkValidatePIN(GtkWidget *parent,gpointer data)
{
    char *pin = gtk_editable_get_chars(GTK_EDITABLE(data),0,-1);
    if(strlen(pin) == 7)
    {
        ValidatePIN(pin,g_token,&g_user);
        fwrite(&g_user,sizeof(user_t),1,g_user_file);
    }
}

void AddUser(void)
{
    if(g_user_file)
    {
        GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window),"Authorize App");
        gtk_window_set_default_size(GTK_WINDOW(window),200,70);
        GtkWidget *vbox,*hbox;
        vbox = gtk_vbox_new(FALSE,2);
        hbox = gtk_hbox_new(FALSE,2);
        gtk_container_add(GTK_CONTAINER(window),vbox);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        GtkWidget *button = gtk_button_new_with_label("Get PIN");
        gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,1);
        hbox = gtk_hbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        GtkWidget *label = gtk_label_new("Insert PIN: ");
        gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,FALSE,1);
        GtkWidget *entry = gtk_entry_new_with_max_length(7);
        gtk_entry_set_editable(GTK_ENTRY(entry),TRUE);
        gtk_box_pack_start(GTK_BOX(hbox),entry,FALSE,FALSE,1);
        GtkWidget *button_auth = gtk_button_new_with_label("Submit");
        hbox = gtk_hbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        gtk_box_pack_start(GTK_BOX(hbox),button_auth,TRUE,TRUE,1);
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(GtkObtainToken),NULL);
        g_signal_connect(G_OBJECT(button_auth),"clicked",G_CALLBACK(GtkValidatePIN),entry);
        gtk_widget_show_all(window);
    }
}

int main (int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	g_user_file = fopen(".gqtuser","r+");
	if(!g_user_file)
	{
		g_user_file = fopen(".gqtuser","w+");
		AddUser();
	}
	else
	{
		fscanf(g_user_file,"%s",g_user.id);
	}
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,300);
	GtkWidget *vbox,*hbox;
	vbox = gtk_vbox_new(FALSE,2);
	hbox = gtk_hbox_new(FALSE,2);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,1);
	GtkWidget *text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view),TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view),GTK_WRAP_CHAR);
	gtk_box_pack_start(GTK_BOX(hbox),text_view,TRUE,TRUE,1);
	GtkWidget *send_button = gtk_button_new_with_label("Tweet it!");
	hbox = gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
	gtk_box_pack_start(GTK_BOX(hbox),send_button,TRUE,FALSE,1);
	g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);
	g_signal_connect(G_OBJECT(send_button),"clicked",G_CALLBACK(GtkSendTweet),text_view);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
