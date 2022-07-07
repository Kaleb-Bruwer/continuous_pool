sudo chown vscode ./enzyme/build
sudo ln -s /usr/bin/clang-12 /usr/bin/clang
sudo ln -s /usr/lib/llvm-12/bin/opt /usr/bin/opt

sudo apt update
sudo apt install libsdl2-dev -y
sudo apt install libsdl2-image-dev -y
sudo apt install libsdl2-ttf-dev -y
sudo apt install libsdl2-mixer-dev -y