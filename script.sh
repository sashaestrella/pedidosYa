#!/bin/bash
str1='export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/tp-2020-2c-ES-triple-B/BibliotecaCompartida'
str2=$(tail -n 1 ~/.bashrc)

if [ "$str1" != "$str2" ]; then
  echo $str1 >> ~/.bashrc
fi
