#/bin/bash

## http://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux

## Black        0;30     Dark Gray     1;30
## Red          0;31     Light Red     1;31
## Green        0;32     Light Green   1;32
## Brown/Orange 0;33     Yellow        1;33
## Blue         0;34     Light Blue    1;34
## Purple       0;35     Light Purple  1;35
## Cyan         0;36     Light Cyan    1;36
## Light Gray   0;37     White         1;37

RED='\033[0;31m'
RED='\x1b[0;31m'
RED='\e[0;31m'

NC='\e[0m' # No Color
printf "I ${RED}love${NC} Stack Overflow\n"

## GB 

BLACK='\e[0;30m'
DARKGRAY='\e[1;30m'
RED='\e[0;31m'     
LIGHTRED='\e[1;31m'
GREEN='\e[0;32m'     
LIGHTGREEN='\e[1;32m'
BROWNORANGE='\e[0;33m'     
YELLOW='\e[1;33m'
BLUE='\e[0;34m'     
LIGHTBLUE='\e[1;34m'
PURPLE='\e[0;35m'     
LIGHTPURPLE='\e[1;35m'
CYAN='\e[0;36m'     
LIGHTCYAN='\e[1;36m'
LIGHTGRAY='\e[0;37m'     
WHITE='\e[1;37m'
NC='\e[0m' # No Color

printf "${RED}I ${GREEN}love ${PURPLE}Stack ${CYAN}Overflow ${BLUE}alot ${NC}\n"
printf "${RED}RED ${GREEN}GREEN ${BLUE}BLUE ${CYAN}CYAN ${PURPLE}PURPLE/MAGENTA ${YELLOW}YELLOW ${BLACK}BLACK ${NC}\n"
printf "${DARKGRAY}DARKGRAY ${LIGHTRED}LIGHTRED ${LIGHTGREEN}LIGHTGREEN ${BROWNORANGE}BROWNORANGE ${LIGHTBLUE}LIGHTBLUE ${LIGHTPURPLE}LIGHTPURPLE ${LIGHTCYAN}LIGHTCYAN ${LIGHTGRAY}LIGHTGRAY ${WHITE}WHITE ${NC}\n"


