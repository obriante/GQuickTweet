/*
 * Copyright © 2010
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
  Creates image button
*/
GtkWidget*
libgadget_button_new_with_image (int width_image,
                                 int height_image,
                                 GtkPositionType align_image,
                                 char *image_name)
{
	GtkWidget *widget;
	GtkWidget *image;
	
	widget = gtk_button_new ();

	/* Set button's sizes */
	gtk_widget_set_size_request (widget, -1, -1);
	
	/* Modify original image sizes */
	image = gtk_image_new_from_pixbuf (libui_gtk_image_create_pixbuf_from_file(image_name, width_image, height_image));

	/* Set image */
	gtk_button_set_image (GTK_BUTTON(widget), image);

	/* Set image position */
	gtk_button_set_image_position (GTK_BUTTON(widget), align_image);

	gtk_button_set_relief (GTK_BUTTON(widget), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click (GTK_BUTTON(widget), FALSE);
	
	return (GtkWidget *) widget;
}
