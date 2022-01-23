# Overheat

## Building and development

Install `cmake`, `python`, `conan` (preferably with pip) and `ninja`. 

As almost all IDEs usually handle cmake projects automatically, just open this 
folder in your favorite one. All the dirty work will be done by cmake and 
conan. On the first run of cmake configuration you may take a lunch 
or watch a film because of quite long compilation of Qt and other dependencies.

**For Windows users:** you may install `cmake` and `ninja` with Visual Studio installer. Also ensure, that you installed VS 2019 and latest `msbuild` v142. If you prefer using other IDE or text editor (e.g. Clion or Visual Studio Code) 
configure it to use `MSVC`. Other compilers are not supported!
