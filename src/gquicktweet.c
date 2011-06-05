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

#include <libnotify/notify.h>

#include "twitter.h"

user_t g_user;
char *g_token = NULL;
FILE *g_user_file = NULL;

void GtkSendTweet(GtkWidget *parent,gpointer data)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
    NotifyNotification *notify;
    GtkTextIter start,end;
    gtk_text_buffer_get_start_iter(buf,&start);
    gtk_text_buffer_get_end_iter(buf,&end);
    char *t_tweet = gtk_text_buffer_get_text(buf,&start,&end,TRUE);
    if(strlen(t_tweet) < 140)
    {
        Tweet(t_tweet,&g_user);
        notify = notify_notification_new("Just tweeted...",t_tweet,NULL,NULL);
        notify_notification_set_icon_from_pixbuf(notify,gdk_pixbuf_new_from_file("twit_logo.png",NULL));
        notify_notification_show(notify,NULL);
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
        GtkWidget *frame = gtk_frame_new("PIN:");
        gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,1);
        GtkWidget *entry = gtk_entry_new_with_max_length(7);
        gtk_entry_set_editable(GTK_ENTRY(entry),TRUE);
        gtk_container_add(GTK_CONTAINER(frame),entry);
        GtkWidget *button_auth = gtk_button_new_with_label("Submit");
        hbox = gtk_hbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        gtk_box_pack_start(GTK_BOX(hbox),button_auth,TRUE,TRUE,1);
        gtk_button_set_image(GTK_BUTTON(button),gtk_image_new_from_file("twit_logo.png"));
        gtk_button_set_image(GTK_BUTTON(button_auth),gtk_image_new_from_stock(GTK_STOCK_APPLY,GTK_ICON_SIZE_BUTTON));
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(GtkObtainToken),NULL);
        g_signal_connect(G_OBJECT(button_auth),"clicked",G_CALLBACK(GtkValidatePIN),entry);
        gtk_widget_show_all(window);
    }
}

int main (int argc, char *argv[])
{
	gtk_init(&argc,&argv);
	notify_init("GQuickTweet");
	GtkSettings *default_settings = gtk_settings_get_default();
	g_object_set(default_settings, "gtk-button-images", TRUE, NULL); 
	g_user_file = fopen(".gqtuser","r+");
	if(g_user_file)
	{
		fscanf(g_user_file,"%s",g_user.id);
	}
	
	if(!g_user_file || strlen(g_user.id) != 9)
	{
		g_user_file = fopen(".gqtuser","w+");
		AddUser();
	}
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,300);
	GtkWidget *vbox,*hbox;
	GtkWidget *frame;
	vbox = gtk_vbox_new(FALSE,2);
	hbox = gtk_hbox_new(FALSE,2);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,1);
	frame = gtk_frame_new("Tweet text");
	gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,1);
	GtkWidget *text_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view),TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view),GTK_WRAP_CHAR);
	gtk_container_add(GTK_CONTAINER(frame),text_view);
	GtkWidget *send_button = gtk_button_new_with_label("Tweet it!");
	gtk_button_set_image(GTK_BUTTON(send_button),gtk_image_new_from_stock(GTK_STOCK_YES,GTK_ICON_SIZE_BUTTON));
	hbox = gtk_hbox_new(FALSE,2);
	gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
	gtk_box_pack_start(GTK_BOX(hbox),send_button,TRUE,FALSE,1);
	g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);
	g_signal_connect(G_OBJECT(send_button),"clicked",G_CALLBACK(GtkSendTweet),text_view);
	gtk_widget_show_all(window);
	gtk_main();
	notify_uninit();
	return 0;
}
