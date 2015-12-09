* Sakuramml for Raspberry Pi (Raspbian)

Sakuramml (Text Music Sakura) is MML Compiler.

- [web site] http://oto.chu.jp/ (only Japanese)
- [Music Data BBS] http://oto.chu.jp/mmlbbs6/

* Enviroment for Raspberry Pi

Please install "timidity".

{{{
sudo apt-get install timidity 
sudo apt-get install fluid-sound-font-gm fluid-sound-font-gs
}}}

* How to compose?

Edit your music data.

{{{
pico hana.mml
}}}

example:

{{{
l4
cde^ cde^ gedc ded^
cde^ cde^ gedc dec^
ggeg aag^ eedd c^^^
}}}

And compile to MIDI file.

{{{
./sakura hana.mml
}}}

play it with timidity.

{{{
timidity hana.mid
}}}

* How to build from source code ?

{{{
# You need ctags (Interim)
$ sudo apt-get install ctags

# Download source code from GitHub
$ git clone https://github.com/kujirahand/sakuramml-c
$ cd sakuramml-c/src
$ make
}}}


