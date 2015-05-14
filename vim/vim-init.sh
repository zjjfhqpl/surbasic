#!/bin/sh

ln -s vim/vimrc .vimrc
ln -s vim/ycm_extra_conf.py .ycm_extra_conf.py
./vim/gen_tags.sh
