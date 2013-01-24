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


DEFUN_DLD(mp3read, args, nargout,
	  "Read a block of (max) N samples and return a Channels x M matrix. If end of file, then M<=N.")
{
  long mp3rate;
  int mp3channels;
  int mp3encoding;
  int err;
  const int nargin = args.length();
  octave_value_list retval;
  int n = 0; // number of samples to read

  if( nargin != 2 ) 
    {
      error("mp3read: need two parameters, mh* and nr samples.");
      return retval;
    }

  mpg123_handle *mh = (mpg123_handle *) args(0).long_value();
  n = args(1).int_value();

  if( mpg123_getformat(mh, &mp3rate, &mp3channels, &mp3encoding) != MPG123_OK )
    {
      error("Cannot read format.");
    }

  const size_t bufsize = n*mp3channels*sizeof(short);
  size_t done=0;
  short buf[mp3channels*n];

  err= mpg123_read(mh,(unsigned char *)buf,bufsize,&done);
  
  if( err != MPG123_OK && err != MPG123_DONE )
    {
      error("Error reading file.");
    }

  dim_vector dv (2);
  dv(0) = mp3channels; 
  dv(1) = done/sizeof(short)/mp3channels;
  int16NDArray samples(dv);
  short *p = (short *) samples.fortran_vec();
  memcpy(p,buf,done);

  retval(1)=octave_value(done/sizeof(short)/mp3channels);
  retval(0)=octave_value(samples);

  return retval;
} 

