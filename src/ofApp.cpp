#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofLog()<<"setup"<<" :"<<"i"<<endl;
    //podria comprimir todo el codigo en arrays

    //   ofSetFrameRate(60);
    ofSetWindowTitle("Video Frames Player");
    // Cargar imágenes desde las carpetas
    loadImagesFromFolder("img1", images1, numImages1);

    // Inicializar variables
    currentFrameIndex1 = 0;
    frameRate = 30; // Framerate deseado
    frameDuration1 = 1000 / frameRate; // Duración de cada frame en ms
    lastFrameTime1 = ofGetElapsedTimeMillis();
    direction1 = 1; // 1 para adelante, -1 para reversa

    myfont.load("arial.ttf", 32);

    mascara.load("data/mascara_hd_rotada.png");
    loadSettingsFromXML("settings.xml");
    tipo.load("future-earth.ttf", tamTipo);
    tipoMv.load("future-earth.ttf", tamTipoMv);
    plotter1.setup(posXPlot, posYPlot, sizeXPlot, sizeYPlot, historiaPlot); // x, y, w, h, historial
    plotter1.setColor(ofColor(0, 255, 0));
    plotter1.setDireccion(direccionPlot);
    plotter1.setLineWidth(2.5);
    plotter1.setSuavizado(suavizado); // Esta linea la agregue-- 14-05-26
    ofLog()<<   posXPlot<<" "<<gapPlot<<" "<<posYPlot<<" "<<sizeXPlot<<" "<< sizeYPlot<<" "<< historiaPlot<<endl; // x, y, w, h, historial

}

