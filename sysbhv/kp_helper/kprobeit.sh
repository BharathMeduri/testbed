#!/bin/sh
# probeit.sh
# Kprobe helper module:
# Wrapper script for Kprobe helper script kp_load.sh .
# 
# Kaiwan N Billimoria, [L]GPL
name=$(basename $0)
if [ `id -u` -ne 0 ]; then
	echo "$name: need to be root."
	exit 1
fi
if [ $# -lt 2 ]; then
	echo "Usage: $name [module-pathname] function-to-probe 0|1
 1st param [OPTIONAL]: module-pathname: pathname of kernel module that has the
            function-to-probe

 2nd param [REQUIRED]: Function name. If module-pathname is not passed (1st param), 
                        then we assume the function to be kprobed is in the kernel itself.

 3rd param [REQUIRED]: verbose flag; pass 1 for verbose mode, 0 for quiet mode.
"
	exit 1
fi

#if [ $2 -ne 1 -a $2 -ne 0 ]; then
#	echo "Usage: $0 function-to-kprobe 0|1[=verbose]"
#	echo " 2nd param has to be 0 or 1 (verbosity)"
#	exit 1
#fi

rmmod helper_kp 2> /dev/null
dmesg -c >/dev/null
make && ./kp_load.sh $@

