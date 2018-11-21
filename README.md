# XPT2046 / ADS7843 Touch sensor chip

A Particle library for XPT2046.

Ported from Paul Stoffregen's Arduino version.


## Usage

Connect XYZ hardware, add the XPT2046 library to your project and follow this simple example:

```
#include "XPT2046.h"
XPT2046 xPT2046;

void setup() {
  xPT2046.begin();
}

void loop() {
  xPT2046.process();
}
```

See the [examples](examples) folder for more details.

## Documentation

TODO: Describe `XPT2046`

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add XPT2046_myname` to add the library to a project on your machine or add the XPT2046_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library. 

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2018 Volker Ritzka

Licensed under the <insert your choice of license here> license
