#!/usr/bin/php
<?php
$cc  = "gcc";
$opt = "-Wall -O2 -g";
$opt_lm = "-lm -Wall -O2 -g";

$files = glob("*.c");
$def_a = array();
$obj_a = array();
foreach ($files as $f) {
  $obj = preg_replace("/\.c$/", ".o", $f);
  // check header file
  $head = preg_replace("/\.c$/", ".h", $f);
  if (!file_exists($head)) $head = "";
  $def_a[] = "$obj:$f $head\n\t\$(cc) \$(opt) -c $f";
  // 
  $obj_a[] = $obj;
}

$obj_s = implode(" ", $obj_a);
$def_s = implode("\n", $def_a);
$src = <<< EOS
# Makefile created by make.php
# ---
cc=$cc
opt=$opt
opt_lm=$opt_lm
objs=$obj_s
# ---
# make sakura
sakurac: \$(objs)
	$(cc) $(opt_lm) -o sakurac $(objs)
	ctags *.c *.h
# ---
# every object
$def_s
# ---
.PHONY:clean
clean:
	rm -f sakurac $(objs)
	rm -f tags
# ---
.PHONY: all
all: sakurac

EOS;

file_put_contents("Makefile","$src");
echo "make.php: ok\n";

system("make");
// system("doxygen");

