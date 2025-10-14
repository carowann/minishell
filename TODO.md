// PARSING //
- [x] ./minishell tnati spazi vuoti deve ritornare il prompt senza errore
- [x]gestione exit status per $?
- [x]gestione segnali
- [x]implementazione heredoc
- [x]FIX SEG FAULT ECHO >
- [x] echo ciao test >test1 >test2 tronca i nomi dei file togliendo la prima lettera
- [x] echo $""

//execution
- [x]cd
- [x]exit
- [x]gestione permessi file e gestione append mode
- [x]gestione redirection

- [x]APRIRE IO PRIMA DEL FORK PER FAILITARE CLEANUP
- [x] !!REDIRECT: leggere "TUTTI" i redirect, invece solo l'ultimo di ogni tipo
- [x] fix echo -nnnnnnnnnnnnnnnnnnn
- [] minishell> cat < nonex | ls  non si vede il messaggio di errore di cat che fallisce :


- [x] rimuovere cartella env e spostare file
- [x] migliorare nomi file
- [x] cwannhed@c1r6p8:~$ cat << 'eof'
> $HOME
> '$HOME'
> "$HOME"
non dovrebbe espandere
controllare altri comportmaneti per espansione in heredoc
