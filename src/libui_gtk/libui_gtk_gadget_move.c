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
 Moves the gadget around the screen
*/
gboolean
libui_gtk_gadget_move (GtkWidget* pWidget, GdkEventButton* pButton, GdkWindowEdge edge)
{
  gtk_window_begin_move_drag (GTK_WINDOW (gtk_widget_get_toplevel (pWidget)),
                              pButton->button,
                              pButton->x_root,
                              pButton->y_root,
                              pButton->time);
  return TRUE;
}

