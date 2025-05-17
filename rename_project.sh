#!/bin/bash

# Check if two arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <new_gtkprocrunr_name> <new_app_name>"
    exit 1
fi

NEW_GTKBASE_NAME="$1"
NEW_APP_NAME="$2"
NEW_APP_NAME_CAPS=${NEW_APP_NAME^^}

# Replace "GtkProcRunr" with NEW_GTKBASE_NAME in file contents (not filenames)
find . -type f -not -path '*/\.git/*' -exec sed -i "s/GtkProcRunr/$NEW_GTKBASE_NAME/g" {} \;

# Replace "process_runner" with NEW_APP_NAME in file contents
find . -type f -not -path '*/\.git/*' -exec sed -i "s/process_runner/$NEW_APP_NAME/g" {} \;
find . -type f -not -path '*/\.git/*' -exec sed -i "s/PROCESS_RUNNER/$NEW_APP_NAME_CAPS/g" {} \;

# Replace "process_runner" with NEW_APP_NAME in filenames
find . -type f -name "*process_runner*" -not -path '*/\.git/*' | while read -r file; do
    new_file=$(echo "$file" | sed "s/process_runner/$NEW_APP_NAME/g")
    mv "$file" "$new_file"
done

echo "Replacements complete:"
echo "  'GtkProcRunr' -> '$NEW_GTKBASE_NAME' in file contents"
echo "  'process_runner' -> '$NEW_APP_NAME' in file contents and filenames"
