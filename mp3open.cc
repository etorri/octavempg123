/*
  ## Copyright (C) 2013   Eero Torri   <et@torri.be>
  ##
  ## This program is free software; you can redistribute it and/or modify
  ## it under the terms of the GNU General Public License as published by
  ## the Free Software Foundation; either version 3 of the License, or
  ## (at your option) any later version.
  ##
  ## This program is distributed in the hope that it will be useful,
  ## but WITHOUT ANY WARRANTY; without even the implied warranty of
  ## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ## GNU General Public License for more details.
  ##
  ## You should have received a copy of the GNU General Public License
  ## along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <octave/oct.h>
#include <octave/parse.h>
#include <mpg123.h>



DEFUN_DLD(mp3open, args, nargout,
	  "Open mp3 file with mpg123. [mh*,rate,channels,bsize] = mp3open(filename)")
{
  int err;
  long mp3rate=0;
  int mp3channels=0;
  int mp3encoding=0;
  const int nargin = args.length();
  octave_value_list retval;
  mpg123_handle *mh;

  octave_stdout << "Starting open\n";

  if( nargin != 1 || !args(0).is_string() ) 
    error("mp3open: expected one string parameter (name of mp3 file)" );


  if( (mh = mpg123_new(NULL, &err)) == NULL ) 
    {
      error("Could not create new  mpg123 handle.");
      return retval;
    }
  
  std::string fname = args(0).string_value();
	  
  if( mpg123_open(mh, fname.c_str()) != MPG123_OK || mpg123_getformat(mh, &mp3rate, &mp3channels, &mp3encoding) != MPG123_OK )
    {
      // cleanup(mh);
      error("mp3open: can not open mp3 file with that name" );
      return retval;
    }

  if( mp3encoding != MPG123_ENC_SIGNED_16 ) 
    {
      mpg123_format_none(mh);
      mp3encoding= MPG123_ENC_SIGNED_16;
      if( mpg123_format(mh, mp3rate, mp3channels, mp3rate) != MPG123_OK )
	{
	    mpg123_close(mh);
	    mpg123_delete(mh);
	    error("Cannot set encoding to signed16.");
	}

    }
  retval(3)=octave_value(mpg123_outblock(mh)/mp3channels/sizeof(short));
  retval(2)=octave_value(mp3channels);
  retval(1)=octave_value(mp3rate);
  retval(0)=octave_value((long) mh);
  return retval;
}


