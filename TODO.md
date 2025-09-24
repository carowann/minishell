// PARSING //
[x] ./minishell tnati spazi vuoti deve ritornare il prompt senza errore
[x]gestione exit status per $?
[]gestione segnali
[x]implementazione heredoc
[x]FIX SEG FAULT ECHO >

//execution
[x]cd
[x]exit
[x]gestione permessi file e gestione append mode
[x]gestione redirection



[x]APRIRE IO PRIMA DEL FORK PER FAILITARE CLEANUP

- [ ] !!REDIRECT: leggere "TUTTI" i redirect, invece solo l'ultimo di ogni tipo
cat < in > out > canttouchthis  > out2 in questo momento crea l'ultimo file anche se canttouchthis non ha permessi

- [ ] fix echo -nnnnnnnnnnnnnnnnnnn

- [ ] minishell> cat < nonex | ls  non si vede il messaggio di errore di cat che fallisce :