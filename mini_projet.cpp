/* =============================================================================
IDRIS YOUSFI

README:


Game Design Document


Game Description:

    The player controls a spaceship that can move horizontally
    and shoot projectiles using the keyboard. Positions use
    complex numbers.

    Projectiles are fired vertically.

    The spaceship is positioned at the bottom of the screen
    and moves horizontally using keyboard input.

    Enemies are located at the top of the window and are static.

    The game checks collision between projectiles and enemies.

    Enemies are represented as rectangles, and projectiles as circles.
    The project uses Complex numbers and particle concepts.



Improvements:

    - Different spaceship types (faster shooting, multiple projectiles, etc.)
    - Score system (100 points per destroyed enemy, win at 3000 points — not implemented yet)
    - Enemies that shoot projectiles at the player
    - Moving enemies instead of static ones


------------------------- TODO LIST -------------------------

- InitGame - done

- MoveSpaceship - done

- ShootProjectile - partially done, but there is a bug in the projectile system.
  I will debug it in the next session. I suspect the issue is around line 276.

- MoveProjectile - not done

- UpdateEnemies - not done

- UpdateGame - not done

- Collision - not done

- Draw - partially done (missing assets and better visuals)


------------------------- Week 1 - April 10 -------------------------

Added InitGame function and main game structures.
Created MoveSpaceship function.

============================================================================= */

#include <ctime>
#include <iostream>
#include <Grapic.h>

using namespace grapic;
using namespace std;


// ------------------------- Complex numbers -------------------------

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
    float theta = (theta_deg * M_PI) / 180.0;
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
    return operator*(lambda, b);
}

Complex scale(Complex p, float cx, float cy, float lambda)
{
    Complex c = make_complex(cx, cy);
    return (p - c) * lambda + c;
}

Complex operator*(Complex a, Complex b)
{
    Complex z;
    z.x = a.x * b.x - a.y * b.y;
    z.y = a.x * b.y + a.y * b.x;
    return z;
}


// ------------------------- Color -------------------------

struct Color
{
    int r;
    int g;
    int b;
};

Color operator+(Color a, Color b)
{
    Color z;
    z.r = a.r + b.r;
    z.g = a.g + b.g;
    z.b = a.b + b.b;
    return z;
}

Color operator*(float lambda, Color b)
{
    Color z;
    z.r = lambda * b.r;
    z.g = lambda * b.g;
    z.b = lambda * b.b;
    return z;
}

Color operator*(Color b, float lambda)
{
    return operator*(lambda, b);
}

Color make_color(int r, int g, int b)
{
    Color c;
    c.r = r;
    c.g = g;
    c.b = b;
    return c;
}


// ------------------------- Distance -------------------------

float distance(Complex V)
{
    return sqrt(V.x * V.x + V.y * V.y);
}

float distance(Complex A, Complex B)
{
    return distance(B - A);
}


// ------------------------- Constants -------------------------

const int DIMW = 600; // window size

const int NB_PROJECTILE = 300;
const int NB_ENNEMY = 30;


// ------------------------- Structures -------------------------

struct Spaceship
{
    Complex pos;
};

struct Projectile
{
    Complex pos;
    Complex vel;
    bool active;
};

struct Enemy
{
    Complex pos;
    bool alive;
};

struct Game
{
    Spaceship spaceship;
    Projectile projectile[NB_PROJECTILE];
    Enemy enemy[NB_ENNEMY];
    int nb_projectile;
    int nb_enemy;
};


//------------------------- Game Logic -------------------------

void InitGame(Game &J, int nb_projectile, int nb_enemy)
{
    int i;
    J.spaceship.pos = make_complex(DIMW / 2, 80);
    J.nb_projectile = nb_projectile;
    J.nb_enemy = nb_enemy;
    for (i = 0; i < nb_projectile; i++)
    {
        J.projectile[i].active = false;
        J.projectile[i].pos = make_complex(J.spaceship.pos.x, J.spaceship.pos.y);
        J.projectile[i].vel = make_complex(J.spaceship.pos.x, J.spaceship.pos.y + 5);
    }
    for (i = 0; i < nb_enemy; i++)
    {
        int row = i / 10;
        int col = i % 10;
        J.enemy[i].alive = true;
        J.enemy[i].pos = make_complex(70 + col * 50, DIMW - 70 - row * 50);
    }
}


// Move spaceship horizontally
void MoveSpaceship(Game &J)
{
    if (isKeyPressed(SDLK_LEFT))
        J.spaceship.pos.x -= 5;

    if (isKeyPressed(SDLK_RIGHT))
        J.spaceship.pos.x += 5;
}


// Shoot projectile
void ShootProjectile(Game &J)
{
    const int PROJECTILE_SPEED = 5;
    for (int i = 0; i < J.nb_projectile; i++)
    {
        if (isKeyPressed(SDLK_SPACE))
        {
            J.projectile[i].pos = make_complex(J.spaceship.pos.x, J.spaceship.pos.y);
            J.projectile[i].vel = make_complex(J.spaceship.pos.x, i * PROJECTILE_SPEED);
            J.projectile[i].active = true;
        }
    }
}


// ------------------------- Draw -------------------------

void Draw(Game &J)
{
    int i;
    color(255, 255, 255);
    rectangleFill(J.spaceship.pos.x - 10, J.spaceship.pos.y - 5, J.spaceship.pos.x + 10, J.spaceship.pos.y + 5);
    color(241, 196, 15);
    for (i = 0; i < J.nb_projectile; i++)
    {
        if (J.projectile[i].active)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }
    color(208, 53, 59);
    for (i = 0; i < J.nb_enemy; i++)
    {
        if (J.enemy[i].alive)
        {
            rectangleFill(J.enemy[i].pos.x - 10, J.enemy[i].pos.y - 5, J.enemy[i].pos.x + 10, J.enemy[i].pos.y + 5);
        }
    }
}


// ------------------------- Main -------------------------

int main(int, char **)
{
    winInit("Shoot'em up", DIMW, DIMW);
    bool stop = false;
    setKeyRepeatMode(true);
    Game game;
    backgroundColor(6, 50, 108);
    InitGame(game, 150, 20);
    while (!stop)
    {
        winClear();
        ShootProjectile(game);
        MoveSpaceship(game);
        Draw(game);
        delay(30);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}
