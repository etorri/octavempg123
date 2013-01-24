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




DEFUN_DLD(mp3close, args, nargout,
	  "Close mp3 file handle.")
{
  const int nargin = args.length();
  octave_value_list retval;

  if( nargin!=1 ) 
    {
      error("Need mh* parameter");
      return retval;
    }

  mpg123_handle *mh = (mpg123_handle *) args(0).long_value();
  mpg123_close(mh);
  mpg123_delete(mh);
  return retval;
}


