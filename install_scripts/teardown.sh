#!/bin/bash

# Remove root xhost entry if present
xhost -SI:localuser:root

# Remove cron entry
tmpfile=mktemp # NOTE: mktemp is Linux-specific
crontab -l > "$tmpfile"
sed /batarim-logger/d < "$tmpfile" > "$tmpfile"
crontab - < "$tmpfile"
rm "$tmpfile"
