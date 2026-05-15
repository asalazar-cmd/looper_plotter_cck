#include "plotter.h"

// Constructor: valores por defecto antes de llamar a setup().
// La posici\u00f3n (x=0, y=0) es provisional; se sobreescribe con setup() o setPosition().
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
// Inicializa el FBO con anti-aliasing y transparencia, y pre-llena el historial
// con ruido Perlin para que el EMA ya est\u00e9 calibrado cuando lleguen datos reales,
// evitando el transitorio visual al arrancar la aplicaci\u00f3n.
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

    // Pre-llenar con ruido perlin para que el EMA esté calibrado desde el inicio.
    // Evita el transitorio inicial y la descalibración al empezar a recibir datos reales.
    for (int i = 0; i < maxHistory; i++) {
        dataHistory.push_back(ofNoise(i * 0.07f)*h);
    }
    emaSuavizado = 0.5f; // EMA normalizado: mitad del rango
    needsUpdate = true;
}


// Redibuha el historial en el FBO usando una curva B\u00e9zier suavizada.
// Solo ejecuta si needsUpdate==true y hay al menos 2 muestras.
// Proceso:
//   1. Recalcula el rango din\u00e1mico (updateRange).
//   2. Aplica EMA sobre los valores normalizados [0,1] para suavizar.
//   3. Mapea cada muestra a coordenadas de p\u00edxel dentro del FBO.
//   4. Suaviza la pol\u00edlnea con getSmoothed(3) antes de dibujarla.
void Plotter::update(){

    if (!needsUpdate || dataHistory.size() < 2) return;
    ofPushStyle();

    updateRange();

    fbo.begin();
    ofClear(0, 0, 0, 0); // Fondo transparente (alpha = 0)

    ofSetLineWidth(lineWidth);
    ofSetColor(plotColor);

    // Construye la polilínea mapeando cada muestra del historial.
    // stepY: separaci\u00f3n vertical uniforme entre muestras en el FBO.
    ofPolyline line;

    float stepY = h / (float)(maxHistory - 1);
    for (int i = 0; i < (int)dataHistory.size(); i++) {
        // Normaliza el valor al rango [0,1] para hacer el EMA independiente
        // de los cambios din\u00e1micos en minVal/maxVal (evita saltos al rellenarse el buffer).
        float normVal = ofMap(dataHistory[i], minVal, maxVal, 0.0f, 1.0f, true);
        // EMA: pondera el estado anterior (suavizado) con la nueva muestra (1 - suavizado)
        emaSuavizado  = emaSuavizado * suavizado + (1.0f - suavizado) * normVal;
        // Mapea de [0,1] a coordenada X en p\u00edxeles (valor alto → X peque\u00f1a = izquierda)
        float px = ofMap(emaSuavizado, 0.0f, 1.0f, w - 10.0f, 10.0f);
        float py;
        // direccionDibujo=true: sample reciente en la parte superior del FBO
        if(direccionDibujo)  py = h - i * stepY;
        else                 py = i * stepY;
        line.addVertex(px, py);
    }

    // Suaviza la polilínea con interpolaci\u00f3n de tipo Chaikin/Bezier (radio 3)
    if (dataHistory.size() > 2) {
        line = line.getSmoothed(3);
    }

    line.draw();

    fbo.end();

    ofPopStyle();
    needsUpdate = false;
}
