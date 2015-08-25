/********  Test sample for ofxInteractiveObject									********/
/********  Make sure you open your console to see all the events being output	********/


#pragma once

#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x00FF00
#define		DOWN_COLOR		0xFF0000
#define		DETECTED_COLOR  0xFF0000



class jellySequence : public ofxMSAInteractiveObject {
public:
    int posx, posy;
    int saveX, saveY;
    ofImage * sourceVideo;
    ofImage cropVideo;
    float differenceVideo;
    float differenceVideo2;
    int jelly_id;
    int widthRatio;
    int heightRatio;

    int videoQuadW;
    int videoQuadH;

    bool doubleSize;
    bool deleteMe;

    unsigned char savedVideo[1440000];
   // MyImgDragPoint dragTopLeft;

    float getVideoMagniW()
    {
        return videoQuadW/sourceVideo->getWidth();
    }


    float getVideoMagniH()
    {
        return videoQuadH/sourceVideo->getHeight();
    }

    float getVideoRatio()
    {
        return sourceVideo->getWidth()/sourceVideo->getHeight();
    }

    float getWidthRatio()
    {
        return sourceVideo->getWidth()/videoQuadW;
    }

    float getHeightRatio()
    {
        return sourceVideo->getHeight()/videoQuadH;
    }

    void setup() {
        printf("MyTestObject::setup() - hello!\n");
        enableMouseEvents();
        enableKeyEvents();
        cropVideo.allocate(100,100, OF_IMAGE_COLOR_ALPHA);
        //this->width = width*getVideoMagniW();
        //this->height= height*getVideoMagniH());
        //this->set
        doubleSize = false;
        deleteMe = false;
    }


    void exit() {
        printf("MyTestObject::exit() - goodbye!\n");
    }
//800 jadi 640
//
    void update() {

        float cropX = x*getWidthRatio();
        float cropY = y*getHeightRatio();
//cropY -= 70;
        float cropW = width/getVideoMagniW();
        float cropH = height/getVideoMagniH();

         cropVideo.cropFrom(*sourceVideo,cropX,cropY,cropW,cropH);
         cropVideo.setImageType(OF_IMAGE_GRAYSCALE);
           // cropVideo.update();
         float crop_width = cropVideo.getWidth();
         float crop_height = cropVideo.getHeight();

         float channel_crop = cropVideo.getPixelsRef().getBytesPerPixel();
         float arrpos_crop = (cropY*channel_crop*cropVideo.getWidth()) + x*channel_crop;
         unsigned char *pix = cropVideo.getPixels();

         float diff;

         float diff2;

         for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
         {
             diff = diff + (abs(savedVideo[x] - pix[x]));
             diff2 = diff2 + ((savedVideo[x] - pix[x]));

         }

         differenceVideo = diff/(crop_width * crop_height * channel_crop);
         differenceVideo2 = diff2/(crop_width * crop_height * channel_crop);


    }


    void draw() {
        ofPushStyle();

        if(isMousePressed()) ofSetHexColor(DOWN_COLOR);
        else if(isMouseOver()) ofSetHexColor(OVER_COLOR);
        else ofSetHexColor(IDLE_COLOR);

        if (differenceVideo2*10 > 10.0){

            float supersize = differenceVideo2 *10;
            float centersize = supersize/2;

            if (differenceVideo2 *10 > 100)
            {
                supersize = differenceVideo2;
            }else{
                supersize = differenceVideo2 *10;
            }


            centersize = supersize/2;



                    ofSetHexColor(DETECTED_COLOR);
                    ofRect(x-centersize-2, y-centersize-2, (width+supersize)+4, (width+supersize)+4);
                    cropVideo.draw(x-centersize,y-centersize,width+supersize,width+supersize);

        }else{
            ofRect(x-2, y-2, (width)+4, (height)+4);
            cropVideo.draw(x,y,width,height);
        }


//        ofLine(x-2,0,x+2,0);
//        ofDrawAxis(3);
        ofDrawBitmapString("id: " + ofToString(jelly_id) + " diff: " + ofToString(differenceVideo2),x-40,y-20);
        ofPopStyle();


    }

    virtual void onRollOver(int x, int y) {
        printf("MyTestObject::onRollOver(x: %i, y: %i)\n", x, y);
    }

    virtual void onRollOut() {
        printf("MyTestObject::onRollOut()\n");
    }

    virtual void onMouseMove(int x, int y){
        printf("MyTestObject::onMouseMove(x: %i, y: %i)\n", x, y);
    }

    virtual void onDragOver(int x, int y, int button) {
        this->x = x - saveX;    // update x position
        this->y = y - saveY;    // update mouse y position
       // dragTopLeft.updatePos(this->x+width,this->y);

    }

    virtual void onDragOutside(int x, int y, int button) {
        printf("MyTestObject::onDragOutside(x: %i, y: %i, button: %i)\n", x, y, button);
    }

    virtual void onPress(int x, int y, int button) {
        printf("MyTestObject::onPress(x: %i, y: %i, button: %i)\n", x, y, button);
        saveX = x - this->x;
        saveY = y - this->y;
    }

    virtual void onRelease(int x, int y, int button) {
        printf("MyTestObject::onRelease(x: %i, y: %i, button: %i)\n", x, y, button);
        grabFrame();
    }

    virtual void onReleaseOutside(int x, int y, int button) {
        printf("MyTestObject::onReleaseOutside(x: %i, y: %i, button: %i)\n", x, y, button);
    }

    virtual void grabFrame()
    {

        int crop_width = cropVideo.getWidth();
        int crop_height = cropVideo.getHeight();

        int channel_crop = cropVideo.getPixelsRef().getBytesPerPixel();
        int arrpos_crop = (y*channel_crop*cropVideo.getWidth()) + x*channel_crop;

        unsigned char *pix = cropVideo.getPixels();

        for (int x = 0 ; x < crop_width * crop_height * channel_crop; x++)
        {
            savedVideo[x] = pix[x];
        }
    }

    virtual void keyPressed(int key) {
        printf("MyTestObject::keyPressed(key: %i)\n", key);
        if(isMouseOver()){

            if (key == 61){
                this->width += 10;
                this->height += 10;
                grabFrame();

            }

            if (key == 45){
                this->width -= 10;
                this->height -= 10;
                grabFrame();

            }

            if (key == 'c')
            {
              grabFrame();
            }

            if (key == 100)
            {
                deleteMe = true;
                printf("deleteme\n");
            }

//            if (this->width > 100) this->width = 100;
  //          if (this->height > 100) this->height = 100;


    //        if (this->width < 50) this->width = 50;
      //      if (this->height < 50) this->height = 50;


        }

    }

    virtual void keyReleased(int key) {
        printf("MyTestObject::keyReleased(key: %i)\n", key);
    }

};
