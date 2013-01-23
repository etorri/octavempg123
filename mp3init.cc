

#include <octave/oct.h>
#include <octave/parse.h>
#include <mpg123.h>



void cleanup(mpg123_handle *mh)
{
  mpg123_close(mh);
  mpg123_delete(mh);
  mpg123_exit();
}



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





DEFUN_DLD(mp3init, args, nargout,
          "Initialize the mpg123 library.")
{
  const int nargin = args.length();
  octave_value_list retval;
  
  if( mpg123_init() != MPG123_OK )
    {
      error("Initialization of mpg123 failed");
    }
  return retval;
}

