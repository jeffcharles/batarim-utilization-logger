#!/bin/bash

# Get installation directory prefix (path passed as val to DESTDIR)
destdir=${1:-"/usr/local"} # Default value of "/usr/local"

# Refresh shared library cache (if root)
if [[ $(whoami) = "root" ]]; then
    ldconfig
fi

# Set up log file
if [[ $(whoami) = "root" ]]; then
    logfile="/var/log/batarim.csv"
else
    read -p "Enter path to where you want to place log file (not including filename): " logpath
    logfile="${logpath}/batarim.csv"
fi
touch "$logfile" || exit 1 # Exit if can't create log file
chmod 0644 $logfile

# Set up crontab entry
tmpfile=mktemp # NOTE: mktemp is Linux-specific
crontab -l > "$tmpfile"
# Delete pre-existing entry if it exists
sed /batarim-logger/d < "$tmpfile" > "$tmpfile"
# Add entry
echo "*/5 * * * * ${destdir}/bin/batarim-logger < ${logfile} >> ${logfile}" >> "$tmpfile"
crontab - < "$tmpfile"
rm "$tmpfile"