//--------------------------------------------------------------
void ofApp::update(){
    uint64_t currentTime = ofGetElapsedTimeMillis();

    autorun();
    // Cambiar frames según el tiempo y la dirección
    if (currentTime - lastFrameTime1 >= frameDuration1) {
        updateFrame(currentFrameIndex1, numImages1, direction1);
        frameDurationSuave = ofLerp(frameDurationSuave, frameDuration1, 0.05);
        plotter1.addValue(frameDurationSuave);
        //plotter1.addValue(frameDuration1); // Agrega un valor -linea original
        lastFrameTime1 = currentTime;
    }

    //actuarlizar el ploter en el timer de cada buffer
    plotter1.setPosition(posXPlot, posYPlot);

    //ofGetElapsedTimeMillis()

    ofHideCursor();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    if(showDebug){
        // máscara en modo debug (respeta rotarMascara)
        if(rotarMascara){
            ofPushMatrix();
            ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f);
            ofRotateDeg(90);
            ofTranslate(-ofGetWidth()*0.5f, -ofGetHeight()*0.5f);
            mascara.draw(0,0,ofGetWidth(),ofGetHeight());
            ofPopMatrix();
        } else {
            mascara.draw(0,0,ofGetWidth(),ofGetHeight());
        }
    }
    h = images1[currentFrameIndex1].getHeight() * scaleY;
    w = images1[currentFrameIndex1].getWidth()  * scaleX;
    // Dibujar las imágenes en la pantalla
    if (images1 != nullptr) {
        if(rotarVideo){
            // Rota el frame 90° alrededor del centro de la imagen
            ofPushMatrix();
            float cx = posX + w * 0.5f;
            float cy = posY + h * 0.5f;
            ofTranslate(cx, cy);
            ofRotateDeg(90);
            ofTranslate(-cx, -cy);
            images1[currentFrameIndex1].draw(posX, posY, w, h);
            ofPopMatrix();
        } else {
            images1[currentFrameIndex1].draw(posX, posY, w, h);
        }
    }


    if(showDebug){
        int  bias =0,amp=0;
        float vel=0,peso=0,  velAcc=0;
        float velD=0, umbral=0;
        if(videoSelect==1){
            vel=vel1, bias=bias1, amp=amp1, peso=peso1, velD=velD1, umbral=umbral1;
            velAcc=velAcc1;

        }

        myfont.drawString("umbral dir(n m): "+to_string(umbral),50.,ofGetHeight()-100.);
        myfont.drawString("vel Dir (o l): "+to_string(velD),50.,ofGetHeight()-150.);
        myfont.drawString("peso (i k): "+to_string(peso),50.,ofGetHeight()-200.);
        myfont.drawString("amp fr(y h): "+to_string(amp),50.,ofGetHeight()-250.);
        myfont.drawString("bias fr(t g): "+to_string(bias),50.,ofGetHeight()-300.);
        myfont.drawString("vel noise(u j): "+to_string(vel),50.,ofGetHeight()-350.);
        myfont.drawString("velAcc(z x): "+to_string(velAcc),50.,ofGetHeight()-400.);


        myfont.drawString(to_string(currentFrameIndex1), posX, posY-15.);


        ofPushStyle();
        ofNoFill();
        ofSetColor(0,255.,0);
        if(videoSelect==1){
            ofDrawRectangle(posX, posY, w, h);
        }
        ofPopStyle();
    }else{

        // máscara en modo normal (respeta rotarMascara)
        if(rotarMascara){
            ofPushMatrix();
            ofTranslate(ofGetWidth()*0.5f, ofGetHeight()*0.5f);
            ofRotateDeg(90);
            ofTranslate(-ofGetWidth()*0.5f, -ofGetHeight()*0.5f);
            mascara.draw(0,0,ofGetWidth(),ofGetHeight());
            ofPopMatrix();
        } else {
            mascara.draw(0,0,ofGetWidth(),ofGetHeight());
        }

        ofPushMatrix();
        if(rotarPlot){
            float cx = posXPlot + sizeXPlot * 0.5f;
            float cy = posYPlot + sizeYPlot * 0.5f;
            // Al rotar 90°, el rect pasa de sizeXPlot×sizeYPlot a sizeYPlot×sizeXPlot en pantalla.
            // Esto desplaza la esquina TL en (sizeXPlot/2 - sizeYPlot/2, sizeYPlot/2 - sizeXPlot/2).
            // La compensación inversa mantiene la TL del plotter en (posXPlot, posYPlot).
            ofTranslate((sizeYPlot - sizeXPlot) * 0.5f, (sizeXPlot - sizeYPlot) * 0.5f);
            ofTranslate(cx, cy);
            ofRotateDeg(90);
            ofTranslate(-cx, -cy);
        }
        plotter1.draw();
        ofPopMatrix();

        ofPushMatrix();
        // rotarTexto cicla 4 pasos de 90°: 0=0°, 1=90°, 2=180°, 3=270°
        if(rotarTexto != 0){
            ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
            ofRotateDeg(rotarTexto * 90);
        }
        // Dibujar el texto en vertical
        float txt1 = float(currentFrameIndex1)/float(numImages1-1);
        // myfont.drawString(ofToString(txt1, 2), posTxtX, posTxtY);
        tipo.drawString(ofToString(txt1, 2), posTxtX, posTxtY);

        float txtMv1 =ofNoise(txt1*100.)*0.45f;
        tipo.drawString(ofToString(txtMv1, 4), posTxtMvX, posTxtMvY);




        // Restaurar la matriz original
        ofPopMatrix();
        //ofRotateDeg(90);

    }
    /*if(showDebug){
        myfont.drawString(to_string(frameDuration3), 3*204.,ofGetHeight()/2-100.);
        myfont.drawString(to_string(frameDuration2), 2.*204,ofGetHeight()/2.-100);
        myfont.drawString(to_string(frameDuration1), 204 ,ofGetHeight()/2.-100);

        myfont.drawString(to_string(currentFrameIndex3), 3*204.,ofGetHeight()/2.);
        myfont.drawString(to_string(currentFrameIndex2), 2.*204,ofGetHeight()/2.);
        myfont.drawString(to_string(currentFrameIndex1), 204 ,ofGetHeight()/2.);
    }else {


    }*/
}

