let 
  pkgs = import (fetchTarball "https://github.com/NixOS/nixpkgs/archive/cd63096d6d887d689543a0b97743d28995bc9bc3.tar.gz") { overlays = [ 
    (self: super: {
      openocdpico = super.callPackage ./openocd/with_picoprobe.nix {}; 
    })
  ]; };
in pkgs.mkShell {
  buildInputs = [
    pkgs.cmake
    pkgs.gcc-arm-embedded
    pkgs.openocdpico
  ];
}
