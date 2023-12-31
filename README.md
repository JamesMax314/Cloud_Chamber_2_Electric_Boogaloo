# Cloud Chamber

## How to compile

- Install Emscripten
- Activate Emscripten
- Compile
```bash
mkdir build
cd build
emcmake cmake ..
make
```
- Run
```bash
python -m http.server
```

- View in browser at http://localhost:8000/ui/

## Refs
- https://gist.github.com/WesThorburn/00c47b267a0e8c8431e06b14997778e4#
- https://marcelbraghetto.github.io/a-simple-triangle/2019/03/10/part-06/
- https://github.com/ArthurSonzogni/OpenGL_CMake_Skeleton/tree/master
- https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/2.2.hello_triangle_indexed/hello_triangle_indexed.cpp
- https://stackoverflow.com/questions/16904340/webgl-particle-engine-using-fbo-how-to-correctly-write-and-sample-particle-pos

- https://open.gl/feedback

Ray Marching
- https://www.vertexfragment.com/ramblings/volumetric-cloud-banding/#raymarch-offset
- https://www.scratchapixel.com/lessons/3d-basic-rendering/volume-rendering-for-developers/ray-marching-get-it-right.html
- SDFs: https://iquilezles.org/articles/distfunctions/

Clouds
- https://hal.science/hal-01730789/file/Procedural-Cloudscapes-EG2018-article.pdf
- https://arxiv.org/pdf/1608.08570.pdf

Frame buffers
- https://learnopengl.com/Advanced-OpenGL/Framebuffers

## Class Structure

```mermaid
classDiagram

      class App
    App :-int npoints, nlevels
    App :-Bubble bubbles[]
    App :-Track tracks[]
    App :-Canvas ctx
    App :+App(string id)
    App :+new_track~T~(args)
    App :+find_track(int x,y)
    App :+draw()
    App :+random(double mi, ma)
```

# TODO:
- [ ] Make rays not a subclass of sim;
- [ ] Rays should take a reference to the gpu data for particle tracks when it generates its density grid;
