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

- Update - not done

- Collision - not done

- Draw - partially done, missing images / better visuals



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
    return a + b;
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


// ------------------------- Game constants -------------------------

const int DIMW = 600;

const int NB_PROJECTILE = 300;
const int NB_ENNEMI = 30;
const int PROJECTILE_VITESSE = 5;


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


// ------------------------- Game initialization -------------------------

void InitJeu(Jeu &J, int nb_projectile, int nb_ennemi)
{
    imgShip = image("/home/idris/cours/S2/lifami/grapic-24.01.29/data/images/ship2.png");
    imgEnemy = image("/home/idris/cours/S2/lifami/grapic-24.01.29/data/images/ennemy.png");
    imgBg = image("/home/idris/cours/S2/lifami/grapic-24.01.29/data/images/stars.jpg");

    J.vaisseau.pos = make_complex(DIMW/2, 80);
    J.nb_projectile = nb_projectile;
    J.nb_ennemi = nb_ennemi;

    for(int i = 0; i < nb_projectile; i++)
    {
        J.projectile[i].actif = false;
        J.projectile[i].pos = J.vaisseau.pos;
        J.projectile[i].vit = make_complex(0, -PROJECTILE_VITESSE);
    }

    for(int i = 0; i < nb_ennemi; i++)
    {
        int ligne = i / 10;
        int colonne = i % 10;

        J.ennemi[i].vivant = true;
        J.ennemi[i].pos = make_complex(70 + colonne*50, DIMW-70 - ligne*50);
    }
}


// ------------------------- Ship movement -------------------------

void MouvementVaisseau(Jeu &J)
{
    if(isKeyPressed(SDLK_LEFT)) J.vaisseau.pos.x -= 5;
    if(isKeyPressed(SDLK_RIGHT)) J.vaisseau.pos.x += 5;
}


// ------------------------- Shooting -------------------------

void TirerProjectile(Jeu &J)
{
    if(isKeyPressed(SDLK_SPACE))
    {
        bool tire = false;

        for(int i = 0; i < J.nb_projectile && !tire; i++)
        {
            if(!J.projectile[i].actif)
            {
                J.projectile[i].pos = J.vaisseau.pos;
                J.projectile[i].vit = make_complex(0, 5);
                J.projectile[i].actif = true;
                tire = true;
            }
        }
    }
}


// ------------------------- Projectile movement -------------------------

void MouvementProjectile(Jeu &J)
{
    for(int i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif)
        {
            J.projectile[i].pos = J.projectile[i].pos + J.projectile[i].vit;

            if(J.projectile[i].pos.y < 0 || J.projectile[i].pos.y > DIMW)
                J.projectile[i].actif = false;
        }
    }
}


// ------------------------- Draw -------------------------

void Draw(Jeu &J)
{
    image_draw(imgBg, 0, 0);

    image_draw(imgShip, J.vaisseau.pos.x - 20, J.vaisseau.pos.y - 20);

    for(int i = 0; i < J.nb_projectile; i++)
        if(J.projectile[i].actif)
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);

    for(int i = 0; i < J.nb_ennemi; i++)
        if(J.ennemi[i].vivant)
            image_draw(imgEnemy, J.ennemi[i].pos.x - 20, J.ennemi[i].pos.y - 20);
}


// ------------------------- Main -------------------------

int main(int, char **)
{
    winInit("Shoot'em up", DIMW, DIMW);

    Jeu jeu;
    InitJeu(jeu, 150, 20);

    setKeyRepeatMode(true);

    bool stop = false;

    while(!stop)
    {
        winClear();

        TirerProjectile(jeu);
        MouvementVaisseau(jeu);
        MouvementProjectile(jeu);

        Draw(jeu);

        delay(30);
        stop = winDisplay();
    }

    winQuit();
    return 0;
}
```
