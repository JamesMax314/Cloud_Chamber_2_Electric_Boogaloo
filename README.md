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