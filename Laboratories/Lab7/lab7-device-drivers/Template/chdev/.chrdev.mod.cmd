savedcmd_/home/dicarlo/kernel/chdev/chrdev.mod := printf '%s\n'   chrdev.o | awk '!x[$$0]++ { print("/home/dicarlo/kernel/chdev/"$$0) }' > /home/dicarlo/kernel/chdev/chrdev.mod
