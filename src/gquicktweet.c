/*
 * Copyright © 2012 - Alfredo Liguoro <freddy@haxaddicts.net>
 * Orazio Briante <orazio.briante@hotmail.it>
 *
 * Licensed under the GNU General Public License Version 3
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 * or write to the Free Software Foundation, Inc., 51 Franklin St
 * Fifth Floor, Boston, MA  02110-1301  USA
 */



#include <twitc/twitc.h>

#include <appkeys.h>
#include <gquicktweet.h>

#include <libnotify/notify.h>

#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>



static string_t programDir=NULL;
static string_t configDir=NULL;
static string_t configFile=NULL;
static string_t fileLog=NULL;
static twitterURLS_t *twURLS=NULL;

static user_t *g_user=NULL;
static string_t g_token=NULL;

static GtkWidget *window=NULL;
static GtkWidget *rWindow=NULL;

void
program_path_init()
{
  uid_t uid = geteuid();
  passwd_t *pwd= getpwuid(uid);

  asprintf(&programDir, "%s/%s", pwd->pw_dir,PROG_PATH);
  debug("programDir:\t%s",programDir);

  asprintf(&configDir, "%s/%s", programDir,CONFIG_DIR);
  debug("configDir:\t%s",configDir);

  asprintf(&configFile, "%s/%s",configDir,CONFIG_FILE);
  debug("configFile:\t%s",configFile);

  asprintf(&fileLog, "%s/%s.log",configDir,PROG_NAME);
  debug("fileLog:\t%s",fileLog);

}


void printGError(GError* error)
{
  if(error)
    {
      error((string_t)error->message);
      g_error_free(error);
      error=NULL;
    }
}

void GtkSendTweet(GtkWidget *parent,gpointer data)
{
  GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));
  NotifyNotification *notify;
  GtkTextIter start,end;
  gtk_text_buffer_get_start_iter(buf,&start);
  gtk_text_buffer_get_end_iter(buf,&end);
  string_t t_tweet = gtk_text_buffer_get_text(buf,&start,&end,TRUE);

  if(strlen(t_tweet) < 140 && strlen(t_tweet) >0)
    {

      if(!sendTweet(twURLS, g_user, t_tweet))
        info("Message correctly tweetted!");
      else
        info("Message not tweetted!");

      notify = notify_notification_new("Just tweeted...",t_tweet,NULL);
      GError *error=NULL;
      notify_notification_set_icon_from_pixbuf(notify,gdk_pixbuf_new_from_file(ICONS_DIR"twit_logo.png",&error)); //TODO - Use avatar as icon
      printGError(error);
      notify_notification_show(notify,&error);
      printGError(error);
    }
}

void GtkObtainToken(GtkWidget *parent,gpointer data)
{
  g_token=tokenTempBrowser(twURLS, CONSUMER_KEY, CONSUMER_SECRET);
}

void GtkValidatePIN(GtkWidget *parent,gpointer data)
{
  if(g_token){

      string_t pin = gtk_editable_get_chars(GTK_EDITABLE(data),0,-1);
      if(strlen(pin))
        {

          g_user=tokenAccess(twURLS, pin, g_token);

          if(g_user)
            {
              writeUserFile(g_user,configFile);
              gtk_widget_destroy(rWindow);
            }
        }
  }
}

void
AddUser()
{
  rWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(rWindow),"Authorize App");
  gtk_window_set_resizable(GTK_WINDOW(rWindow),FALSE);
  gtk_window_set_icon(GTK_WINDOW(rWindow),gdk_pixbuf_new_from_file(ICONS_DIR"gqt_logo.png",NULL));

  GtkWidget *vbox = gtk_vbox_new(FALSE,2);
  GtkWidget *hbox = gtk_hbox_new(FALSE,2);
  gtk_container_add(GTK_CONTAINER(rWindow),vbox);
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
  gtk_button_set_image(GTK_BUTTON(button),gtk_image_new_from_file(ICONS_DIR"twit_logo.png"));
  gtk_button_set_image(GTK_BUTTON(button_auth),gtk_image_new_from_stock(GTK_STOCK_APPLY,GTK_ICON_SIZE_BUTTON));
  g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(GtkObtainToken),NULL);
  g_signal_connect(G_OBJECT(button_auth),"clicked",G_CALLBACK(GtkValidatePIN),entry);
  gtk_widget_show_all(rWindow);

}

byte_t
main (int argc, char *argv[])
{

  debug("PROG_DIR:\t%s", PROG_DIR);
  debug("ICONS_DIR:\t%s", ICONS_DIR);
  debug("PACKAGE_LOCALE_DIR:\t%s", PACKAGE_LOCALE_DIR);

  fprintf(stdout, "\n\nGQuickTweet- GTK+ Desktop Gadget for quickly sending tweets \n");
  fprintf(stdout, "Copyright (C) 2012 - Alfredo Liguoro <freddy@haxaddicts.net>,\nOrazio Briante <orazio.briante@hotmail.it>\n\n");

  program_path_init();

  if(createDirectory(programDir))
    createDirectory(configDir);

  initLog(fileLog, high, 1024*1000);
  info("~~~~~~~~~~\t%s started\t~~~~~~~~~~", PROG_NAME);

  if(gtk_init_check(&argc, &argv))
    {
      debug("GTK started");

      notify_init(PROG_NAME);

      twURLS=initURLS(OAUTH_API_URL_DEFAULT, HTTPS_API_URL_DEFAULT);

      g_user=readUserFile(configFile); // later

      if(!g_user)
        {
          AddUser();
        }

      // this is the MAIN

      GtkSettings *default_settings = gtk_settings_get_default();
      g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_default_size(GTK_WINDOW(window),250,300);
      gtk_window_set_icon(GTK_WINDOW(window),gdk_pixbuf_new_from_file(ICONS_DIR"gqt_logo.png",NULL));

      GtkWidget* vbox = gtk_vbox_new(FALSE,2);
      GtkWidget* hbox = gtk_hbox_new(FALSE,2);
      gtk_container_add(GTK_CONTAINER(window),vbox);
      gtk_box_pack_start(GTK_BOX(vbox),hbox,TRUE,TRUE,1);

      GtkWidget *frame = gtk_frame_new("Tweet text");
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

    }
  else
    error("GTK can't be initialized");


  notify_uninit();

  info("~~~~~~~~~~\t%s stopped\t~~~~~~~~~~", PROG_NAME);
  uninitLog();

  return EXIT_SUCCESS;
}
