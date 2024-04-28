#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include <map>
#include <iterator>
using namespace std;
using namespace sf;
float collisionThinkness = 2;
const int noCollisions = 150;
const int noCoins = 676;
int pagenum = -1;
int index = 0;

struct Player {
    Sprite playerSprite;
    int a, s, d, w , pressed;
    FloatRect rect;   
    float move_x, move_y;
    void sp(Texture& sprite) {
        a = 1;
        d = 5;
        s = 7;
        w = 3;
        pressed = 0;
        move_x = 0;
        move_y = 0;
        playerSprite.setTexture(sprite);
        playerSprite.setTextureRect(IntRect(0 , 0, 26, 26));
        playerSprite.setScale(1.35, 1.35);
    }
    void update(){
        rect.left += move_x;
        rect.top += move_y;
        

        if (rect.left < -10) {
            rect.left = 640;
        }
        if (rect.left > 640) {
            rect.left = -10 ;
        }
        
        if (move_x > 0) {
            if (pressed == 5) {
                d++;
                if (d > 6){
                    d = 5;
                }
                pressed = 0;
            }
            playerSprite.setTextureRect(IntRect(d * 26 , 0, 26, 26));
            pressed++;
        }
        else if (move_x < 0) {
            if (pressed == 5) {
                a++;
                if (a > 2) {
                    a = 1;
                }
                pressed = 0;
            }
            playerSprite.setTextureRect(IntRect(a * 26, 0, 26, 26));
            pressed++;
        }
        else if (move_y < 0) {
            if (pressed == 5) {
                w++;
                if (w > 4) {
                    w = 3;
                }
                pressed = 0;
            }
            playerSprite.setTextureRect(IntRect(w * 26, 0, 26, 26));
            pressed++;
        }
        else if (move_y > 0) {
            if (pressed == 5) {
                s++;
                if (s > 8) {
                    s = 7;
                }
                pressed = 0;
            }
            playerSprite.setTextureRect(IntRect(s * 26, 0, 26, 26));
            pressed++;
        }
        
        playerSprite.setPosition(rect.left, rect.top);
        
    }
};
/*
    < page route >
    -1  :: menu page
    0   :: gameplay page
    1   :: instrections page
    2   :: exit
*/
bool isEmptyPlace(int xIndex , int yIndex);
void drawPoints(Sprite points[] , Texture& point);
void drawCollision2(RectangleShape block, Player &player , char dr);
void drawCollision(RectangleShape &block, float X, float Y, float X_position, float Y_position);
void mapCollision(RectangleShape blocks[] , map <int, char>& map);
void gamePlayPage(RenderWindow& window);
int main() {
    RenderWindow window(VideoMode(672, 695), "PAC MAN");
    Menu menu(672, 695);
    window.setFramerateLimit(60);
    
    while (true) {
        while (window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed) {
                    window.close();
                    break;
                }
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Up) {
                        menu.Move_up();
                    }
                    if (event.key.code == Keyboard::Down) {
                        menu.Move_down();
                    }
                    if (event.key.code == Keyboard::Return) {
                        if (menu.pressed() == 0) {
                            pagenum = 0;
                        }
                        if (menu.pressed() == 1) {
                            pagenum = 1;
                        }
                        if (menu.pressed() == 2) {
                            pagenum = 2;
                        }
                    }
                }
            }
            window.clear();
            if (pagenum != -1) {
                break;
            }
            menu.draw(window);
            window.display();         
        }
        if (pagenum == 2) {
            window.close();
        }
        if (pagenum == 0) {
            gamePlayPage(window);
        }
    }
    
    return 0;
}
void gamePlayPage(RenderWindow& window) {
    bool isColl = true;
    Texture tx;
    tx.loadFromFile("spritesheet.png"); 
    Player player;
    player.sp(tx);
    player.rect.left = 315;
    player.rect.top = 650;

    Texture sPo , bPo;
    sPo.loadFromFile("big-1.png");
    bPo.loadFromFile("big-0.png");
    Sprite points[noCoins];
    
    Texture back;
    back.loadFromFile("map.png");
    Sprite background;
    background.setTexture(back);
    background.setScale(1.5, 1.4);

    RectangleShape blocks[noCollisions];
    map<int, char> blockMap;
    mapCollision(blocks , blockMap);
    drawPoints(points, sPo);
 
    while (window.isOpen()) {
        
        Event event;
        
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) 
                window.close();                    
        }
        if (Mouse::isButtonPressed(Mouse::Left)) {
            Vector2i pos = Mouse::getPosition(window);
            cout << pos.x << " " << pos.y << endl;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) {
            player.move_x = -2;
            player.move_y = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::D)) {
            player.move_x = 2;
            player.move_y = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::W)) {
            player.move_y = -2;
            player.move_x = 0;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
            player.move_y = 2;
            player.move_x = 0;
        }
        player.update();
        
        for (int i = 0; i < noCollisions; i++)
        {  
                if (player.playerSprite.getGlobalBounds().intersects(blocks[i].getGlobalBounds())) {
                    
                    drawCollision2(blocks[i], player , blockMap[i]);
                    player.move_x = 0;
                    player.move_y = 0;   
                }
        }
        

        window.clear();
        window.draw(background);
        window.draw(player.playerSprite);
        for (int i = 0; i < noCoins; i++)
        {
            window.draw(points[i]);
            if (player.playerSprite.getGlobalBounds().intersects(points[i].getGlobalBounds())) {
                points[i].setScale(0, 0);
            }
        }
        /**/
            for (int i = 0; i < noCollisions; i++)
            {
                //window.draw(blocks[i]);
            }
        
        
        window.display();
    }
}
void drawPoints(Sprite points[], Texture& point) {
    for (int i = 0, xIndex = 0, yIndex = 0, xpos, ypos; i < noCoins; i++, xIndex++)
    {
        if (i < 312) {
            if (xIndex == 26) {
                xIndex = 0;
                yIndex++;
            }
            if (isEmptyPlace(xIndex, yIndex)) {
                xpos = xIndex * 24 + 25;
                ypos = yIndex * 23 + 25;
                points[i].setTexture(point);
                points[i].setPosition(float(xpos), float(ypos));
            }  
        }
        else if (i >= 312 && i < 442){
            if (xIndex == 26) {
                xIndex = 0;
                yIndex++;
            }
            if (isEmptyPlace(xIndex, yIndex)) {
                xpos = xIndex * 24 + 25;
                ypos = yIndex * 24 + 27;
                points[i].setTexture(point);
                points[i].setPosition(float(xpos), float(ypos)); 
            }
        }
        else if (i >= 442 && i < 546) {
            if (xIndex == 26) {
                xIndex = 0;
                yIndex++;
            }
            if (isEmptyPlace(xIndex, yIndex)) {
                xpos = xIndex * 24 + 25;
                ypos = yIndex * 23 + 60;
                points[i].setTexture(point);
                points[i].setPosition(float(xpos), float(ypos));
            }
        }
        else if (i >= 546 && i < 572) {
            if (xIndex == 26) {
                xIndex = 0;
                yIndex++;
            }
            if (isEmptyPlace(xIndex, yIndex)) {
                xpos = xIndex * 24 + 25;
                ypos = yIndex * 23 + 70;
                points[i].setTexture(point);
                points[i].setPosition(float(xpos), float(ypos));
            }
        }
        else {
            if (xIndex == 26) {
                xIndex = 0;
                yIndex++;
            }
            if (isEmptyPlace(xIndex, yIndex)) {
                xpos = xIndex * 24 + 25;
                ypos = yIndex * 23 + 78;
                points[i].setTexture(point);
                points[i].setPosition(float(xpos), float(ypos));
            }
        }
           
    }
}
void drawCollision(RectangleShape& block, float X, float Y, float X_position, float Y_position) {
    block.setSize(Vector2f(X, Y));
    block.setPosition(X_position, Y_position);
}
void drawCollision2(RectangleShape block, Player &player , char dr) {
    
           if (block.getSize().x == collisionThinkness) {
                if (dr == 'r')
                    player.rect.left = block.getPosition().x - 39;
                    
                else if (dr == 'l')
                    player.rect.left = block.getPosition().x + 5;
           }
           else if (block.getSize().y == collisionThinkness) {
                if (dr == 'r')
                    player.rect.top = block.getPosition().y - 39;
                else if (dr == 'l')
                    player.rect.top = block.getPosition().y + 5;
           }
           
    
}
void mapCollision(RectangleShape blocks[] , map <int, char>& map) {

    drawCollision(blocks[0], collisionThinkness, 74, 540, 490);
    map[0] = 'r';
    drawCollision(blocks[1], 10, collisionThinkness, 547, 568);
    map[1] = 'l';
    drawCollision(blocks[2], collisionThinkness, 54, 561, 510);
    map[2] = 'l';
    drawCollision(blocks[3], 32, collisionThinkness, 570, 502);
    map[3] = 'l';
    drawCollision(blocks[4], collisionThinkness, 10, 609, 489);
    map[4] = 'l';
    drawCollision(blocks[5], 55, collisionThinkness, 548, 483);
    map[5] = 'r';
    drawCollision(blocks[6], 53, collisionThinkness, 548, 57);
    map[6] = 'r';
    drawCollision(blocks[7], collisionThinkness,28, 540, 63);
    map[7] = 'r';
    drawCollision(blocks[8], 53, collisionThinkness, 548, 98);
    map[8] = 'l';
    drawCollision(blocks[9], collisionThinkness,28, 608, 65);
    map[9] = 'l';
    drawCollision(blocks[10], 55, collisionThinkness, 548, 146);
    map[10] = 'r';
    drawCollision(blocks[11], collisionThinkness,10, 540, 153);
    map[11] = 'r';
    drawCollision(blocks[12], 55, collisionThinkness, 548, 166);
    map[12] = 'l';
    drawCollision(blocks[13], collisionThinkness,10, 608, 153);
    map[13] = 'l';
    drawCollision(blocks[14], collisionThinkness,192, 660, 15);
    map[14] = 'r';
    drawCollision(blocks[15], 300,collisionThinkness, 353, 10);
    map[15] = 'l';
    drawCollision(blocks[16], 300,collisionThinkness, 18, 10);
    map[16] = 'l';
    drawCollision(blocks[17],collisionThinkness,192, 10, 15);
    map[17] = 'l';
    drawCollision(blocks[18],104,collisionThinkness, 18, 213);
    map[18] = 'r';
    drawCollision(blocks[19],collisionThinkness,75 ,130, 219);
    map[19] = 'l';
    drawCollision(blocks[20],110,collisionThinkness ,10, 300);
    map[20] = 'l';
    drawCollision(blocks[21],110,collisionThinkness ,10, 346);
    map[21] = 'r';
    drawCollision(blocks[22],collisionThinkness , 75 ,130, 352);
    map[22] = 'l';
    drawCollision(blocks[23],104,collisionThinkness ,18, 436);
    map[23] = 'l';
    drawCollision(blocks[24], collisionThinkness, 100, 10, 442);
    map[24] = 'l';
    drawCollision(blocks[25], 32,collisionThinkness,18, 548);
    map[25] = 'r';
    drawCollision(blocks[26], collisionThinkness, 10, 56, 554);
    map[26] = 'l';
    drawCollision(blocks[27], 32, collisionThinkness, 18, 570);
    map[27] = 'l';
    drawCollision(blocks[28], collisionThinkness, 100, 10, 576);
    map[28] = 'l';
    drawCollision(blocks[29], 638 ,collisionThinkness, 18, 680);
    map[29] = 'r';
    drawCollision(blocks[30], collisionThinkness, 100, 660, 576);
    map[30] = 'r';
    drawCollision(blocks[31], 32, collisionThinkness, 621, 570);
    map[31] = 'l';
    drawCollision(blocks[32], collisionThinkness, 10, 614, 554);
    map[32] = 'r';
    drawCollision(blocks[33], 32, collisionThinkness, 620, 548);
    map[33] = 'r';
    drawCollision(blocks[34], collisionThinkness, 100, 660, 442);
    map[34] = 'r';
    drawCollision(blocks[35], 104, collisionThinkness, 550, 436);
    map[35] = 'l';
    drawCollision(blocks[36], collisionThinkness, 75, 541, 352);
    map[36] = 'r';
    drawCollision(blocks[37], 110, collisionThinkness, 550, 346);
    map[37] = 'r';
    drawCollision(blocks[38], 110, collisionThinkness, 550, 300);
    map[38] = 'l';
    drawCollision(blocks[39], collisionThinkness, 75, 541, 219);
    map[39] = 'r';
    drawCollision(blocks[40], 104, collisionThinkness, 550, 213);
    map[40] = 'r';
    drawCollision(blocks[41], collisionThinkness, 75, 346, 15);
    map[41] = 'l';
    drawCollision(blocks[42], 10 ,collisionThinkness, 330, 98);
    map[42] = 'l';
    drawCollision(blocks[43], collisionThinkness, 75, 324, 15);
    map[43] = 'r';
    drawCollision(blocks[44], 80, collisionThinkness, 403, 57);
    map[44] = 'r';
    drawCollision(blocks[45], collisionThinkness, 28, 397, 63);
    map[45] = 'r';
    drawCollision(blocks[46], 80, collisionThinkness, 403, 98);
    map[46] = 'l';
    drawCollision(blocks[47], collisionThinkness, 28, 490, 65);
    map[47] = 'l';
    drawCollision(blocks[48], 80, collisionThinkness, 187, 57);
    map[48] = 'r';
    drawCollision(blocks[49], collisionThinkness, 28, 181, 63);
    map[49] = 'r';
    drawCollision(blocks[50], 80, collisionThinkness, 187, 98);
    map[50] = 'l';
    drawCollision(blocks[51], collisionThinkness, 28, 274, 65);
    map[51] = 'l';
    drawCollision(blocks[52], 55, collisionThinkness, 68, 57);
    map[52] = 'r';
    drawCollision(blocks[53], collisionThinkness, 28, 61, 63);
    map[53] = 'r';
    drawCollision(blocks[54], 55, collisionThinkness, 68, 98);
    map[54] = 'l';
    drawCollision(blocks[55], collisionThinkness, 28, 129, 65);
    map[55] = 'l';
    drawCollision(blocks[56], 55, collisionThinkness, 68, 146);
    map[56] = 'r';
    drawCollision(blocks[57], collisionThinkness, 10, 60, 153);
    map[57] = 'r';
    drawCollision(blocks[58], 55, collisionThinkness, 68, 166);
    map[58] = 'l';
    drawCollision(blocks[59], collisionThinkness, 10, 129, 153);
    map[59] = 'l';
    drawCollision(blocks[60], 152, collisionThinkness, 260, 146);
    map[60] = 'r';
    drawCollision(blocks[61], collisionThinkness, 10, 253, 153);
    map[61] = 'r';
    drawCollision(blocks[62], collisionThinkness, 10, 417, 153);
    map[62] = 'l';
    drawCollision(blocks[63], 58, collisionThinkness, 260, 166);
    map[63] = 'l';
    drawCollision(blocks[64], 58, collisionThinkness, 355, 166);
    map[64] = 'l';
    drawCollision(blocks[65],  collisionThinkness, 54,325, 175);
    map[65] = 'r';
    drawCollision(blocks[66], collisionThinkness, 54, 345, 175);
    map[66] = 'l';
    drawCollision(blocks[67], 10,collisionThinkness,  331, 232);
    map[67] = 'l';
    drawCollision(blocks[68], 152, collisionThinkness, 260, 414);
    map[68] = 'r';
    drawCollision(blocks[69], collisionThinkness, 10, 253, 420);
    map[69] = 'r';
    drawCollision(blocks[70], collisionThinkness, 10, 417, 420);
    map[70] = 'l';
    drawCollision(blocks[71], 58, collisionThinkness, 260, 433);
    map[71] = 'l';
    drawCollision(blocks[72], 58, collisionThinkness, 355, 433);
    map[72] = 'l';
    drawCollision(blocks[73], collisionThinkness, 54, 325, 442);
    map[73] = 'r';
    drawCollision(blocks[74], collisionThinkness, 54, 345, 442);
    map[74] = 'l';
    drawCollision(blocks[75], 10, collisionThinkness, 331, 501);
    map[75] = 'l';
    drawCollision(blocks[76], 152, collisionThinkness, 260, 549);
    map[76] = 'r';
    drawCollision(blocks[77], collisionThinkness, 10, 253, 555);
    map[77] = 'r';
    drawCollision(blocks[78], collisionThinkness, 10, 417, 555);
    map[78] = 'l';
    drawCollision(blocks[79], 58, collisionThinkness, 260, 568);
    map[79] = 'l';
    drawCollision(blocks[80], 58, collisionThinkness, 355, 568);
    map[80] = 'l';
    drawCollision(blocks[81], collisionThinkness, 54, 325, 575);
    map[81] = 'r';
    drawCollision(blocks[82], collisionThinkness, 54, 345, 575);
    map[82] = 'l';
    drawCollision(blocks[83], 10, collisionThinkness, 331, 636);
    map[83] = 'l';
    drawCollision(blocks[84], 80, collisionThinkness, 404, 482);
    map[84] = 'r';
    drawCollision(blocks[85], collisionThinkness, 10, 396, 489);
    map[85] = 'r';
    drawCollision(blocks[86], 80, collisionThinkness, 404, 502);
    map[86] = 'l';
    drawCollision(blocks[87], collisionThinkness, 10, 489, 489);
    map[87] = 'l';
    drawCollision(blocks[88], 80, collisionThinkness, 187, 482);
    map[88] = 'r';
    drawCollision(blocks[89], collisionThinkness, 10, 181, 489);
    map[89] = 'r';
    drawCollision(blocks[90], 80, collisionThinkness, 187, 502);
    map[90] = 'l';
    drawCollision(blocks[91], collisionThinkness, 10, 273, 489);
    map[91] = 'l';
    drawCollision(blocks[92],55, collisionThinkness, 69, 482);
    map[92] = 'r';
    drawCollision(blocks[93], collisionThinkness, 74, 130, 489);
    map[93] = 'l';
    drawCollision(blocks[94], 10, collisionThinkness, 116, 568);
    map[94] = 'l';
    drawCollision(blocks[95], collisionThinkness, 54, 109, 509);
    map[95] = 'r';
    drawCollision(blocks[96], 32, collisionThinkness, 69, 501);
    map[96] = 'l';
    drawCollision(blocks[97], collisionThinkness, 10, 62, 489);
    map[97] = 'r';
    drawCollision(blocks[98], 104, collisionThinkness, 69, 616);
    map[98] = 'r';
    drawCollision(blocks[99], collisionThinkness, 10, 62, 623);
    map[99] = 'r';
    drawCollision(blocks[100], 198, collisionThinkness, 69, 637);
    map[100] = 'l';
    drawCollision(blocks[101], collisionThinkness, 10, 274, 623);
    map[101] = 'l';
    drawCollision(blocks[102], 57, collisionThinkness, 211, 616);
    map[102] = 'r';
    drawCollision(blocks[103], collisionThinkness, 55, 202, 556);
    map[103] = 'l';
    drawCollision(blocks[104], 10, collisionThinkness, 187, 549);
    map[104] = 'r';
    drawCollision(blocks[105], collisionThinkness, 55, 180, 556);
    map[105] = 'r';
    drawCollision(blocks[106], 198, collisionThinkness, 404, 637);
    map[106] = 'l';
    drawCollision(blocks[107], collisionThinkness, 10, 609, 623);
    map[107] = 'l';
    drawCollision(blocks[108], 104, collisionThinkness, 498, 616);
    map[108] = 'r';
    drawCollision(blocks[109], 57, collisionThinkness, 404, 616);
    map[109] = 'r';
    drawCollision(blocks[110], collisionThinkness, 55, 490, 556);
    map[110] = 'l';
    drawCollision(blocks[111], 10, collisionThinkness, 475, 549);
    map[111] = 'r';
    drawCollision(blocks[112], collisionThinkness, 55, 468, 556);
    map[112] = 'r';
    drawCollision(blocks[113], collisionThinkness, 10, 397, 623);
    map[113] = 'r';
    drawCollision(blocks[114], collisionThinkness, 74, 180, 355);
    map[114] = 'r';
    drawCollision(blocks[115], collisionThinkness, 74, 202, 355);
    map[115] = 'l';
    drawCollision(blocks[116], 10, collisionThinkness, 187, 347);
    map[116] = 'r';
    drawCollision(blocks[117], 10, collisionThinkness, 187, 434);
    map[117] = 'l';
    drawCollision(blocks[118], collisionThinkness, 74, 469, 355);
    map[118] = 'r';
    drawCollision(blocks[119], collisionThinkness, 74, 490, 355);
    map[119] = 'l';
    drawCollision(blocks[120], 10, collisionThinkness, 475, 347);
    map[120] = 'r';
    drawCollision(blocks[121], 10, collisionThinkness, 475, 434);
    map[121] = 'l';
    drawCollision(blocks[122], collisionThinkness, 84, 253, 283);
    map[122] = 'r';
    drawCollision(blocks[123], collisionThinkness, 84, 417, 283);
    map[123] = 'l';
    drawCollision(blocks[124], 160, collisionThinkness, 256, 368);
    map[124] = 'l';
    drawCollision(blocks[125], collisionThinkness, 142, 180, 153);
    map[125] = 'r';
    drawCollision(blocks[126], collisionThinkness, 142, 490, 153);
    map[126] = 'l';
    drawCollision(blocks[127], collisionThinkness, 56, 202, 153);
    map[127] = 'l';
    drawCollision(blocks[128], collisionThinkness, 56, 202, 239);
    map[128] = 'l';
    drawCollision(blocks[129], collisionThinkness, 56, 468, 153);
    map[129] = 'r';
    drawCollision(blocks[130], collisionThinkness, 56, 468, 239);
    map[130] = 'r';
    drawCollision(blocks[131], 10, collisionThinkness, 187, 147);
    map[131] = 'r';
    drawCollision(blocks[132], 10, collisionThinkness, 187, 300);
    map[132] = 'l';
    drawCollision(blocks[133], 10, collisionThinkness, 474, 147);
    map[133] = 'r';
    drawCollision(blocks[134], 10, collisionThinkness, 474, 300);
    map[134] = 'l';
    drawCollision(blocks[135], 56 ,collisionThinkness, 209, 213);
    map[135] = 'r';
    drawCollision(blocks[136], 56, collisionThinkness, 209, 232);
    map[136] = 'l';
    drawCollision(blocks[137], 56, collisionThinkness, 403, 213);
    map[137] = 'r';
    drawCollision(blocks[138], 56, collisionThinkness, 403, 232);
    map[138] = 'l';
    drawCollision(blocks[139], collisionThinkness,10, 272, 219);
    map[139] = 'l';
    drawCollision(blocks[140], collisionThinkness,10, 397, 219);
    map[140] = 'r';
    drawCollision(blocks[141], 52, collisionThinkness, 257, 279);
    map[141] = 'r';
    drawCollision(blocks[142], 52, collisionThinkness, 363, 279);
    map[142] = 'r';
}
bool isEmptyPlace(int xIndex , int yIndex) {
    if (xIndex == 12 || xIndex == 13) {
        if (yIndex != 4 && yIndex != 10) {
            return false;
        }
        else
            return true;
    }
    if (yIndex == 1 || yIndex == 2 || yIndex == 3) {
        if (xIndex != 5 && xIndex != 11 && xIndex != 12 && xIndex != 13 && xIndex != 14 && xIndex != 20 && xIndex != 25 && xIndex != 0 )
            return false;
        else
            return true;
    } 
    if (yIndex == 5 || yIndex == 6) {
        if (xIndex != 5 && xIndex != 8 && xIndex != 17 && xIndex != 20  && xIndex != 25 && xIndex != 0 )
            return false;
        else
            return true;

    }if (yIndex == 7) {
        if (xIndex == 6 || xIndex == 7 || xIndex == 18  || xIndex == 19)
            return false;
        else
            return true;
    }
    if (yIndex == 8 || yIndex == 9) {
        if (xIndex != 5 && xIndex != 11 && xIndex != 14 && xIndex != 20 )
            return false;
        else
            return true;
    }if (yIndex == 10 ) {
        if (xIndex == 0 || xIndex == 1 || xIndex == 2 || xIndex == 3 || xIndex == 4 || xIndex == 6 || xIndex == 7 || xIndex == 18 || xIndex == 19 || xIndex == 21|| xIndex == 22 || xIndex == 23 || xIndex == 24 || xIndex == 25 )
            return false;
        else
            return true;
    }if (yIndex == 11) {
        if (xIndex != 5 && xIndex != 8 && xIndex != 17 && xIndex != 20)
            return false;
        else
            return true;
    }if (yIndex == 12) {
        if (xIndex != 9 && xIndex != 10 && xIndex != 11 && xIndex != 14 && xIndex != 15 && xIndex != 16)
            return true;
        else
            return false;
    }if (yIndex == 13 || yIndex == 14 ||yIndex == 16) {
        if (xIndex != 5 && xIndex != 8 && xIndex != 17 && xIndex != 20 )
            return false;
        else
            return true;
    }if (yIndex == 15) {
        if (xIndex != 5 && xIndex != 8 && xIndex != 9 && xIndex != 10 && xIndex != 11 && xIndex != 14 && xIndex != 15 && xIndex != 16 && xIndex != 17 && xIndex != 20)
            return false;
        else
            return true;
    }if (yIndex == 18 || yIndex == 19) {
        if (xIndex != 0 && xIndex != 5 && xIndex != 11  && xIndex != 14 && xIndex != 20 && xIndex != 25 )
            return false;
        else
            return true;
    }if (yIndex == 20) {
        if (xIndex != 3 && xIndex != 4 && xIndex != 21 && xIndex != 22)
            return true;
        else
            return false;
    }if (yIndex == 21) {
        if (xIndex != 2 && xIndex != 5 && xIndex != 8 && xIndex != 23 && xIndex != 20 && xIndex != 17)
            return false;
        else
            return true;
    }if (yIndex == 22) {
        if (xIndex != 6 && xIndex != 7 && xIndex != 18 && xIndex != 19 )
            return true;
        else
            return false;
    }if (yIndex == 23 || yIndex == 24) {
        if (xIndex != 0 && xIndex != 11 && xIndex != 14 && xIndex != 25)
            return false;
        else
            return true;
    }
}