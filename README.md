
This is a simple binding to libmpg123 from octave. Contents:

Functions:

mp3init()
- Initializes the libmpg123 library

[mh,rate,channels,bsize] = mp3open( filename )
- open a mp3 file 
- return 
  a pointer to memory where the mpg123 handle is
  Sampling rate in Hz
  Number of channels
  Recommended minimum read block size
- Initializes the library to int16 samples on 2 channels with 44100 Hz rate

[nread,data] = mp3read( mh, size )
- try read from the mp3 file represented by mpg123 handle mh "size" number of samples
- return
  nread - number of read samples (==size in the middle, <size at end, ==0 at EOF)
  data, a int16(channels,nread) containing the read data

mp3close( mh )
- close the mp3 file

