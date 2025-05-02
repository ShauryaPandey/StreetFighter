#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <gl/GLU.h>
#include "SOIL.h"
#include <assert.h>
#include <string.h>
#include "Object.h"

/// <summary>
// Typedefs and function pointers
/// </summary>
typedef void (*PlayStartScreenSoundCB)();
PlayStartScreenSoundCB g_startScreenCB;
typedef void (*StopStartScreenSoundCB)();
StopStartScreenSoundCB g_stopStartScreenCB;
/// <summary>
/// Static data for file
/// </summary>
static const char BACKGROUND_PAGE[] = "asset/background.png";
static const char PORTRAITS_PAGE[] = "asset/portraits.png";
static const char STARTSCREEN_PAGE[] = "asset/streetfighterintro.png";
static const char STARTBUTTON_PAGE[] = "asset/startButton.png";
static const char FONTS_PAGE[] = "asset/fonts2.png";
static const char SHEN_LONG_PAGE[] = "asset/ShengLong.png";
static GLuint _backgroundTexture = 0;
static GLuint _portraitsTexture = 0;
static GLuint _startscreenTexture = 0;
static GLuint _startButtonTexture = 0;
static GLuint _fontsTexture = 0;
static GLuint _shenTexture = 0;
static Bounds2D backgroundDimension = { {8,16},{775,240} };
static uint32_t backgroundImgWidth = 912;
static uint32_t backgroundImgHeight = 440;
static uint32_t portraitsImgWidth = 632;
static uint32_t portraitsImgHeight = 512;
static uint32_t startScreenImgHeight = 689;
static uint32_t startScreenImgWidth = 1182;
static Bounds2D kenImgpixs = { {80,428},{143,451} };
static Bounds2D KenCoords = { {800,100},{900,150} };
static Bounds2D ryuImgPixs = { {9,428},{71,451} };
static Bounds2D RyuCoords = { {200,100},{300,150} };
static Bounds2D titlePixs = { {752,470},{1009,566} };
static Bounds2D titleCoords = { {362,50},{662,350} };
static uint32_t startButtonImgHeight = 85;
static uint32_t startButtonImgWidth = 554;
static Bounds2D startButtonPixs = { {0,0},{553,84} };
static Bounds2D startButtonCoords = { {300,350},{800,400} };


