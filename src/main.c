#include "core.h"
#include <libnotify/notify.h>


gboolean on_expose_event(GtkWidget *parent,GdkEventExpose *event,GQT_Core *core) //Thanks to andypc :)
{
  cairo_t *cr;

  double x0      = 5.0;
  double y0      = 10.0;
  double rect_width  = WIDTH - 10;
  double rect_height = HEIGHT - 15;
  double radius = 30;
  double x1, y1;

  cr = gdk_cairo_create (parent->window);
	
  /* Set background gadget */
  cairo_set_source_rgba (cr, 1.0f, 1.0f, 1.0f, 0.0f);
  cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint (cr);

  x1 = x0 + rect_width;
  y1 = y0 + rect_height;

  if (!rect_width || !rect_height)
    return;

  cairo_move_to  (cr, x0, y0 + radius);
  cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
  cairo_line_to (cr, x1 - radius, y0);
  cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
  cairo_line_to (cr, x1 , y1 - radius);
  cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
  cairo_line_to (cr, x0 + radius, y1);
  cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);

  cairo_close_path (cr);

  cairo_set_source_rgba (cr,
                         30025.0 / 65535.0,
                         50534.0 / 65535.0,
                         1.0,
                         1.0);
  cairo_fill_preserve (cr);
	
  cairo_set_source_rgba (cr,
                         1.0,
                         1.0,
                         1.0,
                         0.5);

  cairo_set_line_width (cr, 2.0);
  cairo_stroke (cr);
  cairo_destroy(cr);
  return FALSE;
}

static void on_quit_clicked(GtkWidget *parent,GQT_Core *core)
{
	g_free(core->user);
	g_free(core);
	gtk_main_quit();
}

void on_tweet_clicked(GtkWidget *parent,GQT_Core *data)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data->tw_view));
    NotifyNotification *notify;
    GtkTextIter start,end;
    gtk_text_buffer_get_start_iter(buf,&start);
    gtk_text_buffer_get_end_iter(buf,&end);
    char *t_tweet = gtk_text_buffer_get_text(buf,&start,&end,TRUE);
    if(strlen(t_tweet) < 140)
    {
        Tweet(t_tweet,data->user);
        notify = notify_notification_new("Just tweeted...",t_tweet,NULL,NULL);
        notify_notification_set_icon_from_pixbuf(notify,gdk_pixbuf_new_from_file("img/tweet.png",NULL)); //TODO - Use avatar as icon
        notify_notification_show(notify,NULL);
    }
}

void GtkObtainToken(GtkWidget *parent,GQT_Core *data)
{
    data->token = ObtainToken();
}

void GtkValidatePIN(GtkWidget *parent,GQT_Core *data)
{
	char *pin = gtk_editable_get_chars(GTK_EDITABLE(data->pinentry),0,-1);
    if(strlen(pin) == 7)
    {
        ValidatePIN(pin,data->token,data->user);
        fwrite(data->user,sizeof(Twitter_user),1,data->userconf);
        gtk_widget_destroy(data->authwin);
    }
}

static void GQT_SetGTKRC(void)
{
	gtk_rc_parse_string("style \"default\" \
						{\
							GtkButton::focus-padding = 0\
							GtkButton::default-border = { 0, 0, 0, 0 }\
							GtkButton::focus-line-width = 0\
							GtkButton::inner-border = { 0, 0, 0, 0 }\
						}\
						class \"GtkButton\" style \"default\"");
}

