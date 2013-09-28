#! /bin/sh

ls *.ts > "$1"

lupdate *.cpp *.h -ts @"$1"
