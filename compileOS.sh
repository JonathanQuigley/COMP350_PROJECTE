dd if=/dev/zero of=diskc.img bs=512 count=1000
nasm bootload.asm
dd if=bootload of=diskc.img conv=notrunc
bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d  kernel_c.o kernel_asm.o
as86 -o userlib.o userlib.asm
bcc -ansi -c -o tstpr1.o tstpr1.c
ld86 -d -o tstpr1 tstpr1.o userlib.o
bcc -ansi -c -o tstpr2.o tstpr2.c
ld86 -d -o tstpr2 tstpr2.o userlib.o
bcc -ansi -c -o shell_c.o shell.c
ld86 -d -o shell shell_c.o userlib.o
bcc -ansi -c -o number_c.o number.c
ld86 -d -o number number_c.o userlib.o
bcc -ansi -c -o letter_c.o letter.c
ld86 -d -o letter letter_c.o userlib.o
gcc -o loadFile loadFile.c
./loadFile kernel
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2
./loadFile shell
./loadFile number
./loadFile letter
