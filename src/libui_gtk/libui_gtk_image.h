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

#ifndef LIBUI_GTK_IMAGE_H_
#define LIBUI_GTK_IMAGE_H_

/*
 Creates a GdkPixbuf from a file
*/
GdkPixbuf *libui_gtk_image_create_pixbuf_from_file (const gchar * filename);

/*
  Creates a GdkPixbuf from a file. Size required!
*/
GdkPixbuf *libui_gtk_image_create_pixbuf_from_file_with_size (const gchar * filename, int width, int height);

#endif /* LIBUI_GTK_IMAGE_H_ */
