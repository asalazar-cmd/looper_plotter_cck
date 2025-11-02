#include "plotter.h"

Plotter:: Plotter() {
    x = 0;
    y = 0;
    w = 700;
    h = 900;
    maxHistory = 100;
    plotColor = ofColor(0, 255, 0);
    lineWidth = 2.0;
    minVal = 0;
    maxVal = 1;
    needsUpdate = true;
}
void Plotter::setup(float _x, float _y, float _w, float _h, int _maxHistory ) {
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    maxHistory = _maxHistory;

    // Configura el FBO con transparencia
    ofFboSettings settings;
    settings.width = w;
    settings.height = h;
    settings.internalformat = GL_RGBA;
    settings.numSamples = 4; // Anti-aliasing
    fbo.allocate(settings);

    clear();
}


void Plotter::update(){

    if (!needsUpdate || dataHistory.size() < 2) return;
    ofPushStyle();

    updateRange();

    fbo.begin();
    ofClear(0, 0, 0, 0); // Fondo transparente

    ofSetLineWidth(lineWidth);
    ofSetColor(plotColor);

    // Crea la curva Bezier continua
    ofPolyline line;

    float stepY = h / (float)(maxHistory - 1);
    //for (int i = dataHistory.size()-1 ; i >0 ; i --) {
    for (int i = 0; i < dataHistory.size(); i++) {
        emaSuavizado= emaSuavizado*suavizado+(1.-suavizado)*mapValueW(dataHistory[i]);
        float px =emaSuavizado;// mapValueW(dataHistory[i]);
        float py ;
        if(direccionDibujo)  py  = h- i * stepY;
        else       py  = i * stepY;
        line.addVertex(px, py);
        //ofLog()<<py<<endl;

    }

    // Suaviza la línea con Bezier
    if (dataHistory.size() > 2) {
        line = line.getSmoothed(3); // Suavizado
    }

    line.draw();

    fbo.end();

    ofPopStyle();
    needsUpdate = false;
}
