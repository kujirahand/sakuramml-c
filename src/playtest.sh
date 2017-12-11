#!/bin/bash
make
./sakurac -debug test.mml
timidity test.mid


