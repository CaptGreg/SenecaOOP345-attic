#!/usr/bin/ksh93

########################################################################
#                                                                      #
#               This software is part of the ast package               #
#                 Copyright (c) 2006-2012 Roland Mainz                 #
#                      and is licensed under the                       #
#                 Eclipse Public License, Version 1.0                  #
#                    by AT&T Intellectual Property                     #
#                                                                      #
#                A copy of the License is available at                 #
#          http://www.eclipse.org/org/documents/epl-v10.html           #
#         (with md5 checksum b35adb5213ca9657e911e9befb180842)         #
#                                                                      #
#                                                                      #
#                 Roland Mainz <roland.mainz@nrubsig.org>              #
#                                                                      #
########################################################################

#
# Copyright (c) 2006, 2013, Roland Mainz. All rights reserved.
#

#
# mandelbrotset1 - a simple mandelbrot set generation and
# parallel execution demo
#

# Solaris needs /usr/xpg6/bin:/usr/xpg4/bin because the tools in /usr/bin are not POSIX-conformant
export PATH='/usr/xpg6/bin:/usr/xpg4/bin:/bin:/usr/bin'

# Make sure all math stuff runs in the "C" locale to avoid problems
# with alternative # radix point representations (e.g. ',' instead of
# '.' in de_DE.*-locales). This needs to be set _before_ any
# floating-point constants are defined in this script).
if [[ "${LC_ALL-}" != '' ]] ; then
	export \
		LC_MONETARY="${LC_ALL}" \
		LC_MESSAGES="${LC_ALL}" \
		LC_COLLATE="${LC_ALL}" \
		LC_CTYPE="${LC_ALL}"
		unset LC_ALL
fi
export LC_NUMERIC='C'


function printmsg
{
	print -u2 -f "$@"
	return 0
}

function fatal_error
{
	print -u2 -n "${progname}: "
	print -u2 -f "$@"
	exit 1
}

# Get terminal size and put values into a compound variable with the integer
# members "columns" and "lines"
function get_term_size
{
	nameref rect=$1
    
	rect.columns=${ tput cols ; } || return 1
	rect.lines=${ tput lines ; }  || return 1
    
	return 0
}

# mandelbrot argument list
# We use one compound variable here because this is faster than passing multiple arguments
typeset -T mandelarg_t=(
	float x
	float y
	float x1
	float y1
	integer iteration
	integer max_iteration
	float max_mag
)

function mandelbrot
{
	nameref result=$1 # in: name of variable where we store the results
	nameref args=$2 # in: name of list of arguments
	float   x
	float   y
	float   xx
	float   yy
	float   x1
	float   y1
	integer iteration
	integer max_iteration
	float   mag
	float	max_mag
	
	# copy arguments into local variables (which are faster)
	((
		x=args.x , y=args.y ,
		x1=args.x1 , y1=args.y1 ,
		iteration=args.iteration , max_iteration=args.max_iteration ,
		max_mag=args.max_mag
	))
	
	for (( mag=0. ; mag < max_mag && iteration < max_iteration ; iteration++ )) ; do
		# "y=fma(x, y*2., y1)" is identical to "y=x*y*2.+y1"
		((
			xx=pow(x, 2.) ,
			yy=pow(y, 2.) ,
			mag=xx+yy ,
			y=fma(x, y*2., y1) ,
			x=xx-yy+x1
		))
	done

	(( result=iteration ))

	return 0
}

# build mandelbrot image serially
function loop_serial
{
	float x y
	integer value
	typeset -a lines # array to store the lines (we use this since line+="$str" does not scale for large strings)
	typeset line='' # single line buffer
	mandelarg_t ma
	typeset IFS

	for (( y=y_min ; y < y_max ; y+=stepwidth )) ; do
		for (( x=x_min ; x < x_max ; x+=stepwidth )) ; do
			(( ma.x=x , ma.y=y , ma.x1=x , ma.y1=y, ma.iteration=0 , ma.max_iteration=symbollist.len , ma.max_mag=max_mag ))
			mandelbrot value ma
			line+="${symbollist.symbols[value]}"
		done

		lines+=( "${line}" )
		line=''
	done
	IFS=$'\n' ; cat <<<"${lines[*]}" ; IFS=$' \t\n'
	
	return 0
}

