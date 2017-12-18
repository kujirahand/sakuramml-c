#!/bin/bash
make
./sakurac -nostdmsg -debug test.mml
timidity test.mid


