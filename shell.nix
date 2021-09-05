let 
  pkgs = import <nixpkgs> { overlays = [ 
    (self: super: {
      openocdpico = super.callPackage ./openocd/with_picoprobe.nix {}; 
    })
  ]; };
in pkgs.mkShell {
  buildInputs = [
    pkgs.cmake
    pkgs.gcc-arm-embedded
    # pkgs.openocdpico
  ];
}
