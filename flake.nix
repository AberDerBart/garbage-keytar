{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      #picoprobe_overlay = _: prev: {
      #  openocd = prev.callPackage ./openocd/with_picoprobe.nix {};
      #};
      #
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = [
          pkgs.cmake
          pkgs.gcc-arm-embedded
          pkgs.openocd
          pkgs.minicom
        ];
        
        PICO_SDK_PATH = "./firmware/pico-sdk";
      };
    });
  }
