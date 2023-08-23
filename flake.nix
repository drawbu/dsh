{
  description = "drawbu's shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-23.05";
    utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        cc = pkgs.gcc12;

        pname = "dsh";
        libs = with pkgs; [ criterion ];

        mkCApp = name: pkgs.stdenv.mkDerivation rec {
          inherit name;
          src = ./.;

          makeFlags = [ "CC=${cc}/bin/gcc" ];
          buildInputs = libs ++ [ pkgs.ncurses ];

          hardeningDisable = [ "format" "fortify" ];
          enableParallelBuilding = true;

          buildPhase = ''
            make ${name} NO_COV=1
          '';

          installPhase = ''
            mkdir -p $out/bin
            cp ${name} $out/bin
          '';
        };

      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            glibc
            gcovr
            ltrace
            gnumake
            valgrind
            python311Packages.compiledb
            man-pages
          ] ++ [ cc ] ++ libs;
        };

        formatter = pkgs.nixpkgs-fmt;
        packages = {
          default = mkCApp pname;
          debug = mkCApp "debug";
          test = mkCApp "test";
        };
      });
}