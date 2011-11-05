#!/bin/bash

set -e

# <_postfix_> should be substituted with a string by sed before running the
# script
log_postfix=<_postfix_>

# Get installation directory prefix (path passed as val to DESTDIR)
DESTDIR="${DESTDIR}/usr/local" # Default value of "/usr/local"

# Refresh shared library cache, update mandb, add user, and add xhost entry
# (if root)
if [[ $(whoami) = "root" ]]; then
    ldconfig
    mandb -pq
    adduser --system --no-create-home --group --quiet batarim
    
    # Need to get access to main user's X session to allow batarim access
    main_user=$SUDO_USER # First check to see which user is sudo-ing (if any)

    # If no one is sudo-ing, assume this is the first currently logged in
    # non-root user
    if [ -z $main_user ]; then
        for user in $(users); do
            if [[ $user != "root" ]]; then
                main_user=$user
                break
            fi
        done
    fi
    
    main_user=${main_user:-"root"} # If still no user, then assume root
    
    xhostcmd="xhost +SI:localuser:batarim"
    export DISPLAY=":0" # NOTE: assumption that this is the correct display
    su -c "${xhostcmd}" $main_user # Needed to be able to retrieve focused window
    homedir="/home/${main_user}"
    if [[ $main_user == "root" ]]; then
        homedir="/root"
    fi
    echo "${xhostcmd}" >> $homedir/.gnomerc # NOTE: assumes Gnome desktop env
fi

# Set up log file
if [[ $(whoami) = "root" ]]; then
    logfile="/var/log/batarim_${log_postfix}.csv"
else
    read -p "Enter path to where you want to place log file (not including filename): " logpath
    logfile="${logpath}/batarim_${log_postfix}.csv"
fi
touch "$logfile"
if [[ $(whoami) = "root" ]]; then
    chown batarim:batarim "$logfile" 
fi
chmod 0644 $logfile

# Set up crontab entry
tmpfile=mktemp # NOTE: mktemp is Linux-specific
if [[ $(whoami) = "root" ]]; then
    crontab -u batarim -l > "$tmpfile" || : # noop if no crontab
else
    crontab -l > "$tmpfile" || : # noop if no crontab
fi
# Delete pre-existing entry if it exists
sed -i /batarim-logger/d "$tmpfile"
# Add entry
echo "*/5 * * * * \"${DESTDIR}/bin/batarim-logger\" \"${logfile}\"" >> "$tmpfile"
if [[ $(whoami) = "root" ]]; then
    crontab -u batarim - < "$tmpfile"
else
    crontab - < "$tmpfile"
fi
rm "$tmpfile"
