#ifndef PLOTTER_H
#define PLOTTER_H

#include "ofMain.h"

class Plotter
{
public:
private:
    ofFbo fbo;
    vector<float> dataHistory;

    float x, y;           // Posición superior izquierda
    float w, h;           // Ancho y alto
    int maxHistory;       // Número de puntos en el historial

    float minVal, maxVal; // Rango automático
    ofColor plotColor;
    float lineWidth;
    bool direccionDibujo= false;
    bool needsUpdate;
    float suavizado= 0.5;
    float emaSuavizado=  0.01;

    // Calcula el rango de valores
    void updateRange() {
        if (dataHistory.empty()) {
            minVal = 0;
            maxVal = 1;
            return;
        }

        minVal = *min_element(dataHistory.begin(), dataHistory.end());
        maxVal = *max_element(dataHistory.begin(), dataHistory.end());

        // Evita división por cero
        if (abs(maxVal - minVal) < 0.0001) {
            maxVal = minVal + 1;
        }
    }

    // Mapea un valor al rango de altura
    float mapValueW(float val) {
        return ofMap(val, minVal, maxVal, w - 10, 10, true);
    }
    float mapValueH(float val) {
        return ofMap(val, minVal, maxVal, h - 10, 10, true);
    }
public:
    Plotter() ;
    // Configuración
    void setup(float _x, float _y, float _w, float _h, int _maxHistory = 100) ;

    // Setters
    void setPosition(float _x, float _y) {
        x = _x;
        y = _y;
    }
    void setSuavizado(float s){

        suavizado=s;
        suavizado=max(suavizado,0.0f);
        suavizado=min(suavizado,1.0f);
    }
    void setDireccion(bool b){
        direccionDibujo=b;
    }
    void setSize(float _w, float _h) {
        if (w != _w || h != _h) {
            w = _w;
            h = _h;

            ofFboSettings settings;
            settings.width = w;
            settings.height = h;
            settings.internalformat = GL_RGBA;
            settings.numSamples = 4;
            fbo.allocate(settings);

            needsUpdate = true;
        }
    }

    void setMaxHistory(int _maxHistory) {
        maxHistory = _maxHistory;
        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }
        needsUpdate = true;
    }

    void setColor(ofColor color) {
        plotColor = color;
        needsUpdate = true;
    }

    void setLineWidth(float width) {
        lineWidth = width;
        needsUpdate = true;
    }

    // Agrega un nuevo valor
    void addValue(float value) {
        dataHistory.push_back(value);

        // Mantiene el tamaño del historial
        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }

        needsUpdate = true;
    }

    // Agrega múltiples valores
    void addValues(const vector<float>& values) {
        for (float val : values) {
            dataHistory.push_back(val);
        }

        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }

        needsUpdate = true;
    }

    // Limpia el historial
    void clear() {
        dataHistory.clear();
        needsUpdate = true;
    }

    // Actualiza y dibuja en el FBO
    void update() ;

    // Dibuja el FBO en pantalla
    void draw() {
        update();
       // ofSetColor(255);
        fbo.draw(x, y);
    }

    // Dibuja con posición personalizada
    void draw(float _x, float _y) {
        update();
        //ofSetColor(255);
        fbo.draw(_x, _y);
    }

    // Getters
    float getX() { return x; }
    float getY() { return y; }
    float getWidth() { return w; }
    float getHeight() { return h; }
    int getHistorySize() { return dataHistory.size(); }
    int getMaxHistory() { return maxHistory; }
    float getMinValue() { return minVal; }
    float getMaxValue() { return maxVal; }
    ofFbo& getFbo() { return fbo; }
};

#endif // PLOTTER_H
