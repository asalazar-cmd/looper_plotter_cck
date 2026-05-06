#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "plotter.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    ofImage* images1 = nullptr;
    int numImages1 = 0;

    ofTrueTypeFont tipoMv;
    int tamTipoMv =18;
    float posTxtMvX =500, posTxtMvY =500, gapTxtMv=50;
    Plotter plotter1;

    float h, w;
    float scaleX = 1.0;
    float scaleY = 1.0;
    float posX = 118 ;
    float posY = 105 ;
    float gap = 104;//distancia entre imgs
    float posXPlot = 118 ;
    float posYPlot = 105 ;
    float gapPlot = 104;//distancia entre imgs

    float suavizado =0;
    float sizeXPlot = 700.0;
    float sizeYPlot =  900.0;
    bool direccionPlot = true ;
    // --- Flags de rotación 90° por elemento (se persisten en XML) ---
    // rotarPlot   : rota el plotter alrededor de su centro
    // rotarVideo  : rota el frame de imagen alrededor de su centro
    // rotarMascara: rota la imagen de máscara alrededor del centro de pantalla
    // rotarTexto  : aplica la rotación 270° al bloque de texto (comportamiento original)
    bool rotarPlot    = false ;
    bool rotarVideo   = false ;
    bool rotarMascara = false ;
    bool rotarTexto   = true  ;
    int historiaPlot = 2000 ;

    ofTrueTypeFont tipo;
    int tamTipo =14;
    int posTxtX=10, posTxtY=10, gapTxt;
    int videoSelect=0;
    //config direcccion;
    float velD1=0.001; //velocidad para cambios de direccion
    float umbral1=0.5;
    float velAcc1=0.005;
    float vel1=0.001;
    int bias1= 42;
    int amp1=30;
    float peso1=0.5;

    ofImage mascara;
    int currentFrameIndex1;
    int frameRate;
    int frameDuration1;
    uint64_t lastFrameTime1;
    int direction1;
    ofTrueTypeFont myfont;

    bool showDebug = false;
    void freeImages(ofImage*& images, int numImages);

    void loadImagesFromFolder(const string& folderPath, ofImage*& images, int& numImages){
        ofDirectory dir(folderPath);
        dir.allowExt("png");
        dir.allowExt("jpeg");
        dir.allowExt("jpg");
        dir.listDir();
        dir.sort();

        numImages = dir.size();
        images = new ofImage[numImages]; // Asignar memoria para el array

        for (int i = 0; i < numImages; i++) {
           
            images[i].load(dir.getPath(i));
            ofLog() << dir.getPath(i) << endl;
        }

    }

    void updateFrame(int& currentFrameIndex, int totalFrames, int& direction) {
        currentFrameIndex += direction;

        if (currentFrameIndex >= totalFrames) {
            currentFrameIndex = 0; // Loop
        } else if (currentFrameIndex < 0) {
            currentFrameIndex = totalFrames - 1; // Reversa
        }
    }


    void saveSettingsToXML(const string& filename) {
        ofxXmlSettings xml;

        // Crear un nodo raíz llamado "settings"
        xml.addTag("settings");
        xml.pushTag("settings");

        // Agregar los valores como etiquetas
        xml.addValue("tamTipoMv", tamTipoMv);
        xml.addValue("posTxtMvX", posTxtMvX);
        xml.addValue("posTxtMvY", posTxtMvY);
        xml.addValue("gapTxtMv", gapTxtMv);

        xml.addValue("tamTipo", tamTipo);
        xml.addValue("posTxtX", posTxtX);
        xml.addValue("posTxtY", posTxtY);
        xml.addValue("gapTxt", gapTxt);

        xml.addValue("scaleX", scaleX);
        xml.addValue("scaleY", scaleY);
        xml.addValue("posX", posX);
        xml.addValue("posY", posY);
        xml.addValue("gap", gap);

        xml.addValue("suavizado", suavizado);
        xml.addValue("posXPlot", posXPlot);
        xml.addValue("posYPlot", posYPlot);
        xml.addValue("gapPlot", gapPlot);
        xml.addValue("sizeXPlot", sizeXPlot);
        xml.addValue("sizeYPlot", sizeYPlot);
        xml.addValue("direccionPlot", direccionPlot);
        xml.addValue("rotarPlot",     rotarPlot);
        xml.addValue("rotarVideo",    rotarVideo);
        xml.addValue("rotarMascara",  rotarMascara);
        xml.addValue("rotarTexto",    rotarTexto);
        xml.addValue("historiaPlot", historiaPlot);

        xml.addValue("height", h);
        xml.addValue("width", w);

        //vel 1
        xml.addValue("vel1", vel1);
        xml.addValue("velAcc1", velAcc1);
        xml.addValue("bias1", bias1);
        xml.addValue("amp1", amp1);
        xml.addValue("peso1", peso1);
        xml.addValue("velD1",velD1);
        xml.addValue("umbral1", umbral1);

        // Guardar el archivo XML
        if (xml.saveFile(filename)) {
            ofLogNotice() << "Settings saved to " << filename;
        } else {
            ofLogError() << "Failed to save settings to " << filename;
        }
    }

    void loadSettingsFromXML(const string& filename) {
        ofxXmlSettings xml;

        // Cargar el archivo XML
        if (xml.loadFile(filename)) {
            // Moverse al nodo raíz "settings"
            if (xml.tagExists("settings")) {
                xml.pushTag("settings");
                tamTipo= xml.getValue("tamTipo", 14);
                gapTxt= xml.getValue("gapTxt", 150);
                posTxtX= xml.getValue("posTxtX", 32);
                posTxtY= xml.getValue("posTxtY", 32);

                tamTipoMv= xml.getValue("tamTipoMv", 18);
                gapTxtMv= xml.getValue("gapTxtMv", 450);
                posTxtMvX= xml.getValue("posTxtMvX", 320);
                posTxtMvY= xml.getValue("posTxtMvY", 320);

                // Leer los valores y asignarlos a las variables globales
                scaleX = xml.getValue("scaleX", 1.0);
                scaleY = xml.getValue("scaleY", 1.0);
                posX = xml.getValue("posX", 118.0);
                posY = xml.getValue("posY", 105.0);
                gap = xml.getValue("gap", 104.0);
                h = xml.getValue("height", 0.0);
                w = xml.getValue("width", 0.0);


                suavizado = xml.getValue("suavizado", 0.0);
                posXPlot = xml.getValue("posXPlot", 118.0);
                posYPlot = xml.getValue("posYPlot", 105.0);
                gapPlot = xml.getValue("gapPlot", 104.0);
                sizeXPlot = xml.getValue("sizeXPlot", 700.0);
                sizeYPlot = xml.getValue("sizeYPlot", 900.0);
                direccionPlot = xml.getValue("direccionPlot", true);
                rotarPlot     = xml.getValue("rotarPlot",    false);
                rotarVideo    = xml.getValue("rotarVideo",   false);
                rotarMascara  = xml.getValue("rotarMascara", false);
                rotarTexto    = xml.getValue("rotarTexto",   true);
                historiaPlot = xml.getValue("historiaPlot", 2000);


                // Lectura para vel 1
                vel1 = xml.getValue("vel1",0.001);
                bias1 = xml.getValue("bias1",42);
                amp1 = xml.getValue("amp1",30);
                peso1 = xml.getValue("peso1",0.5);
                velD1 = xml.getValue("velD1",0.001);
                umbral1 = xml.getValue("umbral1",0.5);
                xml.popTag(); // Salir del nodo "settings"

                ofLogNotice() << "Settings loaded from " << filename;
            } else {
                ofLogError() << "Invalid XML structure in " << filename;
            }
        } else {
            ofLogError() << "Failed to load settings from " << filename;
        }
    }

    void autorun(){

        float td1 =ofGetElapsedTimeMillis()*velD1;
        direction1= (ofNoise(td1)>=umbral1) ?1:-1;

        float t1 =ofGetElapsedTimeMillis()*(vel1+ofNoise(ofGetElapsedTimeMillis()*velAcc1));

        frameDuration1 = ((sin(t1 )*0.5+0.5)*peso1 + (1.0-peso1)*ofNoise(t1))*amp1+bias1;
    }

};
