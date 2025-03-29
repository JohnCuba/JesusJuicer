#!/bin/bash

new_version="$1"

fw_file="./src/main.cpp"
fw_regex="([0-9]+)\.([0-9]+)\.([0-9]+)"

fs_file="./frontend/package.json"
fs_regex="(\"version\": \")[0-9]+\.[0-9]+\.[0-9]+(\")"
fs_new_version="\"version\": \"$new_version\""


# Replace using sed -i on the files.
sed -E -i '' "s/$fw_regex/$(echo "$new_version")/g" "$fw_file"
sed -E -i '' "s/$fs_regex/$(echo "$fs_new_version")/g" "$fs_file"

# Check if the sed commands was successful
if [ $? -eq 0 ]; then
  echo "Successfully bumped to '$new_version' in '$fw_file' and '$fs_file'."
else
  echo "Error: Failed to replace substring."
  exit 1
fi

exit 0