#!/bin/sh
# Wrapper startup script that guesses the full hostname to use and calls
# the real calculation engine.
set -e

gave_hostname=0
for i; do
	if [ "$i" = "-ORBpoa_iiop_name_port" ]; then
		gave_hostname=1
	fi
done

real_server=/usr/lib/regina-normal/bin/regina-engine-corba
if [ "$gave_hostname" = "0" ]; then
	$real_server -ORBpoa_iiop_name_port "`/bin/hostname --fqdn`" "$@"
else
	$real_server "$@"
fi

