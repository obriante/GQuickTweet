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
  Creates a GdkPixbuf from a file, changing its sizes
*/
GdkPixbuf *libui_gtk_image_create_pixbuf_from_file(const gchar * filename,int width, int height)
{
  GdkPixbuf *pixbuf;
  GError *error = NULL;
  pixbuf = gdk_pixbuf_new_from_file (filename, &error);
  if (!pixbuf)
  {
    fprintf (stderr, "%s\n", error->message);
    g_error_free (error);
  }

  /* Scale it */
  if(width && height)
	pixbuf = gdk_pixbuf_scale_simple (pixbuf, width, height, GDK_INTERP_BILINEAR);
	
  return pixbuf;
}