static void GQT_GTKUserAdd(GQT_Core *data)
{
	if(data->userconf)
    {
        data->authwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(data->authwin),"Authorize App");
        gtk_window_set_resizable(GTK_WINDOW(data->authwin),FALSE);
        gtk_window_set_icon(GTK_WINDOW(data->authwin),gdk_pixbuf_new_from_file("img/gqt_logo.png",NULL));
        GtkWidget *vbox,*hbox;
        vbox = gtk_vbox_new(FALSE,2);
        hbox = gtk_hbox_new(FALSE,2);
        gtk_container_add(GTK_CONTAINER(data->authwin),vbox);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        GtkWidget *button = gtk_button_new_with_label("Get PIN");
        gtk_box_pack_start(GTK_BOX(hbox),button,TRUE,TRUE,1);
        hbox = gtk_hbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        GtkWidget *frame = gtk_frame_new("PIN:");
        gtk_box_pack_start(GTK_BOX(hbox),frame,TRUE,TRUE,1);
        data->pinentry = gtk_entry_new_with_max_length(7);
        gtk_entry_set_editable(GTK_ENTRY(data->pinentry),TRUE);
        gtk_container_add(GTK_CONTAINER(frame),data->pinentry);
        GtkWidget *button_auth = gtk_button_new_with_label("Submit");
        hbox = gtk_hbox_new(FALSE,2);
        gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,TRUE,1);
        gtk_box_pack_start(GTK_BOX(hbox),button_auth,TRUE,TRUE,1);
        gtk_button_set_image(GTK_BUTTON(button),gtk_image_new_from_file("img/tweet.png"));
        gtk_button_set_image(GTK_BUTTON(button_auth),gtk_image_new_from_stock(GTK_STOCK_APPLY,GTK_ICON_SIZE_BUTTON));
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(GtkObtainToken),data);
        g_signal_connect(G_OBJECT(button_auth),"clicked",G_CALLBACK(GtkValidatePIN),data);
        gtk_widget_show_all(data->authwin);
    }
}

int main(int argc,char **argv)
{
	gtk_init(&argc,&argv);
	notify_init("GQuickTweet");
	GtkSettings *default_settings = gtk_settings_get_default();
	g_object_set(default_settings, "gtk-button-images", TRUE, NULL); 
	GdkColor bgcolor = {
		.blue = 65535,
		.red = 30025,
		.green = 50534
	};
	GQT_Core *core = g_new0(GQT_Core, 1);
	core->user = g_new0(Twitter_user, 1);
	core->userconf = fopen(".gqtuser","r+");
	if(core->userconf)
	{
		fscanf(core->userconf,"%s",core->user->id);
	}
	g_set_application_name("GQuickTweet");
	if(!core->userconf || strlen(core->user->id) != 9)
	{
		core->userconf = fopen(".gqtuser","w+");
		GQT_GTKUserAdd(core);
	}
	core->presence = TRUE;
	GQT_SetGTKRC();
	core->window = libui_gtk_gadget_window_new(WIDTH, HEIGHT, TRUE, TRUE, 
												FALSE, TRUE, TRUE, FALSE,
												GDK_WINDOW_TYPE_HINT_DOCK,
												TRUE);
	gtk_window_set_position(GTK_WINDOW(core->window),GTK_WIN_POS_CENTER);
	core->button_close = libgadget_button_new_with_image(7, 7, GTK_POS_TOP, "img/close.png");
	core->button_move = libgadget_button_new_with_image(7, 7, GTK_POS_TOP, "img/move.png");
	core->button_tweet = libgadget_button_new_with_image(30, 30, GTK_POS_TOP, "img/tweet.png");
	core->frame = gtk_frame_new("Tweet Content");
	core->tw_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(core->tw_view),TRUE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(core->tw_view),GTK_WRAP_CHAR);
	gtk_widget_set_size_request (core->tw_view, WIDTH - 30, HEIGHT - 75);
	gtk_widget_modify_base(core->tw_view, GTK_STATE_NORMAL, &bgcolor);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW (core->tw_view),5);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW (core->tw_view),5);
	gtk_container_add(GTK_CONTAINER(core->frame),core->tw_view);
	core->fixed = gtk_fixed_new();
	gtk_fixed_put(GTK_FIXED(core->fixed),core->frame,13,15);
	gtk_fixed_put(GTK_FIXED(core->fixed),core->button_move,WIDTH - 20,0);
	gtk_fixed_put(GTK_FIXED(core->fixed),core->button_close, WIDTH - 30,0);
	gtk_fixed_put(GTK_FIXED(core->fixed),core->button_tweet, (WIDTH / 2) - 15, HEIGHT - 40);
	gtk_container_add(GTK_CONTAINER(core->window),core->fixed);
	g_signal_connect(G_OBJECT(core->window), "expose-event", G_CALLBACK (on_expose_event), core);
	g_signal_connect(G_OBJECT(core->button_close), "clicked", G_CALLBACK (on_quit_clicked), core);
	g_signal_connect (G_OBJECT(core->button_move), "button-press-event", G_CALLBACK (libui_gtk_gadget_move), core);
	g_signal_connect(G_OBJECT(core->button_tweet), "clicked", G_CALLBACK(on_tweet_clicked), core);
	gtk_widget_show_all(core->window);
	gtk_main();
	notify_uninit();
	return 0;
}
