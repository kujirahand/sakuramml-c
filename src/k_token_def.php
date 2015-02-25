#!/usr/bin/php
<?php
// CUT
$marker_begin = "///<TOKEN_DEFINE>";
$marker_end   = "///</TOKEN_DEFINE>";
$src = file_get_contents("k_token_def.h");
$begin_i = strpos($src, $marker_begin);
$end_i = strpos($src, $marker_end);
if ($begin_i === false || $end_i === false) {
  echo "no marker"; exit;
}
$len = strlen($marker_begin);
$def = substr($src, $begin_i+$len, $end_i-$begin_i-$len);
// read define
$res = "";
$lines = explode("\n", $def);
foreach ($lines as $line) {
  $line = trim($line);
  if ($line == "") continue;
  if (substr($line, 0, 2) == "//") continue;
  if (preg_match('/\#define (\w+)\s+(0x[0-9a-fA-F]+)/', $line, $m)) {
    $name = $m[1];
    $val  = $m[2];
    echo "$name=$val\n";
    $res .= "  if (n==$val) { return \"$name\"; }\n";
  }
}
// write
$head = <<< EOS
// ---
// auto generated from k_token_def.h by k_token_def.php
// ---
#include <stdio.h>
#include "k_token_def.h"
// definet to string
char *KTokenDef_toString(int n) {

EOS;
$foot = <<< EOS
  // UNKNOWN
  return NULL;
}

EOS;
echo $res;

file_put_contents("k_token_def.c",
  $head.$res.$foot);
echo "ok\n";


