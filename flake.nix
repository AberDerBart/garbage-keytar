{
  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
    in {
      packages.bin2c = pkgs.stdenv.mkDerivation {
        name = "bin2c";
        src = pkgs.fetchFromGitHub {
          owner = "daschr";
          repo = "pico-ssd1306";
          rev = "8467f5b06ebd7bfa7b53e4ebc83dfdc6f396e4eb";
          sha256 = "sha256-dwd67nhH9SRAbbMBIecUXf2lKCT6Yxc8hdn0qxqdp/8=";
        };
        #buildPhase = "make -C $src/firmware/pico-ssd1306/tools";
        buildPhase = "make -C tools";
        installPhase = ''
          mkdir -p $out/bin
          cp tools/bin2c $out/bin/bin2c
        '';
      };
      devShells.default = pkgs.mkShell {
        nativeBuildInputs = [
          pkgs.cmake
          pkgs.gcc-arm-embedded
          pkgs.openocd-rp2040
          pkgs.minicom
          pkgs.imagemagick
          self.packages.${system}.bin2c
        ];
        
        PICO_SDK_PATH = "./firmware/pico-sdk";
      };
    });
  }
