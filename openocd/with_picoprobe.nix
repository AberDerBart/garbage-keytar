{stdenv, automake, which, libftdi1, libusb1, hidapi, libtool, autoconf, git, jimtcl, pkgconfig, fetchFromGitHub}:

stdenv.mkDerivation rec {
  pname = "openocd-picoprobe";
  version = "0.10.0";

  src = fetchFromGitHub {
    owner = "raspberrypi";
    repo = "openocd";
    rev = "eb22aceb524ee0460d4de48e6b3c867502be937f";
    sha256 = "0lw5xh5zj35j405bcvcsg75jvzki2wscz7mlwv0rfd1xlzcbrc29";
  };

  nativeBuildInputs = [ pkgconfig ];
  buildInputs = [ automake which libftdi1 libusb1 hidapi libtool autoconf git jimtcl ];

  preConfigure = ''SKIP_SUBMODULE=1 ./bootstrap'' ;

  configureFlags = [
    "--enable-picoprobe"
    "--disable-internal-jimtcl"
    "--disable-internal-libjaylink"
    #--prefix=/usr/
  ];

  # makeFlags = [ "DESTDIR=$(out)" ];
}
