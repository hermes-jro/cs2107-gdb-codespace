CFLAGS = -Og -fno-stack-protector -fno-omit-frame-pointer -Wno-deprecated-declarations -Wno-implicit-function-declaration

login: login.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f login
