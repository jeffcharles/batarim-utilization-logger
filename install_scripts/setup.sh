#!/bin/bash

log_postfix=$1

# Get installation directory prefix (path passed as val to DESTDIR)
DESTDIR=${DESTDIR:-"/usr/local"} # Default value of "/usr/local"

# Refresh shared library cache, update mandb, add xhost entry (if root)
if [[ $(whoami) = "root" ]]; then
    ldconfig
    mandb -pq
    xhost +SI:localuser:root # Needed to be able to retrieve focused window
fi

# Set up log file
if [[ $(whoami) = "root" ]]; then
    logfile="/var/log/batarim_${log_postfix}.csv"
else
    read -p "Enter path to where you want to place log file (not including filename): " logpath
    logfile="${logpath}/batarim_${log_postfix}.csv"
fi
touch "$logfile" || exit 1 # Exit if can't create log file
chmod 0644 $logfile

# Set up crontab entry
tmpfile=mktemp # NOTE: mktemp is Linux-specific
crontab -l > "$tmpfile"
# Delete pre-existing entry if it exists
sed /batarim-logger/d < "$tmpfile" > "$tmpfile"
# Add entry
echo "*/5 * * * * \"${DESTDIR}/bin/batarim-logger\" \"${logfile}\"" >> "$tmpfile"
crontab - < "$tmpfile"
rm "$tmpfile"
