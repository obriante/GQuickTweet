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


/*
  Creates entry
*/
GtkWidget* libui_gtk_entry_new (gboolean set_max_size,
                                int max_size,
                                char *text,
                                int width_entry,
                                int height_entry,
                                GdkColor *color)
{
  GtkWidget *widget;

  if (set_max_size == TRUE)
    widget = gtk_entry_new_with_max_length (max_size);

  if (text != NULL)
		gtk_entry_set_text (GTK_ENTRY(widget), text);

  /* Set label's sizes */
  gtk_widget_set_size_request (widget, width_entry, height_entry);

  if (color != NULL)
    /* Set text color */
    gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, color);

  return widget;
}


/*
  Creates entry using a descriptor
*/
GtkWidget* libui_gtk_entry_new_with_descriptor (gboolean set_max_size, int max_size, char *text, char *descriptor, int width_entry, int height_entry, GdkColor *color)
{
  GtkWidget *widget;

  if (set_max_size == TRUE)
    widget = gtk_entry_new_with_max_length (max_size);

  /* Set label's sizes */
  gtk_widget_set_size_request (widget, width_entry, height_entry);

  if (text != NULL)
		gtk_entry_set_text (GTK_ENTRY(widget), text);

  if (descriptor != NULL)
    gtk_widget_modify_font (widget, pango_font_description_from_string (descriptor));

  if (color != NULL)
    /* Set text color */
    gtk_widget_modify_fg (widget, GTK_STATE_NORMAL, color);
	
  return widget;
}