/// <summary>
/// all textures relevant to background are loaded here.
/// </summary>
void backgroundInitTexture()
{
    if (_backgroundTexture == 0)
    {
        _backgroundTexture = SOIL_load_OGL_texture(BACKGROUND_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_backgroundTexture != 0);

    }
    if (_portraitsTexture == 0)
    {
        _portraitsTexture = SOIL_load_OGL_texture(PORTRAITS_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_portraitsTexture != 0);
    }
    if (_startscreenTexture == 0)
    {
        _startscreenTexture = SOIL_load_OGL_texture(STARTSCREEN_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_startscreenTexture != 0);
    }
    if (_startButtonTexture == 0)
    {
        _startButtonTexture = SOIL_load_OGL_texture(STARTBUTTON_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_startButtonTexture != 0);
    }
    if (_fontsTexture == 0)
    {
        _fontsTexture = SOIL_load_OGL_texture(FONTS_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_fontsTexture != 0);
    }
    if (_shenTexture == 0)
    {
        _shenTexture = SOIL_load_OGL_texture(SHEN_LONG_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        assert(_shenTexture != 0);
    }
}

/// <summary>
/// Render the background
/// </summary>
void backgroundDraw()
{
    assert(_backgroundTexture != 0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDisable(0x809D); //For GL_Multisample
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_TRIANGLE_STRIP);
    {
        //put v from 0 to 1
        //Calculate bounding box - entire screen
        GLfloat xPositionLeft = 0;
        GLfloat xPositionRight = 1024; //hardcoded values 
        GLfloat yPositionTop = 0;
        GLfloat yPositionBottom = 448;

        float startingU = 0;
        float endingU = 0;
        float startingV = 0;
        float endingV = 0;

        startingU = backgroundDimension.topLeft.x / (float)backgroundImgWidth;
        endingU = startingU + (xPositionRight / 2.0f) / (float)backgroundImgWidth;
        //The division by 2 is done with the idea that the screen size is increased in multiple of the original sprite and only a limited portion of the background should be visible at a point 
        // so that I can later implement a scrollable background the way it is in the OG game.
        startingV = 1 - (backgroundDimension.botRight.y) / (float)backgroundImgHeight;
        endingV = 1 - (backgroundDimension.topLeft.y) / (float)backgroundImgHeight;

        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, _portraitsTexture); //The names of the characters being displayed below their health bar
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_TRIANGLE_STRIP);
    {
        float startingU = 0;
        float endingU = 0;
        float startingV = 0;
        float endingV = 0;

        startingU = kenImgpixs.topLeft.x / (float)portraitsImgWidth;
        endingU = kenImgpixs.botRight.x / (float)portraitsImgWidth;
        startingV = 1 - (float)kenImgpixs.botRight.y / (float)portraitsImgHeight;
        endingV = 1 - (float)kenImgpixs.topLeft.y / (float)portraitsImgHeight;
        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

        glTexCoord2f(startingU, endingV);
        glVertex3f(KenCoords.topLeft.x, KenCoords.topLeft.y, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(KenCoords.topLeft.x, KenCoords.botRight.y, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(KenCoords.botRight.x, KenCoords.topLeft.y, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(KenCoords.botRight.x, KenCoords.botRight.y, BG_DEPTH);

        startingU = ryuImgPixs.topLeft.x / (float)portraitsImgWidth;
        endingU = ryuImgPixs.botRight.x / (float)portraitsImgWidth;
        startingV = 1 - (float)ryuImgPixs.botRight.y / (float)portraitsImgHeight;
        endingV = 1 - (float)ryuImgPixs.topLeft.y / (float)portraitsImgHeight;

        glTexCoord2f(startingU, endingV);
        glVertex3f(RyuCoords.topLeft.x, RyuCoords.topLeft.y, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(RyuCoords.topLeft.x, RyuCoords.botRight.y, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(RyuCoords.botRight.x, RyuCoords.topLeft.y, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(RyuCoords.botRight.x, RyuCoords.botRight.y, BG_DEPTH);
    }
    glEnd();

}
//The following code is written but its not being used, idea is to implement animation on the screen in the future. 

/// <summary>
/// In Street fighter, background moves to adjust the 2 players in the same screen space.
/// </summary>
typedef struct startScreenFrame_t
{
    Bounds2D screenCoordinates;
    Bounds2D frame;
    uint32_t frameTime;
}StartScreenFrame;
/// <summary>
/// Animations for the start screen.
/// </summary>
typedef struct startScreenAnimation_t
{
    StartScreenFrame* frames;

}StartScreenAnimation;

void InitStartScreenAnimation()
{

}

/// <summary>
/// Puts a word on the screen using letters images from a sprite sheet.
/// </summary>
/// <param name="str"></param>
/// <param name="len"></param>
/// <param name="startX"></param>
/// <param name="startY"></param>
/// <param name="letterSize"></param>
void PrintWord(char* str, int len, float startX, float startY, float letterSize);

/// <summary>
/// draw function for the start screen. This is drawn as the game starts from where the user can press space bar to start game
/// </summary>
void StartScreenDraw()
{
    assert(_startscreenTexture != 0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, _startscreenTexture);
    glBegin(GL_TRIANGLE_STRIP);
    {
        //put v from 0 to 1
        GLfloat xPositionLeft = titleCoords.topLeft.x;
        GLfloat xPositionRight = titleCoords.botRight.x;
        GLfloat yPositionTop = titleCoords.topLeft.y;
        GLfloat yPositionBottom = titleCoords.botRight.y;

        float startingU = 0;
        float endingU = 0;
        float startingV = 0;
        float endingV = 0;

        startingU = titlePixs.topLeft.x / (float)startScreenImgWidth;
        endingU = titlePixs.botRight.x / (float)startScreenImgWidth;

        startingV = 1 - (titlePixs.botRight.y) / (float)startScreenImgHeight;
        endingV = 1 - (titlePixs.topLeft.y) / (float)startScreenImgHeight;

        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, _startButtonTexture); //This is drawing the image which has the prompt for use to press space to continue
    glBegin(GL_TRIANGLE_STRIP);
    {
        //put v from 0 to 1
        GLfloat xPositionLeft = startButtonCoords.topLeft.x;
        GLfloat xPositionRight = startButtonCoords.botRight.x;
        GLfloat yPositionTop = startButtonCoords.topLeft.y;
        GLfloat yPositionBottom = startButtonCoords.botRight.y;

        float startingU = 0;
        float endingU = 0;
        float startingV = 0;
        float endingV = 0;

        startingU = startButtonPixs.topLeft.x / (float)startButtonImgWidth;
        endingU = startButtonPixs.botRight.x / (float)startButtonImgWidth;

        startingV = 1 - (startButtonPixs.botRight.y) / (float)startButtonImgHeight;
        endingV = 1 - (startButtonPixs.topLeft.y) / (float)startButtonImgHeight;

        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();

}

/// <summary>
/// Displaying images at the end declaring who the winner is.
/// So this displays 2 images and a text. The displayed images can vary based on if Ryu won or Ken won.
/// </summary>
/// <param name="victorName"></param>
void endScreenShow(char* victorName)
{
    const int rows = 5;
    const int cols = 6;
    const float width = 632;
    const float height = 512;
    Bounds2D victorCoords = { {10,10},{200,200} };
    Bounds2D loserCoords = { {300,10},{490,200} };
    Bounds2D messageCoords = { {150,300},{450,450} };

    int rowid = -1;
    int colid = -1;

    float startingU = 0;
    float endingU = 0;
    float startingV = 0;
    float endingV = 0;

    GLfloat xPositionLeft;
    GLfloat xPositionRight;
    GLfloat yPositionTop;
    GLfloat yPositionBottom;
    //If victor is Ryu
    if (strcmp(victorName, "Ryu") == 0)
    {

        rowid = 0;
        colid = 0;

        startingU = colid * (float)width / (float)cols;
        endingU = startingU + (float)width / (float)cols;
        startingU /= width;
        endingU /= width;
        startingV = height - (((rowid + 1) * (float)height / (float)rows));
        endingV = height - (rowid * (float)height / (float)rows);
        startingV /= height;

        endingV /= height;

        xPositionLeft = victorCoords.topLeft.x;
        xPositionRight = victorCoords.botRight.x;
        yPositionTop = victorCoords.topLeft.y;
        yPositionBottom = victorCoords.botRight.y;

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _portraitsTexture);
        glBegin(GL_TRIANGLE_STRIP);
        {
            const float BG_DEPTH = -0.90f;
            glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
            // Top Left
            glTexCoord2f(startingU, endingV);
            glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

            // Bottom Left
            glTexCoord2f(startingU, startingV);
            glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

            // Top Right
            glTexCoord2f(endingU, endingV);
            glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

            // Bottom Right
            glTexCoord2f(endingU, startingV);
            glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
        }
        glEnd();

        rowid = 2;
        colid = 1;

        startingU = colid * (float)width / (float)cols;
        endingU = startingU + (float)width / (float)cols;
        startingU /= width;
        endingU /= width;
        startingV = height - (((rowid + 1) * (float)height / (float)rows));
        endingV = height - (rowid * (float)height / (float)rows);
        startingV /= height;

        endingV /= height;

        xPositionLeft = loserCoords.topLeft.x;
        xPositionRight = loserCoords.botRight.x;
        yPositionTop = loserCoords.topLeft.y;
        yPositionBottom = loserCoords.botRight.y;

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _portraitsTexture);
        glBegin(GL_TRIANGLE_STRIP);
        {
            const float BG_DEPTH = -0.90f;
            glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
            // Top Left
            glTexCoord2f(startingU, endingV);
            glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

            // Bottom Left
            glTexCoord2f(startingU, startingV);
            glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

            // Top Right
            glTexCoord2f(endingU, endingV);
            glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

            // Bottom Right
            glTexCoord2f(endingU, startingV);
            glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
        }
        glEnd();
    }

    //If victor is Ken
    if (strcmp("Ken", victorName) == 0)
    {

        rowid = 0;
        colid = 1;

        startingU = colid * (float)width / (float)cols;
        endingU = startingU + (float)width / (float)cols;
        startingU /= width;
        endingU /= width;
        startingV = height - (((rowid + 1) * (float)height / (float)rows));
        endingV = height - (rowid * (float)height / (float)rows);
        startingV /= height;

        endingV /= height;

        xPositionLeft = victorCoords.topLeft.x;
        xPositionRight = victorCoords.botRight.x;
        yPositionTop = victorCoords.topLeft.y;
        yPositionBottom = victorCoords.botRight.y;

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _portraitsTexture);
        glBegin(GL_TRIANGLE_STRIP);
        {
            const float BG_DEPTH = -0.90f;
            glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
            // Top Left
            glTexCoord2f(startingU, endingV);
            glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

            // Bottom Left
            glTexCoord2f(startingU, startingV);
            glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

            // Top Right
            glTexCoord2f(endingU, endingV);
            glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

            // Bottom Right
            glTexCoord2f(endingU, startingV);
            glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
        }
        glEnd();

        rowid = 2;
        colid = 0;

        startingU = colid * (float)width / (float)cols;
        endingU = startingU + (float)width / (float)cols;
        startingU /= width;
        endingU /= width;
        startingV = height - (((rowid + 1) * (float)height / (float)rows));
        endingV = height - (rowid * (float)height / (float)rows)/*(startingV + (float)height / (float)cols)*/;
        startingV /= height;
        //	startingV = 1 - startingV;
        endingV /= height;

        xPositionLeft = loserCoords.topLeft.x;
        xPositionRight = loserCoords.botRight.x;
        yPositionTop = loserCoords.topLeft.y;
        yPositionBottom = loserCoords.botRight.y;

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _portraitsTexture);
        glBegin(GL_TRIANGLE_STRIP);
        {
            const float BG_DEPTH = -0.90f;
            glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
            // Top Left
            glTexCoord2f(startingU, endingV);
            glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

            // Bottom Left
            glTexCoord2f(startingU, startingV);
            glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

            // Top Right
            glTexCoord2f(endingU, endingV);
            glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

            // Bottom Right
            glTexCoord2f(endingU, startingV);
            glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
        }
        glEnd();
    }

    //Display message Shen - just some trash talk message text
    startingU = 0;
    endingU = 1;
    startingV = 0;
    endingV = 1;

    xPositionLeft = messageCoords.topLeft.x;
    xPositionRight = messageCoords.botRight.x;
    yPositionTop = messageCoords.topLeft.y;
    yPositionBottom = messageCoords.botRight.y;

    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, _shenTexture);
    glBegin(GL_TRIANGLE_STRIP);
    {
        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();
    //Elif victor is Ken
}

/// <summary>
/// The round number is rendered - like round0, round1...round75...roundn
/// </summary>
/// <param name="roundNumber"></param>
void RoundDisplayDraw(uint32_t roundNumber)
{
    char dest[10] = "round";
    char numStr[10];
    sprintf(numStr, "%d", roundNumber);
    strcat(dest, numStr);
    PrintWord(dest, 6, 350, 200, 50);
}

void printLetter(char letter, float x, float y, float size);

/// <summary>
/// Print a word using the sprite sheet
/// This function can not print sentences, only words since the sprite sheet doesn't have something for space.
/// We would be requiring a different implementation for printing sentences.
/// </summary>
/// <param name="str"></param>
/// <param name="len"></param>
/// <param name="startX"></param>
/// <param name="startY"></param>
/// <param name="letterSize"></param>
void PrintWord(char* str, int len, float startX, float startY, float letterSize)
{
    assert(str != NULL);
    for (int i = 0; i < len && str[i] != '\0'; ++i)
    {
        printLetter(str[i], startX + letterSize * i + letterSize / 2.0f, startY + letterSize / 2.0f, letterSize);
    }
}

/// <summary>
/// Using the sprite sheet that contains letters in a grid like fashion,
/// we can extract out the coordinates of the requested letter and render that on screen.
/// </summary>
/// <param name="letter"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="size"></param>
void printLetter(char letter, float x, float y, float size)
{
    const int rows = 7;
    const int cols = 6;
    const float width = 95.0f;
    const float height = 112.0f;
    int rowid = -1; //the letters are arranged in a way tht ew can use the correct row nd cols to isolate tgose images
    int colid = -1;
    if (letter >= '0' && letter <= '9')
    {
        letter -= '0';
        letter += 1;
        letter += 'z'; //the numbers in the img start after x,y,z hence this logic
    }

    letter -= 'a'; //to get the index
    rowid = letter / cols;
    colid = letter - rowid * cols;
    assert(colid < cols);
    assert(rowid < rows);
    assert(colid > -1); //The image is very symmetrical so we can use combination of rows and cols to isolate the correct letter/number
    assert(rowid > -1);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, _fontsTexture);
    glBegin(GL_TRIANGLE_STRIP);
    {
        GLfloat xPositionLeft = x - size / 2.0f;
        GLfloat xPositionRight = x + size / 2.0f;
        GLfloat yPositionTop = y - size / 2.0f;
        GLfloat yPositionBottom = y + size / 2.0f;

        float startingU = colid * (float)width / (float)cols;

        float endingU = startingU + (float)width / (float)cols;
        startingU /= width;
        endingU /= width;
        float startingV = height - (((rowid + 1) * (float)height / (float)rows) - 1.0f);
        float endingV = height - (rowid * (float)height / (float)rows);
        startingV /= height;
        //	startingV = 1 - startingV;
        endingV /= height;

        const float BG_DEPTH = -0.90f;
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
        // Top Left
        glTexCoord2f(startingU, endingV);
        glVertex3f(xPositionLeft, yPositionTop, BG_DEPTH);

        // Bottom Left
        glTexCoord2f(startingU, startingV);
        glVertex3f(xPositionLeft, yPositionBottom, BG_DEPTH);

        // Top Right
        glTexCoord2f(endingU, endingV);
        glVertex3f(xPositionRight, yPositionTop, BG_DEPTH);

        // Bottom Right
        glTexCoord2f(endingU, startingV);
        glVertex3f(xPositionRight, yPositionBottom, BG_DEPTH);
    }
    glEnd();
}

