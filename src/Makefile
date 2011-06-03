CFLAGS = `pkg-config --cflags gtk+-2.0 oauth libcurl`
LIBS   = `pkg-config --libs gtk+-2.0 oauth libcurl` 

all: gquicktweet.o twitter.o
	gcc $(LIBS) -o GQuickTweet gquicktweet.o twitter.o

clean:
	rm -rf *.o GQuickTweet

%.o: %.c
	gcc $(CFLAGS) -c $<