# build mandelbrot image using parallel worker jobs
function loop_parallel
{
	integer numjobs=0
	float x y
	integer lines_per_job
	typeset tmpjobdir
	bool aborted=false
	typeset jid # job id for INT/HUP trap
	mandelarg_t ma
	typeset IFS

	# the following calculation suffers from rounding errors
	(( lines_per_job=int((m_height+(numcpus-1)) / numcpus) ))
	
	printmsg $"# lines_per_job=%d\n" ${lines_per_job}
	printmsg $"# numcpus=%d\n" ${numcpus}

	# "renice" worker jobs
	set -o bgnice

	tmpjobdir="$(mktemp --default=/tmp --directory "mandelbrotset1${PPID}_$$_XXXXXX")" || fatal_error $"Could not create temporary directory.\n"
	trap "rm -r ${tmpjobdir}" EXIT # cleanup

	# try to generate a job identifer prefix which is unique across multiple hosts
	jobident="job_host_$(uname -n)pid_$$_ppid${PPID}"

	printmsg $"## prepare...\n"
	for (( y=y_min ; y < y_max ; y+=(stepwidth*lines_per_job) )) ; do
		rm -f "${tmpjobdir}/${jobident}_child_$y.joboutput"

		(( numjobs++ ))
	done

	printmsg $"## running %d children...\n" ${numjobs}
	for (( y=y_min ; y < y_max ; y+=(stepwidth*lines_per_job) )) ; do
		{
			# we use a serialised compound variable to transport the data
			# between worker child and parent. This is more efficient
			# than manual parsing
			compound c=(
				# like "loop_serial" we use an array to store the lines
				# because line+="$str" does not scale for large strings
				typeset -a lines
			)
			typeset line=''
			# save file name since we're going to modify "y"
			typeset filename="${tmpjobdir}/${jobident}_child_$y.joboutput"
			integer value

			for (( ; y < y_max && lines_per_job-- > 0 ; y+=stepwidth )) ; do
				for (( x=x_min ; x < x_max ; x+=stepwidth )) ; do
					(( ma.x=x , ma.y=y , ma.x1=x , ma.y1=y, ma.iteration=0 , ma.max_iteration=symbollist.len , ma.max_mag=max_mag ))
					mandelbrot value ma
					line+="${symbollist.symbols[value]}"
				done

				c.lines+=( "${line}" )
				line=''
			done
			
			# store results as serialised compound variable
			# (we use "print -C" because it is more compact than "print -v")
			print -C c >"${filename}"
			
			exit 0
		} &
	done

	printmsg $"## waiting for %d children...\n" ${numjobs}
	
	# register trap to terminate worker children if we are asked to quit
	# fixme: this should use TERM, not KILL. ast-ksh.2010-07-01 sometimes ignores signals for the worker children
	trap 'set -o xtrace ; aborted="true" ; LC_ALL="C" jobs -l | fgrep "Running" | while read jid ; do kill -s KILL "%${jid/~(Elr)\[([^ ]*)\] .*/\1}" ; done' INT HUP

	# wait for all children
	# we loop to make sure any extra signals aborting "wait" do not
	# interfere with waiting for all children
	while ! wait ; do true ; done

	if (( aborted )) ; then
		# EXIT trap will cleanup temporary files
		return 1
	fi

	printmsg $"## output:\n"
	for (( y=y_min ; y < y_max ; y+=(stepwidth*lines_per_job) )) ; do
		read -C job_c <"${tmpjobdir}/${jobident}_child_$y.joboutput"
		IFS=$'\n' ; cat <<<"${job_c.lines[*]}" ; IFS=$' \t\n'
		# EXIT trap will cleanup temporary files
	done
	
	return 0
}

function usage
{
	OPTIND=0
	getopts -a "${progname}" "${mandelbrotset1_usage}" OPT '-?'
	exit 2
}

