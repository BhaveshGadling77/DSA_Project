#!/bin/bash
sudo apt-get install -y libgtk-4-dev > /dev/null 2>&1

gcc ./GUI/practice.c -o ./GUI/practice1 `pkg-config --cflags --libs gtk4`

./GUI/practice1
