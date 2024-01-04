{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = [
          pkgs.cmake
          pkgs.gcc-arm-embedded
          pkgs.openocd-rp2040
          pkgs.minicom
        ];
        
        PICO_SDK_PATH = "./firmware/pico-sdk";
      };
    });
  }
