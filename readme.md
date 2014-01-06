# ofxGpuParticles

This openFrameworks addon makes it slightly easier to create particle systems in GLSL.

**N.B.** Does not work in OF 0.8 due to a bug with ofFbo...

https://github.com/openframeworks/openFrameworks/issues/2687

To fix it you can comment out the following line in **ofFbo.cpp**...

```cpp
settings.numColorbuffers = settings.colorFormats.size();
```

## Usage

Coming soon, for now, see example.

## TODO

Add in functionality to use programmable renderer in addition to stone age OpenGL.
