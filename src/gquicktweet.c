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


#include <logc/logc.h>

#include <twitc/twitc.h>

#include <appkeys.h>
#include <console.h>
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


static GtkWidget *window=NULL;
static GtkWidget* charcount=NULL;

static GtkWidget *dialog=NULL;
static GtkWidget *entry=NULL;
static string_t g_token=NULL;

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
		log(ERROR,(string_t)error->message);
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

		if(!updateStatus(twURLS, g_user, t_tweet))
		{
			log(INFO,"Message correctly tweetted!");
			gtk_text_buffer_delete(buf, &start, &end);
		}
		else
			log(INFO,"Message not tweetted!");

		notify = notify_notification_new("Just tweeted...",t_tweet,NULL);
		GError *error=NULL;
		notify_notification_set_icon_from_pixbuf(notify,gdk_pixbuf_new_from_file(ICONS_DIR"twit_logo.png",&error)); //TODO - Use avatar as icon
		printGError(error);
		notify_notification_show(notify,&error);
		printGError(error);
	}
}

void
init_charcount(string_t msg)
{
	if(!charcount)
		charcount=gtk_statusbar_new ();

	gtk_statusbar_push (GTK_STATUSBAR(charcount), 0, msg);
}


void updatecount(GtkTextBuffer *buffer, GtkStatusbar *charcount)
{
	gtk_statusbar_pop(charcount, 0);

	GtkTextIter iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&iter, gtk_text_buffer_get_insert(buffer));

	gint totChar = 140 - gtk_text_iter_get_line_offset(&iter);
	totChar	= totChar - gtk_text_iter_get_line(&iter);

	string_t msg = g_strdup_printf("%d", totChar);

	if(totChar <= 0)
	{
		msg = g_strdup_printf("%d", 0);
		gtk_text_buffer_backspace(buffer, &iter, TRUE, TRUE);
	}

	/* Push numer of char to statusbar */
	init_charcount(msg);
	g_free(msg);

}

void run_window()
{

	if(!window)
	{
		GtkSettings *default_settings = gtk_settings_get_default();
		g_object_set(default_settings, "gtk-button-images", TRUE, NULL);

		window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_default_size(GTK_WINDOW(window), 400, 180);
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

		GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
		gtk_text_buffer_set_text (buffer, "\0", -1);

		init_charcount("140");
		gtk_box_pack_start(GTK_BOX(vbox),charcount,FALSE,FALSE,0);

		g_signal_connect(buffer, "changed", G_CALLBACK(updatecount), charcount);
		gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
		gtk_box_pack_start(GTK_BOX(hbox),send_button,TRUE,FALSE,1);

		g_signal_connect(G_OBJECT(window), "destroy", gtk_main_quit, NULL);
		g_signal_connect(G_OBJECT(send_button),"clicked",G_CALLBACK(GtkSendTweet),text_view);
	}

	gtk_widget_show_all(window);

}

void GtkObtainToken(GtkWidget *parent,gpointer data)
{
	g_token=tokenTempBrowser(twURLS, CONSUMER_KEY, CONSUMER_SECRET);
}

void GtkValidatePIN(GtkWidget *parent,gpointer data)
{
	if(g_token)
	{

		string_t pin = gtk_editable_get_chars(GTK_EDITABLE(data),0,-1);
		if(strlen(pin))
		{

			g_user=tokenAccess(twURLS, pin, g_token);

			if(g_user)
			{
				writeUserFile(g_user,configFile);
				gtk_entry_set_text (GTK_ENTRY(entry), "\0");
				gtk_widget_destroy(dialog);

				run_window();


			}
		}
	}
}

void
AddUser()
{

	gtk_widget_destroy(dialog);

	twURLS=initURLS(OAUTH_API_URL_DEFAULT, HTTPS_API_URL_DEFAULT, SEARCH_URL_DEFAULT);

	/* Set all dialog options (color, size, position, logo, icon, etc) */
	dialog = gtk_dialog_new();
	gtk_window_set_title (GTK_WINDOW(dialog), "New User");
	gtk_container_set_border_width (GTK_CONTAINER (dialog), 0);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

	gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (window));

	GError *error = NULL;
	gtk_window_set_icon_from_file (GTK_WINDOW(dialog), ICONS_DIR"gqt_logo.png", &error);

	if(error)
	{
		log(ERROR,(string_t)error->message);
		g_error_free(error);
		error=NULL;
	}

	/* Attach twitter-login image */
	GtkWidget *twitterLogin = gtk_button_new_with_label("Get PIN");
	gtk_button_set_image(GTK_BUTTON(twitterLogin),gtk_image_new_from_file(ICONS_DIR"twit_logo.png"));
	GtkWidget *table = gtk_table_new (10, 10, TRUE);
	gtk_table_attach (GTK_TABLE (table), twitterLogin, 0, 10, 1, 3, GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND, 0, 0);

	/* Call oAuth function */
	g_signal_connect (G_OBJECT (twitterLogin), "clicked", G_CALLBACK(GtkObtainToken), NULL);

	/* Attach Box for PIN */
	GtkWidget *label = gtk_label_new ("Insert PIN");
	entry = gtk_entry_new_with_max_length(7);
	gtk_label_set_justify(GTK_LABEL (label),GTK_JUSTIFY_LEFT);
	gtk_entry_set_text (GTK_ENTRY(entry), "\0");
	gtk_table_attach (GTK_TABLE (table), label, 1, 9, 3, 5, GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND, 0, 0);
	gtk_table_attach (GTK_TABLE (table), entry, 1, 9, 5, 6, GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND, 0, 0);

	/* Press Button and call function for verify PIN */
	GtkWidget *button = gtk_button_new_with_label ("Create Accont");
	gtk_table_attach (GTK_TABLE (table), button, 1, 9,7, 9, GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND, 0, 0);
	gtk_dialog_add_action_widget (GTK_DIALOG (dialog), table, GTK_RESPONSE_CLOSE);
	g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK(GtkValidatePIN), entry);

	/* Exit event and Widget Show */
	g_signal_connect (G_OBJECT (dialog), "delete_event", G_CALLBACK (gtk_widget_destroy), dialog);

	gtk_widget_show_all (dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));

}


byte_t
main (int argc, char *argv[])
{

	initLog(FILE_VIDEO_LOG, DISABLED_LOG);

	byte_t shparam=shellParam(argc,argv);

	if (shparam == EXIT_FAILURE)
		return EXIT_FAILURE;

	debug("PROG_DIR:\t%s", PROG_DIR);
	debug("ICONS_DIR:\t%s", ICONS_DIR);
	debug("PACKAGE_LOCALE_DIR:\t%s", PACKAGE_LOCALE_DIR);

	program_path_init();

	if(createDirectory(programDir))
		createDirectory(configDir);

	checkFileSize(fileLog, (1024 * 1000));
	openLogFile(fileLog);

	log(INFO,"~~~~~~~~~~\t%s started\t~~~~~~~~~~", PROG_NAME);

	if(gtk_init_check(&argc, &argv))
	{
		debug("GTK started");

		notify_init(PROG_NAME);

		twURLS=initURLS(OAUTH_API_URL_DEFAULT, HTTPS_API_URL_DEFAULT,SEARCH_URL_DEFAULT);

		g_user=readUserFile(configFile); // later

		if(!g_user)
		{
			AddUser();
		}else

			run_window();

		gtk_main();

	}
	else
		log(ERROR,"GTK can't be initialized");


	notify_uninit();

	log(INFO,"~~~~~~~~~~\t%s stopped\t~~~~~~~~~~", PROG_NAME);
	uninitLog();

	return EXIT_SUCCESS;
}
