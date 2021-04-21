let 
  pkgs = import <nixpkgs> {};
in pkgs.mkShell {
  buildInputs = [
    pkgs.cmake
    pkgs.gcc-arm-embedded
    (pkgs.vscode-with-extensions.override {
      vscodeExtensions = with pkgs.vscode-extensions; [ ms-vscode.cpptools ];
    })
  ];
}
