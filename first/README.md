# A "Simple" Rookit

If you look at the code, you can see it is not simple but it was a fun 
experience.

# Funtionality

Right now, this rootkit can:

- Grant root privilieges to a userland process
- Hide/unhide itself
- Hide/Unhide a process by PID
- Hide/unhide files or directories
- protect against being unloaded by the user
- Disable the unload proctection

# Platforms

The rootkit was tested to work on 
Linux kernels 2.6.32-38 and 4.4.0-22

# Installation

Load rootkit:

``
insmod rootkit.ko
``

Use rootkit:

```sh
$ ./client [option]...

"Options:\n"
            "  --root-shell            Grants you root shell access.\n"
            "  --hide-pid=PID          Hides the specified PID.\n"
            "  --unhide-pid=PID        Unhides the specified PID.\n"
            "  --hide-file=FILENAME    Hides the specified FILENAME globally.\n"
            "                          Must be a filename without any path.\n"
            "  --unhide-file=FILENAME  Unhides the specified FILENAME.\n"
            "  --hide                  Hides the rootkit LKM.\n"
            "  --unhide                Unhides the rootkit LKM.\n"
            "  --help                  Print this help message.\n"
            "  --protect               Protects the rootkit from rmmod.\n"
            "  --unprotect             Disables the rmmod protection.\n\n",
```

Unload rootkit:

```$sh
./client --unhide
./client --unprotect
rmmod rootkit.ko
```


