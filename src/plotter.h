#ifndef PLOTTER_H
#define PLOTTER_H

#include "ofMain.h"

// ============================================================
// Plotter — gráfico de historial de valores en tiempo real
// ============================================================
// Dibuja el historial de valores recibidos como una curva
// Bezier suavizada dentro de un FBO con transparencia.
//
// Flujo típico de uso:
//   1. Plotter p;
//   2. p.setup(x, y, w, h, maxHistory);  // configura FBO y posición
//   3. p.addValue(val);                   // en update(), agrega muestras
//   4. p.draw();                          // en draw(), vuelca el FBO
//
// El eje de dibujo es VERTICAL: los valores más recientes aparecen
// en la parte inferior (direccionDibujo=false) o superior (=true).
// El eje horizontal mapea el valor numérico con rango automático.
// ============================================================
class Plotter
{
public:
private:
    ofFbo fbo;                  // Buffer de renderizado off-screen con alpha
    vector<float> dataHistory;  // Historial circular de valores recibidos

    float x, y;           // Posición superior izquierda en pantalla
    float w, h;           // Ancho y alto del FBO en píxeles
    int maxHistory;       // Número máximo de muestras almacenadas

    float minVal, maxVal; // Rango dinámico calculado sobre el historial
    ofColor plotColor;    // Color de la línea
    float lineWidth;      // Grosor de la línea en píxeles

    // Si true: sample[0] en la parte superior, avanza hacia abajo.
    // Si false (default): sample[0] en la parte inferior.
    bool direccionDibujo = false;

    bool needsUpdate;     // Flag: true cuando el historial cambió y hay que redibujar

    // Factor de suavizado EMA (Exponential Moving Average) en rango [0, 1].
    // 0 = sin suavizado (señal cruda), 1 = máximo suavizado (línea plana).
    float suavizado = 0.5f;

    // Estado interno del EMA, mantenido en espacio normalizado [0,1]
    // para que sea independiente de cambios en minVal/maxVal.
    float emaSuavizado = 0.5f;

    // Recalcula minVal/maxVal sobre todo el historial.
    // Garantiza maxVal > minVal para evitar división por cero.
    void updateRange() {
        if (dataHistory.empty()) {
            minVal = 0;
            maxVal = 1;
            return;
        }

        minVal = *min_element(dataHistory.begin(), dataHistory.end());
        maxVal = *max_element(dataHistory.begin(), dataHistory.end());

        // Evita división por cero cuando todos los valores son iguales
        if (abs(maxVal - minVal) < 0.0001) {
            maxVal = minVal + 1;
        }
    }

    // Mapea 'val' al eje horizontal del FBO (con margen de 10 px a cada lado).
    // Nota: valores altos → x pequeña (izquierda), valores bajos → x grande.
    float mapValueW(float val) {
        return ofMap(val, minVal, maxVal, w - 10, 10, true);
    }
    // Mapea 'val' al eje vertical del FBO.
    float mapValueH(float val) {
        return ofMap(val, minVal, maxVal, h - 10, 10, true);
    }
public:
    Plotter();

    // Inicializa el FBO y rellena el historial con ruido Perlin para
    // evitar el transitorio inicial del EMA cuando llegan datos reales.
    // Parámetros:
    //   _x, _y      — posición superior izquierda en pantalla
    //   _w, _h      — dimensiones del FBO en píxeles
    //   _maxHistory — número máximo de muestras (default 100)
    void setup(float _x, float _y, float _w, float _h, int _maxHistory = 100);

    // Setters
    // Mueve el plotter a (_x, _y) sin reasignar el FBO.
    void setPosition(float _x, float _y) {
        x = _x;
        y = _y;
    }

    // Ajusta el factor de suavizado EMA (se clampea a [0, 1]).
    // 0 = señal cruda, 1 = señal plana (sin variación visible).
    void setSuavizado(float s){
        suavizado = max(0.0f, min(1.0f, s));
    }

    // Cambia la dirección de dibujo vertical.
    // false (default): muestra más reciente en la parte inferior.
    // true           : muestra más reciente en la parte superior.
    void setDireccion(bool b){
        direccionDibujo = b;
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

    // Cambia el número máximo de muestras en el historial.
    // Si el historial actual es mayor, descarta las muestras más antiguas.
    void setMaxHistory(int _maxHistory) {
        maxHistory = _maxHistory;
        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }
        needsUpdate = true;
    }

    // Cambia el color de la línea del plotter.
    void setColor(ofColor color) {
        plotColor = color;
        needsUpdate = true;
    }

    // Cambia el grosor de la línea en píxeles.
    void setLineWidth(float width) {
        lineWidth = width;
        needsUpdate = true;
    }

    // Agrega una nueva muestra al historial.
    // Si se supera maxHistory, descarta la muestra más antigua.
    void addValue(float value) {
        dataHistory.push_back(value);

        // Mantiene el tamaño del historial
        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }

        needsUpdate = true;
    }

    // Agrega un vector de muestras en lote al historial.
    void addValues(const vector<float>& values) {
        for (float val : values) {
            dataHistory.push_back(val);
        }

        while (dataHistory.size() > maxHistory) {
            dataHistory.erase(dataHistory.begin());
        }

        needsUpdate = true;
    }

    // Vacía el historial y marca el FBO para redibujar.
    void clear() {
        dataHistory.clear();
        needsUpdate = true;
    }

    // Redibuha el FBO si needsUpdate==true; es llamado automáticamente por draw().
    void update();

    // Actualiza el FBO si es necesario y lo vuelca en la posición configurada.
    void draw() {
        update();
        fbo.draw(x, y);
    }

    // Igual que draw() pero permite sobreescribir la posición en pantalla.
    void draw(float _x, float _y) {
        update();
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
