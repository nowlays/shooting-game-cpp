```cpp
/* =============================================================================
IDRIS YOUSFI

README:


Specification


Game description:

    The player controls a spaceship that can move horizontally
    and shoot projectiles using the keyboard. Positions use
    complex numbers. Projectiles are fired vertically.
    The spaceship is placed at the bottom of the screen and moves with the keyboard.
    Enemies are placed at the top of the screen and are static.
    We check if a projectile collides with an enemy.
    An enemy is represented by a rectangle, projectiles by a circle.
    We use Complex numbers and particles.



Improvements:

    - different ships (faster shooting, multiple projectiles at once).
    - a score counting destroyed enemies (100 points each, win at 3000 points — not implemented yet).
    - enemies that shoot projectiles to make the player lose.
    - moving enemies.
    - enemies with health points (not killed in one hit).


------------------------- Tasks -------------------------


- InitGame - done

- MoveShip - done

- ShootProjectile - done

- MoveProjectile - done

- UpdateEnemy - not done

- Update - done

- Collision - done

- Draw - done / better visuals



------------------------- Week 1 - April 10 -------------------------

Added InitGame procedure and main game structures.
Created MoveShip procedure.

------------------------- Week 2 - April 17 -------------------------

Added ShootProjectile and MoveProjectile procedures.
Started Draw procedure using images for ship and enemies.


============================================================================= */

#include<ctime>
#include<iostream>
#include<Grapic.h>
using namespace grapic;
using namespace std;


struct Complex
{
    float x;
    float y;
};


Complex make_complex(float x, float y)
{
    Complex z;
    z.x = x;
    z.y = y;
    return z;
}

Complex make_complex_exp(float r, float theta_deg)
{
    Complex z;
    float theta = (theta_deg * M_PI)/180.0;
    z.x = r * cos(theta);
    z.y = r * sin(theta);
    return z;
}

Complex operator+(Complex a, Complex b)
{
    Complex z;
    z.x = a.x + b.x;
    z.y = a.y + b.y;
    return z;
}

Complex translate(Complex a, Complex b)
{
    Complex z;
    z = a + b;
    return z;
}

Complex operator-(Complex a, Complex b)
{
    Complex z;
    z.x = a.x - b.x;
    z.y = a.y - b.y;
    return z;
}

Complex operator*(float lambda, Complex b)
{
    Complex z;
    z.x = lambda * b.x;
    z.y = lambda * b.y;
    return z;
}

Complex operator*(Complex b, float lambda)
{
    return operator*(lambda,b);
}

Complex scale(Complex p, float cx, float cy, float lambda)
{
    Complex c = make_complex(cx,cy);
    return (p-c)*lambda+c;
}

Complex operator*(Complex a, Complex b)
{
    Complex z;
    z.x = a.x*b.x - a.y*b.y;
    z.y = a.x*b.y + a.y*b.x;
    return z;
}


struct Color
{
    int r;
    int g;
    int b;
};

Color operator+(Color a,Color b)
{
    Color z;
    z.r = a.r + b.r;
    z.g = a.g + b.g;
    z.b = a.b + b.b;
    return z;
}

Color operator*(float lambda,Color b)
{
    Color z;
    z.r = lambda * b.r;
    z.g = lambda * b.g;
    z.b = lambda * b.b;
    return z;
}

Color operator*(Color b,float lambda)
{
    return operator*(lambda,b);
}

Color make_color(int r,int g,int b)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}

float distance(Complex V)
{
    return sqrt(V.x*V.x + V.y*V.y);
}

float distance(Complex A, Complex B)
{
    return distance(B-A);
}


// ------------------------- Constants-Initialization -------------------------


const int DIMW = 600; // window size

const int NB_PROJECTILE = 300;
const int NB_ENNEMI = 30;
const int PROJECTILE_VITESSE = 5;

const int ENEMY_TAILLE = 40; // enemy hitbox size
const int ENEMY_RAYON = 20; // enemy radius for collision
const int PROJECTILE_RAYON = 5; // projectile hitbox radius


// ------------------------- Images -------------------------


Image imgShip;
Image imgEnemy;
Image imgBg;

// ------------------------- Structures -------------------------


struct Vaisseau
{
    Complex pos;
};

struct Projectile
{
    Complex pos;
    Complex vit;
    bool actif;
};

struct Ennemi
{
    Complex pos;
    bool vivant;
};

struct Jeu
{
    Vaisseau vaisseau;
    Projectile projectile[NB_PROJECTILE];
    Ennemi ennemi[NB_ENNEMI];
    int nb_projectile;
    int nb_ennemi;
};


//------------------------- Main-Program -------------------------

void InitJeu(Jeu &J, int nb_projectile, int nb_ennemi)
{
    imgShip = image("/Absolute path/images/ship2.png");
    imgEnemy = image("/Absolute path/images/ennemy.png");
    imgBg = image("/Absolute path/images/stars.jpg");

    int i;
    J.vaisseau.pos = make_complex(DIMW/2, 80); // place the ship at the bottom-center
    J.nb_projectile = nb_projectile;
    J.nb_ennemi = nb_ennemi;
    for(i = 0; i < nb_projectile; i++)
    {
        J.projectile[i].actif = false;
        J.projectile[i].pos = make_complex(J.vaisseau.pos.x,J.vaisseau.pos.y);
        J.projectile[i].vit = make_complex(0, -PROJECTILE_VITESSE); // shoots upwards
    }
    for(i = 0; i < nb_ennemi; i++)
    {
        // Calculate grid position:
        // row: integer division allows 10 enemies per row (e.g., 1/10=0, 11/10=1)
        int ligne = i / 10; 
        // column: modulo wraps the value back to 0 every 10 enemies (e.g., 9%10=9, 10%10=0)
        int colonne = i % 10; 
        
        J.ennemi[i].vivant = true; // all enemies are alive at the start
        // Set position: spaced by 60 pixels, starting from the top of the window
        J.ennemi[i].pos = make_complex(20 + colonne*60, DIMW-60 - ligne *60);  
    }   
}

void MouvementVaisseau(Jeu &J) // Horizontal movement of the ship
{
    if(isKeyPressed(SDLK_LEFT))
    {
        J.vaisseau.pos.x -= 5;
    }
    if(isKeyPressed(SDLK_RIGHT))
    {
        J.vaisseau.pos.x += 5;
    }
}


void TirerProjectile(Jeu &J)
{
    if (isKeyPressed(SDLK_SPACE))
    {
        bool tire = false;

        for (int i = 0; (i < J.nb_projectile) && (tire == false); i++)
        {
            if (!J.projectile[i].actif)
            {
                J.projectile[i].pos = J.vaisseau.pos;
                J.projectile[i].vit = make_complex(0, 5);
                J.projectile[i].actif = true;
                tire = true; // block after 1 shot to avoid spawning all at once
            }
        }
    }
}

void MouvementProjectile(Jeu &J)
{
    for(int i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif)
        {
            J.projectile[i].pos = J.projectile[i].pos + J.projectile[i].vit;
            // if the projectile goes out of the window, deactivate it
            if((J.projectile[i].pos.y < 0)||(J.projectile[i].pos.y > DIMW)) 
            {
                J.projectile[i].actif = false;
            }    
        }
    }
}


/*
void Draw(Jeu &J) // with no image
{
    int i;
    color(255,255,255); // ship color
    rectangleFill(J.vaisseau.pos.x - 10, J.vaisseau.pos.y - 5, J.vaisseau.pos.x + 10, J.vaisseau.pos.y + 5);
    
    color(241, 196, 15); // projectile color
    for(i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }

    color(208, 53, 59); // enemy color
    for(i = 0; i < J.nb_ennemi; i++)
    {
        if(J.ennemi[i].vivant)
        {
            rectangleFill(J.ennemi[i].pos.x - 10, J.ennemi[i].pos.y - 5, J.ennemi[i].pos.x + 10, J.ennemi[i].pos.y + 5);
        }
    }
}
*/

void Draw(Jeu &J)
{
    int i;
    image_draw(imgBg, 0, 0);
    const int SHIP_TAILLE = 40;
    image_draw(imgShip, J.vaisseau.pos.x - SHIP_TAILLE / 2, J.vaisseau.pos.y - SHIP_TAILLE / 2);
    color(0, 200, 0);
    for (i = 0; i < J.nb_projectile; i++)
    {
        if (J.projectile[i].actif)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }
    const int ENEMY_TAILLE = 40;
    for (i = 0; i < J.nb_ennemi; i++)
    {
        if (J.ennemi[i].vivant)
        {
            image_draw(imgEnemy, J.ennemi[i].pos.x - ENEMY_TAILLE / 2, J.ennemi[i].pos.y - ENEMY_TAILLE / 2);
        }
    }
}


void Collision(Jeu &J)
{
    for (int i = 0; i < J.nb_projectile; i++)
    {
        if (J.projectile[i].actif)
        {
            for (int j = 0; j < J.nb_ennemi; j++)
            {
                if (J.ennemi[j].vivant)
                {
                    float d = distance(
                        J.projectile[i].pos,
                        J.ennemi[j].pos
                    );

                    if (d < PROJECTILE_RAYON + ENEMY_RAYON)
                    {
                        J.projectile[i].actif = false;
                        J.ennemi[j].vivant = false;
                    }
                }
            }
        }
    }
}

void CollisionVaisseau(Jeu &J) // bug
{
    if (J.vaisseau.pos.x <= 0)
    {
        J.vaisseau.pos.x = 0;
    }
    if (J.vaisseau.pos.x >= DIMW)
    {
        float delta = J.vaisseau.pos.x - DIMW;
        J.vaisseau.pos.x = DIMW + delta;
    }
}

void update(Jeu &J)
{
    MouvementVaisseau(J);
    TirerProjectile(J);
    MouvementProjectile(J);
    Collision(J);
}

int main(int, char **)
{
    winInit("Shoot'em up", DIMW, DIMW);
    bool stop = false;
    setKeyRepeatMode(true); // enable continuous ship movement when key is held
    Jeu jeu;
    backgroundColor(6, 50, 108);
    InitJeu(jeu, 150, 20);
    while (!stop)
    {
        winClear();
        update(jeu);
        Draw(jeu);
        delay(30);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}```
