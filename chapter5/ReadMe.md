ps -t pts/2 -o pid,ppid,tty,stat,args,wchan
   PID   PPID TT       STAT COMMAND                     WCHAN
 78547  78540 pts/2    Ss   zsh                         sigsuspend
 85701  78547 pts/2    SN   ./ser                       inet_csk_accept
 85996  78547 pts/2    S+   ./cli 127.0.0.1             wait_woken
 85997  85701 pts/2    SN   ./ser                       sk_wait_data


jobs
ctrl+z
bg
fg

EINTR slow system call interrupt by signal , so we must restart the slow system call


sudo tcpdump -i lo 'port 9877 and (host 127.0.0.1)' -vv

1.waitpid
2.ser close, cli is stop by read. use select
3.ser crash, cli unknow. use SO_KeepAlive;
4.to deal with big & litte. communication msg by char, not binary.
