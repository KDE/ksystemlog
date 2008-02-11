#! /bin/sh
$EXTRACTRC */*.kcfg >> rc.cpp || exit 11
$EXTRACTRC src/*.rc >> rc.cpp || exit 12
$EXTRACTRC src/*.ui >> rc.cpp || exit 13
$XGETTEXT `find . -name "*.cpp"` -o $podir/ksystemlog.pot
rm -f rc.cpp
