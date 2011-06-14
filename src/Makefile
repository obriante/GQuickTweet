# libui_gtk
lib =  libui_gtk_dialog_error.o libui_gtk_gadget_move.o libui_gtk_gadget_window.o libui_gtk_button.o libui_gtk_image.o libui_gtk_entry.o

%.o: libui_gtk/%.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0`

# gadget
objects = main.o twitter.o

%.o: %.c
	gcc -c $< -o $@ -g `pkg-config --cflags gtk+-2.0 oauth libnotify`

all: $(lib) $(objects)
	gcc $(lib) $(objects) -o GQuickTweet `pkg-config --libs gtk+-2.0 oauth libnotify`

clean:
	rm *.o -rf
