# lkm-rootkit

Learning to write rootkits.

## Setup

To safely build and test the kernel modules, a Vagrant box is provided. Testing
the modules in the host kernel is not recommended as it may cause a kernel panic
if not properly configured/used.

*Please note that the messages displayed on your computer might be different.*

```console
$ vagrant up  # this will fail because the box creator did not configured the box correctly :(
==> default: Checking if box 'generic/alpine310' version '1.9.18' is up to date...
==> default: Creating image (snapshot of base box volume).
==> default: Creating domain with the following settings...
blah blah blah
==> default: Installing rsync to the VM...
The following SSH command responded with a non-zero exit status.
Vagrant assumes that this means the command failed!

apt-get install -y -qq install rsync


Stdout from the command:



Stderr from the command:

bash: line 4: apt-get: command not found
$ vagrant ssh
localhost:~$ sudo apk add rsync
localhost:~$ logout
Connection to 192.168.121.34 closed.
$ vagrant reload
```

## Build

```console
$ vagrant ssh
localhost:~$ cd /vagrant
localhost:/vagrant$ make
```

You can then load the modules with `sudo insmod module.ko` and unload with
`sudo rmmod module`.
