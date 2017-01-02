if [ -f install_manifest.txt ]; then
    while read p; do
        if [ -f "$p" ]; then
            if [ "$1" = "-v" ]; then
                echo "Removing: $p"
            fi

            rm $p
        fi
    done < install_manifest.txt
else
    echo "Err: No install_manifest.txt exists."
fi
