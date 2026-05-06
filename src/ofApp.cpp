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
    ofLog()<<   posXPlot<<" "<<gapPlot<<" "<<posYPlot<<" "<<sizeXPlot<<" "<< sizeYPlot<<" "<< historiaPlot<<endl; // x, y, w, h, historial

}

//--------------------------------------------------------------
void ofApp::update(){
    uint64_t currentTime = ofGetElapsedTimeMillis();

    autorun();
    // Cambiar frames según el tiempo y la dirección
    if (currentTime - lastFrameTime1 >= frameDuration1) {
        updateFrame(currentFrameIndex1, numImages1, direction1);
        plotter1.addValue(frameDuration1); // Agrega un valor
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
        mascara.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    h = images1[currentFrameIndex1].getHeight() * scaleY;
    w = images1[currentFrameIndex1].getWidth()  * scaleX;
    // Dibujar las imágenes en la pantalla
    if (images1 != nullptr) {
        images1[currentFrameIndex1].draw(posX, posY, w, h);
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

        mascara.draw(0,0,ofGetWidth(),ofGetHeight());

        ofPushMatrix();
        if(rotarPlot){
            float cx = posXPlot + sizeXPlot * 0.5f;
            float cy = posYPlot + sizeYPlot * 0.5f;
            ofTranslate(cx, cy);
            ofRotateDeg(90);
            ofTranslate(-cx, -cy);
        }
        plotter1.draw();
        ofPopMatrix();

        ofPushMatrix();


        // Rotar 90 grados para dibujar el texto en vertical
        //ofRotateDeg(270);
        ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
        ofRotateDeg(270);
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    bool cambiosEnParametros=false;
    // Toggle debug mode
    if(key == OF_KEY_BACKSPACE || key == OF_KEY_RETURN) {
        showDebug = !showDebug;
        return;
    }

    // Video selection (always available)
    if(key == '0') videoSelect = 0;
    if(key == '1') videoSelect = 1;
  
    // ============================================================
    // DEBUG MODE: Control de imágenes/videos
    // ============================================================
    if(showDebug) {
        // --- Posición de imagen (WASD) ---
        if(key == 'w' || key == 'W') posY--;        // Arriba
        if(key == 's' || key == 'S') posY++;        // Abajo
        if(key == 'a' || key == 'A') posX--;        // Izquierda
        if(key == 'd' || key == 'D') posX++;        // Derecha

        // --- Escala X/Y (Q/E ancho, R/F alto) ---
        if(key == 'q' || key == 'Q') { scaleX -= 0.01; if(scaleX < 0.01) scaleX = 0.01; }
        if(key == 'e' || key == 'E') { scaleX += 0.01; }
        if(key == 'r' || key == 'R') { scaleY += 0.01; }
        if(key == 'f' || key == 'F') { scaleY -= 0.01; if(scaleY < 0.01) scaleY = 0.01; }

        // --- Parámetros de video seleccionado ---
        int bias = 0, amp = 0;
        float vel = 0, peso = 0, velAcc = 0, velD = 0, umbral = 0;

        // Cargar valores según video seleccionado
        if(videoSelect == 1) {
            vel = vel1; bias = bias1; amp = amp1; peso = peso1;
            velD = velD1; umbral = umbral1; velAcc = velAcc1;
        }

        // Modificar parámetros
        if(key == 'z' || key == 'Z') velAcc += 0.005;
        if(key == 'x' || key == 'X') velAcc -= 0.005;

        if(key == 't' || key == 'T') bias++;
        if(key == 'g' || key == 'G') bias--;

        if(key == 'y' || key == 'Y') amp++;
        if(key == 'h' || key == 'H') amp--;

        if(key == 'u' || key == 'U') vel += 0.000005;
        if(key == 'j' || key == 'J') vel -= 0.000005;

        if(key == 'i' || key == 'I') peso += 0.005;
        if(key == 'k' || key == 'K') peso -= 0.005;
        if(peso > 1.0f) peso = 1.0f;
        if(peso < 0.0f) peso = 0.05f;

        if(key == 'o' || key == 'O') velD += 0.000005;
        if(key == 'l' || key == 'L') velD -= 0.000005;

        if(key == 'v' || key == 'V') umbral += 0.005;
        if(key == 'b' || key == 'B') umbral -= 0.005;
        if(umbral > 1.0f) umbral = 1.0f;
        if(umbral < 0.0f) umbral = 0.005f;

        // Guardar valores según video seleccionado
        if(videoSelect == 1) {
            vel1 = vel; bias1 = bias; amp1 = amp; peso1 = peso;
            velD1 = velD; umbral1 = umbral; velAcc1 = velAcc;
        }

        // Log de valores
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

    // ============================================================
    // MODO NORMAL: Control de textos
    // ============================================================
    else {
        // --- TEXTO 1: Posición (Flechas) y Gap (V/B) ---
        if(key == OF_KEY_UP)    posTxtY++;      // Arriba
        if(key == OF_KEY_DOWN)  posTxtY--;      // Abajo
        if(key == OF_KEY_LEFT)  posTxtX--;      // Izquierda
        if(key == OF_KEY_RIGHT) posTxtX++;      // Derecha

        if(key == 'v' || key == 'V') gapTxt--;
        if(key == 'b' || key == 'B') gapTxt++;

        // --- TEXTO Mv: Posición (WASD) y Gap (Q/E) ---
        if(key == 'w' || key == 'W') posTxtMvY--;   // Arriba
        if(key == 's' || key == 'S') posTxtMvY++;   // Abajo
        if(key == 'a' || key == 'A') posTxtMvX--;   // Izquierda
        if(key == 'd' || key == 'D') posTxtMvX++;   // Derecha

        if(key == 'q' || key == 'Q') gapTxtMv--;
        if(key == 'e' || key == 'E') gapTxtMv++;


        // --- Posición de plot (jikl) ---
        if(key == 'j' || key == 'J') posYPlot++;
        if(key == 'l' || key == 'L') posYPlot--;
        if(key == 'i' || key == 'I') posXPlot--;
        if(key == 'k' || key == 'K') posXPlot++;

        // --- Gap de plot (N/M) ---
        if(key == 'n' || key == 'N') gapPlot++;
        if(key == 'm' || key == 'M') gapPlot--;
        
        bool suavizadoCambio = false;
        // --- suavizado de plot (u/o) ---
        if(key == 'u' || key == 'U'){ 
            suavizado+=0.01;
            suavizadoCambio = true;
        }
        if(key == 'o' || key == 'O') { 
            suavizado-=0.01;
            suavizadoCambio = true;
        }
        if(suavizadoCambio){
        suavizado = max(suavizado, 0.0f);
        suavizado = min(suavizado, 1.0f);
        plotter1.setSuavizado(suavizado);
        }
        cambiosEnParametros=true;
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
