# Markdown2Ansi

![see](see.png)
![viu](viu.png)

## Usage

```sh
cat markdown.md | see
```

## Install

```sh
git clone --recurse-submodules --depth=1 https://github.com/mrbeardad/Markdown2Ansi
mkdir Markdown2Ansi/build
cd Markdown2Ansi/build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -t see
mv see ~/.local/bin
```

## Depends

```sh
sudo apt install -y libboost-all-dev libsource-highlight-dev
```