function main
{
	integer i
	typeset mode='parallel'

	# terminal size rect
	compound termsize=(
		integer columns=-1
		integer lines=-1
	)
	
	get_term_size termsize || fatal_error $"Could not get terminal size.\n"
	
	(( max_mag=400 ))
	(( stepwidth=0.1 ))
	
	# calculate number of worker CPUs and use 3 as fallback
	(( numcpus=$(getconf 'NPROCESSORS_ONLN' || print '3') ))
	(( numcpus=numcpus*4 ))
	
	(( m_width=termsize.columns-1 , m_height=termsize.lines-2 ))

	while getopts -a "${progname}" "${mandelbrotset1_usage}" OPT ; do 
		case "${OPT}" in
			'w')	m_width="${OPTARG}"		;;
			'h')	m_height="${OPTARG}"		;;
			's')	symbollist.str="${OPTARG}"	;;
			'm')	max_mag="${OPTARG}"		;;
			'p')	stepwidth="${OPTARG}"		;;
			'S')	mode='serial'			;;
			'+S')	mode='parallel'			;;
			'P')	mode='parallel'			;;
			'+P')	mode='serial'			;;
			'M')	mode="${OPTARG}"		;;
			'C')	numcpus="${OPTARG}"		;;
			*)	usage				;;
		esac
	done
	shift $(( OPTIND-1 ))
	
	printmsg "# width=%d\n" ${m_width}
	printmsg "# height=%d\n" ${m_height}
	printmsg "# max_mag=%g\n" ${max_mag}
	printmsg "# stepwidth=%g\n" ${stepwidth}
	printmsg "# symbollist=%q\n" "${symbollist.str}"
	printmsg "# mode=%q\n" "${mode}"
	
	(( symbollist.len=${#symbollist.str}-1 ))
	for (( i=0 ; i <= symbollist.len ; i++ )) ; do
		symbollist.symbols[i]="${symbollist.str:i:1}"
	done
	
	((
		x_max=m_width*stepwidth/2. ,
		x_min=-x_max ,
		y_max=m_height*stepwidth/2. ,
		y_min=-y_max
	))
	
	case "${mode}" in
		'parallel')	loop_parallel	; return $? ;;
		'serial')	loop_serial	; return $? ;;
		*)		fatal_error $"Unknown mode %q.\n" "${mode}" ;;
	esac
	
        # not reached
	return 0
}

# main
builtin basename
builtin cat
builtin rm
builtin uname # loop_parallel needs the ksh93 builtin version to generate unique job file names
builtin mktemp

set -o noglob
set -o nounset

typeset progname="${ basename "${0}" ; }"

# global variables
float x_max
float x_min
float y_max
float y_min
float m_width
float m_height
float max_mag
float stepwidth
integer numcpus

compound symbollist=(
	typeset str='    .:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ%@#'
	integer len
	typeset -a symbols
)

typeset -r mandelbrotset1_usage=$'+
[-?\n@(#)\$Id: mandelbrotset1 (Roland Mainz) 2013-05-13 \$\n]
[-author?Roland Mainz <roland.mainz@nrubsig.org>]
[+NAME?mandelbrotset1 - generate mandelbrot set fractals with ksh93]
[+DESCRIPTION?\bmandelbrotset1\b mandelbrot set fractal generator
	which runs either in serial or parallel mode (using multiple worker jobs).]
[w:width?Width of fractal.]:[width]
[h:height?Height of fractal.]:[height]
[s:symbols?Symbols to build the fractal from.]:[symbolstring]
[m:mag?Magnification level.]:[magnificationlevel]
[p:stepwidth?Width per step.]:[widthperstep]
[S:serial?Run in serial mode.]
[P:parallel?Run in parallel mode.]
[M:mode?Execution mode.]:[mode]
[C:numcpus?Number of processors used for parallel execution.]:[numcpus]
[+SEE ALSO?\bjuliaset1\b(1), \bksh93\b(1)]
'

main "$@"
exit $?

# EOF.
