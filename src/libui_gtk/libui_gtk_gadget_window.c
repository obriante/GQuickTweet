/*
 * Copyright © 2011
 * Andrea Costa <nukemup@hotmail.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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


#include "libui_gtk.h"


static void
on_alpha_screen_changed (GtkWidget* pWidget, GdkScreen* pOldScreen,GtkWidget* pLabel)
{
  GdkScreen* pScreen = gtk_widget_get_screen (pWidget);
  GdkColormap* pColormap = gdk_screen_get_rgba_colormap (pScreen);
  if (!pColormap) pColormap = gdk_screen_get_rgb_colormap (pScreen);
    gtk_widget_set_colormap (pWidget, pColormap);
}


/*
  Create main window
*/
GtkWidget*
libui_gtk_gadget_window_new (int width,
                             int height,
                             gboolean resizable,
                             gboolean paintable,
                             gboolean decorated,
                             gboolean stick,
                             gboolean keep_below,
                             gboolean set_modal,
                             GdkWindowTypeHint hint,
                             gboolean transparent
)
{
  GtkWidget *widget;

	widget = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW(widget), width, height);
	gtk_window_set_resizable (GTK_WINDOW(widget), resizable);
	gtk_widget_set_app_paintable (widget, paintable);
	gtk_window_set_decorated (GTK_WINDOW(widget), decorated);

  if (stick == TRUE)
  	gtk_window_stick (GTK_WINDOW(widget));

	gtk_window_set_keep_below (GTK_WINDOW(widget), keep_below);
	gtk_window_set_modal (GTK_WINDOW(widget), set_modal);
	gtk_window_set_type_hint (GTK_WINDOW(widget), hint);

  if (transparent == TRUE)
    on_alpha_screen_changed (widget, NULL, NULL);

	return widget;
}


/*
  Create GtkStatusIcon and tooltip
*/
GtkWidget*
libui_gtk_gadget_window_icon_new (GtkWidget *window, char *image, char *text)
{
  GtkWidget *widget;

	widget = (GtkWidget *) gtk_status_icon_new_from_pixbuf (libui_gtk_image_create_pixbuf_from_file(image,0,0));
  if (text != NULL)
  {
    gtk_status_icon_set_tooltip_text (GTK_STATUS_ICON(widget), text);
    gtk_widget_set_has_tooltip (window, TRUE);
  }
	return widget;
}

