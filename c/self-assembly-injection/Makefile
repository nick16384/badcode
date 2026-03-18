all:
	gcc -g -masm=intel -o main main.c
	nasm -f bin loaded.s -o loaded.bin

clean:
	rm main loaded.bin