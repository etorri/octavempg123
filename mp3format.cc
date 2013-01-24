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



DEFUN_DLD(mp3format, args, nargout,
	  "Get format of open mp3 file [rate, channels, encoding].")
{
  long mp3rate;
  int mp3channels;
  int mp3encoding;

  const int nargin = args.length();
  octave_value_list retval;

  if( nargin!=1 ) 
    {
      error("Need mh* parameter");
    }

  mpg123_handle *mh = (mpg123_handle *) args(0).long_value();
  if( mpg123_getformat(mh, &mp3rate, &mp3channels, &mp3encoding) != MPG123_OK )
    {
      error("Cannot read format.");
    }
  retval(2)= octave_value(mp3rate);
  retval(1)= octave_value(mp3channels);
  retval(0)= octave_value(mp3encoding);
  return retval;
}