// ============================================================
// keyPressed — Tabla de modificadores de teclado
// ============================================================
//
//  SIEMPRE DISPONIBLE (cualquier modo)
//  ────────────────────────────────────────────────────────────
//  BACKSPACE / ENTER  │ Alterna modo debug / modo normal
//  0                  │ Selecciona video 0
//  1                  │ Selecciona video 1
//  2                  │ Toggle rotarVideo   (+90° al frame de imagen)
//  3                  │ Toggle rotarMascara (+90° a la máscara)
//  4                  │ Cicla rotarTexto    0°→90°→180°→270°→0°...
//  5                  │ Toggle rotarPlot    (+90° al plotter)
//
//  MODO DEBUG  (showDebug = true)
//  ────────────────────────────────────────────────────────────
//  W / S              │ Mueve imagen: arriba / abajo     (posY ± 1)
//  A / D              │ Mueve imagen: izquierda / derecha (posX ± 1)
//  Q / E              │ Escala X imagen: -0.01 / +0.01  (scaleX)
//  R / F              │ Escala Y imagen: +0.01 / -0.01  (scaleY)
//  Z / X              │ Aceleración de velocidad: +0.005 / -0.005 (velAcc)
//  T / G              │ Bias de frames: +1 / -1          (bias)
//  Y / H              │ Amplitud de frames: +1 / -1      (amp)
//  U / J              │ Velocidad de ruido: +5e-6 / -5e-6 (vel)
//  I / K              │ Peso interpolación: +0.005 / -0.005 (peso, rango 0–1)
//  O / L              │ Velocidad de dirección: +5e-6 / -5e-6 (velD)
//  V / B              │ Umbral de dirección: +0.005 / -0.005 (umbral, rango 0–1)
//
//  MODO NORMAL (showDebug = false)
//  ────────────────────────────────────────────────────────────
//  ↑ ↓ ← →           │ Mueve texto1: ±1 en Y / ±1 en X  (posTxtX, posTxtY)
//  V / B              │ Gap texto1: -1 / +1               (gapTxt)
//  W / S              │ Mueve textoMv: arriba / abajo     (posTxtMvY ± 1)
//  A / D              │ Mueve textoMv: izquierda / derecha (posTxtMvX ± 1)
//  Q / E              │ Gap textoMv: -1 / +1              (gapTxtMv)
//  I / K              │ Mueve plotter: izquierda / derecha (posXPlot ± 1)
//  J / L              │ Mueve plotter: abajo / arriba     (posYPlot ± 1)
//  N / M              │ Gap plotter: +1 / -1              (gapPlot)
//  U / O              │ Suavizado EMA: +0.001 / -0.001    (suavizado, rango 0–1)
//  R / F              │ Ancho plotter: +5 / -5            (sizeXPlot, mín 50)
//  T / G              │ Alto plotter: +5 / -5             (sizeYPlot, mín 50)
//  Y / H              │ Historial plotter: +5 / -5        (historiaPlot, mín 4)
// ============================================================
void ofApp::keyPressed(int key){

    bool cambiosEnParametros=false;

    // ── SIEMPRE DISPONIBLE ───────────────────────────────────
    // BACKSPACE / ENTER: alterna entre modo debug y modo normal
    if(key == OF_KEY_BACKSPACE || key == OF_KEY_RETURN) {
        showDebug = !showDebug;
        return;
    }

    // Selección de video activo
    if(key == '0') videoSelect = 0;
    if(key == '1') videoSelect = 1;

    // ── ROTACIONES (cualquier modo, se guardan en XML) ───────
    // 2 → rotarVideo   : rota el frame de imagen 90°
    // 3 → rotarMascara : rota la máscara 90°
    // 4 → rotarTexto   : cicla 0°→90°→180°→270°→0°...
    // 5 → rotarPlot    : rota el plotter 90°
    if(key == '2') { rotarVideo   = !rotarVideo;           cambiosEnParametros = true; }
    if(key == '3') { rotarMascara = !rotarMascara;          cambiosEnParametros = true; }
    if(key == '4') { rotarTexto   = (rotarTexto + 1) % 4;  cambiosEnParametros = true; }
    if(key == '5') { rotarPlot    = !rotarPlot;             cambiosEnParametros = true; }
  
    // ── MODO DEBUG: control de imagen y parámetros de video ──
    if(showDebug) {
        // Posición de la imagen (WASD, paso 1 px)
        if(key == 'w' || key == 'W') posY--;    // Arriba
        if(key == 's' || key == 'S') posY++;    // Abajo
        if(key == 'a' || key == 'A') posX--;    // Izquierda
        if(key == 'd' || key == 'D') posX++;    // Derecha

        // Escala de la imagen (Q/E: ancho, R/F: alto)
        if(key == 'q' || key == 'Q') { scaleX -= 0.01; if(scaleX < 0.01) scaleX = 0.01; }
        if(key == 'e' || key == 'E') { scaleX += 0.01; }
        if(key == 'r' || key == 'R') { scaleY += 0.01; }
        if(key == 'f' || key == 'F') { scaleY -= 0.01; if(scaleY < 0.01) scaleY = 0.01; }

        // Copiar parámetros del video seleccionado a variables locales para edición
        int bias = 0, amp = 0;
        float vel = 0, peso = 0, velAcc = 0, velD = 0, umbral = 0;

        if(videoSelect == 1) {
            vel = vel1; bias = bias1; amp = amp1; peso = peso1;
            velD = velD1; umbral = umbral1; velAcc = velAcc1;
        }

        // Aceleración de velocidad de ruido (Z/X, paso ±0.005)
        if(key == 'z' || key == 'Z') velAcc += 0.005;
        if(key == 'x' || key == 'X') velAcc -= 0.005;

        // Bias del frame (T/G, paso ±1): desplaza la posición base del frame
        if(key == 't' || key == 'T') bias++;
        if(key == 'g' || key == 'G') bias--;

        // Amplitud del frame (Y/H, paso ±1): rango de variación de frameDuration
        if(key == 'y' || key == 'Y') amp++;
        if(key == 'h' || key == 'H') amp--;

        // Velocidad del ruido Perlin (U/J, paso ±5e-6): controla la frecuencia temporal
        if(key == 'u' || key == 'U') vel += 0.000005;
        if(key == 'j' || key == 'J') vel -= 0.000005;

        // Peso de interpolación (I/K, paso ±0.005, rango 0–1): mezcla sin/noise
        if(key == 'i' || key == 'I') peso += 0.005;
        if(key == 'k' || key == 'K') peso -= 0.005;
        if(peso > 1.0f) peso = 1.0f;
        if(peso < 0.0f) peso = 0.05f;

        // Velocidad de cambio de dirección (O/L, paso ±5e-6)
        if(key == 'o' || key == 'O') velD += 0.000005;
        if(key == 'l' || key == 'L') velD -= 0.000005;

        // Umbral de inversión de dirección (V/B, paso ±0.005, rango 0–1)
        if(key == 'v' || key == 'V') umbral += 0.005;
        if(key == 'b' || key == 'B') umbral -= 0.005;
        if(umbral > 1.0f) umbral = 1.0f;
        if(umbral < 0.0f) umbral = 0.005f;

        // Guardar valores editados de vuelta al video seleccionado
        if(videoSelect == 1) {
            vel1 = vel; bias1 = bias; amp1 = amp; peso1 = peso;
            velD1 = velD; umbral1 = umbral; velAcc1 = velAcc;
        }

        // Log de estado en modo debug
        ofLog() << "=== DEBUG MODE ===" << endl;
        ofLog() << "posX: " << posX << " | posY: " << posY << endl;
        ofLog() << "gap: " << gap << " | scaleX: " << scaleX << " | scaleY: " << scaleY << endl;
        ofLog() << "w: " << w << " | h: " << h << endl;
        ofLog() << "videoSelect: " << videoSelect << endl;
        ofLog() << "bias: " << bias << " | amp: " << amp << endl;
        ofLog() << "vel: " << vel << " | peso: " << peso << endl;
        ofLog() << "velD: " << velD << " | umbral: " << umbral << endl;
        ofLog() << "velAcc: " << velAcc << endl;

        cambiosEnParametros=true;
    }

    // ── MODO NORMAL: control de textos y plotter ─────────────
    else {
        // Posición texto1 (flechas, paso 1 px)
        if(key == OF_KEY_UP)    posTxtY++;   // Arriba
        if(key == OF_KEY_DOWN)  posTxtY--;   // Abajo
        if(key == OF_KEY_LEFT)  posTxtX--;   // Izquierda
        if(key == OF_KEY_RIGHT) posTxtX++;   // Derecha

        // Gap del texto1 (V/B, paso ±1): espacio entre líneas de texto
        if(key == 'v' || key == 'V') gapTxt--;
        if(key == 'b' || key == 'B') gapTxt++;

        // Posición textoMv (WASD, paso 1 px): texto de valor de movimiento
        if(key == 'w' || key == 'W') posTxtMvY--;  // Arriba
        if(key == 's' || key == 'S') posTxtMvY++;  // Abajo
        if(key == 'a' || key == 'A') posTxtMvX--;  // Izquierda
        if(key == 'd' || key == 'D') posTxtMvX++;  // Derecha

        // Gap textoMv (Q/E, paso ±1)
        if(key == 'q' || key == 'Q') gapTxtMv--;
        if(key == 'e' || key == 'E') gapTxtMv++;

        // Posición del plotter (IJKL, paso 1 px)
        // I/K: eje X (izquierda / derecha), J/L: eje Y (abajo / arriba)
        if(key == 'j' || key == 'J') posYPlot++;
        if(key == 'l' || key == 'L') posYPlot--;
        if(key == 'i' || key == 'I') posXPlot--;
        if(key == 'k' || key == 'K') posXPlot++;

        // Gap del plotter (N/M, paso ±1): separación entre plotters múltiples
        if(key == 'n' || key == 'N') gapPlot++;
        if(key == 'm' || key == 'M') gapPlot--;
        
        // Suavizado EMA del plotter (U/O, paso ±0.001, rango 0–1)
        // 0 = sin suavizado, 1 = máximo suavizado (línea plana)
        bool suavizadoCambio = false;
        if(key == 'u' || key == 'U'){ suavizado += 0.001; suavizadoCambio = true; }
        if(key == 'o' || key == 'O'){ suavizado -= 0.001; suavizadoCambio = true; }
        if(suavizadoCambio){
            suavizado = max(suavizado, 0.0f);
            suavizado = min(suavizado, 1.0f);
            plotter1.setSuavizado(suavizado);
        }

        // Ancho del plotter (R/F, paso ±5, mín 50 px)
        if(key == 'r' || key == 'R') { sizeXPlot += 5; plotter1.setSize(sizeXPlot, sizeYPlot); }
        if(key == 'f' || key == 'F') { sizeXPlot -= 5; if(sizeXPlot < 50) sizeXPlot = 50; plotter1.setSize(sizeXPlot, sizeYPlot); }

        // Alto del plotter (T/G, paso ±5, mín 50 px)
        if(key == 't' || key == 'T') { sizeYPlot += 5; plotter1.setSize(sizeXPlot, sizeYPlot); }
        if(key == 'g' || key == 'G') { sizeYPlot -= 5; if(sizeYPlot < 50) sizeYPlot = 50; plotter1.setSize(sizeXPlot, sizeYPlot); }

        // Tamaño del historial del plotter (Y/H, paso ±5, mín 4 muestras)
        if(key == 'y' || key == 'Y') { historiaPlot += 5; plotter1.setMaxHistory(historiaPlot); }
        if(key == 'h' || key == 'H') { historiaPlot -= 5; if(historiaPlot < 4) historiaPlot = 4; plotter1.setMaxHistory(historiaPlot); }
    }
    if(cambiosEnParametros)saveSettingsToXML("settings.xml");
}
void ofApp::freeImages(ofImage*& images, int numImages) {
    if (images != nullptr) {
        delete[] images; // Liberar memoria
        images = nullptr;
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
    // Liberar memoria al salir
    freeImages(images1, numImages1);
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
