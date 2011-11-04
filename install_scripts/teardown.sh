#!/bin/bash

# Remove cron entry
tmpfile=mktemp # NOTE: mktemp is Linux-specific
if [[ $(whoami) = "root" ]]; then
    crontab -u batarim -l > "$tmpfile" || : # noop if no crontab
else
    crontab -l > "$tmpfile" || : # noop if no crontab
fi
sed -i /batarim-logger/d "$tmpfile"
if [[ $(whoami) = "root" ]]; then
    crontab -u batarim - < "$tmpfile"
else
    crontab - < "$tmpfile"
fi
rm "$tmpfile"

if [[ -n $(cat /etc/passwd | grep batarim) ]]; then
    user_exists=true
else
    user_exists=false
fi

# Remove batarim xhost entry if present
if $user_exists; then
    # Need to get access to main user's X session to take away batarim access
    main_user=$SUDO_USER # First try to see if a user is sudo-ing in
    
    # If not, assume this is the first currently logged in non-root user
    if [ -z $main_user ]; then
        for user in $(users); do
            if [[ $user != "root" ]]; then
                main_user=$user
                break
            fi
        done
    fi

    main_user=${main_user:-"root"} # If still no user, assume root

    export DISPLAY=":0" # NOTE: Assumes this is the correct display
    su -c "xhost -SI:localuser:batarim" $main_user
fi

# Remove batarim user and group account (except when upgrading)
if [[ $(whoami) = "root" && $1 != "upgrade" && $user_exists ]]; then
    deluser --system batarim
fi
