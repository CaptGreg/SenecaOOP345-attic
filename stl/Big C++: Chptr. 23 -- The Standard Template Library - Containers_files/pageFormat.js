// pageFormat.js -- holds any functions used by all pages in the project
//
// Kurt Schmidt
//
// 12/4/03
//
// REVISIONS
// 12/4/03 - added image(), KS
// 12/4/03 - added iframeTxt(), KS
// 12/4/03 - changed iframeTxt to setupIframe, KS
// 12/12/03 - changed gImageAltTxt, KS
// 12/16/03 - added iframeWrapCode, KS
// 12/16/03 - changed codeDir to gCodeDir, KS
// 12/17/03 - changed text in iframeWrapCode(), KS
// 1/9/04 - removed iframeTxt, changed \" to ' in strings,
//					change image() to take only a filename, prepend gImageDir, KS
// 2/1/04 - changed the local width and height, which clashed with
//					document.width and document.height in Mozilla.
//					Created gIframeAltTxt		KS
// 2/13/04 - Wrote title in instead of using document.title (Mozilla was
//						resisting)	KS
//

//&&&&&&&&  Project Variables   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

var gBookTitle = "Big C++"

var gImageAltTxt = "Check path and permissions"

var gIframeAltTxt =
	"Your browser does not support the &lt;iframe&gt; tag." 

//&&&&&&&&   Functions   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

//////////////////////////////////////////////////////////////////////////
// title - sets the title of the page
// depend:  gBookTitle (this file)
//					gChptrNumber, gChptrTitle (local config.js)

function title()
{
	with( document )
	{
		write( "<title>" )
		write( gBookTitle )
		write( ":  Chptr. " )
		write(  gChptrNumber )
		write( " -- " )
		write( gChptrTitle )
		write( "</title>\n" )
	}
}

//////////////////////////////////////////////////////////////////////////
// image - creates centered image tag, w/alt text defined above
// depend:  gImageAltText (this file), gImageDir (local config.js)

function image( src )
{
	var path = gImageDir + "/" + src

	with( document )
	{
		write( "<p align='center'><img src='" )
		write( path )
		write( "'\n" )
		write( "\talt='" )
		write( path )
		write( " - " )
		write( gImageAltTxt )
		write( "'></p>\n" )
	}
}

//////////////////////////////////////////////////////////////////////////
// setupIframe - sets up
// inputs:  src, the html file to be placed into the iframe
//		width, height, strings, optional.  Legal values for the associated
//			attributes of the iframe tag

function setupIframe( src, fWidth, fHeight )
{
	with( document )
	{
		// open iframe tag
		write( "<iframe src='" )
		write( src )
		write( "'" )
		if( fWidth.length != 0 )	// default width?
		{
			write( " width='" )
			write( fWidth )
			write( "'" )
		}
		if( fHeight.length != 0 )	// default height?
		{
			write( " height='" )
			write( fHeight )
			write( "'" )
		}

		write( ">\n<p>" )
		write( gIframeAltTxt )
		write( "</p>\n" )
		write( "<br>Click <a target='bigc' href='" )
		write( src )
		write( "'>\n" )
		write( src )
		write( "</a> to view file.<br>" )
		write( "\n</iframe>" )
	}
}

////////////////////////////////////////////////////////////////////////////
// iframeWrapCode - given a source file, s, assumes s.html exists.
//		Sets up iframe, and creates a link to the source, as well.
// inputs:  name of source file,
//		width, height, strings, optional.  Legal values for the associated
//		attributes of the iframe tag
// Note:  needs gCodeDir to be set (see config.js).  
//

function iframeWrapCode( srcName, fWidth, fHeight )
{
	// build path to target (code, not HTML)
	var targ = gCodeDir + "/" + srcName

	// create the iframe
	setupIframe( targ+".html", fWidth, fHeight )

	// add a link to the source:
	with( document )
	{
		write( "\n<br><font size='-1'><i>Click <a target='bigc' href='" )
		write( targ )
		write( "'>" + srcName + "</a> to download plain text")
		write( "</i></font>\n" )
	}
}

