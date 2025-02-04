#!/usr/bin/perl -w

# Generate usys.S, the stubs for syscalls.

print "# generated by usys.pl - do not edit\n";

print "#include \"kernel/syscall.h\"\n";

# syscall number -> a7
sub entry {
    my $name = shift;
    print ".global $name\n";
    print "${name}:\n";
    print " li a7, SYS_${name}\n";
    print " ecall\n";
    print " ret\n";
}
	
entry("fork");
entry("exit");
entry("wait");
entry("pipe");
entry("read");
entry("write");
entry("close");
entry("kill");
entry("exec");
entry("open");
entry("mknod");
entry("unlink");
entry("fstat");
entry("link");
entry("mkdir");
entry("chdir");
entry("dup");
entry("getpid");
entry("sbrk");
entry("sleep");
entry("uptime");
entry("getyear");
entry("gettimeofday");
entry("trace");
entry("set_priority");
entry("chrt");
entry("printMlfq");
entry("shmget");
entry("shmat");
entry("shmdt");
entry("shmctl");
entry("vfork");
entry("semget");
entry("semop");
entry("semctl");
entry("msgget");
entry("msgsnd");
entry("msgrcv");
entry("msgctl");
entry("ticks");
entry("cycles");
entry("neofetch");