## http://bitmote.com/index.php?post/2012/11/19/Using-ANSI-Color-Codes-to-Colorize-Your-Bash-Prompt-on-Linux
#
# This file echoes a bunch of color codes to the terminal to demonstrate
# what's available. Each line is the color code of one forground color,
# out of 17 (default + 16 escapes), followed by a test use of that color
# on all nine background colors (default + 8 escapes).
#
T='gYw'   # The test text
echo -e "\n                 40m     41m     42m     43m     44m     45m     46m     47m";
for FGs in '    m' '   1m' '  30m' '1;30m' '  31m' '1;31m' '  32m' '1;32m' '  33m' '1;33m' '  34m' '1;34m' '  35m' '1;35m' '  36m' '1;36m' '  37m' '1;37m';
  do FG=${FGs// /}
  echo -en " $FGs \033[$FG  $T  "
  for BG in 40m 41m 42m 43m 44m 45m 46m 47m;
    do echo -en "$EINS \033[$FG\033[$BG  $T \033[0m\033[$BG \033[0m";
  done
  echo;
done
echo
#
# generates an 8 bit color table (256 colors) for
# reference purposes, using the \033[48;5;${val}m
# ANSI CSI+SGR (see "ANSI Code" on Wikipedia)
#
echo -en "\n   +  "
for i in {0..35}; do
  printf "%2b " $i
done

printf "\n\n %3b  " 0
for i in {0..15}; do
  echo -en "\033[48;5;${i}m  \033[m "
done

#for i in 16 52 88 124 160 196 232; do
for i in {0..6}; do
  let "i = i*36 +16"
  printf "\n\n %3b  " $i
  for j in {0..35}; do
    let "val = i+j"
    echo -en "\033[48;5;${val}m  \033[m "
  done
done

echo -e "\n"


## https://en.wikipedia.org/wiki/ANSI_escape_code

echo a b c d e WARN 1 2 3 4 5 | sed -e 's/.*\bWARN.*/\x1b[7m&\x1b[0m/i' -e 's/.*\bERR.*/\x1b[93;41m&\x1b[0m/i'
echo a b c d e ERR  1 2 3 4 5 | sed -e 's/.*\bWARN.*/\x1b[7m&\x1b[0m/i' -e 's/.*\bERR.*/\x1b[93;41m&\x1b[0m/i'

flasher () { while true; do printf \\e[?5h; sleep 0.1; printf \\e[?5l; read -s -n1 -t1 && break; done; }

echo press a key 
flasher

RESET='\e[0m'
BOLD='\e[1m'
DIM='\e[2'
UNDERLINE='\e[4m'
BLINK='\e[5'
REVERSE='\e[7m'
HIDDEN='\e[8m'



# save, clear screen
tput smcup
clear

# example "application" follows...
read -n1 -p "Press any key to continue..."
# example "application" ends here

# restore
tput rmcup

sleep 1

echo -e '\033[?47h' # save screen
read -n1 -p "Press any key to continue..."
echo -e '\033[?47l' # restore screen

## http://wiki.bash-hackers.org/scripting/terminalcodes

# Charles Cooke's 16-color Mandelbrot
# http://earth.gkhs.net/ccooke/shell.html
# Combined Bash/ksh93 flavors by Dan Douglas (ormaaj)

function doBash {
	typeset P Q X Y a b c i v x y 
	for ((P=10**8,Q=P/100,X=320*Q/cols,Y=210*Q/lines,y=-105*Q,v=-220*Q,x=v;y<105*Q;x=v,y+=Y)); do
		for ((;x<P;a=b=i=c=0,x+=X)); do
			for ((;a**2+b**2<4*P**2&&i++<99;a=((c=a)**2-b**2)/P+x,b=2*c*b/P+y)); do :
			done
			colorBox $((i<99?i%16:0))
		done
		echo
	done
}

function doKsh {
	integer i
	float a b c x=2.2 y=-1.05 X=3.2/cols Y=2.1/lines 
	while
		for ((a=b=i=0;(c=a)**2+b**2<=2&&i++<99&&(a=a**2-b**2+x,b=2*c*b+y);)); do :
		done
		. colorBox $((i<99?i%16:0))
		if ((x<1?!(x+=X):(y+=Y,x=-2.2))); then
			print
			((y<1.05)) 
		fi
		do :
	done
}

function colorBox {
	(($1==lastclr)) || printf %s "${colrs[lastclr=$1]:=$(tput setaf "$1")}"
	printf '\u2588'
}

unset -v lastclr
((cols=$(tput cols)-1, lines=$(tput lines)))
typeset -a colrs
trap 'tput sgr0; echo' EXIT
${KSH_VERSION+. doKsh} ${BASH_VERSION+doBash}

sleep 4


DATA[0]="     _/  _/    _/                            _/    "
DATA[1]="  _/_/_/_/_/  _/_/_/      _/_/_/    _/_/_/  _/_/_/ "
DATA[2]="   _/  _/    _/    _/  _/    _/  _/_/      _/    _/"
DATA[3]="_/_/_/_/_/  _/    _/  _/    _/      _/_/  _/    _/ "
DATA[4]=" _/  _/    _/_/_/      _/_/_/  _/_/_/    _/    _/  "

# virtual coordinate system is X*Y ${#DATA} * 5

REAL_OFFSET_X=0
REAL_OFFSET_Y=0

draw_char() {
  V_COORD_X=$1
  V_COORD_Y=$2

  tput cup $((REAL_OFFSET_Y + V_COORD_Y)) $((REAL_OFFSET_X + V_COORD_X))

  printf %c ${DATA[V_COORD_Y]:V_COORD_X:1}
}


trap 'exit 1' INT TERM
trap 'tput setaf 9; tput cvvis; clear' EXIT

tput civis
clear

# while :; do

  for ((c=1; c <= 7; c++)); do
    tput setaf $c
    for ((x=0; x<${#DATA[0]}; x++)); do
      for ((y=0; y<=4; y++)); do
        draw_char $x $y
      done
    done
  done

# done


ansi-test()
{
  for a in 0 1 4 5 7; do
    echo "a=$a " 
    for (( f=0; f<=9; f++ )) ; do
      for (( b=0; b<=9; b++ )) ; do
        #echo -ne "f=$f b=$b" 
        echo -ne "\\033[${a};3${f};4${b}m"
        echo -ne "\\\\\\\\033[${a};3${f};4${b}m"
        echo -ne "\\033[0m "
      done
      echo
    done
    echo
  done
  echo
}

ansi-test
