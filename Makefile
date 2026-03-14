make:
	gcc main.c cli.c playlist.c song.c album.c log.c -o app
clean:
	rm -f app