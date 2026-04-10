/* =============================================================================
IDRIS YOUSFI

README :


Cahier des charges


Description du jeu:

    Le joueur controle un vaisseau qui peut se deplacer horizontalement
    et tier des projectilespour deplacant au clavier. Les positiond vont utliser
    des nombres complex. Les projectiles sont tirés verticalement. Le vaisseau va etre 
    positionner en bas et se deplacant au clavier. Les ennemis eux sont en haut de la
    fenetre et fixe. On va regarder si un projectile rentre en collision avec un ennemi.
    Un ennemi sera representer par un rectangle, les projectiles par un cercle on 
    utilisera les les Complex et les Particules.  



Amelioration:  

    - faire different vaisseau(tire plus rapide, tire plusieur projectile en meme temps).
    - un score qui compte les ennemis detruit par 100 et à 3 000 on gagne (pour 
    l'instant cette fonction ne sera pas dans le code).
    - des ennemis qui envoie des projectile pour nous faire perdre.
    - des ennemis qui ne sont pas fixe.


------------------------- Fonction-à-faire -------------------------


- InitJeu - fait

- MouvementVaisseau - fait

- TirerProjectile - partiellement fait mais probleme au niveau du projectile je le debuggerai à la prochaine scéance je suppose que le probleme vient de la ligne 276

- MouvementProjectile - non fait

- UpdateEnnemi - non fait

- Update - non fait

- Collision - non fait

- Draw - partiellement faite il me manque des images un meilleur visuel


------------------------- 1er-Semaine-10-avril -------------------------

Ajout de la procédure initJeu ainsi que les structures principal du jeu. 
Creation de la procédure MouvementVaisseau


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


// ------------------------- Initialisation-des-constantes -------------------------


const int DIMW = 600; // taille de la fenetre

const int NB_PROJECTILE = 300;
const int NB_ENNEMI = 30;


// ------------------------- Les-Structures -------------------------


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


//------------------------- Programme-principal -------------------------

void InitJeu(Jeu &J, int nb_projectile, int nb_ennemi)
{
    int i;
    J.vaisseau.pos = make_complex(DIMW/2, 80); // on place le vaisseau au milleur-bas
    J.nb_projectile = nb_projectile;
    J.nb_ennemi = nb_ennemi;
    for(i = 0; i < nb_projectile; i++)
    {
        J.projectile[i].actif = false;
        J.projectile[i].pos = make_complex(J.vaisseau.pos.x,J.vaisseau.pos.y);
        J.projectile[i].vit = make_complex(J.vaisseau.pos.x,J.vaisseau.pos.y + 5); // tire vers le haut
    }
    for(i = 0; i < nb_ennemi; i++)
    {
        int ligne = i / 10; // c'est un int il prend la valeur entiere donc il va faire un parcourir 10 ennemis a chaque ligne exemple : 1/10=0 ; 2/10=0 ; 9/10=0 ; 11/10=1 ; 22/10=2
        int colonne = i % 10; // il prend le reste donc il parcours les collones il parcours et une fois arriver a 10 le modulo va le ramener a 0 : 1%10=1 ; 2%10=2 ; 9%10=9; 10%10=0 ; 15%10=5 ; 27%10=7
        
        J.ennemi[i].vivant = true; // tout les ennemis seront present au debut
        J.ennemi[i].pos = make_complex(70 + colonne*50, DIMW-70 - ligne *50);  // j'ai mis 50 pour les espacés et DIMW-70 pour les faire mettre à partir du haut de la fenetre 
    }
}

void MouvementVaisseau(Jeu &J) // Deplacement du vaisseau en horizontal
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
    const int PROJECTILE_VITESSE = 5;
    for(int i = 0; i < J.nb_projectile; i++)
    {
        if(isKeyPressed(SDLK_SPACE))
        {
           J.projectile[i].pos = make_complex(J.vaisseau.pos.x, J.vaisseau.pos.y);
           J.projectile[i].vit = make_complex(J.vaisseau.pos.x, i * PROJECTILE_VITESSE);
           J.projectile[i].actif = true;
        }
    }
}

void Draw(Jeu &J)
{
    int i;
    color(255,255,255); // couleur du vaisseau
    rectangleFill(J.vaisseau.pos.x - 10, J.vaisseau.pos.y - 5, J.vaisseau.pos.x + 10, J.vaisseau.pos.y + 5);
    
    color(241, 196, 15); // couleur des projectiles
    for(i = 0; i < J.nb_projectile; i++)
    {
        if(J.projectile[i].actif)
        {
            circleFill(J.projectile[i].pos.x, J.projectile[i].pos.y, 5);
        }
    }

    color(208, 53, 59);
    for(i = 0; i < J.nb_ennemi; i++)
    {
        if(J.ennemi[i].vivant)
        {
            rectangleFill(J.ennemi[i].pos.x - 10, J.ennemi[i].pos.y - 5, J.ennemi[i].pos.x + 10, J.ennemi[i].pos.y + 5);
        }
    }
}






int main(int, char **)
{
    winInit("Shoot'em up", DIMW, DIMW);
    bool stop = false;
    setKeyRepeatMode(true); 
    Jeu jeu;
    backgroundColor(6, 50, 108);
    InitJeu(jeu, 150, 20);
    while (!stop)
    {
        winClear();
        TirerProjectile(jeu);
        MouvementVaisseau(jeu);
        Draw(jeu);
        delay(30);
        stop = winDisplay();
    }
    winQuit();
    return 0;
}